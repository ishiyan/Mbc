using System.Collections.Generic;
using System.Windows.Media;

// ReSharper disable UnusedMethodReturnValue.Global
// Used in XAML

namespace Mbcsh5view
{
    public static class ColorHelper
    {
        internal static readonly string[] MyColors =
        {
            "Black", "DimGray", "DarkSlateGray", "SlateGray", "Gray", "DarkGray", "Silver", "Gainsboro", "LightGray",
            "Navy", "DarkBlue", "MidnightBlue", "DarkSlateBlue", "MediumBlue", "Blue", "RoyalBlue",
            "DodgerBlue", "SlateBlue", "MediumSlateBlue", "SteelBlue", "LightSteelBlue", "CadetBlue", "CornflowerBlue",
            "DeepSkyBlue", "SkyBlue", "LightSkyBlue", "PowderBlue", "LightBlue", "DarkTurquoise", "Turquoise",
            "MediumTurquoise", "PaleTurquoise", "DarkCyan", "Aqua", "Aquamarine", "Cyan", "MediumAquamarine",
            "LightCyan", "Azure", "DarkGreen", "DarkOliveGreen", "Green", "ForestGreen", "SeaGreen", "Teal",
            "MediumSeaGreen", "LightSeaGreen", "MediumSpringGreen", "SpringGreen", "DarkSeaGreen", "OliveDrab",
            "Olive", "LimeGreen", "Lime", "LawnGreen", "Chartreuse", "GreenYellow", "YellowGreen", "PaleGreen",
            "LightGreen", "DarkGoldenrod", "SaddleBrown", "Sienna", "Brown", "Peru", "Chocolate", "Goldenrod",
            "DarkOrange", "Orange", "SandyBrown", "Gold", "Yellow", "Tan", "BurlyWood", "Wheat", "PapayaWhip", "PaleGoldenrod", "DarkKhaki",
            "Khaki", "LightYellow", "Maroon", "DarkRed", "Firebrick", "IndianRed", "Crimson", "Red", "Tomato", "Salmon",
            "OrangeRed", "DeepPink", "Indigo", "DarkViolet", "Purple", "DarkMagenta", "DarkOrchid", "BlueViolet",
            "MediumPurple", "MediumOrchid", "MediumVioletRed", "Magenta", "Fuchsia", "HotPink", "Orchid", "Plum",
            "Pink", "Violet", "PaleVioletRed", "LightCoral", "LightPink", "Coral", "DarkSalmon", "LightSalmon",
            "PeachPuff", "RosyBrown", "Thistle", "MistyRose", "Moccasin", "NavajoWhite", "Seashell", "Linen",
            "Lavender", "LavenderBlush", "Honeydew", "BlanchedAlmond", "Beige", "Bisque", "Cornsilk", "AntiqueWhite",
            "AliceBlue", "FloralWhite", "Ivory", "LemonChiffon", "LightGoldenrodYellow", "OldLace", "WhiteSmoke",
            "MintCream", "GhostWhite", "Snow", "White"
        };

        public static IEnumerable<string> GetColorNames()
        {
            return MyColors;
        }
    }

    public static class ColoredPenHelper
    {
        public static IEnumerable<Pen> GetColoredPens()
        {
            var brushConverter = new BrushConverter();
            // foreach (PropertyInfo p in typeof(Colors).GetProperties(BindingFlags.Public | BindingFlags.Static))
            foreach (var p in ColorHelper.MyColors)
            {
                // brush = (Brush)brushConverter.ConvertFromInvariantString(p.Name);
                var brush = (Brush)brushConverter.ConvertFromInvariantString(p);

                var pen = new Pen(brush, 1.0) {DashStyle = DashStyles.Solid};
                yield return pen;
                pen = new Pen(brush, 1.0) {DashStyle = DashStyles.Dot};
                yield return pen;
                pen = new Pen(brush, 1.0) {DashStyle = DashStyles.Dash};
                yield return pen;

                pen = new Pen(brush, 3.0) {DashStyle = DashStyles.Solid};
                yield return pen;
                pen = new Pen(brush, 3.0) {DashStyle = DashStyles.Dot};
                yield return pen;
                pen = new Pen(brush, 3.0) {DashStyle = DashStyles.Dash};
                yield return pen;
            }
        }
    }

    public static class FontStretchHelper
    {
        public static IEnumerable<string> GetFontStretchNames()
        {
            yield return "UltraCondensed";
            yield return "ExtraCondensed";
            yield return "Condensed";
            yield return "SemiCondensed";
            yield return "Normal";
            yield return "SemiExpanded";
            yield return "Expanded";
            yield return "ExtraExpanded";
            yield return "UltraExpanded";
        }
    }

    public static class FontStyleHelper
    {
        public static IEnumerable<string> GetFontStyleNames()
        {
            yield return "Normal";
            yield return "Italic";
            yield return "Oblique";
        }
    }

    public static class FontWeightHelper
    {
        public static IEnumerable<string> GetFontWeightNames()
        {
            yield return "Thin";
            yield return "ExtraLight";
            yield return "UltraLight";
            yield return "Light";
            yield return "Normal";
            yield return "Regular";
            yield return "Medium";
            yield return "DemiBold";
            yield return "SemiBold";
            yield return "Bold";
            yield return "ExtraBold";
            yield return "UltraBold";
            yield return "Black";
            yield return "Heavy";
            yield return "ExtraBlack";
            yield return "UltraBlack";
        }
    }
}
