using System;
using System.Runtime.InteropServices;
using System.Security;
using Mbcs.H5;

namespace Mbcs.NativeMethods
{
    internal static partial class NativeMethods
    {
        /// <summary>
        /// Flushes and closes an access to the data. The <see paramref="dataHandle"/> is not valid after this call.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        [DllImport(DllName, EntryPoint = "h5ScalarDataClose", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void H5ScalarDataClose(IntPtr dataHandle);

        /// <summary>
        /// Flushes the data.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        [DllImport(DllName, EntryPoint = "h5ScalarDataFlush", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void H5ScalarDataFlush(IntPtr dataHandle);

        /// <summary>
        /// The date and time ticks of the first data item.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        [DllImport(DllName, EntryPoint = "h5ScalarDataFirstTicks", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern long H5ScalarDataFirstTicks(IntPtr dataHandle);

        /// <summary>
        /// The date and time ticks of the last data item.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        [DllImport(DllName, EntryPoint = "h5ScalarDataLastTicks", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern long H5ScalarDataLastTicks(IntPtr dataHandle);

        /// <summary>
        /// The number of the data items.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        [DllImport(DllName, EntryPoint = "h5ScalarDataCount", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern ulong H5ScalarDataCount(IntPtr dataHandle);

        /// <summary>
        /// Gets the maximal size in bytes of the read buffer, 0 means unlimited.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <returns>The maximal size in bytes of the read buffer.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataGetMaximumReadBufferBytes", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern ulong H5ScalarDataGetMaximumReadBufferBytes(IntPtr dataHandle);

        /// <summary>
        /// Sets the maximal size in bytes of the read buffer, 0 means unlimited.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="value">The new value in bytes, 0 means unlimited.</param>
        [DllImport(DllName, EntryPoint = "h5ScalarDataSetMaximumReadBufferBytes", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void H5ScalarDataSetMaximumReadBufferBytes(IntPtr dataHandle, ulong value);

        /// <summary>
        /// Spreads the duplicate time ticks making one-tick increments.
        /// </summary>
        /// <param name="items">An array of <paramref name="itemCount"/> data items.</param>
        /// <param name="itemCount">The number of items in the <paramref name="items"/> array.</param>
        /// <param name="verbose">Trace information messages.</param>
        /// <returns>A boolean indicating if the time ticks were spread.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataSpreadDuplicateTimeTicks", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        // public static extern bool H5ScalarDataSpreadDuplicateTimeTicks([In, Out] Scalar[] items, ulong itemCount, [MarshalAs(UnmanagedType.U1)] bool verbose);
        public static extern bool H5ScalarDataSpreadDuplicateTimeTicks(IntPtr items, ulong itemCount, [MarshalAs(UnmanagedType.U1)] bool verbose);

        /// <summary>
        /// Adds an array of items to the dataset.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="items">An array of <paramref name="itemCount"/> data items.</param>
        /// <param name="itemCount">The number of items in the <paramref name="items"/> array.</param>
        /// <param name="duplicateTimeTicks">The duplicate time tick policy.</param>
        /// <param name="verbose">Trace information messages.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataAdd", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataAdd(IntPtr dataHandle, [In] Scalar[] items, ulong itemCount, DuplicateTimeTicks duplicateTimeTicks, [MarshalAs(UnmanagedType.U1)] bool verbose);

        /// <summary>
        /// Fetches all stored data items.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="appender">Callback function to append data items.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataFetchAll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataFetchAll(IntPtr dataHandle, ScalarData.Appender appender);

        /// <summary>
        /// Fetches a specified range of stored data items.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="appender">Callback function to append data items.</param>
        /// <param name="ticksFrom">A starting date and time in the range.</param>
        /// <param name="ticksTo">An ending date and time in the range.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataFetchTickRange", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataFetchTickRange(IntPtr dataHandle, ScalarData.Appender appender, long ticksFrom, long ticksTo);

        /// <summary>
        /// Fetches a specified range of stored data items.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="appender">Callback function to append data items.</param>
        /// <param name="index">The index of the first item in the range.</param>
        /// <param name="count">The number of items in the range.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataFetchIndexRange", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataFetchIndexRange(IntPtr dataHandle, ScalarData.Appender appender, ulong index, ulong count);

        /// <summary>
        /// Converts ticks to a nearest index.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="ticks">The ticks to convert.</param>
        /// <param name="index">The index of the ticks.</param>
        /// <param name="match">If the index matches the ticks exactly or is a lower neighbor.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataTicksIndex", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataTicksIndex(IntPtr dataHandle, long ticks, out ulong index, out bool match);

        /// <summary>
        /// Converts a ticks range to an index range.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="ticksFrom">The first ticks of the range..</param>
        /// <param name="ticksTo">The last ticks of the range..</param>
        /// <param name="indexFrom">The index of the first item in the range.</param>
        /// <param name="indexTo">The index of the last item in the range.</param>
        /// <param name="matchFrom">If the index of the first item in the range matches the ticks exactly or is a lower neighbor.</param>
        /// <param name="matchTo">If the index of the last item in the range matches the ticks exactly or is a lower neighbor.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataTicksIndexRange", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataTicksIndexRange(IntPtr dataHandle, long ticksFrom, long ticksTo, out ulong indexFrom, out ulong indexTo, out bool matchFrom, out bool matchTo);

        /// <summary>
        /// Deletes a range of indices from the dataset.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="indexFrom">The first index of the range (inclusive).</param>
        /// <param name="indexTo">The last index of the range (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataDeleteIndexRange", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataDeleteIndexRange(IntPtr dataHandle, ulong indexFrom, ulong indexTo);

        /// <summary>
        /// Deletes a range of ticks from the dataset.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="ticksFrom">The first ticks of the range (inclusive).</param>
        /// <param name="ticksTo">The last ticks of the range (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataDeleteTickRange", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataDeleteTickRange(IntPtr dataHandle, long ticksFrom, long ticksTo);

        /// <summary>
        /// Deletes all ticks before a specified index from the dataset.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="index">The index (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataDeleteAllBeforeIndex", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataDeleteAllBeforeIndex(IntPtr dataHandle, ulong index);

        /// <summary>
        /// Deletes all ticks before a specified ticks from the dataset.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="ticks">The ticks (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataDeleteAllBeforeTick", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataDeleteAllBeforeTick(IntPtr dataHandle, long ticks);

        /// <summary>
        /// Deletes all ticks after a specified index from the dataset.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="index">The index (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataDeleteAllAfterIndex", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataDeleteAllAfterIndex(IntPtr dataHandle, ulong index);

        /// <summary>
        /// Deletes all ticks after a specified ticks from the dataset.
        /// </summary>
        /// <param name="dataHandle">The data handle.</param>
        /// <param name="ticks">The ticks (inclusive).</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5ScalarDataDeleteAllAfterTick", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5ScalarDataDeleteAllAfterTick(IntPtr dataHandle, long ticks);
    }
}
