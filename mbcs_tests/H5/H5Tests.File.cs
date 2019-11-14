using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Mbcs.H5;

namespace Mbcs.Tests.H5
{
    public partial class H5Tests
    {
        [TestMethod]
        public void H5_File_InterceptErrorStack_Set_LogsHdf5Errors()
        {
            List<string> actualLog = new List<string>();
            File.InterceptErrorStack((message, isError) =>
            {
                actualLog.Add($"Error={isError}: {message}");
            });

            var fileName = NewTestFileName;
            var file = File.OpenReadOnly(fileName);

            File.InterceptErrorStack(null);

            Assert.IsNull(file);
            Assert.AreEqual(1, actualLog.Count);
            Assert.AreEqual($"Error=True: h5: Error: Failed to open the file \"{fileName}\" for the read-only access", actualLog[0]);
        }

        [TestMethod]
        public void H5_File_InterceptErrorStack_Set_LogsHdf5Infos()
        {
            List<string> actualLog = new List<string>();
            File.InterceptErrorStack((message, isError) =>
            {
                actualLog.Add($"Error={isError}: {message}");
            });

            Scalar[] array = { new Scalar { Ticks = 1001L, Value = 2.2 }, new Scalar { Ticks = 1001L, Value = 3.3 } };
            bool status = ScalarData.SpreadDuplicateTimeTicks(array, true);

            File.InterceptErrorStack(null);

            Assert.IsTrue(status);
            Assert.AreEqual(2, actualLog.Count);
            Assert.IsTrue(actualLog[0].StartsWith("Error=False: h5: Info: Spreading duplicate input time tick: start input[0]"));
            Assert.IsTrue(actualLog[1].StartsWith("Error=False: h5: Info: Spreading duplicate input time tick: start input[1]"));
        }

        [TestMethod]
        public void H5_File_Opened_FilePathProperty_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            string filePath;

            using (var file = File.OpenReadWrite(fileName))
            {
                filePath = file.FilePath;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(fileName, filePath);
        }

        [TestMethod]
        public void H5_File_Opened_FileDriverProperty_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            FileDriver fileDriverActual, fileDriverExpected = FileDriver.StdIo;

            using (var file = File.OpenReadWrite(fileName, true, fileDriverExpected))
            {
                fileDriverActual = file.FileDriver;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(fileDriverExpected, fileDriverActual);
        }

        [TestMethod]
        public void H5_File_Opened_IsReadOnlyProperty_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            bool isReadOnlyFalse, isReadOnlyTrue;

            using (var file = File.OpenReadWrite(fileName))
            {
                isReadOnlyFalse = file.IsReadOnly;
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                isReadOnlyTrue = file.IsReadOnly;
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isReadOnlyFalse);
            Assert.IsTrue(isReadOnlyTrue);
        }

