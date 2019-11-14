using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Windows.Input;
using DataVirtualization;
using System.Text;
using System.ComponentModel;
using System.Threading;

using Mbh5;

namespace Mbhv
{
    /// <summary>
    /// Interaction logic for Contents.xaml
    /// </summary>
    // ReSharper disable once RedundantExtendsListEntry
    public sealed partial class Contents : Window, IDisposable
    {
        #region Variables
        private List<ContentsViewModel> treeList;
        private List<ContentsViewModel> flatList;
        private GroupInfo rootGroupInfo;
        private ContentsViewModel selectedItem;
        private Repository repository;
        private FileInfo repositoryFileInfo;
        private Instrument instrument;
        private object dataset;
        private DataTimeFrame timeFrame;
        private Step step = Step.Day;
        private readonly int pageSize = Properties.Settings.Default.VirtualizingCollectionPageSize;
        private readonly int timePageInMemory = Properties.Settings.Default.VirtualizingCollectionTicksPageInMemory;
        private readonly BackgroundWorker backgroundWorker = new BackgroundWorker();
        private readonly ProgressObject progressObject = new ProgressObject();
        private ResourceDictionary defaultSkin, farSkin, glassSkin;
        private string skin = "Default";

        #endregion

        #region Lists
        private readonly List<string> tradeHeaderList = new List<string>
        {
            "date/time ← ", "time ticks ", "price ", "volume "
        };
        private readonly List<string> tradePriceOnlyHeaderList = new List<string>
        {
            "date/time ← ", "time ticks ", "price "
        };
        private readonly List<string> quoteHeaderList = new List<string>
        {
            "date/time ← ", "time ticks ", "ask ", "bid ", "ask size ", "bid size "
        };
        private readonly List<string> quotePriceOnlyHeaderList = new List<string>
        {
            "date/time ← ", "time ticks ", "ask ", "bid "
        };
        private readonly List<string> ohlcvHeaderList = new List<string>
        {
            "date/time ← ", "time ticks ", "open ", "high ", "low ", "close ", "volume "
        };
        private readonly List<string> ohlcvPriceOnlyHeaderList = new List<string>
        {
            "date/time ← ", "time ticks ", "open ", "high ", "low ", "close "
        };
        private readonly List<string> scalarHeaderList = new List<string>
        {
            "date/time ← ", "time ticks ", "value "
        };
        private readonly List<string> tradeBindingPropertyList = new List<string>
        {
            "Data.DateTimeStampWithFractionOfSecond", "Data.Ticks", "Data.Price", "Data.Volume"
        };
        private readonly List<string> tradePriceOnlyBindingPropertyList = new List<string>
        {
            "Data.DateTimeStampWithFractionOfSecond", "Data.Ticks", "Data.Price"
        };
        private readonly List<string> quoteBindingPropertyList = new List<string>
        {
            "Data.DateTimeStampWithFractionOfSecond", "Data.Ticks", "Data.AskPrice", "Data.BidPrice", "Data.AskSize", "Data.BidSize"
        };
        private readonly List<string> quotePriceOnlyBindingPropertyList = new List<string>
        {
            "Data.DateTimeStampWithFractionOfSecond", "Data.Ticks", "Data.AskPrice", "Data.BidPrice"
        };
        private readonly List<string> ohlcvBindingPropertyList = new List<string>
        {
            "Data.DateTimeStampWithFractionOfSecond", "Data.Ticks", "Data.Open", "Data.High", "Data.Low", "Data.Close", "Data.Volume"
        };
        private readonly List<string> ohlcvPriceOnlyBindingPropertyList = new List<string>
        {
            "Data.DateTimeStampWithFractionOfSecond", "Data.Ticks", "Data.Open", "Data.High", "Data.Low", "Data.Close"
        };
        private readonly List<string> scalarBindingPropertyList = new List<string>
        {
            "Data.DateTimeStampWithFractionOfSecond", "Data.Ticks", "Data.Value"
        };
        #endregion

        #region Step
        private enum Step
        {
            Year, Month, Week, Day, Hour, Minute, Second
        }
        #endregion

        #region CsvExportArg
        private class CsvExportArg
        {
            internal readonly ProgressObject Progress;
            internal readonly string CsvFileName, TimeFormat, HeaderLine;
            internal readonly DataType ParsedDataType;

            internal CsvExportArg(ProgressObject progress, string csvFileName, string timeFormat, string headerLine, DataType parsedDataType)
            {
                Progress = progress;
                CsvFileName = csvFileName;
                TimeFormat = timeFormat;
                HeaderLine = headerLine;
                ParsedDataType = parsedDataType;
            }
        }
        #endregion

        #region Overrides
        // Apply "aero" effect to the entire window.
        // ReSharper disable once RedundantOverriddenMember
        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            // This doesn't work on Windows 10 anymore.
            // We use WindowComposition.EnableBlur/DisableBlur to enable/disable the Windows 10 acrylic blur.
            // Dwm.ExtendGlassFrame(this, new Thickness(-1));
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            CloseRepository();
        }
        #endregion

