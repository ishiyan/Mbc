using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using static Mbcs.NativeMethods.NativeMethods;

namespace Mbcs.H5
{
    /// <summary>
    /// Encapsulates the access to the instrument's Trade data.
    /// </summary>
    public sealed class TradeData : Data
    {
        internal TradeData(Instrument instrument, string dataName, IntPtr dataHandle) :
            base(instrument, dataName, dataHandle)
        {
        }

        /// <inheritdoc />
        protected override void ReleaseUnmanagedResources()
        {
            Close();
        }

        /// <summary>
        /// Flushes and closes an access to the dataset. The instance of the class is not usable after this call.
        /// </summary>
        public void Close()
        {
            if (dataHandle.Handle != IntPtr.Zero)
            {
                H5TradeDataClose(dataHandle.Handle);
                dataHandle = new HandleRef(this, IntPtr.Zero);
            }
        }

        /// <summary>
        /// Flushes the dataset.
        /// </summary>
        public void Flush()
        {
            if (dataHandle.Handle != IntPtr.Zero)
                H5TradeDataFlush(dataHandle.Handle);
        }

        /// <summary>
        /// The date and time ticks of the first data item.
        /// </summary>
        public long FirstTicks => dataHandle.Handle == IntPtr.Zero ? 0L : H5TradeDataFirstTicks(dataHandle.Handle);

        /// <summary>
        /// The date and time ticks of the last data item.
        /// </summary>
        public long LastTicks => dataHandle.Handle == IntPtr.Zero ? 0L : H5TradeDataLastTicks(dataHandle.Handle);

        /// <summary>
        /// The number of data items.
        /// </summary>
        public ulong Count => dataHandle.Handle == IntPtr.Zero ? 0L : H5TradeDataCount(dataHandle.Handle);

        /// <summary>
        /// The maximal size in bytes of the read buffer, 0 means unlimited.
        /// </summary>
        public ulong MaximumReadBufferBytes
        {
            get => dataHandle.Handle == IntPtr.Zero ? 0L : H5TradeDataGetMaximumReadBufferBytes(dataHandle.Handle);
            set
            {
                if (dataHandle.Handle != IntPtr.Zero)
                    H5TradeDataSetMaximumReadBufferBytes(dataHandle.Handle, value);
            }
        }

        /// <summary>
        /// Spreads the duplicate time ticks making one-tick increments.
        /// </summary>
        /// <param name="array">An array of data items.</param>
        /// <param name="verbose">Trace information messages.</param>
        /// <returns>A boolean indicating if the time ticks were spread.</returns>
        public static bool SpreadDuplicateTimeTicks(Trade[] array, bool verbose)
        {
            // return NativeMethods.H5TradeDataSpreadDuplicateTimeTicks(array, (ulong)array.LongLength, verbose);
            GCHandle handle = GCHandle.Alloc(array, GCHandleType.Pinned);
            bool status;
            try
            {
                status = H5TradeDataSpreadDuplicateTimeTicks(handle.AddrOfPinnedObject(), (ulong)array.LongLength, verbose);
            }
            finally
            {
                handle.Free();
            }
            return status;
        }

        /// <summary>
        /// Adds an array of items to the dataset.
        /// </summary>
        /// <param name="array">An array of data items.</param>
        /// <param name="duplicateTimeTicks">The duplicate time tick policy.</param>
        /// <param name="verbose">Trace information messages.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool Add(Trade[] array, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataAdd(dataHandle.Handle, array, (ulong)array.LongLength, duplicateTimeTicks, verbose);
        }

        /// <summary>
        /// Callback function to append trade data items.
        /// </summary>
        /// <param name="ticks">The ticks.</param>
        /// <param name="price">The price.</param>
        /// <param name="volume">The volume.</param>
        public delegate void Appender(long ticks, double price, double volume);