        [TestMethod]
        public void H5_File_Closed_FileHandleIsUnusable()
        {
            string fileName = NewTestFileName;
            var file = File.OpenReadWrite(fileName);
            file.Flush();
            file.Close();

            List<DataInfo> list = new List<DataInfo>();
            bool status = file.EnumerateDatasets(list, false);

            DeleteTestFile(fileName);

            Assert.IsFalse(status);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_File_OpenedReadWrite_OpenInstrument_CreateNonExistentFalse_NonExistingInstrumentCannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                var instrument = file.OpenInstrument("foo/bar", false);
                isNull = instrument == null;
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_File_OpenedReadWrite_OpenInstrument_CreateNonExistentTrue_NonExistingInstrumentIsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                isNull = instrument == null;
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_File_OpenedReadOnly_OpenInstrument_CreateNonExistentFalse_NonExistingInstrumentCannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (File.OpenReadWrite(fileName)) { }
            using (var file = File.OpenReadOnly(fileName))
            {
                var instrument = file.OpenInstrument("foo/bar", false);
                isNull = instrument == null;
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_File_OpenedReadOnly_OpenInstrument_CreateNonExistentTrue_NonExistingInstrumentCannotBeCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (File.OpenReadWrite(fileName)) { }
            using (var file = File.OpenReadOnly(fileName))
            {
                var instrument = file.OpenInstrument("foo/bar", true);
                isNull = instrument == null;
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_File_ContentTree_Unsorted_EnumeratesContentCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            GroupInfo gi;
            using (var file = File.OpenReadOnly(fileName))
            {
                gi = file.ContentTree(false);
            }

            DeleteTestFile(fileName);

            Assert.AreEqual("/", gi.Name);
            Assert.AreEqual("", gi.Path);
            Assert.IsNull(gi.Parent);
            Assert.AreEqual(0, gi.Datasets.Count);
            Assert.AreEqual(3, gi.Groups.Count);

            var item = gi.Groups[0];
            Assert.AreEqual("a", item.Name);
            Assert.AreEqual("/a", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(2, item.Datasets.Count);
            Assert.AreEqual(0, item.Groups.Count);
            var ds = item.Datasets[0];
            Assert.AreEqual(item, ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("c____", ds.Name);
            Assert.AreEqual("/a/c____", ds.Path);
            Assert.AreEqual(DataType.Ohlcv, ds.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, ds.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Datasets[1];
            Assert.AreEqual(item, ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("j_d01", ds.Name);
            Assert.AreEqual("/a/j_d01", ds.Path);
            Assert.AreEqual(DataType.OhlcvAdjusted, ds.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAdjustedDay1, ds.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, ds.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Day1, ds.ParsedTimeFrame);

            item = gi.Groups[1];
            Assert.AreEqual(0, item.Datasets.Count);
            Assert.AreEqual(1, item.Groups.Count);
            Assert.AreEqual("b", item.Name);
            Assert.AreEqual("/b", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(2, item.Groups[0].Datasets.Count);
            Assert.AreEqual(0, item.Groups[0].Groups.Count);
            Assert.AreEqual(item, item.Groups[0].Parent);
            Assert.AreEqual("c", item.Groups[0].Name);
            Assert.AreEqual("/b/c", item.Groups[0].Path);
            ds = item.Groups[0].Datasets[0];
            Assert.AreEqual(item.Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("q____", ds.Name);
            Assert.AreEqual("/b/c/q____", ds.Path);
            Assert.AreEqual(DataType.Quote, ds.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, ds.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Groups[0].Datasets[1];
            Assert.AreEqual(item.Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("t____", ds.Name);
            Assert.AreEqual("/b/c/t____", ds.Path);
            Assert.AreEqual(DataType.Trade, ds.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, ds.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);

            item = gi.Groups[2];
            Assert.AreEqual(0, item.Datasets.Count);
            Assert.AreEqual(1, item.Groups.Count);
            Assert.AreEqual("d", item.Name);
            Assert.AreEqual("/d", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(0, item.Groups[0].Datasets.Count);
            Assert.AreEqual(1, item.Groups[0].Groups.Count);
            Assert.AreEqual("e", item.Groups[0].Name);
            Assert.AreEqual("/d/e", item.Groups[0].Path);
            Assert.AreEqual(item, item.Groups[0].Parent);
            Assert.AreEqual(2, item.Groups[0].Groups[0].Datasets.Count);
            Assert.AreEqual(0, item.Groups[0].Groups[0].Groups.Count);
            Assert.AreEqual("f", item.Groups[0].Groups[0].Name);
            Assert.AreEqual("/d/e/f", item.Groups[0].Groups[0].Path);
            ds = item.Groups[0].Groups[0].Datasets[0];
            Assert.AreEqual(item.Groups[0].Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("s_m03", ds.Name);
            Assert.AreEqual("/d/e/f/s_m03", ds.Path);
            Assert.AreEqual(DataType.Scalar, ds.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, ds.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, ds.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, ds.ParsedTimeFrame);
            ds = item.Groups[0].Groups[0].Datasets[1];
            Assert.AreEqual(item.Groups[0].Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("T____", ds.Name);
            Assert.AreEqual("/d/e/f/T____", ds.Path);
            Assert.AreEqual(DataType.TradePriceOnly, ds.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, ds.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_ContentTree_Sorted_EnumeratesContentCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            GroupInfo gi;
            using (var file = File.OpenReadOnly(fileName))
            {
                gi = file.ContentTree(true);
            }

            DeleteTestFile(fileName);

            Assert.AreEqual("/", gi.Name);
            Assert.AreEqual("", gi.Path);
            Assert.IsNull(gi.Parent);
            Assert.AreEqual(0, gi.Datasets.Count);
            Assert.AreEqual(3, gi.Groups.Count);

            var item = gi.Groups[0];
            Assert.AreEqual("a", item.Name);
            Assert.AreEqual("/a", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(2, item.Datasets.Count);
            Assert.AreEqual(0, item.Groups.Count);
            var ds = item.Datasets[0];
            Assert.AreEqual(item, ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("c____", ds.Name);
            Assert.AreEqual("/a/c____", ds.Path);
            Assert.AreEqual(DataType.Ohlcv, ds.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, ds.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Datasets[1];
            Assert.AreEqual(item, ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("j_d01", ds.Name);
            Assert.AreEqual("/a/j_d01", ds.Path);
            Assert.AreEqual(DataType.OhlcvAdjusted, ds.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAdjustedDay1, ds.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, ds.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Day1, ds.ParsedTimeFrame);

            item = gi.Groups[1];
            Assert.AreEqual(0, item.Datasets.Count);
            Assert.AreEqual(1, item.Groups.Count);
            Assert.AreEqual("b", item.Name);
            Assert.AreEqual("/b", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(2, item.Groups[0].Datasets.Count);
            Assert.AreEqual(0, item.Groups[0].Groups.Count);
            Assert.AreEqual(item, item.Groups[0].Parent);
            Assert.AreEqual("c", item.Groups[0].Name);
            Assert.AreEqual("/b/c", item.Groups[0].Path);
            ds = item.Groups[0].Datasets[0];
            Assert.AreEqual(item.Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("q____", ds.Name);
            Assert.AreEqual("/b/c/q____", ds.Path);
            Assert.AreEqual(DataType.Quote, ds.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, ds.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Groups[0].Datasets[1];
            Assert.AreEqual(item.Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("t____", ds.Name);
            Assert.AreEqual("/b/c/t____", ds.Path);
            Assert.AreEqual(DataType.Trade, ds.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, ds.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);

            item = gi.Groups[2];
            Assert.AreEqual(0, item.Datasets.Count);
            Assert.AreEqual(1, item.Groups.Count);
            Assert.AreEqual("d", item.Name);
            Assert.AreEqual("/d", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(0, item.Groups[0].Datasets.Count);
            Assert.AreEqual(1, item.Groups[0].Groups.Count);
            Assert.AreEqual("e", item.Groups[0].Name);
            Assert.AreEqual("/d/e", item.Groups[0].Path);
            Assert.AreEqual(item, item.Groups[0].Parent);
            Assert.AreEqual(2, item.Groups[0].Groups[0].Datasets.Count);
            Assert.AreEqual(0, item.Groups[0].Groups[0].Groups.Count);
            Assert.AreEqual("f", item.Groups[0].Groups[0].Name);
            Assert.AreEqual("/d/e/f", item.Groups[0].Groups[0].Path);
            ds = item.Groups[0].Groups[0].Datasets[0];
            Assert.AreEqual(item.Groups[0].Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("T____", ds.Name);
            Assert.AreEqual("/d/e/f/T____", ds.Path);
            Assert.AreEqual(DataType.TradePriceOnly, ds.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, ds.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Groups[0].Groups[0].Datasets[1];
            Assert.AreEqual(item.Groups[0].Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("s_m03", ds.Name);
            Assert.AreEqual("/d/e/f/s_m03", ds.Path);
            Assert.AreEqual(DataType.Scalar, ds.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, ds.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, ds.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, ds.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_ContentTreeStatic_Unsorted_EnumeratesContentCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            GroupInfo gi = File.ContentTree(fileName, false);

            DeleteTestFile(fileName);

            Assert.AreEqual("/", gi.Name);
            Assert.AreEqual("", gi.Path);
            Assert.IsNull(gi.Parent);
            Assert.AreEqual(0, gi.Datasets.Count);
            Assert.AreEqual(3, gi.Groups.Count);

            var item = gi.Groups[0];
            Assert.AreEqual("a", item.Name);
            Assert.AreEqual("/a", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(2, item.Datasets.Count);
            Assert.AreEqual(0, item.Groups.Count);
            var ds = item.Datasets[0];
            Assert.AreEqual(item, ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("c____", ds.Name);
            Assert.AreEqual("/a/c____", ds.Path);
            Assert.AreEqual(DataType.Ohlcv, ds.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, ds.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Datasets[1];
            Assert.AreEqual(item, ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("j_d01", ds.Name);
            Assert.AreEqual("/a/j_d01", ds.Path);
            Assert.AreEqual(DataType.OhlcvAdjusted, ds.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAdjustedDay1, ds.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, ds.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Day1, ds.ParsedTimeFrame);

            item = gi.Groups[1];
            Assert.AreEqual(0, item.Datasets.Count);
            Assert.AreEqual(1, item.Groups.Count);
            Assert.AreEqual("b", item.Name);
            Assert.AreEqual("/b", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(2, item.Groups[0].Datasets.Count);
            Assert.AreEqual(0, item.Groups[0].Groups.Count);
            Assert.AreEqual(item, item.Groups[0].Parent);
            Assert.AreEqual("c", item.Groups[0].Name);
            Assert.AreEqual("/b/c", item.Groups[0].Path);
            ds = item.Groups[0].Datasets[0];
            Assert.AreEqual(item.Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("q____", ds.Name);
            Assert.AreEqual("/b/c/q____", ds.Path);
            Assert.AreEqual(DataType.Quote, ds.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, ds.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Groups[0].Datasets[1];
            Assert.AreEqual(item.Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("t____", ds.Name);
            Assert.AreEqual("/b/c/t____", ds.Path);
            Assert.AreEqual(DataType.Trade, ds.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, ds.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);

            item = gi.Groups[2];
            Assert.AreEqual(0, item.Datasets.Count);
            Assert.AreEqual(1, item.Groups.Count);
            Assert.AreEqual("d", item.Name);
            Assert.AreEqual("/d", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(0, item.Groups[0].Datasets.Count);
            Assert.AreEqual(1, item.Groups[0].Groups.Count);
            Assert.AreEqual("e", item.Groups[0].Name);
            Assert.AreEqual("/d/e", item.Groups[0].Path);
            Assert.AreEqual(item, item.Groups[0].Parent);
            Assert.AreEqual(2, item.Groups[0].Groups[0].Datasets.Count);
            Assert.AreEqual(0, item.Groups[0].Groups[0].Groups.Count);
            Assert.AreEqual("f", item.Groups[0].Groups[0].Name);
            Assert.AreEqual("/d/e/f", item.Groups[0].Groups[0].Path);
            ds = item.Groups[0].Groups[0].Datasets[0];
            Assert.AreEqual(item.Groups[0].Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("s_m03", ds.Name);
            Assert.AreEqual("/d/e/f/s_m03", ds.Path);
            Assert.AreEqual(DataType.Scalar, ds.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, ds.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, ds.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, ds.ParsedTimeFrame);
            ds = item.Groups[0].Groups[0].Datasets[1];
            Assert.AreEqual(item.Groups[0].Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("T____", ds.Name);
            Assert.AreEqual("/d/e/f/T____", ds.Path);
            Assert.AreEqual(DataType.TradePriceOnly, ds.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, ds.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_ContentTreeStatic_Sorted_EnumeratesContentCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            GroupInfo gi = File.ContentTree(fileName, true);

            DeleteTestFile(fileName);

            Assert.AreEqual("/", gi.Name);
            Assert.AreEqual("", gi.Path);
            Assert.IsNull(gi.Parent);
            Assert.AreEqual(0, gi.Datasets.Count);
            Assert.AreEqual(3, gi.Groups.Count);

            var item = gi.Groups[0];
            Assert.AreEqual("a", item.Name);
            Assert.AreEqual("/a", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(2, item.Datasets.Count);
            Assert.AreEqual(0, item.Groups.Count);
            var ds = item.Datasets[0];
            Assert.AreEqual(item, ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("c____", ds.Name);
            Assert.AreEqual("/a/c____", ds.Path);
            Assert.AreEqual(DataType.Ohlcv, ds.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, ds.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Datasets[1];
            Assert.AreEqual(item, ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("j_d01", ds.Name);
            Assert.AreEqual("/a/j_d01", ds.Path);
            Assert.AreEqual(DataType.OhlcvAdjusted, ds.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAdjustedDay1, ds.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, ds.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Day1, ds.ParsedTimeFrame);

            item = gi.Groups[1];
            Assert.AreEqual(0, item.Datasets.Count);
            Assert.AreEqual(1, item.Groups.Count);
            Assert.AreEqual("b", item.Name);
            Assert.AreEqual("/b", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(2, item.Groups[0].Datasets.Count);
            Assert.AreEqual(0, item.Groups[0].Groups.Count);
            Assert.AreEqual(item, item.Groups[0].Parent);
            Assert.AreEqual("c", item.Groups[0].Name);
            Assert.AreEqual("/b/c", item.Groups[0].Path);
            ds = item.Groups[0].Datasets[0];
            Assert.AreEqual(item.Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("q____", ds.Name);
            Assert.AreEqual("/b/c/q____", ds.Path);
            Assert.AreEqual(DataType.Quote, ds.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, ds.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Groups[0].Datasets[1];
            Assert.AreEqual(item.Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("t____", ds.Name);
            Assert.AreEqual("/b/c/t____", ds.Path);
            Assert.AreEqual(DataType.Trade, ds.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, ds.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);

            item = gi.Groups[2];
            Assert.AreEqual(0, item.Datasets.Count);
            Assert.AreEqual(1, item.Groups.Count);
            Assert.AreEqual("d", item.Name);
            Assert.AreEqual("/d", item.Path);
            Assert.AreEqual(gi, item.Parent);
            Assert.AreEqual(0, item.Groups[0].Datasets.Count);
            Assert.AreEqual(1, item.Groups[0].Groups.Count);
            Assert.AreEqual("e", item.Groups[0].Name);
            Assert.AreEqual("/d/e", item.Groups[0].Path);
            Assert.AreEqual(item, item.Groups[0].Parent);
            Assert.AreEqual(2, item.Groups[0].Groups[0].Datasets.Count);
            Assert.AreEqual(0, item.Groups[0].Groups[0].Groups.Count);
            Assert.AreEqual("f", item.Groups[0].Groups[0].Name);
            Assert.AreEqual("/d/e/f", item.Groups[0].Groups[0].Path);
            ds = item.Groups[0].Groups[0].Datasets[0];
            Assert.AreEqual(item.Groups[0].Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("T____", ds.Name);
            Assert.AreEqual("/d/e/f/T____", ds.Path);
            Assert.AreEqual(DataType.TradePriceOnly, ds.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, ds.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, ds.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, ds.ParsedTimeFrame);
            ds = item.Groups[0].Groups[0].Datasets[1];
            Assert.AreEqual(item.Groups[0].Groups[0], ds.Parent);
            Assert.IsTrue(ds.IsValidName);
            Assert.AreEqual("s_m03", ds.Name);
            Assert.AreEqual("/d/e/f/s_m03", ds.Path);
            Assert.AreEqual(DataType.Scalar, ds.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, ds.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, ds.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, ds.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_EnumerateDatasets_Unsorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.EnumerateDatasets(Enumerator, false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(6, list.Count);

            Assert.AreEqual("/a/c____", list[0]);
            Assert.AreEqual("/a/j_d01", list[1]);
            Assert.AreEqual("/b/c/q____", list[2]);
            Assert.AreEqual("/b/c/t____", list[3]);
            Assert.AreEqual("/d/e/f/s_m03", list[4]);
            Assert.AreEqual("/d/e/f/T____", list[5]);
        }

        [TestMethod]
        public void H5_File_EnumerateDatasets_Sorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.EnumerateDatasets(Enumerator, true);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(6, list.Count);

            Assert.AreEqual("/a/c____", list[0]);
            Assert.AreEqual("/a/j_d01", list[1]);
            Assert.AreEqual("/b/c/q____", list[2]);
            Assert.AreEqual("/b/c/t____", list[3]);
            Assert.AreEqual("/d/e/f/T____", list[4]);
            Assert.AreEqual("/d/e/f/s_m03", list[5]);
        }

        [TestMethod]
        public void H5_File_EnumerateDatasets_Unsorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<DataInfo>();
            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.EnumerateDatasets(list, false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(6, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("c____", item.Name);
            Assert.AreEqual("/a/c____", item.Path);
            Assert.AreEqual(DataType.Ohlcv, item.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAperiodic, item.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, item.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("j_d01", item.Name);
            Assert.AreEqual("/a/j_d01", item.Path);
            Assert.AreEqual(DataType.OhlcvAdjusted, item.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAdjustedDay1, item.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, item.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Day1, item.ParsedTimeFrame);

            item = list[2];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("q____", item.Name);
            Assert.AreEqual("/b/c/q____", item.Path);
            Assert.AreEqual(DataType.Quote, item.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, item.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, item.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[3];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("t____", item.Name);
            Assert.AreEqual("/b/c/t____", item.Path);
            Assert.AreEqual(DataType.Trade, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[4];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("s_m03", item.Name);
            Assert.AreEqual("/d/e/f/s_m03", item.Path);
            Assert.AreEqual(DataType.Scalar, item.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, item.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, item.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, item.ParsedTimeFrame);

            item = list[5];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("T____", item.Name);
            Assert.AreEqual("/d/e/f/T____", item.Path);
            Assert.AreEqual(DataType.TradePriceOnly, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_EnumerateDatasets_Sorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<DataInfo>();
            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.EnumerateDatasets(list, true);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(6, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("c____", item.Name);
            Assert.AreEqual("/a/c____", item.Path);
            Assert.AreEqual(DataType.Ohlcv, item.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAperiodic, item.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, item.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("j_d01", item.Name);
            Assert.AreEqual("/a/j_d01", item.Path);
            Assert.AreEqual(DataType.OhlcvAdjusted, item.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAdjustedDay1, item.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, item.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Day1, item.ParsedTimeFrame);

            item = list[2];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("q____", item.Name);
            Assert.AreEqual("/b/c/q____", item.Path);
            Assert.AreEqual(DataType.Quote, item.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, item.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, item.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[3];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("t____", item.Name);
            Assert.AreEqual("/b/c/t____", item.Path);
            Assert.AreEqual(DataType.Trade, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[4];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("T____", item.Name);
            Assert.AreEqual("/d/e/f/T____", item.Path);
            Assert.AreEqual(DataType.TradePriceOnly, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[5];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("s_m03", item.Name);
            Assert.AreEqual("/d/e/f/s_m03", item.Path);
            Assert.AreEqual(DataType.Scalar, item.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, item.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, item.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_EnumerateDatasetsStatic_Unsorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status = File.EnumerateDatasets(fileName, Enumerator, false);

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(6, list.Count);

            Assert.AreEqual("/a/c____", list[0]);
            Assert.AreEqual("/a/j_d01", list[1]);
            Assert.AreEqual("/b/c/q____", list[2]);
            Assert.AreEqual("/b/c/t____", list[3]);
            Assert.AreEqual("/d/e/f/s_m03", list[4]);
            Assert.AreEqual("/d/e/f/T____", list[5]);
        }

        [TestMethod]
        public void H5_File_EnumerateDatasetsStatic_Sorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status = File.EnumerateDatasets(fileName, Enumerator, true);

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(6, list.Count);

            Assert.AreEqual("/a/c____", list[0]);
            Assert.AreEqual("/a/j_d01", list[1]);
            Assert.AreEqual("/b/c/q____", list[2]);
            Assert.AreEqual("/b/c/t____", list[3]);
            Assert.AreEqual("/d/e/f/T____", list[4]);
            Assert.AreEqual("/d/e/f/s_m03", list[5]);
        }

        [TestMethod]
        public void H5_File_EnumerateDatasetsStatic_Unsorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<DataInfo>();
            bool status = File.EnumerateDatasets(fileName, list, false);

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(6, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("c____", item.Name);
            Assert.AreEqual("/a/c____", item.Path);
            Assert.AreEqual(DataType.Ohlcv, item.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAperiodic, item.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, item.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("j_d01", item.Name);
            Assert.AreEqual("/a/j_d01", item.Path);
            Assert.AreEqual(DataType.OhlcvAdjusted, item.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAdjustedDay1, item.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, item.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Day1, item.ParsedTimeFrame);

            item = list[2];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("q____", item.Name);
            Assert.AreEqual("/b/c/q____", item.Path);
            Assert.AreEqual(DataType.Quote, item.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, item.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, item.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[3];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("t____", item.Name);
            Assert.AreEqual("/b/c/t____", item.Path);
            Assert.AreEqual(DataType.Trade, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[4];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("s_m03", item.Name);
            Assert.AreEqual("/d/e/f/s_m03", item.Path);
            Assert.AreEqual(DataType.Scalar, item.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, item.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, item.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, item.ParsedTimeFrame);

            item = list[5];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("T____", item.Name);
            Assert.AreEqual("/d/e/f/T____", item.Path);
            Assert.AreEqual(DataType.TradePriceOnly, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_EnumerateDatasetsStatic_Sorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<DataInfo>();
            bool status = File.EnumerateDatasets(fileName, list, true);

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(6, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("c____", item.Name);
            Assert.AreEqual("/a/c____", item.Path);
            Assert.AreEqual(DataType.Ohlcv, item.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAperiodic, item.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, item.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("j_d01", item.Name);
            Assert.AreEqual("/a/j_d01", item.Path);
            Assert.AreEqual(DataType.OhlcvAdjusted, item.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAdjustedDay1, item.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, item.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Day1, item.ParsedTimeFrame);

            item = list[2];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("q____", item.Name);
            Assert.AreEqual("/b/c/q____", item.Path);
            Assert.AreEqual(DataType.Quote, item.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, item.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, item.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[3];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("t____", item.Name);
            Assert.AreEqual("/b/c/t____", item.Path);
            Assert.AreEqual(DataType.Trade, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[4];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("T____", item.Name);
            Assert.AreEqual("/d/e/f/T____", item.Path);
            Assert.AreEqual(DataType.TradePriceOnly, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[5];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("s_m03", item.Name);
            Assert.AreEqual("/d/e/f/s_m03", item.Path);
            Assert.AreEqual(DataType.Scalar, item.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, item.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, item.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_EnumerateInstrumentDatasets_Unsorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.EnumerateDatasets(Enumerator, "/b/c", false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            Assert.AreEqual("/q____", list[0]);
            Assert.AreEqual("/t____", list[1]);
        }

        [TestMethod]
        public void H5_File_EnumerateInstrumentDatasets_Sorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.EnumerateDatasets(Enumerator, "/b/c", true);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            Assert.AreEqual("/q____", list[0]);
            Assert.AreEqual("/t____", list[1]);
        }

        [TestMethod]
        public void H5_File_EnumerateInstrumentDatasets_Unsorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<DataInfo>();
            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.EnumerateDatasets(list, "/b/c", false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("q____", item.Name);
            Assert.AreEqual("/q____", item.Path);
            Assert.AreEqual(DataType.Quote, item.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, item.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, item.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("t____", item.Name);
            Assert.AreEqual("/t____", item.Path);
            Assert.AreEqual(DataType.Trade, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_EnumerateInstrumentDatasets_Sorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<DataInfo>();
            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.EnumerateDatasets(list, "/b/c", true);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("q____", item.Name);
            Assert.AreEqual("/q____", item.Path);
            Assert.AreEqual(DataType.Quote, item.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, item.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, item.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("t____", item.Name);
            Assert.AreEqual("/t____", item.Path);
            Assert.AreEqual(DataType.Trade, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_EnumerateInstrumentDatasetsStatic_Unsorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status = File.EnumerateDatasets(fileName, Enumerator, "/b/c", false);

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            Assert.AreEqual("/q____", list[0]);
            Assert.AreEqual("/t____", list[1]);
        }

        [TestMethod]
        public void H5_File_EnumerateInstrumentDatasetsStatic_Sorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status = File.EnumerateDatasets(fileName, Enumerator, "/b/c", true);

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            Assert.AreEqual("/q____", list[0]);
            Assert.AreEqual("/t____", list[1]);
        }

        [TestMethod]
        public void H5_File_EnumerateInstrumentDatasetsStatic_Unsorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<DataInfo>();
            bool status = File.EnumerateDatasets(fileName, list, "/b/c", false);

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("q____", item.Name);
            Assert.AreEqual("/q____", item.Path);
            Assert.AreEqual(DataType.Quote, item.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, item.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, item.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("t____", item.Name);
            Assert.AreEqual("/t____", item.Path);
            Assert.AreEqual(DataType.Trade, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_EnumerateInstrumentDatasetsStatic_Sorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<DataInfo>();
            bool status = File.EnumerateDatasets(fileName, list, "/b/c", true);

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("q____", item.Name);
            Assert.AreEqual("/q____", item.Path);
            Assert.AreEqual(DataType.Quote, item.ParsedDataType);
            Assert.AreEqual(DataKind.QuoteAperiodic, item.ParsedDataKind);
            Assert.AreEqual(QuoteKind.Default, item.ParsedQuoteKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("t____", item.Name);
            Assert.AreEqual("/t____", item.Path);
            Assert.AreEqual(DataType.Trade, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradeAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_DeleteObject_ReadWrite_ExistingPath_EnumeratesDatasetsCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<DataInfo>();
            bool status1, status2, status3, status4;
            using (var file = File.OpenReadWrite(fileName))
            {
                status1 = file.DeleteObject("/b/c", true);
                status2 = file.DeleteObject("/d", true);
                status3 = file.DeleteObject("/a/j_d01", true);

                status4 = file.EnumerateDatasets(list, false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status1);
            Assert.IsTrue(status2);
            Assert.IsTrue(status3);
            Assert.IsTrue(status4);
            Assert.AreEqual(1, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("c____", item.Name);
            Assert.AreEqual("/a/c____", item.Path);
            Assert.AreEqual(DataType.Ohlcv, item.ParsedDataType);
            Assert.AreEqual(DataKind.OhlcvAperiodic, item.ParsedDataKind);
            Assert.AreEqual(OhlcvKind.Default, item.ParsedOhlcvKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_File_DeleteObject_ReadWrite_ExistingPath_CannotBeOpenAgain()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            bool status, isNull;
            using (var file = File.OpenReadWrite(fileName))
            {
                status = file.DeleteObject("/a", true);

                isNull = null == file.OpenInstrument("/a", false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_File_DeleteObject_ReadOnly_ExistingPath_CannotDelete()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.DeleteObject("/a", true);
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(status);
        }

        [TestMethod]
        public void H5_File_DeleteObject_ReadWrite_NonExistingPath_CannotDelete()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            bool status;
            using (var file = File.OpenReadWrite(fileName))
            {
                status = file.DeleteObject("/foobar", true);
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(status);
        }

        [TestMethod]
        public void H5_File_MoveInstrument_ReadWrite_ExistingPath_EnumeratesDatasetsCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status1, status2, status3;
            using (var file = File.OpenReadWrite(fileName))
            {
                status1 = file.MoveInstrument("/b/c", "/foo");
                status2 = file.MoveInstrument("/d/e/f", "/bar");

                status3 = file.EnumerateDatasets(Enumerator, false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status1);
            Assert.IsTrue(status2);
            Assert.IsTrue(status3);
            Assert.AreEqual(6, list.Count);

            Assert.AreEqual("/a/c____", list[0]);
            Assert.AreEqual("/a/j_d01", list[1]);
            Assert.AreEqual("/foo/q____", list[2]);
            Assert.AreEqual("/foo/t____", list[3]);
            Assert.AreEqual("/bar/s_m03", list[4]);
            Assert.AreEqual("/bar/T____", list[5]);
        }

        [TestMethod]
        public void H5_File_MoveInstrument_ReadWrite_ExistingPath_CannotBeOpenAgain()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            bool status, isNull;
            using (var file = File.OpenReadWrite(fileName))
            {
                status = file.MoveInstrument("/a", "/foo/bar");

                isNull = null == file.OpenInstrument("/a", false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_File_MoveInstrument_ReadOnly_ExistingPath_CannotMove()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.MoveInstrument("/a", "/foo/bar");
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(status);
        }

        [TestMethod]
        public void H5_File_MoveInstrument_ReadWrite_NonExistingPath_CannotMove()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            bool status;
            using (var file = File.OpenReadWrite(fileName))
            {
                status = file.MoveInstrument("/foobar", "/foo/bar");
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(status);
        }

        [TestMethod]
        public void H5_File_RenameInstrument_ReadWrite_ExistingPath_EnumeratesDatasetsCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(path);
            }

            bool status1, status2, status3;
            using (var file = File.OpenReadWrite(fileName))
            {
                status1 = file.RenameInstrument("/b/c", "foo");
                status2 = file.RenameInstrument("/d/e/f", "bar");

                status3 = file.EnumerateDatasets(Enumerator, false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status1);
            Assert.IsTrue(status2);
            Assert.IsTrue(status3);
            Assert.AreEqual(6, list.Count);

            Assert.AreEqual("/a/c____", list[0]);
            Assert.AreEqual("/a/j_d01", list[1]);
            Assert.AreEqual("/b/foo/q____", list[2]);
            Assert.AreEqual("/b/foo/t____", list[3]);
            Assert.AreEqual("/d/e/bar/s_m03", list[4]);
            Assert.AreEqual("/d/e/bar/T____", list[5]);
        }

        [TestMethod]
        public void H5_File_RenameInstrument_ReadWrite_ExistingPath_CannotBeOpenAgain()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            bool status, isNull;
            using (var file = File.OpenReadWrite(fileName))
            {
                status = file.RenameInstrument("/a", "foobar");

                isNull = null == file.OpenInstrument("/a", false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_File_RenameInstrument_ReadOnly_ExistingPath_CannotRename()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                status = file.RenameInstrument("/a", "foobar");
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(status);
        }

        [TestMethod]
        public void H5_File_RenameInstrument_ReadWrite_NonExistingPath_CannotRename()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            bool status;
            using (var file = File.OpenReadWrite(fileName))
            {
                status = file.RenameInstrument("/foo/bar", "foobar");
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(status);
        }
    }
}
