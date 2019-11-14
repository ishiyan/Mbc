namespace Mbcs
{
    /// <summary>
    /// Msvc-specific crt debugging flags.
    /// </summary>
    public static class CrtDebug
    {
        /// <summary>
        /// Clears the Windows-specific CRT debugging flags.
        /// You can call this in the assembly initialization of your unit tests when they are executed in bulk in debug mode.
        /// </summary>
        public static void ClearWindowsCrtDebugFlags() => NativeMethods.NativeMethods.ClearWindowsCrtDebugFlags();

        /// <summary>
        /// Sets the partial Windows-specific CRT debugging flags.
        /// You can call this in the assembly initialization of your unit tests
        /// when they are executed in bulk to track the memory allocation of the native code in debug mode.
        /// The checking is relatively fast.
        /// </summary>
        public static void SetWindowsCrtDebugFlags() => NativeMethods.NativeMethods.SetWindowsCrtDebugFlags();

        /// <summary>
        /// Sets the full Windows-specific CRT debugging flags.
        /// You can call this to track the memory allocation of the native code in debug mode.
        /// It is useful when you execute only single unit test, but for bulk test execution it slows down performance to almost unacceptable level.
        /// </summary>
        public static void SetWindowsCrtDebugFlagsFull() => NativeMethods.NativeMethods.SetWindowsCrtDebugFlagsFull();
    }
}
