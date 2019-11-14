using System.Runtime.InteropServices;

namespace Mbcs.H5
{
    /// <summary>
    /// The scalar structure.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Scalar
    {
        /// <summary>
        /// The DateTime.Ticks -- the number of 100 nanoseconds since 12:00 A.M., January, 1, 0001.
        /// </summary>
        public long Ticks { get; set; }

        /// <summary>
        /// The value.
        /// </summary>
        public double Value { get; set; }

        /// <summary>
        /// Returns the string that represents this object.
        /// </summary>
        /// <returns>Returns the string that represents this object.</returns>
        public override string ToString() => $"t:{Ticks}, v:{Value}";
    }
}
