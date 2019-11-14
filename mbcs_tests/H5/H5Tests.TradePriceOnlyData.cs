using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Mbcs.H5;

namespace Mbcs.Tests.H5
{
    public partial class H5Tests
    {
        private const string TradePriceOnlyDataInstrument = "foo/bar";

        [TestMethod]
        public void H5_TradePriceOnlyData_MaximumReadBufferBytes_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            ulong value;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                value = data.MaximumReadBufferBytes;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(1024UL * 1024UL, value);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_MaximumReadBufferBytes_WhenSet_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            var expectedValue = 42UL;
            ulong newValue;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                data.MaximumReadBufferBytes = expectedValue;
                newValue = data.MaximumReadBufferBytes;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(expectedValue, newValue);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Opened_FilePathProperty_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            string filePath;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                filePath = data.FilePath;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(fileName, filePath);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Opened_InstrumentPathProperty_HasCorrectValue()
        {
            const string expectedInstrumentPath = TradePriceOnlyDataInstrument;
            string fileName = NewTestFileName;
            string instrumentPath;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(expectedInstrumentPath, true);
                using var data = instrument.OpenTradePriceOnly(true);
                instrumentPath = data.InstrumentPath;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(expectedInstrumentPath, instrumentPath);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Opened_DataPathProperty_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            string dataPath;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                dataPath = data.DataPath;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual("T____", dataPath);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Opened_IsReadOnlyProperty_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            bool isReadOnlyFalse, isReadOnlyTrue;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                isReadOnlyFalse = data.IsReadOnly;
            }

            using (var file = File.OpenReadOnly(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, false);
                using var data = instrument.OpenTradePriceOnly(false);
                isReadOnlyTrue = data.IsReadOnly;
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(isReadOnlyFalse);
            Assert.IsTrue(isReadOnlyTrue);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Closed_TradePriceOnlyDataHandleIsUnusable()
        {
            string fileName = NewTestFileName;
            bool status;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                data.Flush();
                data.Close();

                status = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsFalse(status);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_WhenEmpty_FirstTicks_IsZero()
        {
            string fileName = NewTestFileName;
            long ticks;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                ticks = data.FirstTicks;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(0L, ticks);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_WhenEmpty_LastTicks_IsZero()
        {
            string fileName = NewTestFileName;
            long ticks;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                ticks = data.LastTicks;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(0L, ticks);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_WhenEmpty_Count_IsZero()
        {
            string fileName = NewTestFileName;
            ulong countValue;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                countValue = data.Count;
            }

            DeleteTestFile(fileName);

            Assert.AreEqual(0UL, countValue);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FirstTicks_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            long ticksActual, ticksExpected = 42L;
            bool status;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new [] {
                    new TradePriceOnly { Ticks = ticksExpected },
                    new TradePriceOnly { Ticks = ticksExpected + 1 }
                }, DuplicateTimeTicks.Fail, false);

                ticksActual = data.FirstTicks;
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(ticksExpected, ticksActual);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_LastTicks_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            long ticksActual, ticksExpected = 42L;
            bool status;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = ticksExpected - 1 },
                    new TradePriceOnly { Ticks = ticksExpected }
                }, DuplicateTimeTicks.Fail, false);

