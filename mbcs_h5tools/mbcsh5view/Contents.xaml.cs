using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Windows.Input;
using System.Text;
using System.ComponentModel;
using System.Threading;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Configuration;

using DataVirtualization;
using Mbcs.H5;

namespace Mbcsh5view
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
        private ContentsViewModel selectedItem;
        private Mbcs.H5.File file;
        private FileInfo fileInfo;
        private Instrument instrument;
        private object dataset;
        private DataTimeFrame timeFrame;
        private Step step = Step.Day;
        private readonly int pageSize;
        private readonly int timePageInMemory;
        private readonly BackgroundWorker backgroundWorker = new BackgroundWorker();
        private readonly ProgressObject progressObject = new ProgressObject();
        private ResourceDictionary defaultSkin, farSkin, glassSkin;
        private string skin = "Default";
        private readonly ILogger<Contents> logger;
        private readonly IConfiguration configuration;
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
            "Data.Ticks", "Data.Ticks", "Data.Price", "Data.Volume"
        };
        private readonly List<string> tradePriceOnlyBindingPropertyList = new List<string>
        {
            "Data.Ticks", "Data.Ticks", "Data.Price"
        };
        private readonly List<string> quoteBindingPropertyList = new List<string>
        {
            "Data.Ticks", "Data.Ticks", "Data.AskPrice", "Data.BidPrice", "Data.AskSize", "Data.BidSize"
        };
        private readonly List<string> quotePriceOnlyBindingPropertyList = new List<string>
        {
            "Data.Ticks", "Data.Ticks", "Data.AskPrice", "Data.BidPrice"
        };
        private readonly List<string> ohlcvBindingPropertyList = new List<string>
        {
            "Data.Ticks", "Data.Ticks", "Data.Open", "Data.High", "Data.Low", "Data.Close", "Data.Volume"
        };
        private readonly List<string> ohlcvPriceOnlyBindingPropertyList = new List<string>
        {
            "Data.Ticks", "Data.Ticks", "Data.Open", "Data.High", "Data.Low", "Data.Close"
        };
        private readonly List<string> scalarBindingPropertyList = new List<string>
        {
            "Data.Ticks", "Data.Ticks", "Data.Value"
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
        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            CloseFile();
        }
        #endregion

        #region Construction
        public Contents(ILogger<Contents> logger, IConfiguration configuration)
        {
            this.logger = logger;
            this.configuration = configuration;
            InitializeComponent();
            BeginInit();
            progressBar.DataContext = progressObject;
            gotoRememberSelectedDateCheckBox.IsChecked = configuration.GetSection("GotoRememberDate").Get<bool>();
            csvHeaderLineCheckBox.IsChecked = configuration.GetSection("CsvHeaderLineChecked").Get<bool>();
            csvTimeFormatTextBox.Text = configuration.GetSection("CsvTimeFormat").Get<string>();
            pageSize = configuration.GetSection("VirtualizingCollectionPageSize").Get<int>();
            timePageInMemory = configuration.GetSection("VirtualizingCollectionTicksPageInMemory").Get<int>();
            Data.DefaultMaximumReadBufferBytes = configuration.GetSection("Hdf5MaxReadBufferBytes").Get<ulong>();
            Mbcs.H5.File.InterceptErrorStack((message, isError) =>
            {
                if (isError)
                    logger.LogError(message);
                else
                    logger.LogInformation(message);
            });
            string newSkin = configuration.GetSection("Skin").Get<string>();
            defaultSkin = Resources;
            if (!string.IsNullOrEmpty(newSkin))
            {
                if (newSkin[0] == 'F')
                    ApplyFarSkin();
                else if (newSkin[0] == 'G')
                    ApplyGlassSkin();
            }
            else
                FontSize = configuration.GetSection("FontSize").Get<int>();
            string[] args = Environment.GetCommandLineArgs();
            if (configuration.GetSection("ContentsTreeView").Get<bool>())
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
                logger.LogDebug("closing current ohlcv data");
                ohlcvData.Close();
            }
            else if (dataset is OhlcvPriceOnlyData ohlcvPriceOnlyData)
            {
                logger.LogDebug("closing current ohlcv price only data");
                ohlcvPriceOnlyData.Close();
            }
            else if (dataset is TradeData tradeData)
            {
                logger.LogDebug("closing current trade data");
                tradeData.Close();
            }
            else if (dataset is TradePriceOnlyData tradePriceOnlyData)
            {
                logger.LogDebug("closing current price only trade data");
                tradePriceOnlyData.Close();
            }
            else if (dataset is ScalarData scalarData)
            {
                logger.LogDebug("closing current scalar data");
                scalarData.Close();
            }
            else if (dataset is QuoteData quoteData)
            {
                logger.LogDebug("closing current quote data");
                quoteData.Close();
            }
            else if (dataset is QuotePriceOnlyData quotePriceOnlyData)
            {
                logger.LogDebug("closing current price only quote data");
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
            logger.LogDebug("closing current instrument");
            instrument.Close();
            instrument = null;
        }

        private void CloseFile()
        {
            CloseDataset();
            CloseInstrument();
            if (null != file)
            {
                file.Close();
                file = null;
            }
            fileInfo = null;
        }

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
            FontSize = configuration.GetSection("FontSize").Get<int>();
        }

        private void LoadFile(string path)
        {
            treeList?.Clear();
            flatList?.Clear();
            DataGridClear();
            CloseFile();
            if (System.IO.File.Exists(path))
            {
                var info = new FileInfo(path);
                Title = info.FullName;
                bool sort = configuration.GetSection("SortContents").Get<bool>();
                using GroupInfo rootGroupInfo = Mbcs.H5.File.ContentTree(info.FullName, sort);
                file = Mbcs.H5.File.OpenReadOnly(info.FullName);
                fileInfo = info;
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
                                treeList[0].ExpandSuperTree(cvm);
                                cvm.IsSelected = true;
                            }
                            break;
                        }
                    }
                }
            }
            else
            {
                Title = "Mbcsh5view";
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
            logger.LogDebug("clearing dataGrid");
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

            // First column is always the date-time stamp.
            // Bind it using the date-time stamp converter. 
            dataGrid.Columns[1].Header = columnHeaderList[0];
            ((DataGridTextColumn)(dataGrid.Columns[1])).Binding = new Binding(columnBindingPropertyList[0]) { Converter = new DateTimeStampConverter() };

            // The rest of columns are bound to properties directly.
            for (int i = 2, j = 1; i < countNew; ++i, ++j)
            {
                dataGrid.Columns[i].Header = columnHeaderList[j];
                ((DataGridTextColumn)(dataGrid.Columns[i])).Binding = new Binding(columnBindingPropertyList[j]);
            }
        }

        private static ulong DataCount(Data d)
        {
            if (d is OhlcvData ohlcv) return ohlcv.Count;
            if (d is OhlcvPriceOnlyData ohlcvPriceOnly) return ohlcvPriceOnly.Count;
            if (d is TradeData trade) return trade.Count;
            if (d is TradePriceOnlyData tradePriceOnly) return tradePriceOnly.Count;
            if (d is ScalarData scalar) return scalar.Count;
            if (d is QuoteData quote) return quote.Count;
            if (d is QuotePriceOnlyData quotePriceOnly) return quotePriceOnly.Count;
            return 0L;
        }

        private static long DataFirstTicks(Data d)
        {
            if (d is OhlcvData ohlcv) return ohlcv.FirstTicks;
            if (d is OhlcvPriceOnlyData ohlcvPriceOnly) return ohlcvPriceOnly.FirstTicks;
            if (d is TradeData trade) return trade.FirstTicks;
            if (d is TradePriceOnlyData tradePriceOnly) return tradePriceOnly.FirstTicks;
            if (d is ScalarData scalar) return scalar.FirstTicks;
            if (d is QuoteData quote) return quote.FirstTicks;
            if (d is QuotePriceOnlyData quotePriceOnly) return quotePriceOnly.FirstTicks;
            return 0L;
        }

        private static long DataLastTicks(Data d)
        {
            if (d is OhlcvData ohlcv) return ohlcv.LastTicks;
            if (d is OhlcvPriceOnlyData ohlcvPriceOnly) return ohlcvPriceOnly.LastTicks;
            if (d is TradeData trade) return trade.LastTicks;
            if (d is TradePriceOnlyData tradePriceOnly) return tradePriceOnly.LastTicks;
            if (d is ScalarData scalar) return scalar.LastTicks;
            if (d is QuoteData quote) return quote.LastTicks;
            if (d is QuotePriceOnlyData quotePriceOnly) return quotePriceOnly.LastTicks;
            return 0L;
        }

        private long UpdateVisualInfo(Data data)
        {
            ulong count = (ulong)fileInfo.Length;
            fileSizeTextBlock.Text = string.Concat(count.ToString(CultureInfo.InvariantCulture), " bytes");
            count = DataCount(data);
            sampleCountTextBlock.Text = string.Concat("# ", count.ToString(CultureInfo.InvariantCulture));
            timeRangeTextBlock.Text = string.Concat(new DateTime(DataFirstTicks(data)).DateTimeStamp(), " ÷ ", new DateTime(DataLastTicks(data)).DateTimeStamp());
            AdjustStepVisibility();
            return (long)count;
        }

        private void DataGridLoad()
        {
            if (null == selectedItem || null == file)
                return;
            logger.LogDebug("loading dataGrid");
            DataInfo dataInfo = selectedItem.DataInfo;
            CloseInstrument();
            instrument = file.OpenInstrument(dataInfo.Parent.Path, false);
            if (null == instrument)
                return;
            switch (dataInfo.ParsedDataType)
            {
                case DataType.Trade:
                    {
                        UpdateDataGridColumns(tradeHeaderList, tradeBindingPropertyList);
                        CloseDataset();
                        TradeData data = instrument.OpenTrade(false);
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<Trade>(new TradeDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.TradePriceOnly:
                    {
                        UpdateDataGridColumns(tradePriceOnlyHeaderList, tradePriceOnlyBindingPropertyList);
                        CloseDataset();
                        TradePriceOnlyData data = instrument.OpenTradePriceOnly(false);
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<TradePriceOnly>(new TradePriceOnlyDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.Quote:
                    {
                        UpdateDataGridColumns(quoteHeaderList, quoteBindingPropertyList);
                        CloseDataset();
                        QuoteData data = instrument.OpenQuote(false);
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<Quote>(new QuoteDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.QuotePriceOnly:
                    {
                        UpdateDataGridColumns(quotePriceOnlyHeaderList, quotePriceOnlyBindingPropertyList);
                        CloseDataset();
                        QuotePriceOnlyData data = instrument.OpenQuotePriceOnly(false);
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
                            instrument.OpenOhlcv(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame, false) :
                            instrument.OpenOhlcvAdjusted(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame, false);
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
                            instrument.OpenOhlcvPriceOnly(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame, false) :
                            instrument.OpenOhlcvAdjustedPriceOnly(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame, false);
                        dataset = data;
                        var provider = new AsyncVirtualizingCollection<OhlcvPriceOnly>(new OhlcvPriceOnlyDataProvider(data, UpdateVisualInfo(data)), pageSize, timePageInMemory);
                        dataGrid.ItemsSource = provider;
                    }
                    break;
                case DataType.Scalar:
                    {
                        UpdateDataGridColumns(scalarHeaderList, scalarBindingPropertyList);
                        CloseDataset();
                        ScalarData data = instrument.OpenScalar(dataInfo.ParsedTimeFrame, false);
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
            logger.LogDebug($"contents selection: old={(null == itemOld ? "null" : itemOld.Path)}, new={(null == itemNew ? "null" : itemNew.Path)}");
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
                        treeList[0].ExpandSuperTree(itemNew);
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
                treeList[0].ExpandSuperTree(itemNew);
            }
            DataInfo dataInfo = itemNew.DataInfo;
            timeFrame = dataInfo.ParsedTimeFrame;
            logger.LogDebug($"contents selection: instrument=[{dataInfo.Parent.Path}], dataset=[{dataInfo.Name}]");
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
                            ticks = item.Ticks;
                        }
                        break;
                    case DataType.QuotePriceOnly:
                        {
                            QuotePriceOnly item = ((DataWrapper<QuotePriceOnly>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.Ticks;
                        }
                        break;
                    case DataType.Trade:
                        {
                            Trade item = ((DataWrapper<Trade>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.Ticks;
                        }
                        break;
                    case DataType.TradePriceOnly:
                        {
                            TradePriceOnly item = ((DataWrapper<TradePriceOnly>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.Ticks;
                        }
                        break;
                    case DataType.Ohlcv:
                    case DataType.OhlcvAdjusted:
                        {
                            Ohlcv item = ((DataWrapper<Ohlcv>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.Ticks;
                        }
                        break;
                    case DataType.OhlcvPriceOnly:
                    case DataType.OhlcvAdjustedPriceOnly:
                        {
                            OhlcvPriceOnly item = ((DataWrapper<OhlcvPriceOnly>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.Ticks;
                        }
                        break;
                    case DataType.Scalar:
                        {
                            Scalar item = ((DataWrapper<Scalar>)dataGrid.Items.CurrentItem).Data;
                            ticks = item.Ticks;
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

        private static void DataTicksIndex(Data d, long t, out ulong i, out bool b)
        {
            if (d is OhlcvData ohlcv) ohlcv.TicksIndex(t, out i, out b);
            else if (d is OhlcvPriceOnlyData ohlcvPriceOnly) ohlcvPriceOnly.TicksIndex(t, out i, out b);
            else if (d is TradeData trade) trade.TicksIndex(t, out i, out b);
            else if (d is TradePriceOnlyData tradePriceOnly) tradePriceOnly.TicksIndex(t, out i, out b);
            else if (d is ScalarData scalar) scalar.TicksIndex(t, out i, out b);
            else if (d is QuoteData quote) quote.TicksIndex(t, out i, out b);
            else if (d is QuotePriceOnlyData quotePriceOnly) quotePriceOnly.TicksIndex(t, out i, out b);
            else
            {
                i = 0;
                b = false;
            }
        }

        private void DoStep(bool backwards)
        {
            if (!(dataset is Data d))
            {
                logger.LogTrace("DoStep: dataset is null");
                return;
            }
            long t = StepTicks(step, backwards ? -1 : 1, SelectedTicks);
            DataTicksIndex(d, t, out ulong i, out bool b);
            if (!b)
            {
                if (!backwards)
                    ++i;
                if (i >= (ulong) dataGrid.Items.Count)
                    i = (ulong) dataGrid.Items.Count - 1UL;
                // else if (i < 0)
                //    i = 0;
            }
            GotoTo((int)i);
        }

        private void CsvExportProgress(string csvFileName, string timeFormat, string headerLine, DataType parsedDataType)
        {
            var arg = new CsvExportArg(progressObject, csvFileName, timeFormat, headerLine, parsedDataType);
            ulong page = Data.DefaultMaximumReadBufferBytes, count = DataCount((Data)dataset);
            progressObject.ProgressValue = 0;
            progressBar.Maximum = count;
            backgroundWorker.WorkerReportsProgress = true;
            backgroundWorker.WorkerSupportsCancellation = true;
            backgroundWorker.DoWork += (s, e) =>
            {
                char delimiter = configuration.GetSection("CsvDelimiterChar").Get<char>();
                var sb = new StringBuilder();
                var a = (CsvExportArg)e.Argument;
                ProgressObject progress = a.Progress;
                var info = new FileInfo(a.CsvFileName);
                using StreamWriter streamWriter = info.CreateText();
                try
                {
                    if (null != a.HeaderLine)
                        streamWriter.WriteLine(a.HeaderLine);
                    string fmt = a.TimeFormat;
                    switch (a.ParsedDataType)
                    {
                        case DataType.Quote:
                        {
                            page /= (ulong)System.Runtime.InteropServices.Marshal.SizeOf(typeof(Quote));
                            var list = new List<Quote>((int)page);
                            var data = (QuoteData)dataset;
                            for (ulong i = 0; i < count; i += page)
                            {
                                if (backgroundWorker.CancellationPending)
                                    break;
                                progress.ProgressValue = i;
                                list.Clear();
                                data.FetchIndexRange(list, i, page);
                                foreach (var v in list)
                                {
                                    sb.Clear();
                                    sb.Append(new DateTime(v.Ticks).ToString(fmt, CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.AskPrice.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.BidPrice.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.AskSize.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.BidSize.ToString(CultureInfo.InvariantCulture));
                                    streamWriter.WriteLine(sb.ToString());
                                }
                            }
                            break;
                        }
                        case DataType.QuotePriceOnly:
                        {
                            page /= (ulong)System.Runtime.InteropServices.Marshal.SizeOf(typeof(QuotePriceOnly));
                            var list = new List<QuotePriceOnly>((int)page);
                            var data = (QuotePriceOnlyData)dataset;
                            for (ulong i = 0; i < count; i += page)
                            {
                                if (backgroundWorker.CancellationPending)
                                    break;
                                progress.ProgressValue = i;
                                list.Clear();
                                data.FetchIndexRange(list, i, page);
                                foreach (var v in list)
                                {
                                    sb.Clear();
                                    sb.Append(new DateTime(v.Ticks).ToString(fmt, CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.AskPrice.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.BidPrice.ToString(CultureInfo.InvariantCulture));
                                    streamWriter.WriteLine(sb.ToString());
                                }
                            }
                            break;
                        }
                        case DataType.Trade:
                        {
                            page /= (ulong)System.Runtime.InteropServices.Marshal.SizeOf(typeof(Trade));
                            var list = new List<Trade>((int)page);
                            var data = (TradeData)dataset;
                            for (ulong i = 0; i < count; i += page)
                            {
                                if (backgroundWorker.CancellationPending)
                                    break;
                                progress.ProgressValue = i;
                                list.Clear();
                                data.FetchIndexRange(list, i, page);
                                foreach (var v in list)
                                {
                                    sb.Clear();
                                    sb.Append(new DateTime(v.Ticks).ToString(fmt, CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Price.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Volume.ToString(CultureInfo.InvariantCulture));
                                    streamWriter.WriteLine(sb.ToString());
                                }
                            }
                            break;
                        }
                        case DataType.TradePriceOnly:
                        {
                            page /= (ulong)System.Runtime.InteropServices.Marshal.SizeOf(typeof(TradePriceOnly));
                            var list = new List<TradePriceOnly>((int)page);
                            var data = (TradePriceOnlyData)dataset;
                            for (ulong i = 0; i < count; i += page)
                            {
                                if (backgroundWorker.CancellationPending)
                                    break;
                                progress.ProgressValue = i;
                                list.Clear();
                                data.FetchIndexRange(list, i, page);
                                foreach (var v in list)
                                {
                                    sb.Clear();
                                    sb.Append(new DateTime(v.Ticks).ToString(fmt, CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Price.ToString(CultureInfo.InvariantCulture));
                                    streamWriter.WriteLine(sb.ToString());
                                }
                            }
                            break;
                        }
                        case DataType.Ohlcv:
                        case DataType.OhlcvAdjusted:
                        {
                            page /= (ulong)System.Runtime.InteropServices.Marshal.SizeOf(typeof(Ohlcv));
                            var list = new List<Ohlcv>((int)page);
                            var data = (OhlcvData)dataset;
                            for (ulong i = 0; i < count; i += page)
                            {
                                if (backgroundWorker.CancellationPending)
                                    break;
                                progress.ProgressValue = i;
                                list.Clear();
                                data.FetchIndexRange(list, i, page);
                                foreach (var v in list)
                                {
                                    sb.Clear();
                                    sb.Append(new DateTime(v.Ticks).ToString(fmt, CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Open.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.High.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Low.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Close.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Volume.ToString(CultureInfo.InvariantCulture));
                                    streamWriter.WriteLine(sb.ToString());
                                }
                            }
                            break;
                        }
                        case DataType.OhlcvPriceOnly:
                        case DataType.OhlcvAdjustedPriceOnly:
                        {
                            page /= (ulong)System.Runtime.InteropServices.Marshal.SizeOf(typeof(OhlcvPriceOnly));
                            var list = new List<OhlcvPriceOnly>((int)page);
                            var data = (OhlcvPriceOnlyData)dataset;
                            for (ulong i = 0; i < count; i += page)
                            {
                                if (backgroundWorker.CancellationPending)
                                    break;
                                progress.ProgressValue = i;
                                list.Clear();
                                data.FetchIndexRange(list, i, page);
                                foreach (var v in list)
                                {
                                    sb.Clear();
                                    sb.Append(new DateTime(v.Ticks).ToString(fmt, CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Open.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.High.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Low.ToString(CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Close.ToString(CultureInfo.InvariantCulture));
                                    streamWriter.WriteLine(sb.ToString());
                                }
                            }
                            break;
                        }
                        case DataType.Scalar:
                        {
                            page /= (ulong)System.Runtime.InteropServices.Marshal.SizeOf(typeof(Scalar));
                            var list = new List<Scalar>((int)page);
                            var data = (ScalarData)dataset;
                            for (ulong i = 0; i < count; i += page)
                            {
                                if (backgroundWorker.CancellationPending)
                                    break;
                                progress.ProgressValue = i;
                                list.Clear();
                                data.FetchIndexRange(list, i, page);
                                foreach (var v in list)
                                {
                                    sb.Clear();
                                    sb.Append(new DateTime(v.Ticks).ToString(fmt, CultureInfo.InvariantCulture));
                                    sb.Append(delimiter);
                                    sb.Append(v.Value.ToString(CultureInfo.InvariantCulture));
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
            };
            progressGrid.Visibility = Visibility.Visible;
            backgroundWorker.RunWorkerAsync(arg);
        }
        #endregion

        #region Control callbacks and event handlers
        public void ContentsViewToggleChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            if (null != contentsTreeView && contentsTreeView.Visibility != Visibility.Collapsed)
                contentsTreeView.Visibility = Visibility.Collapsed;
            if (null != contentsListBox && contentsListBox.Visibility != Visibility.Visible)
                contentsListBox.Visibility = Visibility.Visible;
        }

        public void ContentsViewToggleUnchecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            if (null != contentsTreeView && contentsTreeView.Visibility != Visibility.Visible)
                contentsTreeView.Visibility = Visibility.Visible;
            if (null != contentsListBox && contentsListBox.Visibility != Visibility.Collapsed)
                contentsListBox.Visibility = Visibility.Collapsed;
        }

        public void ContentsTreeViewSelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            e.Handled = true;
            if (IsContentsViewTree())
                ContentsSelectionChanged(e.OldValue as ContentsViewModel, e.NewValue as ContentsViewModel, true);
        }

        public void ContentsListBoxSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            e.Handled = true;
            if (IsContentsViewList())
            {
                ContentsViewModel itemOld = (e.RemovedItems.Count > 0) ? e.RemovedItems[0] as ContentsViewModel : null;
                ContentsViewModel itemNew = (e.AddedItems.Count > 0) ? e.AddedItems[0] as ContentsViewModel : null;
                ContentsSelectionChanged(itemOld, itemNew, false);
            }
        }

        public void WindowKeyDown(object sender, KeyEventArgs e)
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

        public void WindowDragOver(object sender, DragEventArgs e)
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
                    logger.LogDebug(string.Format(CultureInfo.InvariantCulture, "drag over: ext [{0}], filename [{1}]", extension, fileNames[0]));
                    if (extension != ".h5" && extension != ".hdf5")
                        dropEnabled = false;
                }
            }
            else
                dropEnabled = false;
            if (!dropEnabled)
            {
                logger.LogDebug("drag over: rejecting");
                e.Effects = DragDropEffects.None;
            }
        }

        public void WindowDrop(object sender, DragEventArgs e)
        {
            e.Handled = true;
            var fileNames = (string[])(e.Data.GetData(DataFormats.FileDrop, true));
            if (fileNames == null || fileNames.Length < 1)
                return;
            logger.LogDebug(string.Format(CultureInfo.InvariantCulture, "drop: filename [{0}]", fileNames[0]));
            LoadFile(fileNames[0]);
        }

        public void StepYearChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Year;
        }

        public void StepMonthChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Month;
        }

        public void StepWeekChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Week;
        }

        public void StepDayChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Day;
        }

        public void StepHourChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Hour;
        }

        public void StepMinuteChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Minute;
        }

        public void StepSecondChecked(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            step = Step.Second;
        }

        public void StepBackClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            DoStep(true);
        }

        public void StepForthClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            DoStep(false);
        }

        public void GotoLightBoxClick(object sender, RoutedEventArgs e)
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

        public void GotoGridOkButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            if (null != datePicker.SelectedDate)
            {
                var d = (Data)dataset;
                DataTicksIndex(d, datePicker.SelectedDate.Value.Ticks, out ulong i, out bool b);
                if (!b)
                {
                    ++i;
                    if (i >= (ulong)dataGrid.Items.Count)
                        --i;
                }
                gotoGrid.Visibility = Visibility.Collapsed;
                GotoTo((int)i);
            }
            else
                gotoGrid.Visibility = Visibility.Collapsed;
        }

        public void GotoGridCancelButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            gotoGrid.Visibility = Visibility.Collapsed;
        }

        public void ExportCsvLightBoxClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            char delimiter = configuration.GetSection("CsvDelimiterChar").Get<char>();
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
            if (configuration.GetSection("CsvFileNamePrependFileName").Get<bool>())
            {
                sb.Append(this.fileInfo.Name);
                sb.Append(configuration.GetSection("CsvFileNameSeparatorAfterFileName").Get<char>());
            }
            sb.Append(selectedItem.DataInfo.Path.Replace('/', configuration.GetSection("CsvFileNamePathSeparatorChar").Get<char>())[1..]);
            sb.Append(".csv");
            csvFileTextBox.Text = sb.ToString();
            var info = new FileInfo(csvFileTextBox.Text);
            exportCsvGridOkButton.Content = info.Exists ? "export (overwrite existing file)" : "export";
            exportCsvGrid.Visibility = Visibility.Visible;
        }

        public void CsvFileTextBoxTextChanged(object sender, TextChangedEventArgs e)
        {
            e.Handled = true;
            var info = new FileInfo(csvFileTextBox.Text);
            exportCsvGridOkButton.Content = info.Exists ? "export (overwrite existing file)" : "export";
        }

        public void ExportCsvGridOkButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            exportCsvGrid.Visibility = Visibility.Collapsed;
            Debug.Assert(csvHeaderLineCheckBox.IsChecked != null, "csvHeaderLineCheckBox.IsChecked != null");
            CsvExportProgress(csvFileTextBox.Text, csvTimeFormatTextBox.Text, csvHeaderLineCheckBox.IsChecked != null && csvHeaderLineCheckBox.IsChecked.Value ? csvHeaderLineTextBox.Text : null, selectedItem.DataInfo.ParsedDataType);
        }

        public void ExportCsvGridCancelButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            exportCsvGrid.Visibility = Visibility.Collapsed;
        }

        public void ProgressGridCancelButtonClick(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            backgroundWorker.CancelAsync();
        }

        private void ApplyDefaultSkin()
        {
            ApplySkin("Default", ref defaultSkin);
            WindowComposition.DisableBlur(this);
        }

        private void ApplyFarSkin()
        {
            ApplySkin("Far", ref farSkin);
            WindowComposition.DisableBlur(this);
        }

        private void ApplyGlassSkin()
        {
            ApplySkin("Glass", ref glassSkin);
            WindowComposition.EnableBlur(this, 103, 0x000000);
        }

        public void SkinButtonClick(object sender, RoutedEventArgs e)
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

        private void Dispose(bool disposing)
        {
            if (disposing)
            {
                instrument?.Dispose();
                file?.Dispose();
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