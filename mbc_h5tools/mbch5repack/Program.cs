using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Reflection;
using System.Text;
using Mbh5;

namespace Mbhrepack
{
/*
    <applicationSettings>
        <Mbhrepack.Properties.Settings>
            <setting name="IgnoreH5Repack" serializeAs="String">
                <value>False</value>
            </setting>
            <setting name="PageSize" serializeAs="String">
                <value>1048576</value>
            </setting>
        </Mbhrepack.Properties.Settings>
    </applicationSettings>

*/
    internal static class Program
    {
        private static long totalBytesBefore, totalBytesAfter, fileCount;
        private static string utilityPath;
        private static readonly long PageSize = Properties.Settings.Default.PageSize;
        private static List<Trade> tradeList;
        private static List<TradePriceOnly> tradePriceOnlyList;
        private static List<Scalar> scalarList;
        private static List<Ohlcv> ohlcvList;
        private static List<OhlcvPriceOnly> ohlcvPriceOnlyList;
        private static List<Quote> quoteList;
        private static List<QuotePriceOnly> quotePriceOnlyList;

        private static string UtilityPath()
        {
            const char separator = '/';
            const string prefix = "file:///";
            const string h5Repack = "h5repack.exe";
            string s = Assembly.GetExecutingAssembly().CodeBase;
            if (s.StartsWith(prefix, StringComparison.Ordinal))
                s = s.Substring(prefix.Length);
            int i = s.LastIndexOf(separator);
            s = s.Substring(0, i + 1);
            s = string.Concat(s, h5Repack);
            if (File.Exists(s))
                Trace.TraceInformation("Found h5repack.exe from [{0}]", s);
            else
            {
                Trace.TraceInformation("Could not find h5repack.exe, file [{0}] does not exist", s);
                s = null;
            }
            return s;
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA2204:Literals should be spelled correctly")]
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Globalization", "CA1303:Do not pass literals as localized parameters")]
        static void Main(string[] args)
        {
            if (!Properties.Settings.Default.IgnoreH5Repack)
            {
                utilityPath = UtilityPath();
            }
            else
                Trace.TraceInformation("Ignoring h5repack.exe");
            if (null == utilityPath)
            {
                tradeList = new List<Trade>((int)PageSize);
                tradePriceOnlyList = new List<TradePriceOnly>((int)PageSize);
                scalarList = new List<Scalar>((int)PageSize);
                ohlcvList = new List<Ohlcv>((int)PageSize);
                ohlcvPriceOnlyList = new List<OhlcvPriceOnly>((int)PageSize);
                quoteList = new List<Quote>((int)PageSize);
                quotePriceOnlyList = new List<QuotePriceOnly>((int)PageSize);
            }
            if (args.Length < 1
                || args[0].StartsWith("-?", StringComparison.OrdinalIgnoreCase)
                || args[0].StartsWith("-h", StringComparison.OrdinalIgnoreCase))
            {
                Console.WriteLine("Argument: file_or_directory_name");
                return;
            }
            Repository.InterceptErrorStack();
            TraverseTree(args[0], RepackFile);
            Trace.TraceInformation("Repacked {0} files, total bytes before = {1}, after = {2}, ratio = {3}%",
                fileCount, totalBytesBefore, totalBytesAfter, 100.0 * totalBytesAfter / (0L == totalBytesBefore ? 1L : totalBytesBefore));
        }

        private static void TraverseTree(string root, Action<string> action)
        {
            if (Directory.Exists(root))
            {
                string[] entries = Directory.GetFiles(root);
                foreach (string entry in entries)
                    action(entry);
                entries = Directory.GetDirectories(root);
                foreach (string entry in entries)
                    TraverseTree(entry, action);
            }
            else if (File.Exists(root))
                action(root);
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1031:DoNotCatchGeneralExceptionTypes")]
        private static void RepackFile(string sourceFileName)
        {
            if (!sourceFileName.EndsWith(".h5", StringComparison.OrdinalIgnoreCase))
                return;
            var fileInfo = new FileInfo(sourceFileName);
            DateTime dateTimeLastAccessUtc = fileInfo.LastAccessTimeUtc;
            DateTime dateTimeCreationUtc = fileInfo.CreationTimeUtc;
            DateTime dateTimeLastWriteUtc = fileInfo.LastWriteTimeUtc;
            bool isReadOnly = fileInfo.IsReadOnly;
            string fullName = fileInfo.FullName;
            var driveInfo = new DriveInfo(fullName);
            if (fileInfo.Length >= driveInfo.AvailableFreeSpace)
            {
                Trace.TraceError("Not enough space to repack file [{0}]: file length {1} bytes, available space {2} bytes, repacking skipped", sourceFileName, fileInfo.Length, driveInfo.AvailableFreeSpace);
                return;
            }
            string backFileName = sourceFileName + ".bak.h5";
            string fullNameBack = fullName + ".bak.h5";
            try
            {
                File.Move(sourceFileName, backFileName);
            }
            catch (Exception ex)
            {
                Trace.TraceError("Exception while renaming file [{0}] to [{1}], repacking skipped: {2}", sourceFileName, backFileName, ex.Message);
                return;
            }
            bool success = null != utilityPath ?
                RunRepack(string.Concat(fullNameBack, " ", fullName)) :
                CopyDatasets(fullNameBack, fullName);
            if (success)
            {
                var fileInfoNew = new FileInfo(sourceFileName)
                {
                    IsReadOnly = isReadOnly,
                    CreationTimeUtc = dateTimeCreationUtc,
                    LastWriteTimeUtc = dateTimeLastWriteUtc,
                    LastAccessTimeUtc = dateTimeLastAccessUtc
                };
                Trace.TraceInformation("Repacked file [{0}], old size = {1} bytes, new size = {2} bytes, ratio = {3}%",
                    sourceFileName, fileInfo.Length, fileInfoNew.Length, 100.0 * fileInfoNew.Length / (0L == fileInfo.Length ? 1L : fileInfo.Length));
                totalBytesBefore += fileInfo.Length;
                totalBytesAfter += fileInfoNew.Length;
                ++fileCount;
                // ReSharper disable RedundantAssignment
                if (isReadOnly)
                    fileInfo = new FileInfo(backFileName) { IsReadOnly = false };
                // ReSharper restore RedundantAssignment
                File.Delete(backFileName);
            }
            else
            {
                Trace.TraceError("Failed to repack file [{0}] to [{1}], restoring back file", backFileName, sourceFileName);
                File.Delete(sourceFileName);
                File.Move(backFileName, sourceFileName);
            }
        }

        private static bool CopyDatasets(string srcFilePath, string tgtFilePath)
        {
            using (Repository srcRepository = Repository.OpenReadOnly(srcFilePath))
            {
                if (null == srcRepository)
                {
                    Trace.TraceError("Failed to open a source file [{0}], repacking skipped", srcFilePath);
                    return false;
                }
                List<DataInfo> dataInfoList = srcRepository.ContentList(true);
                using (Repository tgtRepository = Repository.OpenReadWrite(tgtFilePath, true))
                {
                    if (null == tgtRepository)
                    {
                        Trace.TraceError("Failed to open a target file [{0}], repacking skipped", tgtFilePath);
                        srcRepository.Close();
                        return false;
                    }
                    foreach (var dataInfo in dataInfoList)
                    {
                        string instrumentPath = dataInfo.Parent.Path;
                        using (Instrument srcInstrument = srcRepository.Open(instrumentPath))
                        {
                            if (null == srcInstrument)
                            {
                                Trace.TraceError("Failed to open an instrument [{0}] from the source file [{1}], repacking skipped", instrumentPath, srcFilePath);
                                return false;
                            }

                            using (Instrument tgtInstrument = tgtRepository.Open(instrumentPath, true))
                            {
                                string name = dataInfo.Name;
                                switch (dataInfo.ParsedDataType)
                                {
                                    case DataType.Trade:
                                        using (TradeData srcData = srcInstrument.OpenTrade())
                                        {
                                            if (null == srcData)
                                                return SrcDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                            using (TradeData tgtData = tgtInstrument.OpenTrade(true))
                                            {
                                                if (null == tgtData)
                                                    return TgtDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                long i = 0, count = srcData.Count;
                                                while (count >= PageSize)
                                                {
                                                    tradeList.Clear();
                                                    if (!srcData.Fetch(tradeList, i, PageSize))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(tradeList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    i += PageSize;
                                                    count -= PageSize;
                                                }
                                                tradeList.Clear();
                                                if (0 < count)
                                                {
                                                    if (!srcData.Fetch(tradeList, i, count))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(tradeList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    tradeList.Clear();
                                                }
                                            }
                                        }
                                        break;
                                    case DataType.TradePriceOnly:
                                        using (TradePriceOnlyData srcData = srcInstrument.OpenTradePriceOnly())
                                        {
                                            if (null == srcData)
                                                return SrcDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                            using (TradePriceOnlyData tgtData = tgtInstrument.OpenTradePriceOnly(true))
                                            {
                                                if (null == tgtData)
                                                    return TgtDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                long i = 0, count = srcData.Count;
                                                while (count >= PageSize)
                                                {
                                                    tradePriceOnlyList.Clear();
                                                    if (!srcData.Fetch(tradePriceOnlyList, i, PageSize))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(tradePriceOnlyList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    i += PageSize;
                                                    count -= PageSize;
                                                }
                                                tradePriceOnlyList.Clear();
                                                if (0 < count)
                                                {
                                                    if (!srcData.Fetch(tradePriceOnlyList, i, count))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(tradePriceOnlyList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    tradePriceOnlyList.Clear();
                                                }
                                            }
                                        }
                                        break;
                                    case DataType.Scalar:
                                        using (ScalarData srcData = srcInstrument.OpenScalar(dataInfo.ParsedScalarKind, dataInfo.ParsedTimeFrame))
                                        {
                                            if (null == srcData)
                                                return SrcDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                            using (ScalarData tgtData = tgtInstrument.OpenScalar(dataInfo.ParsedScalarKind, dataInfo.ParsedTimeFrame, true))
                                            {
                                                if (null == tgtData)
                                                    return TgtDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                long i = 0, count = srcData.Count;
                                                while (count >= PageSize)
                                                {
                                                    scalarList.Clear();
                                                    if (!srcData.Fetch(scalarList, i, PageSize))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(scalarList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    i += PageSize;
                                                    count -= PageSize;
                                                }
                                                scalarList.Clear();
                                                if (0 < count)
                                                {
                                                    if (!srcData.Fetch(scalarList, i, count))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(scalarList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    scalarList.Clear();
                                                }

                                            }
                                        }
                                        break;
                                    case DataType.Ohlcv:
                                    case DataType.OhlcvAdjusted:
                                        using (OhlcvData srcData = dataInfo.ParsedDataType == DataType.Ohlcv
                                            ? srcInstrument.OpenOhlcv(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame)
                                            : srcInstrument.OpenOhlcvAdjusted(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame))
                                        {
                                            if (null == srcData)
                                                return SrcDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                            using (OhlcvData tgtData = dataInfo.ParsedDataType == DataType.Ohlcv
                                                ? tgtInstrument.OpenOhlcv(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame, true)
                                                : tgtInstrument.OpenOhlcvAdjusted(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame, true))
                                            {
                                                if (null == tgtData)
                                                    return TgtDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                long i = 0, count = srcData.Count;
                                                while (count >= PageSize)
                                                {
                                                    ohlcvList.Clear();
                                                    if (!srcData.Fetch(ohlcvList, i, PageSize))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(ohlcvList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    i += PageSize;
                                                    count -= PageSize;
                                                }
                                                ohlcvList.Clear();
                                                if (0 < count)
                                                {
                                                    if (!srcData.Fetch(ohlcvList, i, count))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(ohlcvList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    ohlcvList.Clear();
                                                }
                                            }
                                        }
                                        break;
                                    case DataType.OhlcvPriceOnly:
                                    case DataType.OhlcvAdjustedPriceOnly:
                                        using (OhlcvPriceOnlyData srcData = dataInfo.ParsedDataType == DataType.OhlcvPriceOnly
                                                ? srcInstrument.OpenOhlcvPriceOnly(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame)
                                                : srcInstrument.OpenOhlcvAdjustedPriceOnly(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame))
                                        {
                                            if (null == srcData)
                                                return SrcDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                            using (OhlcvPriceOnlyData tgtData = dataInfo.ParsedDataType == DataType.OhlcvPriceOnly
                                                    ? tgtInstrument.OpenOhlcvPriceOnly(dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame, true)
                                                    : tgtInstrument.OpenOhlcvAdjustedPriceOnly( dataInfo.ParsedOhlcvKind, dataInfo.ParsedTimeFrame, true))
                                            {
                                                if (null == tgtData)
                                                    return TgtDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                long i = 0, count = srcData.Count;
                                                while (count >= PageSize)
                                                {
                                                    ohlcvPriceOnlyList.Clear();
                                                    if (!srcData.Fetch(ohlcvPriceOnlyList, i, PageSize))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(ohlcvPriceOnlyList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    i += PageSize;
                                                    count -= PageSize;
                                                }
                                                ohlcvPriceOnlyList.Clear();
                                                if (0 < count)
                                                {
                                                    if (!srcData.Fetch(ohlcvPriceOnlyList, i, count))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(ohlcvPriceOnlyList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    ohlcvPriceOnlyList.Clear();
                                                }
                                            }
                                        }
                                        break;
                                    case DataType.Quote:
                                        using (QuoteData srcData = srcInstrument.OpenQuote())
                                        {
                                            if (null == srcData)
                                                return SrcDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                            using (QuoteData tgtData = tgtInstrument.OpenQuote(true))
                                            {
                                                if (null == tgtData)
                                                    return TgtDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                long i = 0, count = srcData.Count;
                                                while (count >= PageSize)
                                                {
                                                    quoteList.Clear();
                                                    if (!srcData.Fetch(quoteList, i, PageSize))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(quoteList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    i += PageSize;
                                                    count -= PageSize;
                                                }
                                                quoteList.Clear();
                                                if (0 < count)
                                                {
                                                    if (!srcData.Fetch(quoteList, i, count))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(quoteList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    quoteList.Clear();
                                                }
                                            }
                                        }
                                        break;
                                    case DataType.QuotePriceOnly:
                                        using (QuotePriceOnlyData srcData = srcInstrument.OpenQuotePriceOnly())
                                        {
                                            if (null == srcData)
                                                return SrcDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                            using (QuotePriceOnlyData tgtData = tgtInstrument.OpenQuotePriceOnly(true))
                                            {
                                                if (null == tgtData)
                                                    return TgtDataOpenFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                long i = 0, count = srcData.Count;
                                                while (count >= PageSize)
                                                {
                                                    quotePriceOnlyList.Clear();
                                                    if (!srcData.Fetch(quotePriceOnlyList, i, PageSize))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(quotePriceOnlyList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    i += PageSize;
                                                    count -= PageSize;
                                                }
                                                quotePriceOnlyList.Clear();
                                                if (0 < count)
                                                {
                                                    if (!srcData.Fetch(quotePriceOnlyList, i, count))
                                                        return SrcDataFetchFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    if (!tgtData.Add(quotePriceOnlyList, DuplicateTimeTicks.Fail, false))
                                                        return TgtDataAddFailed(name, instrumentPath, srcRepository, tgtRepository, srcInstrument, tgtInstrument);
                                                    quotePriceOnlyList.Clear();
                                                }
                                            }
                                        }
                                        break;
                                }
                            }
                        }
                    }
                }
            }
            return true;
        }

        private static bool SrcDataOpenFailed(string datasetName, string instrumentPath, Repository srcRepository, Repository tgtRepository, Instrument srcInstrument, Instrument tgtInstrument)
        {
            Trace.TraceError("Failed to open a dataset [{0}] from the source instrument [{1}], repacking skipped", datasetName, instrumentPath);
            srcInstrument.Close();
            tgtInstrument.Close();
            srcRepository.Close();
            tgtRepository.Close();
            return false;
        }

        private static bool SrcDataFetchFailed(string datasetName, string instrumentPath, Repository srcRepository, Repository tgtRepository, Instrument srcInstrument, Instrument tgtInstrument)
        {
            Trace.TraceError("Failed to fetch the dataset [{0}] from the source instrument [{1}], repacking skipped", datasetName, instrumentPath);
            srcInstrument.Close();
            tgtInstrument.Close();
            srcRepository.Close();
            tgtRepository.Close();
            return false;
        }

        private static bool TgtDataOpenFailed(string datasetName, string instrumentPath, Repository srcRepository, Repository tgtRepository, Instrument srcInstrument, Instrument tgtInstrument)
        {
            Trace.TraceError("Failed to open a dataset [{0}] from the target instrument [{1}], repacking skipped", datasetName, instrumentPath);
            srcInstrument.Close();
            tgtInstrument.Close();
            srcRepository.Close();
            tgtRepository.Close();
            return false;
        }

        private static bool TgtDataAddFailed(string datasetName, string instrumentPath, Repository srcRepository, Repository tgtRepository, Instrument srcInstrument, Instrument tgtInstrument)
        {
            Trace.TraceError("Failed to add a dataset [{0}] to the target instrument [{1}], repacking skipped", datasetName, instrumentPath);
            srcInstrument.Close();
            tgtInstrument.Close();
            srcRepository.Close();
            tgtRepository.Close();
            return false;
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1031:DoNotCatchGeneralExceptionTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Reliability", "CA2000:Dispose objects before losing scope")]
        private static bool RunRepack(string arguments)
        {
            using (var process = new Process
            {
                StartInfo =
                {
                    FileName = utilityPath,
                    Arguments = arguments,
                    CreateNoWindow = true,
                    WindowStyle = ProcessWindowStyle.Hidden,
                    UseShellExecute = false,
                    RedirectStandardError = true,
                    RedirectStandardOutput = true
                }
            })
            {
                var stdOutput = new StringBuilder();
                process.OutputDataReceived += (sender, args) => stdOutput.Append(args.Data);
                string stdError = null;
                try
                {
                    process.Start();
                    process.BeginOutputReadLine();
                    stdError = process.StandardError.ReadToEnd();
                    process.WaitForExit();
                }
                catch (Exception e)
                {
                    Trace.TraceError("Exception while executing \"{0} {1}\": {2}", utilityPath, arguments, e.Message);
                }

                if (process.ExitCode == 0)
                    return true;

                var message = new StringBuilder();
                if (!string.IsNullOrEmpty(stdError))
                {
                    message.AppendLine("Std error:");
                    message.AppendLine(stdError);
                }
                if (stdOutput.Length != 0)
                {
                    message.AppendLine("Std output:");
                    message.AppendLine(stdOutput.ToString());
                }
                Trace.TraceError("Non-zero exit code executing \"{0} {1}\"", utilityPath, arguments);
                Trace.TraceError(message.ToString());
                return false;
            }
        }
    }
}