                ticksActual = data.LastTicks;
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(ticksExpected, ticksActual);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Count_HasCorrectValue()
        {
            string fileName = NewTestFileName;
            ulong sampleCount;
            bool status;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 1L },
                    new TradePriceOnly { Ticks = 2L },
                    new TradePriceOnly { Ticks = 3L }
                }, DuplicateTimeTicks.Fail, false);

                sampleCount = data.Count;
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.AreEqual(3UL, sampleCount);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_SpreadDuplicateTimeTicks_TicksAreDifferent_DoesNothing()
        {
            TradePriceOnly[] array = { new TradePriceOnly { Ticks = 1001L }, new TradePriceOnly { Ticks = 1010L } };
            bool status = TradePriceOnlyData.SpreadDuplicateTimeTicks(array, false);

            Assert.IsFalse(status);
            Assert.AreEqual(1001L, array[0].Ticks);
            Assert.AreEqual(1010L, array[1].Ticks);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_SpreadDuplicateTimeTicks_TicksAreEqual_SpreadsTicks()
        {
            TradePriceOnly[] array = { new TradePriceOnly { Ticks = 1001L }, new TradePriceOnly { Ticks = 1001L } };
            bool status = TradePriceOnlyData.SpreadDuplicateTimeTicks(array, false);

            Assert.IsTrue(status);
            Assert.AreEqual(1001L, array[0].Ticks);
            Assert.AreEqual(1002L, array[1].Ticks);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Add_DifferentTicks_DuplicatePolicyFail_Success()
        {
            string fileName = NewTestFileName;
            ulong countFirst, countSecond;
            bool statusFirst, statusSecond;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                statusFirst = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L },
                    new TradePriceOnly { Ticks = 4L },
                    new TradePriceOnly { Ticks = 6L }
                }, DuplicateTimeTicks.Fail, false);
                countFirst = data.Count;

                statusSecond = data.Add(new[] {
                    new TradePriceOnly { Ticks = 1L },
                    new TradePriceOnly { Ticks = 3L },
                    new TradePriceOnly { Ticks = 5L },
                    new TradePriceOnly { Ticks = 7L }
                }, DuplicateTimeTicks.Fail, false);
                countSecond = data.Count;
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(statusFirst);
            Assert.IsTrue(statusSecond);
            Assert.AreEqual(3UL, countFirst);
            Assert.AreEqual(7UL, countSecond);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Add_DifferentTicks_DuplicatePolicySkip_Success()
        {
            string fileName = NewTestFileName;
            ulong countFirst, countSecond;
            bool statusFirst, statusSecond;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                statusFirst = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L },
                    new TradePriceOnly { Ticks = 4L },
                    new TradePriceOnly { Ticks = 6L }
                }, DuplicateTimeTicks.Skip, false);
                countFirst = data.Count;

                statusSecond = data.Add(new[] {
                    new TradePriceOnly { Ticks = 1L },
                    new TradePriceOnly { Ticks = 3L },
                    new TradePriceOnly { Ticks = 5L },
                    new TradePriceOnly { Ticks = 7L }
                }, DuplicateTimeTicks.Skip, false);
                countSecond = data.Count;
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(statusFirst);
            Assert.IsTrue(statusSecond);
            Assert.AreEqual(3UL, countFirst);
            Assert.AreEqual(7UL, countSecond);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Add_DifferentTicks_DuplicatePolicyUpdate_Success()
        {
            string fileName = NewTestFileName;
            ulong countFirst, countSecond;
            bool statusFirst, statusSecond;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                statusFirst = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L },
                    new TradePriceOnly { Ticks = 4L },
                    new TradePriceOnly { Ticks = 6L }
                }, DuplicateTimeTicks.Update, false);
                countFirst = data.Count;

                statusSecond = data.Add(new[] {
                    new TradePriceOnly { Ticks = 1L },
                    new TradePriceOnly { Ticks = 3L },
                    new TradePriceOnly { Ticks = 5L },
                    new TradePriceOnly { Ticks = 7L }
                }, DuplicateTimeTicks.Update, false);
                countSecond = data.Count;
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(statusFirst);
            Assert.IsTrue(statusSecond);
            Assert.AreEqual(3UL, countFirst);
            Assert.AreEqual(7UL, countSecond);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Add_AllEqualTicks_DuplicatePolicyFail_Failure()
        {
            string fileName = NewTestFileName;
            ulong count1, count2;
            bool status1, status2, status3;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status1 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                count1 = data.Count;

                status2 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.2 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.2 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.2 }
                }, DuplicateTimeTicks.Fail, false);
                count2 = data.Count;
                status3 = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status1);
            Assert.IsFalse(status2);
            Assert.IsTrue(status3);
            Assert.AreEqual(3UL, count1);
            Assert.AreEqual(3UL, count2);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(6.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Add_SomeEqualTicks_DuplicatePolicyFail_Failure()
        {
            string fileName = NewTestFileName;
            ulong count1, count2;
            bool status1, status2, status3;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status1 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                count1 = data.Count;

                status2 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 1L, Price = 1.2 },
                    new TradePriceOnly { Ticks = 2L, Price = 2.2 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.2 },
                    new TradePriceOnly { Ticks = 7L, Price = 7.2 }
                }, DuplicateTimeTicks.Fail, false);
                count2 = data.Count;
                status3 = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status1);
            Assert.IsFalse(status2);
            Assert.IsTrue(status3);
            Assert.AreEqual(3UL, count1);
            Assert.AreEqual(3UL, count2);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(6.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Add_SomeEqualTicks_DuplicatePolicySkip_Success()
        {
            string fileName = NewTestFileName;
            ulong count1, count2;
            bool status1, status2, status3;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status1 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Skip, false);
                count1 = data.Count;

                status2 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 1L, Price = 1.2 },
                    new TradePriceOnly { Ticks = 2L, Price = 2.2 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.2 },
                    new TradePriceOnly { Ticks = 7L, Price = 7.2 }
                }, DuplicateTimeTicks.Skip, false);
                count2 = data.Count;
                status3 = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status1);
            Assert.IsTrue(status2);
            Assert.IsTrue(status3);
            Assert.AreEqual(3UL, count1);
            Assert.AreEqual(5UL, count2);
            Assert.AreEqual(5, list.Count);
            Assert.AreEqual(1.2, list[0].Price);
            Assert.AreEqual(2.1, list[1].Price);
            Assert.AreEqual(4.1, list[2].Price);
            Assert.AreEqual(6.1, list[3].Price);
            Assert.AreEqual(7.2, list[4].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_Add_SomeEqualTicks_DuplicatePolicyUpdate_Success()
        {
            string fileName = NewTestFileName;
            ulong count1, count2;
            bool status1, status2, status3;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status1 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Update, false);
                count1 = data.Count;

                status2 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 1L, Price = 1.2 },
                    new TradePriceOnly { Ticks = 2L, Price = 2.2 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.2 },
                    new TradePriceOnly { Ticks = 7L, Price = 7.2 }
                }, DuplicateTimeTicks.Update, false);
                count2 = data.Count;
                status3 = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status1);
            Assert.IsTrue(status2);
            Assert.IsTrue(status3);
            Assert.AreEqual(3UL, count1);
            Assert.AreEqual(5UL, count2);
            Assert.AreEqual(5, list.Count);
            Assert.AreEqual(1.2, list[0].Price);
            Assert.AreEqual(2.2, list[1].Price);
            Assert.AreEqual(4.2, list[2].Price);
            Assert.AreEqual(6.1, list[3].Price);
            Assert.AreEqual(7.2, list[4].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchAll_Callback_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            ulong count1;
            bool status1, status2;
            var list = new List<TradePriceOnly>();

            void AppenderFunc(long ticks, double price)
            {
                list.Add(new TradePriceOnly
                {
                    Ticks = ticks,
                    Price = price
                });
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status1 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                count1 = data.Count;
                status2 = data.FetchAll(AppenderFunc);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status1);
            Assert.IsTrue(status2);
            Assert.AreEqual(3UL, count1);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(6.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchAll_List_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            ulong count1;
            bool status1, status2;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status1 = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                count1 = data.Count;
                status2 = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status1);
            Assert.IsTrue(status2);
            Assert.AreEqual(3UL, count1);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(6.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchTickRange_BothInsideRange_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchTickRange(list, new DateTime(3L), new DateTime(5L));
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(5.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchTickRange_FirstOutsideRange_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchTickRange(list, new DateTime(1L), new DateTime(5L));
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(4, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(3.1, list[1].Price);
            Assert.AreEqual(4.1, list[2].Price);
            Assert.AreEqual(5.1, list[3].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchTickRange_LastOutsideRange_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchTickRange(list, new DateTime(3L), new DateTime(7L));
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(4, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(5.1, list[2].Price);
            Assert.AreEqual(6.1, list[3].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchTickRange_BothOutsideRange_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchTickRange(list, new DateTime(1L), new DateTime(7L));
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(5, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(3.1, list[1].Price);
            Assert.AreEqual(4.1, list[2].Price);
            Assert.AreEqual(5.1, list[3].Price);
            Assert.AreEqual(6.1, list[4].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchTickRange_BothBeforeFirst_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchTickRange(list, new DateTime(1L), new DateTime(3L));
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchTickRange_BothAfterLast_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchTickRange(list, new DateTime(7L), new DateTime(9L));
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchTickRange_Appender_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();
            void AppenderFunc(long ticks, double price)
            {
                list.Add(new TradePriceOnly
                {
                    Ticks = ticks,
                    Price = price
                });
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchTickRange(AppenderFunc, new DateTime(3L), new DateTime(5L));
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(5.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchIndexRange_IndexInside_CountInside_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchIndexRange(list, 1UL, 3LU);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(5.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchIndexRange_IndexZero_CountInside_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchIndexRange(list, 0UL, 4LU);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(4, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(3.1, list[1].Price);
            Assert.AreEqual(4.1, list[2].Price);
            Assert.AreEqual(5.1, list[3].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchIndexRange_IndexInside_CountOutside_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchIndexRange(list, 1UL, 5LU);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(4, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(5.1, list[2].Price);
            Assert.AreEqual(6.1, list[3].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchIndexRange_IndexZero_CountOutside_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchIndexRange(list, 0UL, 6LU);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(5, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(3.1, list[1].Price);
            Assert.AreEqual(4.1, list[2].Price);
            Assert.AreEqual(5.1, list[3].Price);
            Assert.AreEqual(6.1, list[4].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_FetchIndexRange_Appender_CountInside_FetchesCorrectly()
        {
            string fileName = NewTestFileName;
            bool status, statusFetch;
            var list = new List<TradePriceOnly>();
            void AppenderFunc(long ticks, double price)
            {
                list.Add(new TradePriceOnly
                {
                    Ticks = ticks,
                    Price = price
                });
            }

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusFetch = data.FetchIndexRange(AppenderFunc, 1UL, 3LU);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(5.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndex_TicksInside_Match_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndex, match;
            ulong index;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndex = data.TicksIndex(3L, out index, out match);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndex);
            Assert.AreEqual(1UL, index);
            Assert.IsTrue(match);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndex_TicksInside_NoMatch_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndex, match;
            ulong index;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndex = data.TicksIndex(4L, out index, out match);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndex);
            Assert.AreEqual(1UL, index);
            Assert.IsFalse(match);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndex_TicksBeforeFirst_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndex, match;
            ulong index;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndex = data.TicksIndex(1L, out index, out match);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndex);
            Assert.AreEqual(0UL, index);
            Assert.IsTrue(match);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndex_TicksAfterLast_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndex, match;
            ulong index;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndex = data.TicksIndex(6L, out index, out match);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndex);
            Assert.AreEqual(2UL, index);
            Assert.IsTrue(match);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndexRange_BothInside_BothMatch_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndexRange, matchFrom, matchTo;
            ulong indexFrom, indexTo;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndexRange = data.TicksIndexRange(3L, 5L, out indexFrom, out indexTo, out matchFrom, out matchTo);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndexRange);
            Assert.AreEqual(1UL, indexFrom);
            Assert.AreEqual(2UL, indexTo);
            Assert.IsTrue(matchFrom);
            Assert.IsTrue(matchTo);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndexRange_BothInside_BothNoMatch_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndexRange, matchFrom, matchTo;
            ulong indexFrom, indexTo;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 1L, Price = 1.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndexRange = data.TicksIndexRange(2L, 4L, out indexFrom, out indexTo, out matchFrom, out matchTo);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndexRange);
            Assert.AreEqual(0UL, indexFrom);
            Assert.AreEqual(1UL, indexTo);
            Assert.IsFalse(matchFrom);
            Assert.IsFalse(matchTo);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndexRange_FirstOutside_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndexRange, matchFrom, matchTo;
            ulong indexFrom, indexTo;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndexRange = data.TicksIndexRange(1L, 4L, out indexFrom, out indexTo, out matchFrom, out matchTo);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndexRange);
            Assert.AreEqual(0UL, indexFrom);
            Assert.AreEqual(1UL, indexTo);
            Assert.IsTrue(matchFrom);
            Assert.IsFalse(matchTo);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndexRange_LastOutside_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndexRange, matchFrom, matchTo;
            ulong indexFrom, indexTo;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndexRange = data.TicksIndexRange(4L, 6L, out indexFrom, out indexTo, out matchFrom, out matchTo);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndexRange);
            Assert.AreEqual(1UL, indexFrom);
            Assert.AreEqual(2UL, indexTo);
            Assert.IsFalse(matchFrom);
            Assert.IsTrue(matchTo);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndexRange_BothBetweenTwoTicks_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndexRange, matchFrom, matchTo;
            ulong indexFrom, indexTo;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndexRange = data.TicksIndexRange(3L, 4L, out indexFrom, out indexTo, out matchFrom, out matchTo);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndexRange);
            Assert.AreEqual(0UL, indexFrom);
            Assert.AreEqual(0UL, indexTo);
            Assert.IsFalse(matchFrom);
            Assert.IsFalse(matchTo);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_TicksIndexRange_BothOutside_OutputIsCorrect()
        {
            string fileName = NewTestFileName;
            bool status, statusTicksIndexRange, matchFrom, matchTo;
            ulong indexFrom, indexTo;

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusTicksIndexRange = data.TicksIndexRange(1L, 6L, out indexFrom, out indexTo, out matchFrom, out matchTo);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusTicksIndexRange);
            Assert.AreEqual(0UL, indexFrom);
            Assert.AreEqual(1UL, indexTo);
            Assert.IsTrue(matchFrom);
            Assert.IsTrue(matchTo);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteIndexRange_BothInside_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteIndexRange(1UL, 3UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(6.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteIndexRange_FirstZeroLastInside_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteIndexRange(0UL, 3UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(1, list.Count);
            Assert.AreEqual(6.1, list[0].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteIndexRange_FirstInsideLastOutside_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteIndexRange(1UL, 5UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsFalse(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(5, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(3.1, list[1].Price);
            Assert.AreEqual(4.1, list[2].Price);
            Assert.AreEqual(5.1, list[3].Price);
            Assert.AreEqual(6.1, list[4].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteIndexRange_FirstZeroLastLast_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteIndexRange(0UL, 4UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteIndexRange_FirstZeroLastOutside_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteIndexRange(0UL, 5UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsFalse(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(5, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(3.1, list[1].Price);
            Assert.AreEqual(4.1, list[2].Price);
            Assert.AreEqual(5.1, list[3].Price);
            Assert.AreEqual(6.1, list[4].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_BothInside_Match_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(3L, 5L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(6.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_BothInside_NoMatch_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(3L, 5L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(6.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_BothInsideTheSame_Match_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(3L, 3L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(4, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(5.1, list[2].Price);
            Assert.AreEqual(6.1, list[3].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_BothInsideTheSame_NoMatch_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(3L, 3L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(4, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(4.1, list[1].Price);
            Assert.AreEqual(5.1, list[2].Price);
            Assert.AreEqual(6.1, list[3].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_FirstOutsideLastInside_Match_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(1L, 4L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(5.1, list[0].Price);
            Assert.AreEqual(6.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_FirstOutsideLastInside_NoMatch_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(1L, 4L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(5.1, list[0].Price);
            Assert.AreEqual(6.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_FirstInsideLastOutside_Match_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 4L, Price = 4.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(4L, 7L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(3.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_FirstInsideLastOutside_NoMatch_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(4L, 7L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(2.1, list[0].Price);
            Assert.AreEqual(3.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_FirstFirstLastLast_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(2L, 6L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteTickRange_BothOutside_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 2L, Price = 2.1 },
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteTickRange(1L, 7L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllBeforeTick_Inside_Match_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllBeforeTick(5L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(1, list.Count);
            Assert.AreEqual(6.1, list[0].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllBeforeTick_Inside_NoMatch_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllBeforeTick(4L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(5.1, list[0].Price);
            Assert.AreEqual(6.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllBeforeTick_BeforeFirst_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllBeforeTick(1L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(5.1, list[0].Price);
            Assert.AreEqual(6.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllBeforeTick_AfterLast_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllBeforeTick(7L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllBeforeIndex_Inside_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllBeforeIndex(1UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(1, list.Count);
            Assert.AreEqual(6.1, list[0].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllBeforeIndex_Zero_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllBeforeIndex(0UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(2, list.Count);
            Assert.AreEqual(5.1, list[0].Price);
            Assert.AreEqual(6.1, list[1].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllBeforeIndex_Outside_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllBeforeIndex(3UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsFalse(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
            Assert.AreEqual(5.1, list[1].Price);
            Assert.AreEqual(6.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllAfterTick_Inside_Match_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllAfterTick(5L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(1, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllAfterTick_Inside_NoMatch_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllAfterTick(4L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(1, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllAfterTick_BeforeFirst_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllAfterTick(1L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllAfterTick_AfterLast_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllAfterTick(7L);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
            Assert.AreEqual(5.1, list[1].Price);
            Assert.AreEqual(6.1, list[2].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllAfterIndex_Inside_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllAfterIndex(1UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(1, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllAfterIndex_Zero_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllAfterIndex(0UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(0, list.Count);
        }

        [TestMethod]
        public void H5_TradePriceOnlyData_DeleteAllAfterIndex_Outside_CorrectAction()
        {
            string fileName = NewTestFileName;
            bool status, statusDelete, statusFetch;
            var list = new List<TradePriceOnly>();

            using (var file = File.OpenReadWrite(fileName))
            {
                using var instrument = file.OpenInstrument(TradePriceOnlyDataInstrument, true);
                using var data = instrument.OpenTradePriceOnly(true);
                status = data.Add(new[] {
                    new TradePriceOnly { Ticks = 3L, Price = 3.1 },
                    new TradePriceOnly { Ticks = 5L, Price = 5.1 },
                    new TradePriceOnly { Ticks = 6L, Price = 6.1 }
                }, DuplicateTimeTicks.Fail, false);
                statusDelete = data.DeleteAllAfterIndex(3UL);
                statusFetch = data.FetchAll(list);
            }

            DeleteTestFile(fileName);

            Assert.IsTrue(status);
            Assert.IsTrue(statusDelete);
            Assert.IsTrue(statusFetch);
            Assert.AreEqual(3, list.Count);
            Assert.AreEqual(3.1, list[0].Price);
            Assert.AreEqual(5.1, list[1].Price);
            Assert.AreEqual(6.1, list[2].Price);
        }
    }
}
