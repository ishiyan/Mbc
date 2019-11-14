using System.Runtime.InteropServices;

namespace Mbcs.H5
{
    /// <summary>
    /// The ohlc structure.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct OhlcvPriceOnly
    {
        /// <summary>
        /// The DateTime.Ticks -- the number of 100 nanoseconds since 12:00 A.M., January, 1, 0001.
        /// </summary>
        public long Ticks { get; set; }

        /// <summary>
        /// The opening price.
        /// </summary>
        public double Open { get; set; }

        /// <summary>
        /// The highest price.
        /// </summary>
        public double High { get; set; }

        /// <summary>
        /// The lowest price.
        /// </summary>
        public double Low { get; set; }

        /// <summary>
        /// The closing price.
        /// </summary>
        public double Close { get; set; }

        /// <summary>
        /// Returns the string that represents this object.
        /// </summary>
        /// <returns>Returns the string that represents this object.</returns>
        public override string ToString() => $"t:{Ticks}, o:{Open}, h:{High}, l:{Low}, c:{Close}";
    }
}
