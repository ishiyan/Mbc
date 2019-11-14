using System.Runtime.InteropServices;

namespace Mbcs.H5
{
    /// <summary>
    /// The quote struct.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Quote
    {
        /// <summary>
        /// The DateTime.Ticks -- the number of 100 nanoseconds since 12:00 A.M., January, 1, 0001.
        /// </summary>
        public long Ticks { get; set; }

        /// <summary>
        /// The ask price.
        /// </summary>
        public double AskPrice { get; set; }

        /// <summary>
        /// The bid price.
        /// </summary>
        public double BidPrice { get; set; }

        /// <summary>
        /// The ask size.
        /// </summary>
        public double AskSize { get; set; }

        /// <summary>
        /// The bid size.
        /// </summary>
        public double BidSize { get; set; }

        /// <summary>
        /// Returns the string that represents this object.
        /// </summary>
        /// <returns>Returns the string that represents this object.</returns>
        public override string ToString() => $"t:{Ticks}, ap:{AskPrice}, bp:{BidPrice}, as:{AskSize}, bs:{BidSize}";
    }
}
