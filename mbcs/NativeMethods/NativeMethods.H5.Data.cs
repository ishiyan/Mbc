using System.Runtime.InteropServices;
using System.Security;

namespace Mbcs.NativeMethods
{
    internal static partial class NativeMethods
    {
        /// <summary>
        /// Gets the default maximal size in bytes of the read buffer, 0 means unlimited.
        /// </summary>
        /// <returns>The default maximal size in bytes of the read buffer.</returns>
        [DllImport(DllName, EntryPoint = "h5DataGetDefaultMaximumReadBufferBytes", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern ulong H5DataGetDefaultMaximumReadBufferBytes();

        /// <summary>
        /// Sets the default maximal size in bytes of the read buffer, 0 means unlimited.
        /// </summary>
        /// <param name="value">The new value in bytes, 0 means unlimited.</param>
        [DllImport(DllName, EntryPoint = "h5DataSetDefaultMaximumReadBufferBytes", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void H5DataSetDefaultMaximumReadBufferBytes(ulong value);
    }
}
