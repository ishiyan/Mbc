using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Mbcs.H5;
using File = Mbcs.H5.File;

namespace Mbcs.Tests.H5
{
    [TestClass]
    public partial class H5Tests
    {
        private static int fileCount;

        private static string NewTestFileName
        {
            get
            {
                string fileName = $"test_{Interlocked.Increment(ref fileCount)}.h5";
                DeleteTestFile(fileName);
                return fileName;
            }
        }

#pragma warning disable IDE0060 // Remove unused parameter
        [ClassInitialize]
        public static void ClassInitialize(TestContext context)
        {
            //CrtDebug.ClearWindowsCrtDebugFlags();
            CrtDebug.SetWindowsCrtDebugFlags();
            //CrtDebug.SetWindowsCrtDebugFlagsFull();

            DeleteAllTestFile();
        }
#pragma warning restore IDE0060 // Remove unused parameter

        [ClassCleanup]
        public static void ClassCleanup()
        {
            DeleteAllTestFile();
        }

        private static void DeleteAllTestFile()
        {
            try
            {
                Directory.GetFiles(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "test_*.h5")
                    .Where(s => s.EndsWith(".h5"))
                    .ToList()
                    .ForEach(DeleteTestFile);
            }
            catch
            {
                // ignored
            }
        }

        private static void DeleteTestFile(string fileName)
        {
            try
            {
                System.IO.File.Delete(fileName);
            }
            catch
            {
                // ignored
            }
        }

        private static void CreateEnumerationTestDatasets(string fileName)
        {
            using var file = File.OpenReadWrite(fileName);
            using (var instrument = file.OpenInstrument("a", true))
            {
                using (instrument.OpenOhlcv(OhlcvKind.Default, DataTimeFrame.Aperiodic, true)) { }

                using (instrument.OpenOhlcvAdjusted(OhlcvKind.Default, DataTimeFrame.Day1, true)) { }
            }
            using (var instrument = file.OpenInstrument("b/c", true))
            {
                using (instrument.OpenQuote(true)) { }
                using (instrument.OpenTrade(true)) { }
            }
            using (var instrument = file.OpenInstrument("d/e/f", true))
            {
                using (instrument.OpenScalar(DataTimeFrame.Minute3, true)) { }
                using (instrument.OpenTradePriceOnly(true)) { }
            }
        }

        private static void CreateEnumerationTestDatasetsInReversedOrder(string fileName)
        {
            using var file = File.OpenReadWrite(fileName);
            using (var instrument = file.OpenInstrument("d/e/f", true))
            {
                using (instrument.OpenScalar(DataTimeFrame.Minute3, true)) { }
                using (instrument.OpenTradePriceOnly(true)) { }
            }
            using (var instrument = file.OpenInstrument("b/c", true))
            {
                using (instrument.OpenQuote(true)) { }
                using (instrument.OpenTrade(true)) { }
            }
            using (var instrument = file.OpenInstrument("a", true))
            {
                using (instrument.OpenOhlcv(OhlcvKind.Default, DataTimeFrame.Aperiodic, true)) { }
                using (instrument.OpenOhlcvAdjusted(OhlcvKind.Default, DataTimeFrame.Day1, true)) { }
            }
        }

        private static int VerifySuccess(bool status, bool verified, int errorCount)
        {
            if (!status || !verified)
            {
                Trace.TraceError(string.Format(CultureInfo.InvariantCulture, "FAILED: error = {0}, verified = {1}", status, verified));
                errorCount++;
            }
            else
                Trace.TraceInformation("PASSED");
            return errorCount;
        }

        private static int VerifyFailure(bool status, bool verified, int errorCount)
        {
            if (status || !verified)
            {
                Trace.TraceError(string.Format(CultureInfo.InvariantCulture, "FAILED: error = {0}, verified = {1}", status, verified));
                errorCount++;
            }
            else
                Trace.TraceInformation("PASSED");
            return errorCount;
        }
    }
}
