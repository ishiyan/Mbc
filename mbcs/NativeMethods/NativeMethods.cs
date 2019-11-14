namespace Mbcs.NativeMethods
{
    internal static partial class NativeMethods
    {
#if DEBUG
        private const string DllName = "mbcd"; // Do not use platform-specific extensions like .dll, .so, etc.
#else // RELEASE
        private const string DllName = "mbc"; // Do not use platform-specific extensions like .dll, .so, etc.
#endif
    }
}
