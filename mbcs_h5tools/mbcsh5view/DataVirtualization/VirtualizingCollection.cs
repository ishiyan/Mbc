using System;
using System.Collections.Generic;
using System.Linq;
using System.Collections;
using System.Diagnostics;
using Mbcsh5view;
using Microsoft.Extensions.Logging;

// ReSharper disable once CheckNamespace
namespace DataVirtualization
{
    /// <summary>
    /// Specialized list implementation that provides data virtualization. The collection is divided up into pages,
    /// and pages are dynamically fetched from the IItemsProvider when required. Stale pages are removed after a
    /// configurable period of time.
    /// Intended for use with large collections on a network or disk resource that cannot be instantiated locally
    /// due to memory consumption or fetch latency.
    /// </summary>
    /// <remarks>
    /// The IList implementation is not fully complete, but should be sufficient for use as read only collection 
    /// data bound to a suitable ItemsControl.
    /// </remarks>
    /// <typeparam name="T"></typeparam>
    public class VirtualizingCollection<T> : IList<DataWrapper<T>>, IList
    {
        private readonly ILogger<VirtualizingCollection<T>> logger = App.LoggerFactory.CreateLogger<VirtualizingCollection<T>>();

        #region Constructors
        /// <summary>
        /// Initializes a new instance of the <see cref="VirtualizingCollection&lt;T&gt;"/> class.
        /// </summary>
        /// <param name="itemsProvider">The items provider.</param>
        /// <param name="pageSize">Size of the page.</param>
        /// <param name="pageTimeout">The page timeout.</param>
        protected VirtualizingCollection(IItemsProvider<T> itemsProvider, int pageSize, int pageTimeout)
        {
            ItemsProvider = itemsProvider;
            PageSize = pageSize;
            PageTimeout = pageTimeout;
            logger.LogDebug($"constructed: page size = {pageSize}, page timeout = {pageTimeout}");
        }
        #endregion

        #region ItemsProvider
        /// <summary>
        /// Gets the items provider.
        /// </summary>
        // ReSharper disable once MemberCanBePrivate.Global
        public IItemsProvider<T> ItemsProvider { get; }
        #endregion

        #region PageSize
        /// <summary>
        /// Gets the size of the page.
        /// </summary>
        /// <value>The size of the page.</value>
        // ReSharper disable once MemberCanBePrivate.Global
        public int PageSize { get; }
        #endregion

        #region PageTimeout
        /// <summary>
        /// Gets the page timeout.
        /// </summary>
        /// <value>The page timeout.</value>
        // ReSharper disable once MemberCanBePrivate.Global
        public long PageTimeout { get; }

        #endregion

        #region IList<DataWrapper<T>>, IList
        #region Count
        private int count = -1;

        /// <summary>
        /// Gets the number of elements contained in the <see cref="T:System.Collections.Generic.ICollection`1"/>.
        /// The first time this property is accessed, it will fetch the count from the IItemsProvider.
        /// </summary>
        /// <value></value>
        /// <returns>
        /// The number of elements contained in the <see cref="T:System.Collections.Generic.ICollection`1"/>.
        /// </returns>
        public int Count
        {
            get
            {
                if (count == -1)
                {
                    count = 0;
                    LoadCount();
                }
                return count;
            }
            protected set => count = value;
        }
        #endregion

        #region Indexer
        /// <summary>
        /// Gets the item at the specified index. This property will fetch
        /// the corresponding page from the IItemsProvider if required.
        /// </summary>
        public DataWrapper<T> this[int index]
        {
            get
            {
                // determine which page and offset within page
                int pageIndex = index / PageSize;
                int pageOffset = index % PageSize;

                // request primary page
                RequestPage(pageIndex);

                // if accessing upper 50% then request next page
                if (pageOffset > PageSize / 2 && pageIndex < Count / PageSize)
                    RequestPage(pageIndex + 1);

                // if accessing lower 50% then request prev page
                if (pageOffset < PageSize / 2 && pageIndex > 0)
                    RequestPage(pageIndex - 1);

                // remove stale pages
                CleanUpPages();

                // return requested item
                return pages[pageIndex].Items[pageOffset];
            }
            set => throw new NotSupportedException();
        }

        object IList.this[int index]
        {
            get => this[index];
            set => throw new NotSupportedException();
        }
        #endregion

        #region IEnumerator<DataWrapper<T>>, IEnumerator
        /// <summary>
        /// Returns an enumerator that iterates through the collection.
        /// </summary>
        /// <remarks>
        /// This method should be avoided on large collections due to poor performance.
        /// </remarks>
        /// <returns>
        /// A <see cref="T:System.Collections.Generic.IEnumerator`1"/> that can be used to iterate through the collection.
        /// </returns>
        public IEnumerator<DataWrapper<T>> GetEnumerator()
        {
            for (int i = 0; i < Count; i++)
            {
                yield return this[i];
            }
        }