        #region Construction
        public Contents()
        {
            InitializeComponent();
            BeginInit();
            progressBar.DataContext = progressObject;
            gotoRememberSelectedDateCheckBox.IsChecked = Properties.Settings.Default.GotoRememberDate;
            csvHeaderLineCheckBox.IsChecked = Properties.Settings.Default.CsvHeaderLineChecked;
            csvTimeFormatTextBox.Text = Properties.Settings.Default.CsvTimeFormat;

            Data.DefaultMaximumReadBufferBytes = Properties.Settings.Default.Hdf5MaxReadBufferBytes;
            Repository.InterceptErrorStack();
            if (!Properties.Settings.Default.TraceLogVisible)
                traceLogRowDefinition.MaxHeight = 0;
            string newSkin = Properties.Settings.Default.Skin;
            defaultSkin = Resources;
            if (!string.IsNullOrEmpty(newSkin))
            {
                if (newSkin[0] == 'F')
                    ApplyFarSkin();
                else if (newSkin[0] == 'G')
                    ApplyGlassSkin();
            }
            else
                FontSize = Properties.Settings.Default.FontSize;
            string[] args = Environment.GetCommandLineArgs();
            if (Properties.Settings.Default.ContentsTreeView)
                ContentsViewShowTree();
            if (args.Length > 1)
                LoadFile(args[1]);
            EndInit();
        }
        #endregion

        #region Implementation

        private void CloseDataset()
        {
            if (dataset == null)
                return;

            if (dataset is OhlcvData ohlcvData)
            {
                Debug.WriteLine("closing current ohlcv data");
                ohlcvData.Close();
            }
            else if (dataset is OhlcvPriceOnlyData ohlcvPriceOnlyData)
            {
                Debug.WriteLine("closing current ohlcv price only data");
                ohlcvPriceOnlyData.Close();
            }
            else if (dataset is TradeData tradeData)
            {
                Debug.WriteLine("closing current trade data");
                tradeData.Close();
            }
            else if (dataset is TradePriceOnlyData tradePriceOnlyData)
            {
                Debug.WriteLine("closing current price only trade data");
                tradePriceOnlyData.Close();
            }
            else if (dataset is ScalarData scalarData)
            {
                Debug.WriteLine("closing current scalar data");
                scalarData.Close();
            }
            else if (dataset is QuoteData quoteData)
            {
                Debug.WriteLine("closing current quote data");
                quoteData.Close();
            }
            else if (dataset is QuotePriceOnlyData quotePriceOnlyData)
            {
                Debug.WriteLine("closing current price only quote data");
                quotePriceOnlyData.Close();
            }
            else
            {
                Debug.Assert(true, "Unknown dataset type");
            }
            dataset = null;
        }

        private void CloseInstrument()
        {
            CloseDataset();
            if (instrument == null)
                return;
            Debug.WriteLine("closing current instrument");
            instrument.Close();
            instrument = null;
        }

