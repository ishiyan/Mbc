using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using static Mbcs.NativeMethods.NativeMethods;

namespace Mbcs.H5
{
    /// <summary>
    /// Encapsulates the access to an instrument.
    /// </summary>
    public sealed class Instrument : IDisposable
    {
        private HandleRef instrumentHandle;

        /// <summary>
        /// The path to the h5 file.
        /// </summary>
        public string FilePath { get; }

        /// <summary>
        /// The path to the instrument within the h5 file.
        /// </summary>
        public string InstrumentPath { get; }

        /// <summary>
        /// If the instrument is read-only.
        /// </summary>
        public bool IsReadOnly { get; }

        internal Instrument(File file, string instrumentPath, IntPtr instrumentHandle, bool isReadOnly)
        {
            FilePath = file.FilePath;
            InstrumentPath = instrumentPath;
            IsReadOnly = isReadOnly;
            this.instrumentHandle = new HandleRef(this, instrumentHandle);
        }

        private void ReleaseUnmanagedResources()
        {
            Close();
        }

        /// <inheritdoc />
        public void Dispose()
        {
            ReleaseUnmanagedResources();
            GC.SuppressFinalize(this);
        }

        /// <inheritdoc />
        ~Instrument()
        {
            ReleaseUnmanagedResources();
        }

        /// <summary>
        /// Flushes the data and closes an access to the instrument. The instance of the class is not usable after this call.
        /// </summary>
        public void Close()
        {
            if (instrumentHandle.Handle != IntPtr.Zero)
            {
                H5InstrumentClose(instrumentHandle.Handle);
                instrumentHandle = new HandleRef(this, IntPtr.Zero);
            }
        }

        /// <summary>
        /// Flushes the instrument.
        /// </summary>
        public void Flush()
        {
            if (instrumentHandle.Handle != IntPtr.Zero)
                H5InstrumentFlush(instrumentHandle.Handle);
        }

        /// <summary>
        /// Enumerates a flat non-recursive list of datasets of the instrument's group.
        /// </summary>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool EnumerateDatasets(DatasetEnumerator datasetEnumerator, bool sort)
        {
            return instrumentHandle.Handle != IntPtr.Zero && H5InstrumentEnumerateDatasets(instrumentHandle.Handle, datasetEnumerator, sort);
        }

        /// <summary>
        /// Enumerates a flat non-recursive list of datasets of the instrument's group.
        /// </summary>
        /// <param name="list">The list to add enumeration into.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool EnumerateDatasets(List<DataInfo> list, bool sort)
        {
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind,
                TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(new DataInfo(name, null)
                {
                    ParsedDataType = dataType, ParsedDataKind = dataKind, ParsedOhlcvKind = ohlcvKind,
                    ParsedScalarKind = scalarKind, ParsedTradeKind = tradeKind, ParsedQuoteKind = quoteKind,
                    ParsedTimeFrame = timeFrame, IsValidName = isValidName, Path = path
                });
            }

            return instrumentHandle.Handle != IntPtr.Zero && H5InstrumentEnumerateDatasets(instrumentHandle.Handle, Enumerator, sort);
        }

        /// <summary>
        /// Opens an instrument's Scalar data.
        /// </summary>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable repository files only.</param>
        /// <returns>A ScalarData object or null in case of error.</returns>
        public ScalarData OpenScalar(DataTimeFrame timeFrame, bool createNonexistent)
        {
            var handle = instrumentHandle.Handle == IntPtr.Zero ? IntPtr.Zero : H5InstrumentOpenScalarData(instrumentHandle.Handle, timeFrame, createNonexistent);
            
            return handle == IntPtr.Zero ? null : new ScalarData(this, $"s_{TimeFrameName(timeFrame)}", handle);
        }

        /// <summary>
        /// Opens an instrument's Trade data.
        /// </summary>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable repository files only.</param>
        /// <returns>A TradeData object or null in case of error.</returns>
        public TradeData OpenTrade(bool createNonexistent)
        {
            var handle = instrumentHandle.Handle == IntPtr.Zero ? IntPtr.Zero : H5InstrumentOpenTradeData(instrumentHandle.Handle, createNonexistent);

            return handle == IntPtr.Zero ? null : new TradeData(this, "t____", handle);
        }