        /// <summary>
        /// Returns an enumerator that iterates through a collection.
        /// </summary>
        /// <returns>
        /// An <see cref="T:System.Collections.IEnumerator"/> object that can be used to iterate through the collection.
        /// </returns>
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
        #endregion

        #region Add
        /// <summary>
        /// Not supported.
        /// </summary>
        /// <param name="item">The object to add to the <see cref="T:System.Collections.Generic.ICollection`1"/>.</param>
        /// <exception cref="T:System.NotSupportedException">
        /// The <see cref="T:System.Collections.Generic.ICollection`1"/> is read-only.
        /// </exception>
        public void Add(DataWrapper<T> item)
        {
            throw new NotSupportedException();
        }

        int IList.Add(object value)
        {
            throw new NotSupportedException();
        }
        #endregion

        #region Contains
        bool IList.Contains(object value)
        {
            return Contains((DataWrapper<T>)value);
        }

        /// <summary>
        /// Not supported.
        /// </summary>
        /// <param name="item">The object to locate in the <see cref="T:System.Collections.Generic.ICollection`1"/>.</param>
        /// <returns>
        /// Always false.
        /// </returns>
        public bool Contains(DataWrapper<T> item)
        {
            foreach (DataPage<T> page in pages.Values)
            {
                if (page.Items.Contains(item))
                {
                    return true;
                }
            }
            return false;
        }
        #endregion

        #region Clear
        public void Clear()
        {
            throw new NotSupportedException();
        }
        #endregion

        #region IndexOf
        int IList.IndexOf(object value)
        {
            return IndexOf((DataWrapper<T>)value);
        }

        /// <summary>
        /// </summary>
        /// <param name="item">The object to locate.</param>
        public int IndexOf(DataWrapper<T> item)
        {
            foreach (KeyValuePair<int, DataPage<T>> keyValuePair in pages)
            {
                int indexWithinPage = keyValuePair.Value.Items.IndexOf(item);
                if (indexWithinPage != -1)
                {
                    return PageSize * keyValuePair.Key + indexWithinPage;
                }
            }
            return -1;
        }
        #endregion

        #region Insert
        /// <summary>
        /// Not supported.
        /// </summary>
        /// <param name="index">The zero-based index at which <paramref name="item"/> should be inserted.</param>
        /// <param name="item">The object to insert into the <see cref="T:System.Collections.Generic.IList`1"/>.</param>
        /// <exception cref="T:System.ArgumentOutOfRangeException">
        ///     <paramref name="index"/> is not a valid index in the <see cref="T:System.Collections.Generic.IList`1"/>.
        /// </exception>
        /// <exception cref="T:System.NotSupportedException">
        /// The <see cref="T:System.Collections.Generic.IList`1"/> is read-only.
        /// </exception>
        public void Insert(int index, DataWrapper<T> item)
        {
            throw new NotSupportedException();
        }

        void IList.Insert(int index, object value)
        {
            Insert(index, (DataWrapper<T>)value);
        }
        #endregion

        #region Remove
        /// <summary>
        /// Not supported.
        /// </summary>
        /// <param name="index">The zero-based index of the item to remove.</param>
        /// <exception cref="T:System.ArgumentOutOfRangeException">
        /// 	<paramref name="index"/> is not a valid index in the <see cref="T:System.Collections.Generic.IList`1"/>.
        /// </exception>
        /// <exception cref="T:System.NotSupportedException">
        /// The <see cref="T:System.Collections.Generic.IList`1"/> is read-only.
        /// </exception>
        public void RemoveAt(int index)
        {
            throw new NotSupportedException();
        }

        void IList.Remove(object value)
        {
            throw new NotSupportedException();
        }

        /// <summary>
        /// Not supported.
        /// </summary>
        /// <param name="item">The object to remove from the <see cref="T:System.Collections.Generic.ICollection`1"/>.</param>
        /// <returns>
        /// true if <paramref name="item"/> was successfully removed from the <see cref="T:System.Collections.Generic.ICollection`1"/>; otherwise, false. This method also returns false if <paramref name="item"/> is not found in the original <see cref="T:System.Collections.Generic.ICollection`1"/>.
        /// </returns>
        /// <exception cref="T:System.NotSupportedException">
        /// The <see cref="T:System.Collections.Generic.ICollection`1"/> is read-only.
        /// </exception>
        public bool Remove(DataWrapper<T> item)
        {
            throw new NotSupportedException();
        }
        #endregion

        #region CopyTo
        public void CopyTo(DataWrapper<T>[] array, int arrayIndex)
        {
            throw new NotSupportedException();
        }

        void ICollection.CopyTo(Array array, int index)
        {
            throw new NotSupportedException();
        }
        #endregion