        /// <summary>
        /// Fetches all stored data items.
        /// </summary>
        /// <param name="appender">A callback function to append data items.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool FetchAll(Appender appender)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataFetchAll(dataHandle.Handle, appender);
        }

        /// <summary>
        /// Fetches all stored data items.
        /// </summary>
        /// <param name="list">A list to fetch into.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool FetchAll(List<Trade> list)
        {
            void Appender(long ticks, double price, double volume)
            {
                list.Add(new Trade
                {
                    Ticks = ticks,
                    Price = price,
                    Volume = volume
                });
            }

            return dataHandle.Handle != IntPtr.Zero && H5TradeDataFetchAll(dataHandle.Handle, Appender);
        }

        /// <summary>
        /// Fetches a specified range of stored items.
        /// </summary>
        /// <param name="appender">A callback function to append data items.</param>
        /// <param name="from">A beginning date and time.</param>
        /// <param name="to">An ending date and time.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool FetchTickRange(Appender appender, DateTime from, DateTime to)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataFetchTickRange(dataHandle.Handle, appender, from.Ticks, to.Ticks);
        }

        /// <summary>
        /// Fetches a specified range of stored items.
        /// </summary>
        /// <param name="list">A list to fetch into.</param>
        /// <param name="from">A beginning date and time.</param>
        /// <param name="to">An ending date and time.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool FetchTickRange(List<Trade> list, DateTime from, DateTime to)
        {
            void Appender(long ticks, double price, double volume)
            {
                list.Add(new Trade
                {
                    Ticks = ticks,
                    Price = price,
                    Volume = volume
                });
            }

            return dataHandle.Handle != IntPtr.Zero && H5TradeDataFetchTickRange(dataHandle.Handle, Appender, from.Ticks, to.Ticks);
        }

        /// <summary>
        /// Fetches a specified range of stored items.
        /// </summary>
        /// <param name="appender">A callback function to append data items.</param>
        /// <param name="index">The index of the first item in the range.</param>
        /// <param name="count">The number of items in the range.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool FetchIndexRange(Appender appender, ulong index, ulong count)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataFetchIndexRange(dataHandle.Handle, appender, index, count);
        }

        /// <summary>
        /// Fetches a specified range of stored items.
        /// </summary>
        /// <param name="list">A list to fetch into.</param>
        /// <param name="index">The index of the first item in the range.</param>
        /// <param name="count">The number of items in the range.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool FetchIndexRange(List<Trade> list, ulong index, ulong count)
        {
            void Appender(long ticks, double price, double volume)
            {
                list.Add(new Trade
                {
                    Ticks = ticks,
                    Price = price,
                    Volume = volume
                });
            }

            return dataHandle.Handle != IntPtr.Zero && H5TradeDataFetchIndexRange(dataHandle.Handle, Appender, index, count);
        }

        /// <summary>
        /// Converts ticks to a nearest index.
        /// </summary>
        /// <param name="ticks">The ticks to convert.</param>
        /// <param name="index">The index of the ticks.</param>
        /// <param name="match">If the index matches the ticks exactly or is a lower neighbor.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool TicksIndex(long ticks, out ulong index, out bool match)
        {
            if (dataHandle.Handle == IntPtr.Zero)
            {
                index = 0;
                match = false;
                return false;
            }

            return H5TradeDataTicksIndex(dataHandle.Handle, ticks, out index, out match);
        }

        /// <summary>
        /// Converts a ticks range to an index range.
        /// </summary>
        /// <param name="ticksFrom">The first ticks of the range.</param>
        /// <param name="ticksTo">The last ticks of the range.</param>
        /// <param name="indexFrom">The index of the first item in the range.</param>
        /// <param name="indexTo">The index of the last item in the range.</param>
        /// <param name="matchFrom">If the index of the first item in the range matches the ticks exactly or is a lower neighbor.</param>
        /// <param name="matchTo">If the index of the last item in the range matches the ticks exactly or is a lower neighbor.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool TicksIndexRange(long ticksFrom, long ticksTo, out ulong indexFrom, out ulong indexTo, out bool matchFrom, out bool matchTo)
        {
            if (dataHandle.Handle == IntPtr.Zero)
            {
                indexFrom = 0;
                indexTo = 0;
                matchFrom = false;
                matchTo = false;
                return false;
            }

            return H5TradeDataTicksIndexRange(dataHandle.Handle, ticksFrom, ticksTo, out indexFrom, out indexTo, out matchFrom, out matchTo);
        }

        /// <summary>
        /// Deletes a range of indices from the dataset.
        /// </summary>
        /// <param name="indexFrom">The first index of the range (inclusive).</param>
        /// <param name="indexTo">The last index of the range (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool DeleteIndexRange(ulong indexFrom, ulong indexTo)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataDeleteIndexRange(dataHandle.Handle, indexFrom, indexTo);
        }

        /// <summary>
        /// Deletes a range of ticks from the dataset.
        /// </summary>
        /// <param name="ticksFrom">The first ticks of the range (inclusive).</param>
        /// <param name="ticksTo">The last ticks of the range (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool DeleteTickRange(long ticksFrom, long ticksTo)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataDeleteTickRange(dataHandle.Handle, ticksFrom, ticksTo);
        }

        /// <summary>
        /// Deletes all ticks before a specified ticks from the dataset.
        /// </summary>
        /// <param name="ticks">The ticks (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool DeleteAllBeforeTick(long ticks)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataDeleteAllBeforeTick(dataHandle.Handle, ticks);
        }

        /// <summary>
        /// Deletes all ticks before a specified index from the dataset.
        /// </summary>
        /// <param name="index">The index (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool DeleteAllBeforeIndex(ulong index)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataDeleteAllBeforeIndex(dataHandle.Handle, index);
        }

        /// <summary>
        /// Deletes all ticks after a specified ticks from the dataset.
        /// </summary>
        /// <param name="ticks">The ticks (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool DeleteAllAfterTick(long ticks)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataDeleteAllAfterTick(dataHandle.Handle, ticks);
        }

        /// <summary>
        /// Deletes all ticks after a specified index from the dataset.
        /// </summary>
        /// <param name="index">The index (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool DeleteAllAfterIndex(ulong index)
        {
            return dataHandle.Handle != IntPtr.Zero && H5TradeDataDeleteAllAfterIndex(dataHandle.Handle, index);
        }
    }
}
