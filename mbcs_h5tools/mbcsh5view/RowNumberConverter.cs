using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Controls;

namespace Mbcsh5view
{
    [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Performance", "CA1812:AvoidUninstantiatedInternalClasses")]
    class RowNumberConverter : IMultiValueConverter
   {
      public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
      {
          if (values == null)
              return "?";

          // Get the grid and the item.
          object item = values[0];
          var grid = values[1] as DataGrid;
          if (null == grid)
              System.Diagnostics.Trace.TraceError("RowNumberConverter: parent grid undefined");
          return null == grid ? "?" : grid.Items.IndexOf(item).ToString(culture);
      }

      public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
      {
            throw new NotImplementedException();
      }
   }
}
