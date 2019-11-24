using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;

namespace Mbcsh5view
{
    internal static class NativeMethods
    {
        [DllImport("user32.dll")]
        public static extern int SetWindowCompositionAttribute(IntPtr hwnd, ref WindowComposition.WindowCompositionAttributeData data);
    }

    internal static class WindowComposition
    {
        public enum WindowCompositionAttribute
        {
            // ReSharper disable InconsistentNaming
            // ...
            WCA_ACCENT_POLICY = 19
            // ...
            // ReSharper restore InconsistentNaming
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct WindowCompositionAttributeData
        {
            public WindowCompositionAttribute Attribute;
            public IntPtr Data;
            public int SizeOfData;
        }

        private enum AccentState
        {
            // ReSharper disable InconsistentNaming
            // ReSharper disable IdentifierTypo
            // ReSharper disable UnusedMember.Local
            // ReSharper disable UnusedMember.Global
            ACCENT_DISABLED = 0,
            ACCENT_ENABLE_GRADIENT = 1,
            ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
            ACCENT_ENABLE_BLURBEHIND = 3,
            AACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
            ACCENT_ENABLE_HOSTBACKDROP = 5,
            ACCENT_INVALID_STATE = 6
            // ReSharper restore UnusedMember.Global
            // ReSharper restore UnusedMember.Local
            // ReSharper restore IdentifierTypo
            // ReSharper restore InconsistentNaming
        }

        [Flags]
        internal enum AccentFlag
        {
            // ...
            // ReSharper disable once UnusedMember.Global
            None = 0,
            DrawLeftBorder = 0x20,
            DrawTopBorder = 0x40,
            DrawRightBorder = 0x80,
            DrawBottomBorder = 0x100,
            DrawAllBorders = (DrawLeftBorder | DrawTopBorder | DrawRightBorder | DrawBottomBorder)
            // ...
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct AccentPolicy
        {
            public AccentState AccentState;
            public uint AccentFlags;
            public uint GradientColor;
            private readonly uint AnimationId;
        }

        /// <summary>
        /// Enables "aero" blur on Windows 10.
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="blurOpacity">Blur opacity, 0 to 255.</param>
        /// <param name="blurBackgroundColor">Blur background color BGR format (e.g., 0x990000).</param>
        internal static void EnableBlur(Window window, uint blurOpacity, uint blurBackgroundColor)
        {
            // 0.6 = 153, 0.7 = 179, 0.8 = 204, 0.9 = 230
            // blurOpacity = 103;
            // blurBackgroundColor = 0x000000;

            var windowHelper = new WindowInteropHelper(window);

            var accent = new AccentPolicy()
            {
                AccentState = AccentState.AACCENT_ENABLE_ACRYLICBLURBEHIND,
                AccentFlags = (uint)AccentFlag.DrawAllBorders,
                GradientColor = (blurOpacity << 24) | (blurBackgroundColor & 0xFFFFFF)
            };

            var accentStructSize = Marshal.SizeOf(accent);

            var accentPtr = Marshal.AllocHGlobal(accentStructSize);
            Marshal.StructureToPtr(accent, accentPtr, false);

            var data = new WindowCompositionAttributeData
            {
                Attribute = WindowCompositionAttribute.WCA_ACCENT_POLICY,
                SizeOfData = accentStructSize,
                Data = accentPtr
            };

            int result = NativeMethods.SetWindowCompositionAttribute(windowHelper.Handle, ref data);
            Trace.WriteLine($"NativeMethods.SetWindowCompositionAttribute (enable) returned {result}");

            Marshal.FreeHGlobal(accentPtr);
        }

        internal static void DisableBlur(Window window)
        {
            var windowHelper = new WindowInteropHelper(window);

            var accent = new AccentPolicy()
            {
                AccentState = AccentState.ACCENT_DISABLED
            };

            var accentStructSize = Marshal.SizeOf(accent);

            var accentPtr = Marshal.AllocHGlobal(accentStructSize);
            Marshal.StructureToPtr(accent, accentPtr, false);

            var data = new WindowCompositionAttributeData
            {
                Attribute = WindowCompositionAttribute.WCA_ACCENT_POLICY,
                SizeOfData = accentStructSize,
                Data = accentPtr
            };

            int result = NativeMethods.SetWindowCompositionAttribute(windowHelper.Handle, ref data);
            Trace.WriteLine($"NativeMethods.SetWindowCompositionAttribute (disable) returned {result}");

            Marshal.FreeHGlobal(accentPtr);
        }
    }
}