        /// <summary>
        /// Opens an instrument's TradePriceOnly data.
        /// </summary>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable repository files only.</param>
        /// <returns>A TradePriceOnlyData object or null in case of error.</returns>
        public TradePriceOnlyData OpenTradePriceOnly(bool createNonexistent)
        {
            var handle = instrumentHandle.Handle == IntPtr.Zero ? IntPtr.Zero : H5InstrumentOpenTradePriceOnlyData(instrumentHandle.Handle, createNonexistent);

            return handle == IntPtr.Zero ? null : new TradePriceOnlyData(this, "T____", handle);
        }

        /// <summary>
        /// Opens an instrument's Ohlcv data.
        /// </summary>
        /// <param name="kind">A kind of the data to open.</param>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable repository files only.</param>
        /// <returns>An OhlcvData object or null in case of error.</returns>
        public OhlcvData OpenOhlcv(OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent)
        {
            var handle = instrumentHandle.Handle == IntPtr.Zero ? IntPtr.Zero : H5InstrumentOpenOhlcvData(instrumentHandle.Handle, kind, timeFrame, createNonexistent);

            return handle == IntPtr.Zero ? null : new OhlcvData(this, $"{OhlcvKindName(kind)}{TimeFrameName(timeFrame)}", handle);
        }

        /// <summary>
        /// Opens an instrument's OhlcvPriceOnly data.
        /// </summary>
        /// <param name="kind">A kind of the data to open.</param>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable repository files only.</param>
        /// <returns>An OhlcvPriceOnlyData object or null in case of error.</returns>
        public OhlcvPriceOnlyData OpenOhlcvPriceOnly(OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent)
        {
            var handle = instrumentHandle.Handle == IntPtr.Zero ? IntPtr.Zero : H5InstrumentOpenOhlcvPriceOnlyData(instrumentHandle.Handle, kind, timeFrame, createNonexistent);

            return handle == IntPtr.Zero ? null : new OhlcvPriceOnlyData(this, $"{OhlcvPriceOnlyKindName(kind)}{TimeFrameName(timeFrame)}", handle);
        }

        /// <summary>
        /// Opens an adjusted instrument's Ohlcv data.
        /// </summary>
        /// <param name="kind">A kind of the data to open.</param>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable repository files only.</param>
        /// <returns>An OhlcvData object or null in case of error.</returns>
        public OhlcvData OpenOhlcvAdjusted(OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent)
        {
            var handle = instrumentHandle.Handle == IntPtr.Zero ? IntPtr.Zero : H5InstrumentOpenOhlcvAdjustedData(instrumentHandle.Handle, kind, timeFrame, createNonexistent);

            return handle == IntPtr.Zero ? null : new OhlcvData(this, $"{OhlcvAdjustedKindName(kind)}{TimeFrameName(timeFrame)}", handle);
        }

        /// <summary>
        /// Opens an adjusted instrument's OhlcvPriceOnly data.
        /// </summary>
        /// <param name="kind">A kind of the data to open.</param>
        /// <param name="timeFrame">A time frame of the data to open.</param>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable repository files only.</param>
        /// <returns>An OhlcvPriceOnlyData object or null in case of error.</returns>
        public OhlcvPriceOnlyData OpenOhlcvAdjustedPriceOnly(OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent)
        {
            var handle = instrumentHandle.Handle == IntPtr.Zero ? IntPtr.Zero : H5InstrumentOpenOhlcvAdjustedPriceOnlyData(instrumentHandle.Handle, kind, timeFrame, createNonexistent);

            return handle == IntPtr.Zero ? null : new OhlcvPriceOnlyData(this, $"{OhlcvAdjustedPriceOnlyKindName(kind)}{TimeFrameName(timeFrame)}", handle);
        }

        /// <summary>
        /// Opens an instrument's Quote data.
        /// </summary>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable repository files only.</param>
        /// <returns>A QuoteData object or null in case of error.</returns>
        public QuoteData OpenQuote(bool createNonexistent)
        {
            var handle = instrumentHandle.Handle == IntPtr.Zero ? IntPtr.Zero : H5InstrumentOpenQuoteData(instrumentHandle.Handle, createNonexistent);

            return handle == IntPtr.Zero ? null : new QuoteData(this, "q____", handle);
        }