        #region Misc
        /// <summary>
        /// Gets an object that can be used to synchronize access to the <see cref="T:System.Collections.ICollection"/>.
        /// </summary>
        /// <value></value>
        /// <returns>
        /// An object that can be used to synchronize access to the <see cref="T:System.Collections.ICollection"/>.
        /// </returns>
        public object SyncRoot => this;

        /// <summary>
        /// Gets a value indicating whether access to the <see cref="T:System.Collections.ICollection"/> is synchronized (thread safe).
        /// </summary>
        /// <value></value>
        /// <returns>Always false.
        /// </returns>
        public bool IsSynchronized => false;

        /// <summary>
        /// Gets a value indicating whether the <see cref="T:System.Collections.Generic.ICollection`1"/> is read-only.
        /// </summary>
        /// <value></value>
        /// <returns>Always true.
        /// </returns>
        public bool IsReadOnly => true;

        /// <summary>
        /// Gets a value indicating whether the <see cref="T:System.Collections.IList"/> has a fixed size.
        /// </summary>
        /// <value></value>
        /// <returns>Always false.
        /// </returns>
        public bool IsFixedSize => false;
        #endregion
        #endregion

        #region Paging
        private Dictionary<int, DataPage<T>> pages = new Dictionary<int, DataPage<T>>();

        /// <summary>
        /// Cleans up any stale pages that have not been accessed in the period dictated by PageTimeout.
        /// </summary>
        // ReSharper disable once MemberCanBePrivate.Global
        public void CleanUpPages()
        {
            int[] keys = pages.Keys.ToArray();
            foreach (int key in keys)
            {
                // page 0 is a special case, since WPF ItemsControl access the first item frequently
                if (key != 0 && (DateTime.Now - pages[key].TouchTime).TotalMilliseconds > PageTimeout)
                {
                    bool removePage = true;
                    if (pages.TryGetValue(key, out var page))
                    {
                        removePage = !page.IsInUse;
                    }

                    if (removePage)
                    {
                        logger.LogDebug($"Removing page {key}");
                        pages.Remove(key);
                    }
                }
            }
        }

        /// <summary>
        /// Makes a request for the specified page, creating the necessary slots in the dictionary,
        /// and updating the page touch time.
        /// </summary>
        /// <param name="pageIndex">Index of the page.</param>
        // ReSharper disable once VirtualMemberNeverOverridden.Global
        protected virtual void RequestPage(int pageIndex)
        {
            if (!pages.ContainsKey(pageIndex))
            {
                // Create a page of empty data wrappers.
                int pageLength = Math.Min(PageSize, Count - pageIndex * PageSize);
                var page = new DataPage<T>(pageIndex * PageSize, pageLength);
                logger.LogDebug($"Adding page {pageIndex}");
                pages.Add(pageIndex, page);
                LoadPage(pageIndex, pageLength);
            }
            else
            {
                pages[pageIndex].TouchTime = DateTime.Now;
            }
        }

        /// <summary>
        /// Populates the page within the dictionary.
        /// </summary>
        /// <param name="pageIndex">Index of the page.</param>
        /// <param name="dataItems">The page.</param>
        // ReSharper disable once VirtualMemberNeverOverridden.Global
        protected virtual void PopulatePage(int pageIndex, IList<T> dataItems)
        {
            logger.LogDebug($"Populating page {pageIndex}");
            if (pages.TryGetValue(pageIndex, out var page))
            {
                page.Populate(dataItems);
            }
        }

        /// <summary>
        /// Removes all cached pages. This is useful when the count of the 
        /// underlying collection changes.
        /// </summary>
        protected void EmptyCache()
        {
            pages = new Dictionary<int, DataPage<T>>();
        }
        #endregion

        #region Load methods
        /// <summary>
        /// Loads the count of items.
        /// </summary>
        protected virtual void LoadCount()
        {
            Count = FetchCount();
        }

        /// <summary>
        /// Loads the page of items.
        /// </summary>
        /// <param name="pageIndex">Index of the page.</param>
        /// <param name="pageLength">Number of items in the page.</param>
        protected virtual void LoadPage(int pageIndex, int pageLength)
        {
            logger.LogDebug($"Loading page {pageIndex}, page length = {pageLength}");
            PopulatePage(pageIndex, FetchPage(pageIndex, pageLength, out var i));
            Count = i;
        }
        #endregion

        #region Fetch methods
        /// <summary>
        /// Fetches the requested page from the IItemsProvider.
        /// </summary>
        /// <param name="pageIndex">Index of the page.</param>
        /// <param name="pageLength">Length of the page.</param>
        /// <param name="overallCount">The overall count.</param>
        protected IList<T> FetchPage(int pageIndex, int pageLength, out int overallCount)
        {
            return ItemsProvider.FetchRange(pageIndex * PageSize, pageLength, out overallCount);
        }

        /// <summary>
        /// Fetches the count of items from the IItemsProvider.
        /// </summary>
        protected int FetchCount()
        {
            return ItemsProvider.FetchCount();
        }
        #endregion
    }
}
