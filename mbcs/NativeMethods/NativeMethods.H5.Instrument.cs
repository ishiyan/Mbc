using System;
using System.Runtime.InteropServices;
using System.Security;
using Mbcs.H5;

namespace Mbcs.NativeMethods
{
    internal static partial class NativeMethods
    {
        /// <summary>
        /// Flushes and closes an access to the instrument. The <see paramref="instrumentHandle"/> is not valid after this call.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        [DllImport(DllName, EntryPoint = "h5InstrumentClose", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void H5InstrumentClose(IntPtr instrumentHandle);

        /// <summary>
        /// Flushes the instrument.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        [DllImport(DllName, EntryPoint = "h5InstrumentFlush", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void H5InstrumentFlush(IntPtr instrumentHandle);

        /// <summary>
        /// Enumerates a flat non-recursive list of datasets of the instrument's group.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentEnumerateDatasets", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5InstrumentEnumerateDatasets(IntPtr instrumentHandle, DatasetEnumerator datasetEnumerator, [MarshalAs(UnmanagedType.U1)] bool sort);

        /// <summary>
        /// Opens an instrument's scalar data.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a data object or \c IntPtr.Zero in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentOpenScalarData", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5InstrumentOpenScalarData(IntPtr instrumentHandle, DataTimeFrame timeFrame, [MarshalAs(UnmanagedType.U1)] bool createNonexistent = false);

        /// <summary>
        /// Opens an instrument's trade data.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a data object or \c IntPtr.Zero in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentOpenTradeData", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5InstrumentOpenTradeData(IntPtr instrumentHandle, [MarshalAs(UnmanagedType.U1)] bool createNonexistent = false);

        /// <summary>
        /// Opens an instrument's trade price-only data.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a data object or \c IntPtr.Zero in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentOpenTradePriceOnlyData", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5InstrumentOpenTradePriceOnlyData(IntPtr instrumentHandle, [MarshalAs(UnmanagedType.U1)] bool createNonexistent = false);

        /// <summary>
        /// Opens an instrument's ohlcv data.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="kind">A kind of the data to open.</param>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a data object or \c IntPtr.Zero in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentOpenOhlcvData", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5InstrumentOpenOhlcvData(IntPtr instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, [MarshalAs(UnmanagedType.U1)] bool createNonexistent = false);

        /// <summary>
        /// Opens an instrument's ohlcv price-only data.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="kind">A kind of the data to open.</param>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a data object or \c IntPtr.Zero in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentOpenOhlcvPriceOnlyData", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5InstrumentOpenOhlcvPriceOnlyData(IntPtr instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, [MarshalAs(UnmanagedType.U1)] bool createNonexistent = false);

        /// <summary>
        /// Opens an adjusted instrument's ohlcv data.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="kind">A kind of the data to open.</param>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a data object or \c IntPtr.Zero in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentOpenOhlcvAdjustedData", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5InstrumentOpenOhlcvAdjustedData(IntPtr instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, [MarshalAs(UnmanagedType.U1)] bool createNonexistent = false);

        /// <summary>
        /// Opens an adjusted instrument's ohlcv price-only data.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="kind">A kind of the data to open.</param>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a data object or \c IntPtr.Zero in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentOpenOhlcvAdjustedPriceOnlyData", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5InstrumentOpenOhlcvAdjustedPriceOnlyData(IntPtr instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, [MarshalAs(UnmanagedType.U1)] bool createNonexistent = false);

        /// <summary>
        /// Opens an instrument's quote data.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a data object or \c IntPtr.Zero in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentOpenQuoteData", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5InstrumentOpenQuoteData(IntPtr instrumentHandle, [MarshalAs(UnmanagedType.U1)] bool createNonexistent = false);

        /// <summary>
        /// Opens an instrument's quote price-only data.
        /// </summary>
        /// <param name="instrumentHandle">The instrument handle.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a data object or \c IntPtr.Zero in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5InstrumentOpenQuotePriceOnlyData", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5InstrumentOpenQuotePriceOnlyData(IntPtr instrumentHandle, [MarshalAs(UnmanagedType.U1)] bool createNonexistent = false);
    }
}
