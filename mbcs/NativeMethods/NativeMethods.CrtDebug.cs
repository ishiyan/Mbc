using System.Runtime.InteropServices;
using System.Security;

namespace Mbcs.NativeMethods
{
    internal static partial class NativeMethods
    {
        /// <summary>
        /// Clears the Windows-specific CRT debugging flags.
        /// You can call this in the assembly initialization of your unit tests when they are executed in bulk in debug mode.
        /// </summary>
        [DllImport(DllName, EntryPoint = "clearWindowsCrtDebugFlags", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void ClearWindowsCrtDebugFlags();

        /// <summary>
        /// Sets the partial Windows-specific CRT debugging flags.
        /// You can call this in the assembly initialization of your unit tests
        /// when they are executed in bulk to track the memory allocation of the native code in debug mode.
        /// The checking is relatively fast.
        /// </summary>
        [DllImport(DllName, EntryPoint = "clearWindowsCrtDebugFlags", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void SetWindowsCrtDebugFlags();

        /// <summary>
        /// Sets the full Windows-specific CRT debugging flags.
        /// You can call this to track the memory allocation of the native code in debug mode.
        /// It is useful when you execute only single unit test, but for bulk test execution it slows down performance to almost unacceptable level.
        /// </summary>
        [DllImport(DllName, EntryPoint = "setWindowsCrtDebugFlagsFull", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void SetWindowsCrtDebugFlagsFull();
    }
}
