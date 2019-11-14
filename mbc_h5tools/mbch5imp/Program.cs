using System;
using System.Collections.Generic;
using System.Globalization;
using System.Diagnostics;
using System.IO;
using Mbh5;

namespace Mbhimp
{
/*
    <applicationSettings>
        <Mbhimp.Properties.Settings>
            <setting name="PageSize" serializeAs="String">
                <value>1024</value>
            </setting>
            <setting name="DateTimeFormat" serializeAs="String">
                <value>yyyy/MM/dd HH:mm:ss.fffffff</value>
            </setting>
            <setting name="UpdateIdenticalTicks" serializeAs="String">
                <value>True</value>
            </setting>
            <setting name="ForbidUpdate" serializeAs="String">
                <value>False</value>
            </setting>
            <setting name="Verbose" serializeAs="String">
                <value>False</value>
            </setting>
            <setting name="SpreadDuplicateTimeTicks" serializeAs="String">
                <value>False</value>
            </setting>
        </Mbhimp.Properties.Settings>
    </applicationSettings>
*/

    internal static class Program
    {
        private static readonly bool Verbose = Properties.Settings.Default.Verbose;
        private static readonly long PageSize = Properties.Settings.Default.PageSize;
        private static string dateTimeFormat = Properties.Settings.Default.DateTimeFormat;
        private const char CsvDelimiter = ';';
        private const char CsvComment = '#';
        private static List<Trade> tradeList;
        private static List<TradePriceOnly> tradePriceOnlyList;
        private static List<Scalar> scalarList;
        private static List<Ohlcv> ohlcvList;
        private static List<OhlcvPriceOnly> ohlcvPriceOnlyList;
        private static List<Quote> quoteList;
        private static List<QuotePriceOnly> quotePriceOnlyList;
        private static Trade trade;
        private static TradePriceOnly tradePriceOnly;
        private static Scalar scalar;
        private static Ohlcv ohlcv;
        private static OhlcvPriceOnly ohlcvPriceOnly;
        private static Quote quote;
        private static QuotePriceOnly quotePriceOnly;
        private static readonly DuplicateTimeTicks DuplicateTimeTicks = Properties.Settings.Default.ForbidUpdate ?
            DuplicateTimeTicks.Fail :
            (Properties.Settings.Default.UpdateIdenticalTicks ? DuplicateTimeTicks.Update : DuplicateTimeTicks.Skip);
        private static readonly bool SpreadDuplicateTimeTicks = Properties.Settings.Default.SpreadDuplicateTimeTicks;

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA2204:Literals should be spelled correctly")]
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Globalization", "CA1303:Do not pass literals as localized parameters")]
        private static void Main(string[] args)
        {
            if (args.Length < 2 ||
                args[0].StartsWith("-?", StringComparison.OrdinalIgnoreCase) ||
                args[0].StartsWith("-h", StringComparison.OrdinalIgnoreCase))
            {
                Console.WriteLine("Arguments: instrumentPath-dataset.csv file.h5 {\"dateTimeFormat\"}");
                Console.WriteLine("Imports the specified csv file as a dataset to a hdf5 file.");
                Console.WriteLine("instrumentPath: dukascopy-fx-GBPUSD-q____.csv -> /dukascopy/fx/GBPUSD/q____");
                Console.WriteLine("dataset naming conventions:");
                Console.WriteLine("t____     trade aperiodic, contains timestamp, the price and the volume");
                Console.WriteLine("T____     trade aperiodic, contains timestamp and the price");
                Console.WriteLine("q____     quote aperiodic, contains timestamp, ask/bid price and the ask/bid volume");
                Console.WriteLine("Q____     quote aperiodic, contains timestamp and the ask/bid price");
                Console.WriteLine("ckttt     candle, contains timestamp, open/high/low/close and volume");
                Console.WriteLine("Ckttt     candle without volume");
                Console.WriteLine("jkttt     adjusted candle, contains timestamp, open/high/low/close and volume");
                Console.WriteLine("Jkttt     adjusted candle without volume");
                Console.WriteLine("          k:   kind; the following values ar accepted:");
                Console.WriteLine("          _   normal");
                Console.WriteLine("          a   ask");
                Console.WriteLine("          b   bid");
                Console.WriteLine("          ttt: time period; the following periods are accepted:");
                Console.WriteLine("          ___   aperiodic");
                Console.WriteLine("          s01, s02, s03, s04, s05, s06, s10, s15, s20, s30   seconds");
                Console.WriteLine("          m01, m02, m03, m04, m05, m06, m10, m15, m20, m30   minutes");
                Console.WriteLine("          h01, h02, h03, h04, h06, h08, h12                  hours");
                Console.WriteLine("          d01                                                days");
                Console.WriteLine("          w01, w02, w03                                      weeks");
                Console.WriteLine("          M01, M02, M03, M04, M06                            months");
                Console.WriteLine("          y01                                                years");
                Console.WriteLine("dateTimeFormat: default is \"yyyy/MM/dd HH:mm:ss.fffffff\", can be set in configuration file");
                Console.WriteLine("the culture is always invariant (en-US)");
                Console.WriteLine("dateTimeFormat conventions:");
                Console.WriteLine("    yyyy      the year as a four-digit number, 1/1/0001 12:00:00 AM -> 0001");
                Console.WriteLine("    yy        the year, from 00 to 99, 1/1/0001 12:00:00 AM -> 01");
                Console.WriteLine("    y         the year, from 0 to 99, 1/1/0001 12:00:00 AM -> 1");
                Console.WriteLine("    MMMM      the full name of the month, 6/15/2009 1:45:30 PM -> June (en-US)");
                Console.WriteLine("    MMM       the abbreviated name of the month, 6/15/2009 1:45:30 PM -> Jun (en-US)");
                Console.WriteLine("    MM        the month, from 01 through 12, 6/15/2009 1:45:30 PM -> 06");
                Console.WriteLine("    M         the month, from 1 through 12, 6/15/2009 1:45:30 PM -> 6");
                Console.WriteLine("    dddd      the full name of the day of the week, 6/15/2009 1:45:30 PM -> Monday (en-US)");
                Console.WriteLine("    ddd       the abbreviated name of the day of the week, 6/15/2009 1:45:30 PM -> Mon (en-US)");
                Console.WriteLine("    dd        the day of the month, from 01 through 31, 6/1/2009 1:45:30 PM -> 01");
                Console.WriteLine("    d         the day of the month, from 1 through 31, 6/1/2009 1:45:30 PM -> 1");
                Console.WriteLine("    HH        the hour, using a 24-hour clock from 00 to 23, 6/15/2009 1:45:30 AM -> 01");
                Console.WriteLine("    H         the hour, using a 24-hour clock from 0 to 23, 6/15/2009 1:45:30 PM -> 13");
                Console.WriteLine("    hh        the hour, using a 12-hour clock from 01 to 12, 6/15/2009 1:45:30 PM -> 01");
                Console.WriteLine("    h         the hour, using a 12-hour clock from 1 to 12, 6/15/2009 1:45:30 PM -> 1");
                Console.WriteLine("    mm        the minute, from 00 through 59, 6/15/2009 1:09:30 PM -> 09");
                Console.WriteLine("    m         the minute, from 0 through 59, 6/15/2009 1:09:30 PM -> 9");
                Console.WriteLine("    ss        the second, from 00 through 59, 6/15/2009 1:45:09 PM -> 09");
                Console.WriteLine("    s         the second, from 0 through 59, 6/15/2009 1:45:09 PM -> 9");
                Console.WriteLine("    FFFFFFF   if non-zero, the ten millionths of a second in a date and time value, 6/15/2009 13:45:30.0001150 -> 000115");
                Console.WriteLine("    fffffff   the hundred thousandths of a second in a date and time value, 6/15/2009 13:45:30.0001150 -> 0001150");
                Console.WriteLine("    tt        the AM/PM designator, 6/15/2009 1:45:30 PM -> PM (en-US)");
                Console.WriteLine("    :         the time separator, 6/15/2009 1:45:30 PM -> : (en-US)");
                Console.WriteLine("    /         the date separator, 6/15/2009 1:45:30 PM -> / (en-US)");
                Console.WriteLine("    zzz       hours and minutes offset from UTC, 6/15/2009 1:45:30 PM -07:00 -> -07:00");
                Console.WriteLine("    zz        hours offset from UTC, with a leading zero for a single-digit value, 6/15/2009 1:45:30 PM -07:00 -> -07");
                Console.WriteLine("    z         hours offset from UTC, with no leading zeros, 6/15/2009 1:45:30 PM -07:00 -> -7");
                return;
            }
            Repository.InterceptErrorStack();

            const string pointCsv = ".csv";
            string file = args[1];
            string csvFile = args[0];
            if (!csvFile.EndsWith(pointCsv, StringComparison.OrdinalIgnoreCase))
            {
                Trace.TraceError("Malformed csv file name [{0}], exiting", csvFile);
                return;
            }
            string instrumentPath = csvFile.Substring(0, csvFile.Length - pointCsv.Length);
            instrumentPath = "/" + instrumentPath.Replace('-', '/');
            int index = instrumentPath.LastIndexOf('/');
            if (index < 1)
            {
                Trace.TraceError("Malformed dataset path [{0}], exiting", instrumentPath);
                return;
            }
            string datasetName = instrumentPath.Substring(index + 1);
            instrumentPath = instrumentPath.Substring(0, index);
            Trace.TraceInformation("Instrument path [{0}], dataset name [{1}]", instrumentPath, datasetName);

            var parsedDatasetName = new ParsedDatasetName(datasetName);
            if (!parsedDatasetName.IsParsed)
            {
                Trace.TraceError("Malformed dataset name [{0}], exiting", datasetName);
                return;
            }

            if (args.Length > 2)
            {
                dateTimeFormat = args[2];
                Trace.TraceInformation("Using specified date-time format [{0}]", dateTimeFormat);
            }
            else
                Trace.TraceInformation("Using default date-time format [{0}]", dateTimeFormat);

            using (Repository repository = Repository.OpenReadWrite(file, true))
            {
                if (null == repository)
                {
                    string msg = string.Format(CultureInfo.InvariantCulture, "[{0}] failed to open file", file);
                    Trace.TraceError(msg);
                    return;
                }
                try
                {
                    if (!Import(csvFile, repository, instrumentPath, datasetName, parsedDatasetName))
                        Trace.TraceError("Failed to import csv file [{0}]", csvFile);
                }
                catch (Exception ex)
                {
                    Trace.TraceError("exception: {0}", ex);
                }
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2202:Do not dispose objects multiple times")]
        private static bool Import(string csvFile, Repository repository, string instrumentPath, string datasetName, ParsedDatasetName parsedDatasetName)
        {
            using (Instrument instrument = repository.Open(instrumentPath, true))
            {
                if (null == instrument)
                {
                    Trace.TraceError("Failed to open an instrument [{0}], import skipped", instrumentPath);
                    return false;
                }
                long lineCount = 0;
                using (var reader = new StreamReader(csvFile))
                {
                    switch (parsedDatasetName.DataType)
                    {
                        case DataType.Trade:
                            using (TradeData data = instrument.OpenTrade(true))
                            {
                                if (null == data)
                                    return DataOpenFailed(datasetName, instrumentPath, instrument);
                                if (null == tradeList)
                                    tradeList = new List<Trade>((int)PageSize);
                                while (true)
                                {
                                    tradeList.Clear();
                                    Read(reader, tradeList, ref lineCount);
                                    if (tradeList.Count == 0)
                                        break;
                                    if (SpreadDuplicateTimeTicks)
                                        data.SpreadDuplicateTimeTicks(tradeList, Verbose);
                                    if (!data.Add(tradeList, DuplicateTimeTicks, Verbose))
                                    {
                                        data.Close();
                                        return DataAddFailed(datasetName, instrumentPath, instrument);
                                    }
                                }
                            }
                            break;
                        case DataType.TradePriceOnly:
                            using (TradePriceOnlyData data = instrument.OpenTradePriceOnly(true))
                            {
                                if (null == data)
                                    return DataOpenFailed(datasetName, instrumentPath, instrument);
                                if (null == tradePriceOnlyList)
                                    tradePriceOnlyList = new List<TradePriceOnly>((int)PageSize);
                                while (true)
                                {
                                    tradePriceOnlyList.Clear();
                                    Read(reader, tradePriceOnlyList, ref lineCount);
                                    if (tradePriceOnlyList.Count == 0)
                                        break;
                                    if (SpreadDuplicateTimeTicks)
                                        data.SpreadDuplicateTimeTicks(tradePriceOnlyList, Verbose);
                                    if (!data.Add(tradePriceOnlyList, DuplicateTimeTicks, Verbose))
                                    {
                                        data.Close();
                                        return DataAddFailed(datasetName, instrumentPath, instrument);
                                    }
                                }
                            }
                            break;
                        case DataType.Scalar:
                            using (ScalarData data = instrument.OpenScalar(parsedDatasetName.ScalarKind, parsedDatasetName.TimeFrame, true))
                            {
                                if (null == data)
                                    return DataOpenFailed(datasetName, instrumentPath, instrument);
                                if (null == scalarList)
                                    scalarList = new List<Scalar>((int)PageSize);
                                while (true)
                                {
                                    scalarList.Clear();
                                    Read(reader, scalarList, ref lineCount);
                                    if (scalarList.Count == 0)
                                        break;
                                    if (SpreadDuplicateTimeTicks)
                                        data.SpreadDuplicateTimeTicks(scalarList, Verbose);
                                    if (!data.Add(scalarList, DuplicateTimeTicks, Verbose))
                                    {
                                        data.Close();
                                        return DataAddFailed(datasetName, instrumentPath, instrument);
                                    }
                                }
                            }
                            break;
                        case DataType.Ohlcv:
                        case DataType.OhlcvAdjusted:
                            using (OhlcvData data = parsedDatasetName.DataType == DataType.Ohlcv
                                ? instrument.OpenOhlcv(parsedDatasetName.OhlcvKind, parsedDatasetName.TimeFrame, true)
                                : instrument.OpenOhlcvAdjusted(parsedDatasetName.OhlcvKind, parsedDatasetName.TimeFrame, true))
                            {
                                if (null == data)
                                    return DataOpenFailed(datasetName, instrumentPath, instrument);
                                if (null == ohlcvList)
                                    ohlcvList = new List<Ohlcv>((int)PageSize);
                                while (true)
                                {
                                    ohlcvList.Clear();
                                    Read(reader, ohlcvList, ref lineCount);
                                    if (ohlcvList.Count == 0)
                                        break;
                                    if (SpreadDuplicateTimeTicks)
                                        data.SpreadDuplicateTimeTicks(ohlcvList, Verbose);
                                    if (!data.Add(ohlcvList, DuplicateTimeTicks, Verbose))
                                    {
                                        data.Close();
                                        return DataAddFailed(datasetName, instrumentPath, instrument);
                                    }
                                }
                            }
                            break;
                        case DataType.OhlcvPriceOnly:
                        case DataType.OhlcvAdjustedPriceOnly:
                            using (OhlcvPriceOnlyData data = parsedDatasetName.DataType == DataType.OhlcvPriceOnly
                                ? instrument.OpenOhlcvPriceOnly(parsedDatasetName.OhlcvKind, parsedDatasetName.TimeFrame, true)
                                : instrument.OpenOhlcvAdjustedPriceOnly(parsedDatasetName.OhlcvKind, parsedDatasetName.TimeFrame, true))
                            {
                                if (null == data)
                                    return DataOpenFailed(datasetName, instrumentPath, instrument);
                                if (null == ohlcvPriceOnlyList)
                                    ohlcvPriceOnlyList = new List<OhlcvPriceOnly>((int)PageSize);
                                while (true)
                                {
                                    ohlcvPriceOnlyList.Clear();
                                    Read(reader, ohlcvPriceOnlyList, ref lineCount);
                                    if (ohlcvPriceOnlyList.Count == 0)
                                        break;
                                    if (SpreadDuplicateTimeTicks)
                                        data.SpreadDuplicateTimeTicks(ohlcvPriceOnlyList, Verbose);
                                    if (!data.Add(ohlcvPriceOnlyList, DuplicateTimeTicks, Verbose))
                                    {
                                        data.Close();
                                        return DataAddFailed(datasetName, instrumentPath, instrument);
                                    }
                                }
                            }
                            break;
                        case DataType.Quote:
                            using (QuoteData data = instrument.OpenQuote(true))
                            {
                                if (null == data)
                                    return DataOpenFailed(datasetName, instrumentPath, instrument);
                                if (null == quoteList)
                                    quoteList = new List<Quote>((int)PageSize);
                                while (true)
                                {
                                    quoteList.Clear();
                                    Read(reader, quoteList, ref lineCount);
                                    if (quoteList.Count == 0)
                                        break;
                                    if (SpreadDuplicateTimeTicks)
                                        data.SpreadDuplicateTimeTicks(quoteList, Verbose);
                                    if (!data.Add(quoteList, DuplicateTimeTicks, Verbose))
                                    {
                                        data.Close();
                                        return DataAddFailed(datasetName, instrumentPath, instrument);
                                    }
                                }
                            }
                            break;
                        case DataType.QuotePriceOnly:
                            using (QuotePriceOnlyData data = instrument.OpenQuotePriceOnly(true))
                            {
                                if (null == data)
                                    return DataOpenFailed(datasetName, instrumentPath, instrument);
                                if (null == quotePriceOnlyList)
                                    quotePriceOnlyList = new List<QuotePriceOnly>((int)PageSize);
                                while (true)
                                {
                                    quotePriceOnlyList.Clear();
                                    Read(reader, quotePriceOnlyList, ref lineCount);
                                    if (quotePriceOnlyList.Count == 0)
                                        break;
                                    if (SpreadDuplicateTimeTicks)
                                        data.SpreadDuplicateTimeTicks(quotePriceOnlyList, Verbose);
                                    if (!data.Add(quotePriceOnlyList, DuplicateTimeTicks, Verbose))
                                    {
                                        data.Close();
                                        return DataAddFailed(datasetName, instrumentPath, instrument);
                                    }
                                }
                            }
                            break;
                    }
                }
            }
            return true;
        }

        private static bool DataOpenFailed(string datasetName, string instrumentPath, Instrument instrument)
        {
            Trace.TraceError("Failed to open a dataset [{0}] from the instrument [{1}], import skipped", datasetName, instrumentPath);
            instrument.Close();
            return false;
        }

        private static bool DataAddFailed(string datasetName, string instrumentPath, Instrument instrument)
        {
            Trace.TraceError("Failed to add to the dataset [{0}] instrument [{1}], import skipped", datasetName, instrumentPath);
            instrument.Close();
            return false;
        }

        private static string MalformedLine(string line, long lineNumber, string what)
        {
            return string.Format(CultureInfo.InvariantCulture, "Malformed line #{0} [{1}]: {2}", lineNumber, line, what);
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2201:DoNotRaiseReservedExceptionTypes")]
        private static DateTime ParseDateTime(string s, string line, long lineNumber)
        {
            if (DateTime.TryParseExact(s, dateTimeFormat, CultureInfo.InvariantCulture, DateTimeStyles.None, out var dt))
                return dt;
            string msg = string.Format(CultureInfo.InvariantCulture, "Malformed date-time [{0}] in line #{1} [{2}]", s, lineNumber, line);
            throw new ApplicationException(msg);
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2201:DoNotRaiseReservedExceptionTypes")]
        private static double ParseDouble(string s, string line, long lineNumber, string what)
        {
            if ("NaN" == s)
                return double.NaN;
            if (double.TryParse(s, NumberStyles.Number | NumberStyles.Float, NumberFormatInfo.InvariantInfo, out var value))
                return value;
            string msg = string.Format(CultureInfo.InvariantCulture, "Malformed {0} number [{1}] in line #{2} [{3}]", what, s, lineNumber, line);
            throw new ApplicationException(msg);
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2201:DoNotRaiseReservedExceptionTypes")]
        private static void Read(StreamReader reader, List<Trade> list, ref long lineCount)
        {
            long count = 0;
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                ++lineCount;
                if (string.IsNullOrEmpty(line) || line[0] == CsvComment)
                    continue;
                string[] splitted = line.Split(CsvDelimiter);
                if (splitted.Length != 3)
                    throw new ApplicationException(MalformedLine(line, lineCount, "wrong number of splitted parts, 3 expected"));

                trade.dateTimeTicks = ParseDateTime(splitted[0], line, lineCount).Ticks;
                trade.price = ParseDouble(splitted[1], line, lineCount, "price");
                trade.volume = ParseDouble(splitted[2], line, lineCount, "volume");
                list.Add(trade);
                if (PageSize == ++count)
                    return;
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2201:DoNotRaiseReservedExceptionTypes")]
        private static void Read(StreamReader reader, List<TradePriceOnly> list, ref long lineCount)
        {
            long count = 0;
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                ++lineCount;
                if (string.IsNullOrEmpty(line) || line[0] == CsvComment)
                    continue;
                string[] splitted = line.Split(CsvDelimiter);
                if (splitted.Length != 2)
                    throw new ApplicationException(MalformedLine(line, lineCount, "wrong number of splitted parts, 2 expected"));

                tradePriceOnly.dateTimeTicks = ParseDateTime(splitted[0], line, lineCount).Ticks;
                tradePriceOnly.price = ParseDouble(splitted[1], line, lineCount, "price");
                list.Add(tradePriceOnly);
                if (PageSize == ++count)
                    return;
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2201:DoNotRaiseReservedExceptionTypes")]
        private static void Read(StreamReader reader, List<Scalar> list, ref long lineCount)
        {
            long count = 0;
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                ++lineCount;
                if (string.IsNullOrEmpty(line) || line[0] == CsvComment)
                    continue;
                string[] splitted = line.Split(CsvDelimiter);
                if (splitted.Length != 2)
                    throw new ApplicationException(MalformedLine(line, lineCount, "wrong number of splitted parts, 2 expected"));

                scalar.dateTimeTicks = ParseDateTime(splitted[0], line, lineCount).Ticks;
                scalar.value = ParseDouble(splitted[1], line, lineCount, "value");
                list.Add(scalar);
                if (PageSize == ++count)
                    return;
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2201:DoNotRaiseReservedExceptionTypes")]
        private static void Read(StreamReader reader, List<Ohlcv> list, ref long lineCount)
        {
            long count = 0;
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                ++lineCount;
                if (string.IsNullOrEmpty(line) || line[0] == CsvComment)
                    continue;
                string[] splitted = line.Split(CsvDelimiter);
                if (splitted.Length != 6)
                    throw new ApplicationException(MalformedLine(line, lineCount, "wrong number of splitted parts, 6 expected"));

                ohlcv.dateTimeTicks = ParseDateTime(splitted[0], line, lineCount).Ticks;
                ohlcv.open = ParseDouble(splitted[1], line, lineCount, "open");
                ohlcv.high = ParseDouble(splitted[2], line, lineCount, "high");
                ohlcv.low = ParseDouble(splitted[3], line, lineCount, "low");
                ohlcv.close = ParseDouble(splitted[4], line, lineCount, "close");
                ohlcv.volume = ParseDouble(splitted[5], line, lineCount, "volume");
                list.Add(ohlcv);
                if (PageSize == ++count)
                    return;
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2201:DoNotRaiseReservedExceptionTypes")]
        private static void Read(StreamReader reader, List<OhlcvPriceOnly> list, ref long lineCount)
        {
            long count = 0;
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                ++lineCount;
                if (string.IsNullOrEmpty(line) || line[0] == CsvComment)
                    continue;
                string[] splitted = line.Split(CsvDelimiter);
                if (splitted.Length != 5)
                    throw new ApplicationException(MalformedLine(line, lineCount, "wrong number of splitted parts, 5 expected"));

                ohlcvPriceOnly.dateTimeTicks = ParseDateTime(splitted[0], line, lineCount).Ticks;
                ohlcvPriceOnly.open = ParseDouble(splitted[1], line, lineCount, "open");
                ohlcvPriceOnly.high = ParseDouble(splitted[2], line, lineCount, "high");
                ohlcvPriceOnly.low = ParseDouble(splitted[3], line, lineCount, "low");
                ohlcvPriceOnly.close = ParseDouble(splitted[4], line, lineCount, "close");
                list.Add(ohlcvPriceOnly);
                if (PageSize == ++count)
                    return;
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2201:DoNotRaiseReservedExceptionTypes")]
        private static void Read(StreamReader reader, List<Quote> list, ref long lineCount)
        {
            long count = 0;
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                ++lineCount;
                if (string.IsNullOrEmpty(line) || line[0] == CsvComment)
                    continue;
                string[] splitted = line.Split(CsvDelimiter);
                if (splitted.Length != 5)
                    throw new ApplicationException(MalformedLine(line, lineCount, "wrong number of splitted parts, 5 expected"));

                quote.dateTimeTicks = ParseDateTime(splitted[0], line, lineCount).Ticks;
                quote.askPrice = ParseDouble(splitted[1], line, lineCount, "askPrice");
                quote.bidPrice = ParseDouble(splitted[2], line, lineCount, "bidPrice");
                quote.askSize = ParseDouble(splitted[3], line, lineCount, "askSize");
                quote.bidSize = ParseDouble(splitted[4], line, lineCount, "bidSize");
                list.Add(quote);
                if (PageSize == ++count)
                    return;
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2201:DoNotRaiseReservedExceptionTypes")]
        private static void Read(StreamReader reader, List<QuotePriceOnly> list, ref long lineCount)
        {
            long count = 0;
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                ++lineCount;
                if (string.IsNullOrEmpty(line) || line[0] == CsvComment)
                    continue;
                string[] splitted = line.Split(CsvDelimiter);
                if (splitted.Length != 3)
                    throw new ApplicationException(MalformedLine(line, lineCount, "wrong number of splitted parts, 3 expected"));

                quotePriceOnly.dateTimeTicks = ParseDateTime(splitted[0], line, lineCount).Ticks;
                quotePriceOnly.askPrice = ParseDouble(splitted[1], line, lineCount, "askPrice");
                quotePriceOnly.bidPrice = ParseDouble(splitted[2], line, lineCount, "bidPrice");
                list.Add(quotePriceOnly);
                if (PageSize == ++count)
                    return;
            }
        }

        private struct ParsedDatasetName
        {
            internal DataType DataType;
            internal OhlcvKind OhlcvKind;
            internal readonly ScalarKind ScalarKind;
            // ReSharper disable MemberCanBePrivate.Local
            // ReSharper disable NotAccessedField.Local
            internal TradeKind TradeKind;
            internal QuoteKind QuoteKind;
            // ReSharper restore NotAccessedField.Local
            // ReSharper restore MemberCanBePrivate.Local
            internal DataTimeFrame TimeFrame;
            internal readonly bool IsParsed;

            internal ParsedDatasetName(string name)
            {
                DataType = DataType.Trade;
                OhlcvKind = OhlcvKind.Default;
                ScalarKind = ScalarKind.Default;
                TradeKind = TradeKind.Default;
                QuoteKind = QuoteKind.Default;
                TimeFrame = DataTimeFrame.Aperiodic;
                IsParsed = false;
                IsParsed = Parse(name);
            }

            private bool Parse(string name)
            {
                if (name.Length != 5)
                    return false;
                char c1 = name[0];
                if (0 == c1)
                    return false;
                char c2 = name[1];
                if (0 == c2)
                    return false;
                char c3 = name[2];
                if (0 == c3)
                    return false;
                char c4 = name[3];
                if (0 == c4)
                    return false;
                char c5 = name[4];
                if (0 == c5)
                    return false;
                if ('t' == c1)
                {
                    if ('_' == c2 && '_' == c3 && '_' == c4 && '_' == c5)
                        return true;
                }
                else if ('T' == c1)
                {
                    DataType = DataType.TradePriceOnly;
                    if ('_' == c2 && '_' == c3 && '_' == c4 && '_' == c5)
                        return true;
                }
                else if ('q' == c1)
                {
                    DataType = DataType.Quote;
                    if ('_' == c2 && '_' == c3 && '_' == c4 && '_' == c5)
                        return true;
                }
                else if ('Q' == c1)
                {
                    DataType = DataType.QuotePriceOnly;
                    if ('_' == c2 && '_' == c3 && '_' == c4 && '_' == c5)
                        return true;
                }
                else if ('c' == c1)
                {
                    DataType = DataType.Ohlcv;
                    goto parseOhlcvKind;
                }
                else if ('C' == c1)
                {
                    DataType = DataType.OhlcvPriceOnly;
                    goto parseOhlcvKind;
                }
                else if ('j' == c1)
                {
                    DataType = DataType.OhlcvAdjusted;
                    goto parseOhlcvKind;
                }
                else if ('J' == c1)
                {
                    DataType = DataType.OhlcvAdjustedPriceOnly;
                    goto parseOhlcvKind;
                }
                else if ('s' == c1)
                {
                    DataType = DataType.Scalar;
                    if ('_' != c2)
                        return false;
                    goto parseTimeFrame;
                }
                return false;
parseOhlcvKind:
                if ('_' != c2)
                {
                    if ('a' == c2)
                        OhlcvKind = OhlcvKind.Ask;
                    else if ('b' == c2)
                        OhlcvKind = OhlcvKind.Bid;
                    else
                        return false;
                }
parseTimeFrame:
                if ('d' == c3)
                {
                    if ('0' == c4 && '1' == c5)
                    {
                        TimeFrame = DataTimeFrame.Day1;
                        return true;
                    }
                }
                else if ('h' == c3)
                {
                    if ('0' == c4)
                    {
                        switch (c5)
                        {
                            case '1':
                                TimeFrame = DataTimeFrame.Hour1;
                                return true;
                            case '2':
                                TimeFrame = DataTimeFrame.Hour2;
                                return true;
                            case '3':
                                TimeFrame = DataTimeFrame.Hour3;
                                return true;
                            case '4':
                                TimeFrame = DataTimeFrame.Hour4;
                                return true;
                            case '6':
                                TimeFrame = DataTimeFrame.Hour6;
                                return true;
                            case '8':
                                TimeFrame = DataTimeFrame.Hour8;
                                return true;
                        }
                    }
                    else if ('1' == c4 && '2' == c5)
                    {
                        TimeFrame = DataTimeFrame.Hour12;
                        return true;
                    }
                }
                else if ('m' == c3)
                {
                    if ('0' == c4)
                    {
                        switch (c5)
                        {
                            case '1':
                                TimeFrame = DataTimeFrame.Minute1;
                                return true;
                            case '2':
                                TimeFrame = DataTimeFrame.Minute2;
                                return true;
                            case '3':
                                TimeFrame = DataTimeFrame.Minute3;
                                return true;
                            case '4':
                                TimeFrame = DataTimeFrame.Minute4;
                                return true;
                            case '5':
                                TimeFrame = DataTimeFrame.Minute5;
                                return true;
                            case '6':
                                TimeFrame = DataTimeFrame.Minute6;
                                return true;
                        }
                    }
                    else if ('1' == c4)
                    {
                        switch (c5)
                        {
                            case '0':
                                TimeFrame = DataTimeFrame.Minute10;
                                return true;
                            case '2':
                                TimeFrame = DataTimeFrame.Minute12;
                                return true;
                            case '5':
                                TimeFrame = DataTimeFrame.Minute15;
                                return true;
                        }
                    }
                    else if ('2' == c4 && '0' == c5)
                    {
                        TimeFrame = DataTimeFrame.Minute20;
                        return true;
                    }
                    else if ('3' == c4 && '0' == c5)
                    {
                        TimeFrame = DataTimeFrame.Minute30;
                        return true;
                    }
                }
                else if ('s' == c3)
                {
                    if ('0' == c4)
                    {
                        switch (c5)
                        {
                            case '1':
                                TimeFrame = DataTimeFrame.Second1;
                                return true;
                            case '2':
                                TimeFrame = DataTimeFrame.Second2;
                                return true;
                            case '3':
                                TimeFrame = DataTimeFrame.Second3;
                                return true;
                            case '4':
                                TimeFrame = DataTimeFrame.Second4;
                                return true;
                            case '5':
                                TimeFrame = DataTimeFrame.Second5;
                                return true;
                            case '6':
                                TimeFrame = DataTimeFrame.Second6;
                                return true;
                        }
                    }
                    else if ('1' == c4)
                    {
                        switch (c5)
                        {
                            case '0':
                                TimeFrame = DataTimeFrame.Second10;
                                return true;
                            case '2':
                                TimeFrame = DataTimeFrame.Second12;
                                return true;
                            case '5':
                                TimeFrame = DataTimeFrame.Second15;
                                return true;
                        }
                    }
                    else if ('2' == c4 && '0' == c5)
                    {
                        TimeFrame = DataTimeFrame.Second20;
                        return true;
                    }
                    else if ('3' == c4 && '0' == c5)
                    {
                        TimeFrame = DataTimeFrame.Second30;
                        return true;
                    }
                }
                else if ('w' == c3)
                {
                    if ('0' == c4)
                    {
                        switch (c5)
                        {
                            case '1':
                                TimeFrame = DataTimeFrame.Week1;
                                return true;
                            case '2':
                                TimeFrame = DataTimeFrame.Week2;
                                return true;
                            case '3':
                                TimeFrame = DataTimeFrame.Week3;
                                return true;
                        }
                    }
                }
                else if ('M' == c3)
                {
                    if ('0' == c4)
                    {
                        switch (c5)
                        {
                            case '1':
                                TimeFrame = DataTimeFrame.Month1;
                                return true;
                            case '2':
                                TimeFrame = DataTimeFrame.Month2;
                                return true;
                            case '3':
                                TimeFrame = DataTimeFrame.Month3;
                                return true;
                            case '4':
                                TimeFrame = DataTimeFrame.Month4;
                                return true;
                            case '6':
                                TimeFrame = DataTimeFrame.Month6;
                                return true;
                        }
                    }
                }
                else if ('y' == c3 && '0' == c4 && '1' == c5)
                {
                    TimeFrame = DataTimeFrame.Year1;
                    return true;
                }
                else if ('_' == c3 && '_' == c4 && '_' == c5)
                    return true;
                return false;
            }
        }
    }
}
