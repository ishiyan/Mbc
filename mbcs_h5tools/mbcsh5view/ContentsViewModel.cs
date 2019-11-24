using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Markup;
using System.ComponentModel;

using Mbcs.H5;

// Some "never used" properties and functions are actually used in XAML.
// Do not remove them or make them private!

namespace Mbcsh5view
{
    /// <summary>
    /// A ViewModel for the GroupInfo/DataInfo hierarchy. This ViewModel adds the notion of node selection and expansion.
    /// </summary>
    [ContentProperty("Items")]
    public class ContentsViewModel : INotifyPropertyChanged
    {
        #region Members and accessors
        private void OnPropertyChanged(string propertyName)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            handler?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        /// <summary>
        /// Event raised when a property changes.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        private bool isExpanded;
        /// <summary>
        /// Determines whether the TreeViewItem associated with this data item is expanded.
        /// </summary>
        public bool IsExpanded
        {
            get => isExpanded;
            set
            {
                isExpanded = value;
                OnPropertyChanged("IsExpanded");
            }
        }

        private bool isSelected;
        /// <summary>
        /// Determines whether the TreeViewItem associated with this data item is selected.
        /// </summary>
        public bool IsSelected
        {
            get => isSelected;
            set
            {
                isSelected = value;
                OnPropertyChanged("IsSelected");
            }
        }

        /// <summary>
        /// The visible name of the item.
        /// </summary>
        public string Label { get; }

        /// <summary>
        /// The full path to the item.
        /// </summary>
        public string Path { get; }

        /// <summary>
        /// If the item is a dataset or a group.
        /// </summary>
        public bool IsDataSet { get; }

        /// <summary>
        /// If the item is successfully parsed.
        /// </summary>
        public bool IsParsed { get; } = true;

        /// <summary>
        /// The associated data info if the item is a dataset; null otherwise.
        /// </summary>
        public DataInfo DataInfo { get; }

        /// <summary>
        /// The associated group info if the item is a group; null otherwise.
        /// </summary>
        public GroupInfo GroupInfo { get; }

        /// <summary>
        /// The subtree of this item.
        /// </summary>
        public List<ContentsViewModel> Items { get; } = new List<ContentsViewModel>();
        #endregion

        #region Construction
        /// <summary>
        /// Constructs a new object.
        /// </summary>
        /// <param name="dataInfo">The associated data info object.</param>
        private ContentsViewModel(DataInfo dataInfo)
        {
            Label = dataInfo.Name;
            Path = dataInfo.Path;
            IsDataSet = true;
            DataInfo = dataInfo;
            IsParsed = dataInfo.IsValidName;
        }

        /// <summary>
        /// Constructs a new object.
        /// </summary>
        /// <param name="groupInfo">The associated group info object.</param>
        private ContentsViewModel(GroupInfo groupInfo)
        {
            Label = groupInfo.Name;
            Path = groupInfo.Path;
            GroupInfo = groupInfo;
        }
        #endregion

        #region Expand/Collapse nodes
        /// <summary>
        /// Traverses the tree in a depth-first non-recursive way and executes the action passed as a parameter on each item.
        /// </summary>
        /// <param name="itemAction">Action to be executed for each item.</param>
        private void ApplyActionToSubTree(Action<ContentsViewModel> itemAction)
        {
            var itemStack = new Stack<ContentsViewModel>();
            itemStack.Push(this);
            while (itemStack.Count != 0)
            {
                ContentsViewModel item = itemStack.Pop();
                itemAction(item);
                foreach (ContentsViewModel childItem in item.Items)
                {
                    itemStack.Push(childItem);
                }
            }
        }

        /// <summary>
        /// Traverses the entire view model hierarchy setting the IsExpanded property to true on each item.
        /// </summary>
        // ReSharper disable once UnusedMember.Global
        public void ExpandSubTree()
        {
            ApplyActionToSubTree(item => item.IsExpanded = true);
        }

        /// <summary>
        /// Traverses the entire view model hierarchy setting the IsExpanded property to false on each item.
        /// </summary>
        // ReSharper disable once UnusedMember.Global
        public void CollapseSubTree()
        {
            // Here, I start collapsing items in the tree starting at the root.
            // It may seem that the tree needs to be collapsed starting at the leaf
            // nodes, but that's not the case because the entire tree will be updated
            // in one single layout pass. So in this case, the order in which items
            // are collapsed makes no difference.
            ApplyActionToSubTree(item => item.IsExpanded = false);
        }

        /// <summary>
        /// Uses recursion to look for the element passed as a parameter in the view model hierarchy and executes the action passed as a parameter to its entire ancestor chain (excluding the item itself).
        /// </summary>
        /// <param name="itemToLookFor">The element this method will look for.</param>
        /// <param name="itemAction">Action to be executed on each superclass in the ancestor chain.</param>
        /// <returns>True if it the itemToLookFor was found, false otherwise.</returns>
        private bool ApplyActionToSuperTree(ContentsViewModel itemToLookFor, Action<ContentsViewModel> itemAction)
        {
            if (itemToLookFor == this)
                return true;
            if (Items.Any(item => item.ApplyActionToSuperTree(itemToLookFor, itemAction)))
            {
                itemAction(this);
                return true;
            }
            return false;
        }

        /// <summary>
        /// Sets IsExpanded to true for each element in the ancestor chain of the item passed as a parameter.
        /// </summary>
        /// <param name="itemToLookFor">The element this method will look for.</param>
        /// <returns>True if it the itemToLookFor was found, false otherwise.</returns>
        public bool ExpandSuperTree(ContentsViewModel itemToLookFor)
        {
            return ApplyActionToSuperTree(itemToLookFor, item => item.IsExpanded = true);
        }
        #endregion

        #region Populate
        private static void Populate(ContentsViewModel rootItem, GroupInfo groupInfo, List<ContentsViewModel> flatList)
        {
            foreach (var v in groupInfo.Groups)
            {
                var item = new ContentsViewModel(v);
                rootItem.Items.Add(item);
                Populate(item, v, flatList);
            }
            foreach (var v in groupInfo.Datasets)
            {
                var item = new ContentsViewModel(v);
                rootItem.Items.Add(item);
                flatList.Add(item);
            }
        }

        /// <summary>
        /// Populates both tree and flat views.
        /// </summary>
        /// <param name="rootGroupInfo">The root group info.</param>
        /// <param name="treeList">The list of tree view items.</param>
        /// <param name="flatList">The list of flat view items.</param>
        public static void Populate(GroupInfo rootGroupInfo, out List<ContentsViewModel> treeList, out List<ContentsViewModel> flatList)
        {
            treeList = new List<ContentsViewModel>(128);
            flatList = new List<ContentsViewModel>(64);
            var rootItem = new ContentsViewModel(rootGroupInfo);
            Populate(rootItem, rootGroupInfo, flatList);
            treeList.Add(rootItem);
            // rootItem.ExpandSubtree();
        }
        #endregion
    }
}
