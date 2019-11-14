using System.Runtime.InteropServices;

namespace Mbcs.H5
{
    /// <summary>
    /// The trade (price only) struct.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct TradePriceOnly
    {
        /// <summary>
        /// The DateTime.Ticks -- the number of 100 nanoseconds since 12:00 A.M., January, 1, 0001.
        /// </summary>
        public long Ticks { get; set; }

        /// <summary>
        /// The trade price.
        /// </summary>
        public double Price { get; set; }

        /// <summary>
        /// Returns the string that represents this object.
        /// </summary>
        /// <returns>Returns the string that represents this object.</returns>
        public override string ToString() => $"t:{Ticks}, p:{Price}";
    }
}