        /// <summary>
        /// Opens an instrument's QuotePriceOnly data.
        /// </summary>
        /// <param name="createNonexistent">Create the data if it does not exist; works for writable repository files only.</param>
        /// <returns>A QuotePriceOnlyData object or null in case of error.</returns>
        public QuotePriceOnlyData OpenQuotePriceOnly(bool createNonexistent)
        {
            var handle = instrumentHandle.Handle == IntPtr.Zero ? IntPtr.Zero : H5InstrumentOpenQuotePriceOnlyData(instrumentHandle.Handle, createNonexistent);

            return handle == IntPtr.Zero ? null : new QuotePriceOnlyData(this, "Q____", handle);
        }

        private static string OhlcvKindName(OhlcvKind kind)
        {
            return kind switch
            {
                OhlcvKind.Ask => "ca",
                OhlcvKind.Bid => "cb",

                // OhlcvKind.Default:
                _ => "c_",
            };
        }

        private static string OhlcvPriceOnlyKindName(OhlcvKind kind)
        {
            return kind switch
            {
                OhlcvKind.Ask => "Ca",
                OhlcvKind.Bid => "Cb",

                // OhlcvKind.Default:
                _ => "C_"
            };
        }

        private static string OhlcvAdjustedKindName(OhlcvKind kind)
        {
            return kind switch
            {
                OhlcvKind.Ask => "ja",
                OhlcvKind.Bid => "jb",

                // OhlcvKind.Default:
                _ => "j_",
            };
        }

        private static string OhlcvAdjustedPriceOnlyKindName(OhlcvKind kind)
        {
            return kind switch
            {
                OhlcvKind.Ask => "Ja",
                OhlcvKind.Bid => "Jb",

                // OhlcvKind.Default:
                _ => "J_",
            };
        }

        private static string TimeFrameName(DataTimeFrame timeFrame)
        {
            return timeFrame switch
            {
                DataTimeFrame.Year1 => "y01",
                DataTimeFrame.Month6 => "M06",
                DataTimeFrame.Month4 => "M04",
                DataTimeFrame.Month3 => "M03",
                DataTimeFrame.Month2 => "M02",
                DataTimeFrame.Month1 => "M01",
                DataTimeFrame.Week3 => "w03",
                DataTimeFrame.Week2 => "w02",
                DataTimeFrame.Week1 => "w01",
                DataTimeFrame.Day1 => "d01",
                DataTimeFrame.Hour12 => "h12",
                DataTimeFrame.Hour8 => "h08",
                DataTimeFrame.Hour6 => "h06",
                DataTimeFrame.Hour4 => "h04",
                DataTimeFrame.Hour3 => "h03",
                DataTimeFrame.Hour2 => "h02",
                DataTimeFrame.Hour1 => "h01",
                DataTimeFrame.Minute30 => "m30",
                DataTimeFrame.Minute20 => "m20",
                DataTimeFrame.Minute15 => "m15",
                DataTimeFrame.Minute12 => "m12",
                DataTimeFrame.Minute10 => "m10",
                DataTimeFrame.Minute6 => "m06",
                DataTimeFrame.Minute5 => "m05",
                DataTimeFrame.Minute4 => "m04",
                DataTimeFrame.Minute3 => "m03",
                DataTimeFrame.Minute2 => "m02",
                DataTimeFrame.Minute1 => "m01",
                DataTimeFrame.Second30 => "s30",
                DataTimeFrame.Second20 => "s20",
                DataTimeFrame.Second15 => "s15",
                DataTimeFrame.Second12 => "s12",
                DataTimeFrame.Second10 => "s10",
                DataTimeFrame.Second6 => "s06",
                DataTimeFrame.Second5 => "s05",
                DataTimeFrame.Second4 => "s04",
                DataTimeFrame.Second3 => "s03",
                DataTimeFrame.Second2 => "s02",
                DataTimeFrame.Second1 => "s01",

                // DataTimeFrame.Aperiodic:
                _ => "___",
            };
        }
    }
}
