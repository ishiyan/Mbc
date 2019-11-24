using System;
using System.Windows.Data;

using Mbcs.H5;

namespace Mbcsh5view
{
    public class DateTimeStampConverter: IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (value != null)
            {
                long ticks = (long) value;
                return new DateTime(ticks).DateTimeStampWithFractionOfSecond();
            }

            return "???";
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotSupportedException();
        }
    }
}
