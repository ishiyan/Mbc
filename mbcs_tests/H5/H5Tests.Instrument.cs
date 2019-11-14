using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Mbcs.H5;

namespace Mbcs.Tests.H5
{
    public partial class H5Tests
    {
        [TestMethod]
        public void H5_Instrument_Opened_FilePathProperty_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            string filePath;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                filePath = instrument.FilePath;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(fileName, filePath);
        }

        [TestMethod]
        public void H5_Instrument_Opened_InstrumentPathProperty_HasCorrectValue()
        {
            const string expectedInstrumentPath = "foo/bar";
            string fileName = NewTestFileName;
            string instrumentPath;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(expectedInstrumentPath, true);
                instrumentPath = instrument.InstrumentPath;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(expectedInstrumentPath, instrumentPath);
        }

        [TestMethod]
        public void H5_Instrument_Opened_IsReadOnlyProperty_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            bool isReadOnlyFalse, isReadOnlyTrue;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foobar", true);
                isReadOnlyFalse = instrument.IsReadOnly;
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foobar", false);
                isReadOnlyTrue = instrument.IsReadOnly;
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isReadOnlyFalse);
            Assert.IsTrue(isReadOnlyTrue);
        }

        [TestMethod]
        public void H5_Instrument_Closed_InstrumentHandleIsUnusable()
        {
            string fileName = NewTestFileName;
            bool status;
            List<DataInfo> list = new List<DataInfo>();

            using (var file = File.OpenReadWrite(fileName))
            {
                var instrument = file.OpenInstrument("foobar", true);
                instrument.Flush();
                instrument.Close();

                status = instrument.EnumerateDatasets(list, false);
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(status);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_Instrument_EnumerateDatasets_Unsorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(name);
            }

            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("/d/e/f", false);
                status = instrument.EnumerateDatasets(Enumerator, false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            Assert.AreEqual("s_m03", list[0]);
            Assert.AreEqual("T____", list[1]);
        }

        [TestMethod]
        public void H5_Instrument_EnumerateDatasets_Sorted_Callback_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<string>();
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(name);
            }

            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("/d/e/f", false);
                status = instrument.EnumerateDatasets(Enumerator, true);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            Assert.AreEqual("T____", list[0]);
            Assert.AreEqual("s_m03", list[1]);
        }

        [TestMethod]
        public void H5_Instrument_EnumerateDatasets_Unsorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasets(fileName);

            var list = new List<DataInfo>();
            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("/d/e/f", false);
                status = instrument.EnumerateDatasets(list, false);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("s_m03", item.Name);
            Assert.AreEqual("/s_m03", item.Path);
            Assert.AreEqual(DataType.Scalar, item.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, item.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, item.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("T____", item.Name);
            Assert.AreEqual("/T____", item.Path);
            Assert.AreEqual(DataType.TradePriceOnly, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_Instrument_EnumerateDatasets_Sorted_List_EnumeratesCorrectly()
        {
            string fileName = NewTestFileName;
            CreateEnumerationTestDatasetsInReversedOrder(fileName);

            var list = new List<DataInfo>();
            bool status;
            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("/d/e/f", false);
                status = instrument.EnumerateDatasets(list, true);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(2, list.Count);

            var item = list[0];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("T____", item.Name);
            Assert.AreEqual("/T____", item.Path);
            Assert.AreEqual(DataType.TradePriceOnly, item.ParsedDataType);
            Assert.AreEqual(DataKind.TradePriceOnlyAperiodic, item.ParsedDataKind);
            Assert.AreEqual(TradeKind.Default, item.ParsedTradeKind);
            Assert.AreEqual(DataTimeFrame.Aperiodic, item.ParsedTimeFrame);

            item = list[1];
            Assert.IsTrue(item.IsValidName);
            Assert.IsNull(item.Parent);
            Assert.AreEqual("s_m03", item.Name);
            Assert.AreEqual("/s_m03", item.Path);
            Assert.AreEqual(DataType.Scalar, item.ParsedDataType);
            Assert.AreEqual(DataKind.ScalarMinute3, item.ParsedDataKind);
            Assert.AreEqual(ScalarKind.Default, item.ParsedScalarKind);
            Assert.AreEqual(DataTimeFrame.Minute3, item.ParsedTimeFrame);
        }

        [TestMethod]
        public void H5_Instrument_OpenScalar_FileOpenedReaOnly_CreateNonExistentTrue_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenScalar(DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenScalar_FileOpenedReaOnly_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenScalar(DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenScalar_FileOpenedReadWrite_CreateNonExistentTrue_NonExistingDate_IsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenScalar(DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenScalar_FileOpenedReadWrite_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenScalar(DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenScalar_FileOpenedReadOnly_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenScalar(DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenScalar(DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenScalar_FileOpenedReadWrite_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenScalar(DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenScalar(DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTrade_FileOpenedReaOnly_CreateNonExistentTrue_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenTrade(true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTrade_FileOpenedReaOnly_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenTrade(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTrade_FileOpenedReadWrite_CreateNonExistentTrue_NonExistingDate_IsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenTrade(true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTrade_FileOpenedReadWrite_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenTrade(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTrade_FileOpenedReadOnly_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenTrade(true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenTrade(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTrade_FileOpenedReadWrite_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenTrade(true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenTrade(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTradePriceOnly_FileOpenedReaOnly_CreateNonExistentTrue_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenTradePriceOnly(true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTradePriceOnly_FileOpenedReaOnly_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenTradePriceOnly(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTradePriceOnly_FileOpenedReadWrite_CreateNonExistentTrue_NonExistingDate_IsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenTradePriceOnly(true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTradePriceOnly_FileOpenedReadWrite_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenTradePriceOnly(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTradePriceOnly_FileOpenedReadOnly_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenTradePriceOnly(true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenTradePriceOnly(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenTradePriceOnly_FileOpenedReadWrite_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenTradePriceOnly(true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenTradePriceOnly(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcv_FileOpenedReaOnly_CreateNonExistentTrue_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcv(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcv_FileOpenedReaOnly_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcv(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcv_FileOpenedReadWrite_CreateNonExistentTrue_NonExistingDate_IsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcv(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcv_FileOpenedReadWrite_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcv(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcv_FileOpenedReadOnly_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenOhlcv(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcv(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcv_FileOpenedReadWrite_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenOhlcv(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcv(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvPriceOnly_FileOpenedReaOnly_CreateNonExistentTrue_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvPriceOnly_FileOpenedReaOnly_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvPriceOnly_FileOpenedReadWrite_CreateNonExistentTrue_NonExistingDate_IsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvPriceOnly_FileOpenedReadWrite_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvPriceOnly_FileOpenedReadOnly_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvPriceOnly_FileOpenedReadWrite_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjusted_FileOpenedReaOnly_CreateNonExistentTrue_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcvAdjusted(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjusted_FileOpenedReaOnly_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcvAdjusted(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjusted_FileOpenedReadWrite_CreateNonExistentTrue_NonExistingDate_IsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcvAdjusted(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjusted_FileOpenedReadWrite_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcvAdjusted(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjusted_FileOpenedReadOnly_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenOhlcvAdjusted(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcvAdjusted(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjusted_FileOpenedReadWrite_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenOhlcvAdjusted(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcvAdjusted(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjustedPriceOnly_FileOpenedReaOnly_CreateNonExistentTrue_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcvAdjustedPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjustedPriceOnly_FileOpenedReaOnly_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcvAdjustedPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjustedPriceOnly_FileOpenedReadWrite_CreateNonExistentTrue_NonExistingDate_IsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcvAdjustedPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjustedPriceOnly_FileOpenedReadWrite_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcvAdjustedPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjustedPriceOnly_FileOpenedReadOnly_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenOhlcvAdjustedPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenOhlcvAdjustedPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenOhlcvAdjustedPriceOnly_FileOpenedReadWrite_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenOhlcvAdjustedPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenOhlcvAdjustedPriceOnly(OhlcvKind.Ask, DataTimeFrame.Hour3, false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuote_FileOpenedReaOnly_CreateNonExistentTrue_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenQuote(true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuote_FileOpenedReaOnly_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenQuote(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuote_FileOpenedReadWrite_CreateNonExistentTrue_NonExistingDate_IsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenQuote(true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuote_FileOpenedReadWrite_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenQuote(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuote_FileOpenedReadOnly_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenQuote(true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenQuote(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuote_FileOpenedReadWrite_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenQuote(true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenQuote(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuotePriceOnly_FileOpenedReaOnly_CreateNonExistentTrue_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenQuotePriceOnly(true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuotePriceOnly_FileOpenedReaOnly_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenQuotePriceOnly(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuotePriceOnly_FileOpenedReadWrite_CreateNonExistentTrue_NonExistingDate_IsCreated()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenQuotePriceOnly(true);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuotePriceOnly_FileOpenedReadWrite_CreateNonExistentFalse_NonExistingData_CannotBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenQuotePriceOnly(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuotePriceOnly_FileOpenedReadOnly_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenQuotePriceOnly(true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", false);
                var data = instrument.OpenQuotePriceOnly(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }

        [TestMethod]
        public void H5_Instrument_OpenQuotePriceOnly_FileOpenedReadWrite_ExistingData_CanBeOpened()
        {
            string fileName = NewTestFileName;
            bool isNull;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                using var data = instrument.OpenQuotePriceOnly(true);
                data.Close();
                instrument.Close();
                file.Close();
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument("foo/bar", true);
                var data = instrument.OpenQuotePriceOnly(false);
                isNull = data == null;
                data?.Close();
                instrument.Close();
                file.Close();
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isNull);
        }
    }
}