        private void CloseRepository()
        {
            CloseDataset();
            CloseInstrument();
            if (null != repository)
            {
                repository.Close();
                repository = null;
            }
            repositoryFileInfo = null;
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1031:DoNotCatchGeneralExceptionTypes")]
        private void ApplySkin(string name, ref ResourceDictionary dict)
        {
            if (null == dict)
            {
                string path = string.Concat(".\\Resources\\Skins\\", name, ".xaml");
                var uri = new Uri(path, UriKind.Relative);
                try
                {
                    dict = Application.LoadComponent(uri) as ResourceDictionary;
                }
                catch
                {
                    dict = null;
                }
                if (null != dict)
                    Resources = dict;
            }
            else
                Resources = dict;
            skin = name;
            FontSize = Properties.Settings.Default.FontSize;
        }

        private void LoadFile(string path)
        {
            treeList?.Clear();
            flatList?.Clear();
            DataGridClear();
            CloseRepository();
            if (File.Exists(path))
            {
                var fileInfo = new FileInfo(path);
                Title = fileInfo.FullName;
                rootGroupInfo = Repository.ContentTree(fileInfo.FullName, Properties.Settings.Default.SortContents);
                repository = Repository.OpenReadOnly(fileInfo.FullName);
                repositoryFileInfo = fileInfo;
                ContentsViewModel.Populate(rootGroupInfo, out treeList, out flatList);
                if (0 == flatList.Count)
                    ContentsViewShowTree();
                contentsListBox.ItemsSource = flatList;
                contentsTreeView.ItemsSource = treeList;
                int count = flatList.Count;
                if (0 < count)
                {
                    // Select the first dataset with a successfully parsed name.
                    for (int i = 0; i < count; ++i)
                    {
                        ContentsViewModel cvm = flatList[i];
                        if (cvm.IsParsed)
                        {
                            if (IsContentsViewList())
                            {
                                contentsListBox.SelectedIndex = i;
                                contentsListBox.ScrollIntoView(cvm);
                            }
                            else
                            {
                                treeList[0].ExpandSupertree(cvm);
                                cvm.IsSelected = true;
                            }
                            break;
                        }
                    }
                }
            }
            else
            {
                Title = "Mbhv";
            }
        }

        private void ContentsViewShowTree()
        {
            Debug.Assert(contentsViewToggle.IsChecked != null, "contentsViewToggle.IsChecked != null");
            if (contentsViewToggle.IsChecked != null && contentsViewToggle.IsChecked.Value)
                contentsViewToggle.IsChecked = false;
        }

        private bool IsContentsViewList()
        {
            Debug.Assert(contentsViewToggle.IsChecked != null, "contentsViewToggle.IsChecked != null");
            return contentsViewToggle.IsChecked != null && contentsViewToggle.IsChecked.Value;
        }

        private bool IsContentsViewTree()
        {
            Debug.Assert(contentsViewToggle.IsChecked != null, "contentsViewToggle.IsChecked != null");
            return contentsViewToggle.IsChecked != null && !contentsViewToggle.IsChecked.Value;
        }

        private void DataGridClear()
        {
            if (null == selectedItem)
                return;
            Debug.WriteLine("clearing dataGrid");
            // int count = dataGrid.Columns.Count - 1;
            // for (int i = 0; i < count; ++i)
            //     dataGrid.Columns.RemoveAt(count - i);
            dataGrid.ItemsSource = null;
            selectedItem = null;
        }

        private void UpdateDataGridColumns(List<string> columnHeaderList, List<string> columnBindingPropertyList)
        {
            dataGrid.ItemsSource = null;
            int countOld = dataGrid.Columns.Count;
            int countNew = columnHeaderList.Count + 1;
            int delta = countNew - countOld;
            if (0 < delta)
            {
                for (int i = 0; i < delta; ++i)
                {
                    var column = new DataGridTextColumn();
                    // column.CanUserResize = true;
                    // column.CanUserReorder = false;
                    // column.CanUserSort = false;
                    dataGrid.Columns.Add(column);
                }
            }
            else if (0 > delta)
            {
                delta = -delta;
                for (int i = 0; i < delta; ++i)
                    dataGrid.Columns.RemoveAt(countOld - 1 - i);
            }
            for (int i = 1, j = 0; i < countNew; ++i, ++j)
            {
                dataGrid.Columns[i].Header = columnHeaderList[j];
                ((DataGridTextColumn)(dataGrid.Columns[i])).Binding = new Binding(columnBindingPropertyList[j]);
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Globalization", "CA1303:Do not pass literals as localized parameters")]
        private long UpdateVisualInfo(Data data)
        {
            long count = repositoryFileInfo.Length;
            fileSizeTextBlock.Text = string.Concat(count.ToString(CultureInfo.InvariantCulture), " bytes");
            count = data.Count;
            sampleCountTextBlock.Text = string.Concat("# ", count.ToString(CultureInfo.InvariantCulture));
            timeRangeTextBlock.Text = string.Concat(data.DateTimeStampFirst, " ÷ ", data.DateTimeStampLast);
            AdjustStepVisibility();
            return count;
        }

        private void DataGridLoad()
        {
            if (null == selectedItem || null == repository)
                return;
            Debug.WriteLine("loading dataGrid");
            DataInfo dataInfo = selectedItem.DataInfo;
            CloseInstrument();
            instrument = repository.Open(dataInfo.Parent.Path);
            if (null == instrument)
                return;
            switch (dataInfo.ParsedDataType)
            {
                case DataType.Trade:
                    {
                        UpdateDataGridColumns(tradeHeaderList, tradeBindingPropertyList);
                        CloseDataset();
                        TradeData data = instrument.OpenTrade();
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<Trade>(new TradeDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.TradePriceOnly:
                    {
                        UpdateDataGridColumns(tradePriceOnlyHeaderList, tradePriceOnlyBindingPropertyList);
                        CloseDataset();
                        TradePriceOnlyData data = instrument.OpenTradePriceOnly();
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<TradePriceOnly>(new TradePriceOnlyDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.Quote:
                    {
                        UpdateDataGridColumns(quoteHeaderList, quoteBindingPropertyList);
                        CloseDataset();
                        QuoteData data = instrument.OpenQuote();
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<Quote>(new QuoteDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.QuotePriceOnly:
                    {
                        UpdateDataGridColumns(quotePriceOnlyHeaderList, quotePriceOnlyBindingPropertyList);
                        CloseDataset();
                        QuotePriceOnlyData data = instrument.OpenQuotePriceOnly();
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<QuotePriceOnly>(new QuotePriceOnlyDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.Ohlcv:
                case DataType.OhlcvAdjusted:
                    {
                        UpdateDataGridColumns(ohlcvHeaderList, ohlcvBindingPropertyList);
                        CloseDataset();
                        OhlcvData data = dataInfo.ParsedDataType == DataType.Ohlcv ?
                            instrument.OpenOhlcv(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame) :
                            instrument.OpenOhlcvAdjusted(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame);
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<Ohlcv>(new OhlcvDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.OhlcvPriceOnly:
                case DataType.OhlcvAdjustedPriceOnly:
                    {
                        UpdateDataGridColumns(ohlcvPriceOnlyHeaderList, ohlcvPriceOnlyBindingPropertyList);
                        CloseDataset();
                        OhlcvPriceOnlyData data = dataInfo.ParsedDataType == DataType.OhlcvPriceOnly ?
                            instrument.OpenOhlcvPriceOnly(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame) :
                            instrument.OpenOhlcvAdjustedPriceOnly(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame);
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<OhlcvPriceOnly>(new OhlcvPriceOnlyDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.Scalar:
                    {
                        UpdateDataGridColumns(scalarHeaderList, scalarBindingPropertyList);
                        CloseDataset();
                        ScalarData data = instrument.OpenScalar(dataInfo.ParsedScalarKind, dataInfo.ParsedTimeFrame);
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<Scalar>(new ScalarDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
            }
            dataGrid.SelectedIndex = 0;
        }

        private void ContentsSelectionChanged(ContentsViewModel itemOld, ContentsViewModel itemNew, bool isTreeView)
        {
            if (selectedItem == itemNew)
                return;
            Debug.WriteLine("contents selection: old={0}, new={1}", (null == itemOld ? "null" : itemOld.Path), (null == itemNew ? "null" : itemNew.Path));
            if (null == itemNew || !itemNew.IsDataSet || !itemNew.IsParsed)
            {
                sampleCountTextBlock.Text = string.Empty;
                timeRangeTextBlock.Text = string.Empty;
                if (null != selectedItem)
                    DataGridClear();
                if (isTreeView)
                {
                    if (null != itemNew && itemNew.IsDataSet)
                    {
                        contentsListBox.SelectedItem = itemNew;
                        contentsListBox.ScrollIntoView(itemNew);
                    }
                    else
                    {
                        contentsListBox.ScrollIntoView(flatList[0]);
                        contentsListBox.SelectedItem = null;
                    }
                }
                else
                {
                    if (null != itemOld)
                        itemOld.IsSelected = false;
                    if (null != itemNew)
                    {
                        itemNew.IsSelected = true;
                        treeList[0].ExpandSupertree(itemNew);
                    }
                }
                return;
            }
            if (isTreeView)
            {
                contentsListBox.SelectedItem = itemNew;
                contentsListBox.ScrollIntoView(itemNew);
            }
            else
            {
                if (null != itemOld)
                    itemOld.IsSelected = false;
                itemNew.IsSelected = true;
                treeList[0].ExpandSupertree(itemNew);
            }
            DataInfo dataInfo = itemNew.DataInfo;
            timeFrame = dataInfo.ParsedTimeFrame;
            Debug.WriteLine("contents selection: instrument=[{0}], dataset=[{1}]", dataInfo.Parent.Path, dataInfo.Name);
            selectedItem = itemNew;
            DataGridLoad();
        }

        private void GotoTo(int index)
        {
            dataGrid.SelectedIndex = index;
            ItemCollection items = dataGrid.Items;
            items.MoveCurrentToPosition(index);
            object item = items.CurrentItem;
            dataGrid.ScrollIntoView(item);
        }

        private long SelectedTicks
        {
            get
            {
                long ticks;
                switch (selectedItem.DataInfo.ParsedDataType)
                {
                    case DataType.Quote:
                        {
                            Quote item = ((DataWrapper<Quote>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.dateTimeTicks;
                        }
                        break;
                    case DataType.QuotePriceOnly:
                        {
                            QuotePriceOnly item = ((DataWrapper<QuotePriceOnly>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.dateTimeTicks;
                        }
                        break;
                    case DataType.Trade:
                        {
                            Trade item = ((DataWrapper<Trade>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.dateTimeTicks;
                        }
                        break;
                    case DataType.TradePriceOnly:
                        {
                            TradePriceOnly item = ((DataWrapper<TradePriceOnly>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.dateTimeTicks;
                        }
                        break;
                    case DataType.Ohlcv:
                    case DataType.OhlcvAdjusted:
                        {
                            Ohlcv item = ((DataWrapper<Ohlcv>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.dateTimeTicks;
                        }
                        break;
                    case DataType.OhlcvPriceOnly:
                    case DataType.OhlcvAdjustedPriceOnly:
                        {
                            OhlcvPriceOnly item = ((DataWrapper<OhlcvPriceOnly>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.dateTimeTicks;
                        }
                        break;
                    case DataType.Scalar:
                        {
                            Scalar item = ((DataWrapper<Scalar>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.dateTimeTicks;
                        }
                        break;
                    default:
                        ticks = 0L;
                        break;
                }
                return ticks;
            }
        }

        private static long StepTicks(Step step, int amount, long ticks)
        {
            if (0L == ticks && 0 >= amount)
                return ticks;
            var dateTime = new DateTime(ticks);
            switch (step)
            {
                case Step.Year:
                    dateTime = new DateTime(dateTime.Year, 1, 1, 0, 0, 0);
                    dateTime = dateTime.AddYears(amount);
                    break;
                case Step.Month:
                    dateTime = new DateTime(dateTime.Year, dateTime.Month, 1, 0, 0, 0);
                    dateTime = dateTime.AddMonths(amount);
                    break;
                case Step.Week:
                    dateTime = new DateTime(dateTime.Year, dateTime.Month, dateTime.Day, 0, 0, 0);
                    dateTime = dateTime.AddDays(amount * 7);
                    break;
                case Step.Day:
                    dateTime = new DateTime(dateTime.Year, dateTime.Month, dateTime.Day, 0, 0, 0);
                    dateTime = dateTime.AddDays(amount);
                    break;
                case Step.Hour:
                    dateTime = new DateTime(dateTime.Year, dateTime.Month, dateTime.Day, dateTime.Hour, 0, 0);
                    dateTime = dateTime.AddHours(amount);
                    break;
                case Step.Minute:
                    dateTime = new DateTime(dateTime.Year, dateTime.Month, dateTime.Day, dateTime.Hour, dateTime.Minute, 0);
                    dateTime = dateTime.AddMinutes(amount);
                    break;
                case Step.Second:
                    dateTime = new DateTime(dateTime.Year, dateTime.Month, dateTime.Day, dateTime.Hour, dateTime.Minute, dateTime.Second);
                    dateTime = dateTime.AddSeconds(amount);
                    break;
            }
            return dateTime.Ticks;
        }

        private void AdjustStepVisibility()
        {
            // ReSharper disable RedundantCheckBeforeAssignment
            switch (timeFrame)
            {
                case DataTimeFrame.Aperiodic:
                    if (Visibility.Visible != stepYear.Visibility)
                        stepYear.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepMonth.Visibility)
                        stepMonth.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepWeek.Visibility)
                        stepWeek.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepDay.Visibility)
                        stepDay.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepHour.Visibility)
                        stepHour.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepMinute.Visibility)
                        stepMinute.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepSecond.Visibility)
                        stepSecond.Visibility = Visibility.Visible;
                    break;
                case DataTimeFrame.Year1:
                    if (Step.Year != step)
                        stepYear.IsChecked = true;
                    if (Visibility.Collapsed != stepYear.Visibility)
                        stepYear.Visibility = Visibility.Collapsed;
                    hideMonthLabel:
                    if (Visibility.Collapsed != stepMonth.Visibility)
                        stepMonth.Visibility = Visibility.Collapsed;
                    hideWeekLabel:
                    if (Visibility.Collapsed != stepWeek.Visibility)
                        stepWeek.Visibility = Visibility.Collapsed;
                    hideDayLabel:
                    if (Visibility.Collapsed != stepDay.Visibility)
                        stepDay.Visibility = Visibility.Collapsed;
                    hideHourLabel:
                    if (Visibility.Collapsed != stepHour.Visibility)
                        stepHour.Visibility = Visibility.Collapsed;
                    hideMinuteLabel:
                    if (Visibility.Collapsed != stepMinute.Visibility)
                        stepMinute.Visibility = Visibility.Collapsed;
                    hideSecondLabel:
                    if (Visibility.Collapsed != stepSecond.Visibility)
                        stepSecond.Visibility = Visibility.Collapsed;
                    break;
                case DataTimeFrame.Month6:
                case DataTimeFrame.Month4:
                case DataTimeFrame.Month3:
                case DataTimeFrame.Month2:
                case DataTimeFrame.Month1:
                    if (Visibility.Visible != stepYear.Visibility)
                        stepYear.Visibility = Visibility.Visible;
                    if (Step.Year != step)
                        stepYear.IsChecked = true;
                    goto hideMonthLabel;
                case DataTimeFrame.Week3:
                case DataTimeFrame.Week2:
                case DataTimeFrame.Week1:
                    if (Visibility.Visible != stepYear.Visibility)
                        stepYear.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepMonth.Visibility)
                        stepMonth.Visibility = Visibility.Visible;
                    if (Step.Year != step && Step.Month != step)
                        stepMonth.IsChecked = true;
                    goto hideWeekLabel;
                case DataTimeFrame.Day1:
                    if (Visibility.Visible != stepYear.Visibility)
                        stepYear.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepMonth.Visibility)
                        stepMonth.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepWeek.Visibility)
                        stepWeek.Visibility = Visibility.Visible;
                    if (Step.Year != step && Step.Month != step && Step.Week != step)
                        stepWeek.IsChecked = true;
                    goto hideDayLabel;
                case DataTimeFrame.Hour12:
                case DataTimeFrame.Hour8:
                case DataTimeFrame.Hour6:
                case DataTimeFrame.Hour4:
                case DataTimeFrame.Hour3:
                case DataTimeFrame.Hour2:
                case DataTimeFrame.Hour1:
                    if (Visibility.Visible != stepYear.Visibility)
                        stepYear.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepMonth.Visibility)
                        stepMonth.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepWeek.Visibility)
                        stepWeek.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepDay.Visibility)
                        stepDay.Visibility = Visibility.Visible;
                    if (Step.Hour == step || Step.Minute == step || Step.Second == step)
                        stepDay.IsChecked = true;
                    goto hideHourLabel;
                case DataTimeFrame.Minute30:
                case DataTimeFrame.Minute20:
                case DataTimeFrame.Minute15:
                case DataTimeFrame.Minute12:
                case DataTimeFrame.Minute10:
                case DataTimeFrame.Minute6:
                case DataTimeFrame.Minute5:
                case DataTimeFrame.Minute4:
                case DataTimeFrame.Minute3:
                case DataTimeFrame.Minute2:
                case DataTimeFrame.Minute1:
                    if (Visibility.Visible != stepYear.Visibility)
                        stepYear.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepMonth.Visibility)
                        stepMonth.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepWeek.Visibility)
                        stepWeek.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepDay.Visibility)
                        stepDay.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepHour.Visibility)
                        stepHour.Visibility = Visibility.Visible;
                    if (Step.Minute == step || Step.Second == step)
                        stepHour.IsChecked = true;
                    goto hideMinuteLabel;
                case DataTimeFrame.Second30:
                case DataTimeFrame.Second20:
                case DataTimeFrame.Second15:
                case DataTimeFrame.Second12:
                case DataTimeFrame.Second10:
                case DataTimeFrame.Second6:
                case DataTimeFrame.Second5:
                case DataTimeFrame.Second4:
                case DataTimeFrame.Second3:
                case DataTimeFrame.Second2:
                case DataTimeFrame.Second1:
                    if (Visibility.Visible != stepYear.Visibility)
                        stepYear.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepMonth.Visibility)
                        stepMonth.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepWeek.Visibility)
                        stepWeek.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepDay.Visibility)
                        stepDay.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepHour.Visibility)
                        stepHour.Visibility = Visibility.Visible;
                    if (Visibility.Visible != stepMinute.Visibility)
                        stepMinute.Visibility = Visibility.Visible;
                    if (Step.Second == step)
                        stepMinute.IsChecked = true;
                    goto hideSecondLabel;
            }
            // ReSharper restore RedundantCheckBeforeAssignment
        }

        private void DoStep(bool backwards)
        {
            var d = dataset as Data;
            if (null == d)
            {
                Trace.TraceError("DoStep: dataset is null");
                return;
            }
            long t = StepTicks(step, backwards ? -1 : 1, SelectedTicks);
            d.TicksIndex(t, out long i, out bool b);
            if (!b)
            {
                if (!backwards)
                    ++i;
                if (i >= dataGrid.Items.Count)
                    i = dataGrid.Items.Count - 1;
                else if (i < 0)
                    i = 0;
            }
            GotoTo((int)i);
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1031:DoNotCatchGeneralExceptionTypes")]
        private void CsvExportProgress(string csvFileName, string timeFormat, string headerLine, DataType parsedDataType)
        {
            var arg = new CsvExportArg(progressObject, csvFileName, timeFormat, headerLine, parsedDataType);
            long page = Data.DefaultMaximumReadBufferBytes, count = ((Data)dataset).Count;
            progressObject.ProgressValue = 0;
            progressBar.Maximum = count;
            backgroundWorker.WorkerReportsProgress = true;
            backgroundWorker.WorkerSupportsCancellation = true;
            backgroundWorker.DoWork += (s, e) =>
            {
                char delimiter = Properties.Settings.Default.CsvDelimiterChar;
                var sb = new StringBuilder();
                var a = (CsvExportArg)e.Argument;
                ProgressObject progress = a.Progress;
                var fileInfo = new FileInfo(a.CsvFileName);
                using (StreamWriter streamWriter = fileInfo.CreateText())
                {
                    try
                    {
                        if (null != a.HeaderLine)
                            streamWriter.WriteLine(a.HeaderLine);
                        string fmt = a.TimeFormat;
                        switch (a.ParsedDataType)
                        {
                            case DataType.Quote:
                            {
                                page /= Quote.SizeOf;
                                var list = new List<Quote>((int)page);
                                var data = (QuoteData)dataset;
                                for (long i = 0; i < count; i += page)
                                {
                                    if (backgroundWorker.CancellationPending)
                                        break;
                                    progress.ProgressValue = i;
                                    list.Clear();
                                    data.Fetch(list, i, page);
                                    foreach (var v in list)
                                    {
                                        sb.Clear();
                                        sb.Append(new DateTime(v.dateTimeTicks).ToString(fmt, CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.askPrice.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.bidPrice.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.askSize.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.bidSize.ToString(CultureInfo.InvariantCulture));
                                        streamWriter.WriteLine(sb.ToString());
                                    }
                                }
                                break;
                            }
                            case DataType.QuotePriceOnly:
                            {
                                page /= QuotePriceOnly.SizeOf;
                                var list = new List<QuotePriceOnly>((int)page);
                                var data = (QuotePriceOnlyData)dataset;
                                for (long i = 0; i < count; i += page)
                                {
                                    if (backgroundWorker.CancellationPending)
                                        break;
                                    progress.ProgressValue = i;
                                    list.Clear();
                                    data.Fetch(list, i, page);
                                    foreach (var v in list)
                                    {
                                        sb.Clear();
                                        sb.Append(new DateTime(v.dateTimeTicks).ToString(fmt, CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.askPrice.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.bidPrice.ToString(CultureInfo.InvariantCulture));
                                        streamWriter.WriteLine(sb.ToString());
                                    }
                                }
                                break;
                            }
                            case DataType.Trade:
                            {
                                page /= Trade.SizeOf;
                                var list = new List<Trade>((int)page);
                                var data = (TradeData)dataset;
                                for (long i = 0; i < count; i += page)
                                {
                                    if (backgroundWorker.CancellationPending)
                                        break;
                                    progress.ProgressValue = i;
                                    list.Clear();
                                    data.Fetch(list, i, page);
                                    foreach (var v in list)
                                    {
                                        sb.Clear();
                                        sb.Append(new DateTime(v.dateTimeTicks).ToString(fmt, CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.price.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.volume.ToString(CultureInfo.InvariantCulture));
                                        streamWriter.WriteLine(sb.ToString());
                                    }
                                }
                                break;
                            }
                            case DataType.TradePriceOnly:
                            {
                                page /= TradePriceOnly.SizeOf;
                                var list = new List<TradePriceOnly>((int)page);
                                var data = (TradePriceOnlyData)dataset;
                                for (long i = 0; i < count; i += page)
                                {
                                    if (backgroundWorker.CancellationPending)
                                        break;
                                    progress.ProgressValue = i;
                                    list.Clear();
                                    data.Fetch(list, i, page);
                                    foreach (var v in list)
                                    {
                                        sb.Clear();
                                        sb.Append(new DateTime(v.dateTimeTicks).ToString(fmt, CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.price.ToString(CultureInfo.InvariantCulture));
                                        streamWriter.WriteLine(sb.ToString());
                                    }
                                }
                                break;
                            }
                            case DataType.Ohlcv:
                            case DataType.OhlcvAdjusted:
                            {
                                page /= Ohlcv.SizeOf;
                                var list = new List<Ohlcv>((int)page);
                                var data = (OhlcvData)dataset;
                                for (long i = 0; i < count; i += page)
                                {
                                    if (backgroundWorker.CancellationPending)
                                        break;
                                    progress.ProgressValue = i;
                                    list.Clear();
                                    data.Fetch(list, i, page);
                                    foreach (var v in list)
                                    {
                                        sb.Clear();
                                        sb.Append(new DateTime(v.dateTimeTicks).ToString(fmt, CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.open.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.high.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.low.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.close.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.volume.ToString(CultureInfo.InvariantCulture));
                                        streamWriter.WriteLine(sb.ToString());
                                    }
                                }
                                break;
                            }
                            case DataType.OhlcvPriceOnly:
                            case DataType.OhlcvAdjustedPriceOnly:
                            {
                                page /= OhlcvPriceOnly.SizeOf;
                                var list = new List<OhlcvPriceOnly>((int)page);
                                var data = (OhlcvPriceOnlyData)dataset;
                                for (long i = 0; i < count; i += page)
                                {
                                    if (backgroundWorker.CancellationPending)
                                        break;
                                    progress.ProgressValue = i;
                                    list.Clear();
                                    data.Fetch(list, i, page);
                                    foreach (var v in list)
                                    {
                                        sb.Clear();
                                        sb.Append(new DateTime(v.dateTimeTicks).ToString(fmt, CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.open.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.high.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.low.ToString(CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.close.ToString(CultureInfo.InvariantCulture));
                                        streamWriter.WriteLine(sb.ToString());
                                    }
                                }
                                break;
                            }
                            case DataType.Scalar:
                            {
                                page /= Scalar.SizeOf;
                                var list = new List<Scalar>((int)page);
                                var data = (ScalarData)dataset;
                                for (long i = 0; i < count; i += page)
                                {
                                    if (backgroundWorker.CancellationPending)
                                        break;
                                    progress.ProgressValue = i;
                                    list.Clear();
                                    data.Fetch(list, i, page);
                                    foreach (var v in list)
                                    {
                                        sb.Clear();
                                        sb.Append(new DateTime(v.dateTimeTicks).ToString(fmt, CultureInfo.InvariantCulture));
                                        sb.Append(delimiter);
                                        sb.Append(v.value.ToString(CultureInfo.InvariantCulture));
                                        streamWriter.WriteLine(sb.ToString());
                                    }
                                }
                                break;
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        Trace.TraceError(ex.Message);
                    }
                    finally
                    {
                        Dispatcher?.Invoke((ThreadStart) delegate
                        {
                            progress.ProgressValue = count;
                            progressGrid.Visibility = Visibility.Collapsed;
                        });
                    }
                }
            };
            progressGrid.Visibility = Visibility.Visible;
            backgroundWorker.RunWorkerAsync(arg);
        }
        #endregion

        #region Control callbacks and event handlers
        private void ContentsViewToggleChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            if (null != contentsTreeView && contentsTreeView.Visibility != Visibility.Collapsed)
                contentsTreeView.Visibility = Visibility.Collapsed;
            if (null != contentsListBox && contentsListBox.Visibility != Visibility.Visible)
                contentsListBox.Visibility = Visibility.Visible;
        }

        private void ContentsViewToggleUnchecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            if (null != contentsTreeView && contentsTreeView.Visibility != Visibility.Visible)
                contentsTreeView.Visibility = Visibility.Visible;
            if (null != contentsListBox && contentsListBox.Visibility != Visibility.Collapsed)
                contentsListBox.Visibility = Visibility.Collapsed;
        }

        private void ContentsTreeViewSelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            e.Handled = true;
            if (IsContentsViewTree())
                ContentsSelectionChanged(e.OldValue as ContentsViewModel, e.NewValue as ContentsViewModel, true);
        }

        private void ContentsListBoxSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            e.Handled = true;
            if (IsContentsViewList())
            {
                ContentsViewModel itemOld = (e.RemovedItems.Count > 0) ? e.RemovedItems[0] as ContentsViewModel : null;
                ContentsViewModel itemNew = (e.AddedItems.Count > 0) ? e.AddedItems[0] as ContentsViewModel : null;
                ContentsSelectionChanged(itemOld, itemNew, false);
            }
        }

        private void WindowKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Escape)
            {
                e.Handled = true;
                if (Visibility.Visible == progressGrid.Visibility)
                    return;
                if (Visibility.Visible == gotoGrid.Visibility)
                    gotoGrid.Visibility = Visibility.Collapsed;
                else if (Visibility.Visible == exportCsvGrid.Visibility)
                    exportCsvGrid.Visibility = Visibility.Collapsed;
                else
                    Close();
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Globalization", "CA1308:NormalizeStringsToUppercase")]
        private void WindowDragOver(object sender, DragEventArgs e)
        {
            e.Handled = true;
            bool dropEnabled = true;
            if (e.Data.GetDataPresent(DataFormats.FileDrop, true))
            {
                var fileNames = (string[])(e.Data.GetData(DataFormats.FileDrop, true));
                if (fileNames == null || fileNames.Length < 1)
                    return;
                string s = Path.GetExtension(fileNames[0]);
                if (s != null)
                {
                    string extension = s.ToLowerInvariant();
                    Debug.WriteLine(string.Format(CultureInfo.InvariantCulture, "drag over: ext [{0}], filename [{1}]", extension, fileNames[0]));
                    if (extension != ".h5" && extension != ".hdf5")
                        dropEnabled = false;
                }
            }
            else
                dropEnabled = false;
            if (!dropEnabled)
            {
                Debug.WriteLine("drag over: rejecting");
                e.Effects = DragDropEffects.None;
            }
        }

        private void WindowDrop(object sender, DragEventArgs e)
        {
            e.Handled = true;
            var fileNames = (string[])(e.Data.GetData(DataFormats.FileDrop, true));
            if (fileNames == null || fileNames.Length < 1)
                return;
            Debug.WriteLine(string.Format(CultureInfo.InvariantCulture, "drop: filename [{0}]", fileNames[0]));
            LoadFile(fileNames[0]);
        }

        private void StepYearChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Year;
        }

        private void StepMonthChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Month;
        }

        private void StepWeekChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Week;
        }

        private void StepDayChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Day;
        }

        private void StepHourChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Hour;
        }

        private void StepMinuteChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Minute;
        }

        private void StepSecondChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Second;
        }

        private void StepBackClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            DoStep(true);
        }

        private void StepForthClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            DoStep(false);
        }

        private void GotoLightBoxClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            Debug.Assert(gotoRememberSelectedDateCheckBox.IsChecked != null, "gotoRememberSelectedDateCheckBox.IsChecked != null");
            if (gotoRememberSelectedDateCheckBox.IsChecked != null && !gotoRememberSelectedDateCheckBox.IsChecked.Value)
            {
                var d = new DateTime(SelectedTicks);
                datePicker.DisplayDate = d;
                datePicker.SelectedDate = d;
            }
            gotoGrid.Visibility = Visibility.Visible;
        }

        private void GotoGridOkButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            if (null != datePicker.SelectedDate)
            {
                var d = (Data)dataset;
                d.TicksIndex(datePicker.SelectedDate.Value.Ticks, out long i, out bool b);
                if (!b)
                {
                    ++i;
                    if (i >= dataGrid.Items.Count)
                        --i;
                }
                gotoGrid.Visibility = Visibility.Collapsed;
                GotoTo((int)i);
            }
            else
                gotoGrid.Visibility = Visibility.Collapsed;
        }

        private void GotoGridCancelButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            gotoGrid.Visibility = Visibility.Collapsed;
        }

        private void ExportCsvLightBoxClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            char delimiter = Properties.Settings.Default.CsvDelimiterChar;
            var sb = new StringBuilder();
            sb.Append("date/time");
            sb.Append(delimiter);
            switch (selectedItem.DataInfo.ParsedDataType)
            {
                case DataType.Quote:
                    sb.AppendFormat(CultureInfo.InvariantCulture, "ask{0}bid{1}ask size{2}bid size", delimiter, delimiter, delimiter);
                    break;
                case DataType.QuotePriceOnly:
                    sb.AppendFormat(CultureInfo.InvariantCulture, "ask{0}bid", delimiter);
                    break;
                case DataType.Trade:
                    sb.AppendFormat(CultureInfo.InvariantCulture, "price{0}volume", delimiter);
                    break;
                case DataType.TradePriceOnly:
                    sb.Append("price");
                    break;
                case DataType.Ohlcv:
                case DataType.OhlcvAdjusted:
                    sb.AppendFormat(CultureInfo.InvariantCulture, "open{0}high{1}low{2}close{3}volume", delimiter, delimiter, delimiter, delimiter);
                    break;
                case DataType.OhlcvPriceOnly:
                case DataType.OhlcvAdjustedPriceOnly:
                    sb.AppendFormat(CultureInfo.InvariantCulture, "open{0}high{1}low{2}close", delimiter, delimiter, delimiter);
                    break;
                // case DataType.Scalar:
                default:
                    sb.Append("value");
                    break;
            }
            csvHeaderLineTextBox.Text = sb.ToString();
            sb.Clear();
            if (Properties.Settings.Default.CsvFileNamePrependRepositoyName)
            {
                sb.Append(repositoryFileInfo.Name);
                sb.Append(Properties.Settings.Default.CsvFiileNameSeparatorAfterRepositoryName);
            }
            sb.Append(selectedItem.DataInfo.Path.Replace('/', Properties.Settings.Default.CsvFileNamePathSeparatorChar).Substring(1));
            sb.Append(".csv");
            csvFileTextBox.Text = sb.ToString();
            var fileInfo = new FileInfo(csvFileTextBox.Text);
            exportCsvGridOkButton.Content = fileInfo.Exists ? "export (overwrite existing file)" : "export";
            // var blur = new System.Windows.Media.Effects.BlurEffect();
            // blur.Radius = 3;
            // mainGrid.Effect = blur;
            exportCsvGrid.Visibility = Visibility.Visible;
        }

        private void CsvFileTextBoxTextChanged(object sender, TextChangedEventArgs e)
        {
            e.Handled = true;
            var fileInfo = new FileInfo(csvFileTextBox.Text);
            exportCsvGridOkButton.Content = fileInfo.Exists ? "export (overwrite existing file)" : "export";
        }

        private void ExportCsvGridOkButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            exportCsvGrid.Visibility = Visibility.Collapsed;
            // mainGrid.Effect = null;
            Debug.Assert(csvHeaderLineCheckBox.IsChecked != null, "csvHeaderLineCheckBox.IsChecked != null");
            CsvExportProgress(csvFileTextBox.Text, csvTimeFormatTextBox.Text, csvHeaderLineCheckBox.IsChecked != null && csvHeaderLineCheckBox.IsChecked.Value ? csvHeaderLineTextBox.Text : null, selectedItem.DataInfo.ParsedDataType);
        }

        private void ExportCsvGridCancelButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            exportCsvGrid.Visibility = Visibility.Collapsed;
            // mainGrid.Effect = null;
        }

        private void ProgressGridCancelButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            backgroundWorker.CancelAsync();
        }

        private void ApplyDefaultSkin()
        {
            traceTextBoxControl.Suppressed = true;
            ApplySkin("Default", ref defaultSkin);
            WindowComposition.DisableBlur(this);
            traceTextBoxControl.Suppressed = false;
        }

        private void ApplyFarSkin()
        {
            traceTextBoxControl.Suppressed = true;
            ApplySkin("Far", ref farSkin);
            WindowComposition.DisableBlur(this);
            traceTextBoxControl.Suppressed = false;
        }

        private void ApplyGlassSkin()
        {
            traceTextBoxControl.Suppressed = true;
            ApplySkin("Glass", ref glassSkin);
            WindowComposition.EnableBlur(this, 0xa0, 0x000000);
            traceTextBoxControl.Suppressed = false;
        }

        private void SkinButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            if (skin[0] == 'D')
                ApplyGlassSkin();
            else if (skin[0] == 'G')
                ApplyFarSkin();
            else // if (skin[0] == 'F')
                ApplyDefaultSkin();
        }
        #endregion

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2213:DisposableFieldsShouldBeDisposed", MessageId = "backgroundWorker")]
        private void Dispose(bool disposing)
        {
            if (disposing)
            {
                instrument?.Dispose();
                repository?.Dispose();
                backgroundWorker?.Dispose();
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        ~Contents()
        {
            Dispose(false);
        }
    }
}