using System;
using System.Runtime.InteropServices;

namespace Mbcs.H5
{
    /// <summary>
    /// Callback function to log h5 messages.
    /// </summary>
    /// <param name="message">The text of the message.</param>
    /// <param name="isError">If the message is an error.</param>
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ErrorLogger([In, MarshalAs(UnmanagedType.LPStr)] string message, [MarshalAs(UnmanagedType.U1)] bool isError);

    /// <summary>
    /// Callback function to enumerate datasets.
    /// </summary>
    /// <param name="dataType">The data type.</param>
    /// <param name="dataKind">The data kind.</param>
    /// <param name="ohlcvKind">The ohlcv kind.</param>
    /// <param name="scalarKind">The scalar kind.</param>
    /// <param name="tradeKind">The trade kind.</param>
    /// <param name="quoteKind">The quote kind.</param>
    /// <param name="timeFrame">The time frame.</param>
    /// <param name="isValidName">If the name is valid and was successfully parsed.</param>
    /// <param name="name">The dataset name.</param>
    /// <param name="path">The dataset path.</param>
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void DatasetEnumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind,
        TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, [MarshalAs(UnmanagedType.U1)] bool isValidName,
        [In, MarshalAs(UnmanagedType.LPStr)] string name, [In, MarshalAs(UnmanagedType.LPStr)] string path);

    /// <summary>
    /// Callback function to add child data info to the group info.
    /// </summary>
    /// <param name="groupInfoHandle">The parent group info handle.</param>
    /// <param name="dataType">The data type.</param>
    /// <param name="dataKind">The data kind.</param>
    /// <param name="ohlcvKind">The ohlcv kind.</param>
    /// <param name="scalarKind">The scalar kind.</param>
    /// <param name="tradeKind">The trade kind.</param>
    /// <param name="quoteKind">The quote kind.</param>
    /// <param name="timeFrame">The time frame.</param>
    /// <param name="isValidName">If the name is valid and was successfully parsed.</param>
    /// <param name="name">The dataset name.</param>
    /// <param name="path">The dataset path.</param>
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    internal delegate void GroupInfoAddDataInfo(IntPtr groupInfoHandle, DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind,
        TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, [MarshalAs(UnmanagedType.U1)] bool isValidName,
        [MarshalAs(UnmanagedType.LPStr)] string name, [MarshalAs(UnmanagedType.LPStr)] string path);

    /// <summary>
    /// Callback function to add child group info to the group info.
    /// </summary>
    /// <param name="groupInfoHandle">The parent group info handle.</param>
    /// <param name="name">The dataset name.</param>
    /// <param name="path">The dataset path.</param>
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    internal delegate IntPtr GroupInfoAddGroupInfo(IntPtr groupInfoHandle, [MarshalAs(UnmanagedType.LPStr)] string name, [MarshalAs(UnmanagedType.LPStr)] string path);
}
