using System.Runtime.InteropServices;

namespace Mbcs.H5
{
    /// <summary>
    /// Encapsulates a HDF5 dataset info.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public sealed class DataInfo : ObjectInfo
    {
        /// <summary>
        /// The data type. Valid if <see cref="IsValidName"/> property is true.
        /// </summary>
        public DataType ParsedDataType { get; internal set; }

        /// <summary>
        /// The data kind. Valid if <see cref="IsValidName"/> property is true.
        /// </summary>
        public DataKind ParsedDataKind { get; internal set; }

        /// <summary>
        /// The ohlcv kind. Valid if <see cref="IsValidName"/> property is true.
        /// </summary>
        public OhlcvKind ParsedOhlcvKind { get; internal set; }

        /// <summary>
        /// The scalar kind. Valid if <see cref="IsValidName"/> property is true.
        /// </summary>
        public ScalarKind ParsedScalarKind { get; internal set; }

        /// <summary>
        /// The trade kind. Valid if <see cref="IsValidName"/> property is true.
        /// </summary>
        public TradeKind ParsedTradeKind { get; internal set; }

        /// <summary>
        /// The quote kind. Valid if <see cref="IsValidName"/> property is true.
        /// </summary>
        public QuoteKind ParsedQuoteKind { get; internal set; }

        /// <summary>
        /// The data time frame. Valid if <see cref="IsValidName"/> property is true.
        /// </summary>
        public DataTimeFrame ParsedTimeFrame { get; internal set; }

        /// <summary>
        /// If the data name is a valid one.
        /// </summary>
        public bool IsValidName { get; internal set; }

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        /// <param name="name">The name of the dataset.</param>
        /// <param name="parent">The parent group info instance.</param>
        internal DataInfo(string name, GroupInfo parent)
            : base(name, parent)
        {
        }
    }
}
