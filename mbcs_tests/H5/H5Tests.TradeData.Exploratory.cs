using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Mbcs.H5;

namespace Mbcs.Tests.H5
{
    public partial class H5Tests
    {
        [TestMethod]
        public void H5_TradeData_Exploratory_01()
        {
            int errorCount = TradeDataExploratoryTest01();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_TradeData_Exploratory_02()
        {
            int errorCount = TradeDataExploratoryTest02();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_TradeData_Exploratory_03()
        {
            int errorCount = TradeDataExploratoryTest03();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_TradeData_Exploratory_04()
        {
            int errorCount = TradeDataExploratoryTest04();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_TradeData_Exploratory_05()
        {
            int errorCount = TradeDataExploratoryTest05();
            Assert.AreEqual(0, errorCount);
        }

        private static bool Add(List<Trade> list, TradeData data, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
        {
            Trade[] array = list.ToArray();
            TradeData.SpreadDuplicateTimeTicks(array, verbose);
            return data.Add(array, duplicateTimeTicks, verbose);
        }

        private static bool Verify(List<Trade> pattern, TradeData data)
        {
            var sample = new List<Trade>();
            if (!data.FetchAll(sample))
                return false;
            int sampleCount = sample.Count, patternCount = pattern.Count;
            if (sampleCount == patternCount)
            {
                for (int i = 0; i < sampleCount; i++)
                {
                    if (sample[i].Ticks != pattern[i].Ticks || Math.Abs(sample[i].Price - pattern[i].Price) > double.Epsilon)
                    {
                        Trace.TraceError("Verification error: index {0}", i);
                        for (int j = 0; j < sampleCount; j++)
                            Trace.TraceError("Verification error: {0} {1} ({2})<-({3})", j, j == i ? "*" : " ", sample[j], pattern[j]);
                        return false;
                    }
                }
                return true;
            }
            Trace.TraceError("Verification error: pattern length:{0}, stored data length:{1}", patternCount, sampleCount);
            int k = sampleCount;
            if (patternCount > k)
                k = patternCount;
            for (int j = 0; j < k; j++)
            {
                if (sampleCount > j && patternCount > j)
                    Trace.TraceError("Verification error: {0} ({1})<-({2})", j, sample[j], pattern[j]);
                else if (sampleCount <= j)
                    Trace.TraceError("Verification error: {0} ()<-({1})", j, pattern[j]);
                else //if (patternCount <= j)
                    Trace.TraceError("Verification error: {0} ({1})<-()", j, sample[j]);
            }
            return false;
        }

        private static bool Verify(List<Trade> pattern, List<Trade> sample)
        {
            int sampleCount = sample.Count, patternCount = pattern.Count;
            if (sampleCount == patternCount)
            {
                for (int i = 0; i < sampleCount; i++)
                {
                    if (sample[i].Ticks != pattern[i].Ticks || Math.Abs(sample[i].Price - pattern[i].Price) > double.Epsilon)
                    {
                        Trace.TraceError("Verification error: index {0}", i);
                        for (int j = 0; j < sampleCount; j++)
                            Trace.TraceError("Verification error: {0} {1} ({2})<-({3})", j, j == i ? "*" : " ", sample[j], pattern[j]);
                        return false;
                    }
                }
                return true;
            }
            Trace.TraceError("Verification error: pattern length:{0}, sample data length:{1}", patternCount, sampleCount);
            int k = sampleCount;
            if (patternCount > k)
                k = patternCount;
            for (int j = 0; j < k; j++)
            {
                if (sampleCount > j && patternCount > j)
                    Trace.TraceError("Verification error: {0} ({1})<-({2})", j, sample[j], pattern[j]);
                else if (sampleCount <= j)
                    Trace.TraceError("Verification error: {0} ()<-({1})", j, pattern[j]);
                else //if (patternCount <= j)
                    Trace.TraceError("Verification error: {0} ({1})<-()", j, sample[j]);
            }
            return false;
        }

        private static int TradeDataExploratoryTest01()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using TradeData data = instrument.OpenTrade(true);

                #region (1) Initial append
                Trace.TraceInformation("--- 01 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                bool verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (2) Update / Update
                Trace.TraceInformation("--- 01 (2) Update/Update ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.01 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.01 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 130, Price = 1.31 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.31 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.51 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.51 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (3) Update / Skip
                Trace.TraceInformation("--- 01 (3) Update/Skip ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.01 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 130, Price = 1.31 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.51 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (4) Update / Fail
                Trace.TraceInformation("--- 01 (4) Update/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.01 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 130, Price = 1.31 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade> {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.51 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                #endregion

                #region (5) Prepend
                Trace.TraceInformation("--- 01 (5) Prepend/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 97, Price = 0.97 },
                    new Trade { Ticks = 98, Price = 0.98 },
                    new Trade { Ticks = 99, Price = 0.99 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 97, Price = 0.97 },
                    new Trade { Ticks = 98, Price = 0.98 },
                    new Trade { Ticks = 99, Price = 0.99 },
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 94, Price = 0.94 },
                    new Trade { Ticks = 94, Price = 0.95 },
                    new Trade { Ticks = 94, Price = 0.96 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 94, Price = 0.94 },
                    new Trade { Ticks = 95, Price = 0.95 },
                    new Trade { Ticks = 96, Price = 0.96 },
                    new Trade { Ticks = 97, Price = 0.97 },
                    new Trade { Ticks = 98, Price = 0.98 },
                    new Trade { Ticks = 99, Price = 0.99 },
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 90, Price = 0.90 },
                    new Trade { Ticks = 90, Price = 0.91 },
                    new Trade { Ticks = 90, Price = 0.92 },
                    new Trade { Ticks = 93, Price = 0.93 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 90, Price = 0.90 },
                    new Trade { Ticks = 91, Price = 0.91 },
                    new Trade { Ticks = 92, Price = 0.92 },
                    new Trade { Ticks = 93, Price = 0.93 },
                    new Trade { Ticks = 94, Price = 0.94 },
                    new Trade { Ticks = 95, Price = 0.95 },
                    new Trade { Ticks = 96, Price = 0.96 },
                    new Trade { Ticks = 97, Price = 0.97 },
                    new Trade { Ticks = 98, Price = 0.98 },
                    new Trade { Ticks = 99, Price = 0.99 },
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 87, Price = 0.87 },
                    new Trade { Ticks = 88, Price = 0.88 },
                    new Trade { Ticks = 88, Price = 0.89 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 87, Price = 0.87 },
                    new Trade { Ticks = 88, Price = 0.88 },
                    new Trade { Ticks = 89, Price = 0.89 },
                    new Trade { Ticks = 90, Price = 0.90 },
                    new Trade { Ticks = 91, Price = 0.91 },
                    new Trade { Ticks = 92, Price = 0.92 },
                    new Trade { Ticks = 93, Price = 0.93 },
                    new Trade { Ticks = 94, Price = 0.94 },
                    new Trade { Ticks = 95, Price = 0.95 },
                    new Trade { Ticks = 96, Price = 0.96 },
                    new Trade { Ticks = 97, Price = 0.97 },
                    new Trade { Ticks = 98, Price = 0.98 },
                    new Trade { Ticks = 99, Price = 0.99 },
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (6) Append
                Trace.TraceInformation("--- 01 (6) Append/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 151, Price = 1.51 },
                    new Trade { Ticks = 151, Price = 1.52 },
                    new Trade { Ticks = 151, Price = 1.53 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 87, Price = 0.87 },
                    new Trade { Ticks = 88, Price = 0.88 },
                    new Trade { Ticks = 89, Price = 0.89 },
                    new Trade { Ticks = 90, Price = 0.90 },
                    new Trade { Ticks = 91, Price = 0.91 },
                    new Trade { Ticks = 92, Price = 0.92 },
                    new Trade { Ticks = 93, Price = 0.93 },
                    new Trade { Ticks = 94, Price = 0.94 },
                    new Trade { Ticks = 95, Price = 0.95 },
                    new Trade { Ticks = 96, Price = 0.96 },
                    new Trade { Ticks = 97, Price = 0.97 },
                    new Trade { Ticks = 98, Price = 0.98 },
                    new Trade { Ticks = 99, Price = 0.99 },
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 150, Price = 1.5 },
                    new Trade { Ticks = 151, Price = 1.51 },
                    new Trade { Ticks = 152, Price = 1.52 },
                    new Trade { Ticks = 153, Price = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (7) Insert
                Trace.TraceInformation("--- 01 (7) Insert/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 131, Price = 1.31 },
                    new Trade { Ticks = 132, Price = 1.32 },
                    new Trade { Ticks = 133, Price = 1.33 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 87, Price = 0.87 },
                    new Trade { Ticks = 88, Price = 0.88 },
                    new Trade { Ticks = 89, Price = 0.89 },
                    new Trade { Ticks = 90, Price = 0.90 },
                    new Trade { Ticks = 91, Price = 0.91 },
                    new Trade { Ticks = 92, Price = 0.92 },
                    new Trade { Ticks = 93, Price = 0.93 },
                    new Trade { Ticks = 94, Price = 0.94 },
                    new Trade { Ticks = 95, Price = 0.95 },
                    new Trade { Ticks = 96, Price = 0.96 },
                    new Trade { Ticks = 97, Price = 0.97 },
                    new Trade { Ticks = 98, Price = 0.98 },
                    new Trade { Ticks = 99, Price = 0.99 },
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 131, Price = 1.31 },
                    new Trade { Ticks = 132, Price = 1.32 },
                    new Trade { Ticks = 133, Price = 1.33 },
                    new Trade { Ticks = 150, Price = 1.5 },
                    new Trade { Ticks = 151, Price = 1.51 },
                    new Trade { Ticks = 152, Price = 1.52 },
                    new Trade { Ticks = 153, Price = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 127, Price = 1.27 },
                    new Trade { Ticks = 128, Price = 1.28 },
                    new Trade { Ticks = 129, Price = 1.29 },
                    new Trade { Ticks = 134, Price = 1.34 },
                    new Trade { Ticks = 135, Price = 1.35 },
                    new Trade { Ticks = 136, Price = 1.36 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 87, Price = 0.87 },
                    new Trade { Ticks = 88, Price = 0.88 },
                    new Trade { Ticks = 89, Price = 0.89 },
                    new Trade { Ticks = 90, Price = 0.90 },
                    new Trade { Ticks = 91, Price = 0.91 },
                    new Trade { Ticks = 92, Price = 0.92 },
                    new Trade { Ticks = 93, Price = 0.93 },
                    new Trade { Ticks = 94, Price = 0.94 },
                    new Trade { Ticks = 95, Price = 0.95 },
                    new Trade { Ticks = 96, Price = 0.96 },
                    new Trade { Ticks = 97, Price = 0.97 },
                    new Trade { Ticks = 98, Price = 0.98 },
                    new Trade { Ticks = 99, Price = 0.99 },
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 127, Price = 1.27 },
                    new Trade { Ticks = 128, Price = 1.28 },
                    new Trade { Ticks = 129, Price = 1.29 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 131, Price = 1.31 },
                    new Trade { Ticks = 132, Price = 1.32 },
                    new Trade { Ticks = 133, Price = 1.33 },
                    new Trade { Ticks = 134, Price = 1.34 },
                    new Trade { Ticks = 135, Price = 1.35 },
                    new Trade { Ticks = 136, Price = 1.36 },
                    new Trade { Ticks = 150, Price = 1.5 },
                    new Trade { Ticks = 151, Price = 1.51 },
                    new Trade { Ticks = 152, Price = 1.52 },
                    new Trade { Ticks = 153, Price = 1.53 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 146, Price = 1.46 },
                    new Trade { Ticks = 147, Price = 1.47 },
                    new Trade { Ticks = 148, Price = 1.48 },
                    new Trade { Ticks = 149, Price = 1.49 },
                    new Trade { Ticks = 150, Price = 1.50 },
                    new Trade { Ticks = 153, Price = 1.53 },
                    new Trade { Ticks = 154, Price = 1.54 },
                    new Trade { Ticks = 155, Price = 1.55 },
                    new Trade { Ticks = 156, Price = 1.56 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 87, Price = 0.87 },
                    new Trade { Ticks = 88, Price = 0.88 },
                    new Trade { Ticks = 89, Price = 0.89 },
                    new Trade { Ticks = 90, Price = 0.90 },
                    new Trade { Ticks = 91, Price = 0.91 },
                    new Trade { Ticks = 92, Price = 0.92 },
                    new Trade { Ticks = 93, Price = 0.93 },
                    new Trade { Ticks = 94, Price = 0.94 },
                    new Trade { Ticks = 95, Price = 0.95 },
                    new Trade { Ticks = 96, Price = 0.96 },
                    new Trade { Ticks = 97, Price = 0.97 },
                    new Trade { Ticks = 98, Price = 0.98 },
                    new Trade { Ticks = 99, Price = 0.99 },
                    new Trade { Ticks = 100, Price = 1.0 },
                    new Trade { Ticks = 110, Price = 1.1 },
                    new Trade { Ticks = 127, Price = 1.27 },
                    new Trade { Ticks = 128, Price = 1.28 },
                    new Trade { Ticks = 129, Price = 1.29 },
                    new Trade { Ticks = 130, Price = 1.3 },
                    new Trade { Ticks = 131, Price = 1.31 },
                    new Trade { Ticks = 132, Price = 1.32 },
                    new Trade { Ticks = 133, Price = 1.33 },
                    new Trade { Ticks = 134, Price = 1.34 },
                    new Trade { Ticks = 135, Price = 1.35 },
                    new Trade { Ticks = 136, Price = 1.36 },
                    new Trade { Ticks = 146, Price = 1.46 },
                    new Trade { Ticks = 147, Price = 1.47 },
                    new Trade { Ticks = 148, Price = 1.48 },
                    new Trade { Ticks = 149, Price = 1.49 },
                    new Trade { Ticks = 150, Price = 1.5 },
                    new Trade { Ticks = 151, Price = 1.51 },
                    new Trade { Ticks = 152, Price = 1.52 },
                    new Trade { Ticks = 153, Price = 1.53 },
                    new Trade { Ticks = 154, Price = 1.54 },
                    new Trade { Ticks = 155, Price = 1.55 },
                    new Trade { Ticks = 156, Price = 1.56 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion
            }

            DeleteTestFile(fileName);

            if (0 == errorCount)
                Trace.TraceInformation("--- All tests passed ---");
            else
                Trace.TraceInformation("--- {0} tests failed ---", errorCount);
            return errorCount;
        }

        private static int TradeDataExploratoryTest02()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using TradeData data = instrument.OpenTrade(true);

                #region (1) Initial append

                Trace.TraceInformation("--- 02 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);

                #endregion

                #region (2)

                Trace.TraceInformation("--- 02 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 700, Price = 0.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1700, Price = 1.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1700, Price = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1650, Price = 1.65 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1700, Price = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1700, Price = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 780, Price = 0.78 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1700, Price = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 750, Price = 0.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1700, Price = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1700, Price = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 710, Price = 0.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 710, Price = 0.71 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1700, Price = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1690, Price = 1.69 },
                    new Trade { Ticks = 1710, Price = 1.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 710, Price = 0.71 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1690, Price = 1.69 },
                    new Trade { Ticks = 1700, Price = 1.7 },
                    new Trade { Ticks = 1710, Price = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 670, Price = 0.67 },
                    new Trade { Ticks = 680, Price = 0.68 },
                    new Trade { Ticks = 720, Price = 0.72 },
                    new Trade { Ticks = 730, Price = 0.73 },
                    new Trade { Ticks = 775, Price = 0.775 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 670, Price = 0.67 },
                    new Trade { Ticks = 680, Price = 0.68 },
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 710, Price = 0.71 },
                    new Trade { Ticks = 720, Price = 0.72 },
                    new Trade { Ticks = 730, Price = 0.73 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 775, Price = 0.775 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1690, Price = 1.69 },
                    new Trade { Ticks = 1700, Price = 1.7 },
                    new Trade { Ticks = 1710, Price = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1685, Price = 1.685 },
                    new Trade { Ticks = 1695, Price = 1.695 },
                    new Trade { Ticks = 1697, Price = 1.697 },
                    new Trade { Ticks = 1698, Price = 1.698 },
                    new Trade { Ticks = 1715, Price = 1.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 670, Price = 0.67 },
                    new Trade { Ticks = 680, Price = 0.68 },
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 710, Price = 0.71 },
                    new Trade { Ticks = 720, Price = 0.72 },
                    new Trade { Ticks = 730, Price = 0.73 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 775, Price = 0.775 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1685, Price = 1.685 },
                    new Trade { Ticks = 1690, Price = 1.69 },
                    new Trade { Ticks = 1695, Price = 1.695 },
                    new Trade { Ticks = 1697, Price = 1.697 },
                    new Trade { Ticks = 1698, Price = 1.698 },
                    new Trade { Ticks = 1700, Price = 1.7 },
                    new Trade { Ticks = 1710, Price = 1.71 },
                    new Trade { Ticks = 1715, Price = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 669, Price = 0.669 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 669, Price = 0.669 },
                    new Trade { Ticks = 670, Price = 0.67 },
                    new Trade { Ticks = 680, Price = 0.68 },
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 710, Price = 0.71 },
                    new Trade { Ticks = 720, Price = 0.72 },
                    new Trade { Ticks = 730, Price = 0.73 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 775, Price = 0.775 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1685, Price = 1.685 },
                    new Trade { Ticks = 1690, Price = 1.69 },
                    new Trade { Ticks = 1695, Price = 1.695 },
                    new Trade { Ticks = 1697, Price = 1.697 },
                    new Trade { Ticks = 1698, Price = 1.698 },
                    new Trade { Ticks = 1700, Price = 1.7 },
                    new Trade { Ticks = 1710, Price = 1.71 },
                    new Trade { Ticks = 1715, Price = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1716, Price = 1.76 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 669, Price = 0.669 },
                    new Trade { Ticks = 670, Price = 0.67 },
                    new Trade { Ticks = 680, Price = 0.68 },
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 710, Price = 0.71 },
                    new Trade { Ticks = 720, Price = 0.72 },
                    new Trade { Ticks = 730, Price = 0.73 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 775, Price = 0.775 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1685, Price = 1.685 },
                    new Trade { Ticks = 1690, Price = 1.69 },
                    new Trade { Ticks = 1695, Price = 1.695 },
                    new Trade { Ticks = 1697, Price = 1.697 },
                    new Trade { Ticks = 1698, Price = 1.698 },
                    new Trade { Ticks = 1700, Price = 1.7 },
                    new Trade { Ticks = 1710, Price = 1.71 },
                    new Trade { Ticks = 1715, Price = 1.75 },
                    new Trade { Ticks = 1716, Price = 1.76 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 665, Price = 0.665 },
                    new Trade { Ticks = 1720, Price = 1.72 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 665, Price = 0.665 },
                    new Trade { Ticks = 669, Price = 0.669 },
                    new Trade { Ticks = 670, Price = 0.67 },
                    new Trade { Ticks = 680, Price = 0.68 },
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 710, Price = 0.71 },
                    new Trade { Ticks = 720, Price = 0.72 },
                    new Trade { Ticks = 730, Price = 0.73 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 775, Price = 0.775 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1685, Price = 1.685 },
                    new Trade { Ticks = 1690, Price = 1.69 },
                    new Trade { Ticks = 1695, Price = 1.695 },
                    new Trade { Ticks = 1697, Price = 1.697 },
                    new Trade { Ticks = 1698, Price = 1.698 },
                    new Trade { Ticks = 1700, Price = 1.7 },
                    new Trade { Ticks = 1710, Price = 1.71 },
                    new Trade { Ticks = 1715, Price = 1.75 },
                    new Trade { Ticks = 1716, Price = 1.76 },
                    new Trade { Ticks = 1720, Price = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 668, Price = 0.668 },
                    new Trade { Ticks = 1717, Price = 1.717 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 665, Price = 0.665 },
                    new Trade { Ticks = 668, Price = 0.668 },
                    new Trade { Ticks = 669, Price = 0.669 },
                    new Trade { Ticks = 670, Price = 0.67 },
                    new Trade { Ticks = 680, Price = 0.68 },
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 710, Price = 0.71 },
                    new Trade { Ticks = 720, Price = 0.72 },
                    new Trade { Ticks = 730, Price = 0.73 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 775, Price = 0.775 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1685, Price = 1.685 },
                    new Trade { Ticks = 1690, Price = 1.69 },
                    new Trade { Ticks = 1695, Price = 1.695 },
                    new Trade { Ticks = 1697, Price = 1.697 },
                    new Trade { Ticks = 1698, Price = 1.698 },
                    new Trade { Ticks = 1700, Price = 1.7 },
                    new Trade { Ticks = 1710, Price = 1.71 },
                    new Trade { Ticks = 1715, Price = 1.75 },
                    new Trade { Ticks = 1716, Price = 1.76 },
                    new Trade { Ticks = 1717, Price = 1.717 },
                    new Trade { Ticks = 1720, Price = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                status = Add(new List<Trade>
                {
                    new Trade { Ticks = 666, Price = 0.666 },
                    new Trade { Ticks = 667, Price = 0.667 },
                    new Trade { Ticks = 1718, Price = 1.718 },
                    new Trade { Ticks = 1719, Price = 1.719 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 665, Price = 0.665 },
                    new Trade { Ticks = 666, Price = 0.666 },
                    new Trade { Ticks = 667, Price = 0.667 },
                    new Trade { Ticks = 668, Price = 0.668 },
                    new Trade { Ticks = 669, Price = 0.669 },
                    new Trade { Ticks = 670, Price = 0.67 },
                    new Trade { Ticks = 680, Price = 0.68 },
                    new Trade { Ticks = 690, Price = 0.69 },
                    new Trade { Ticks = 700, Price = 0.7 },
                    new Trade { Ticks = 710, Price = 0.71 },
                    new Trade { Ticks = 720, Price = 0.72 },
                    new Trade { Ticks = 730, Price = 0.73 },
                    new Trade { Ticks = 750, Price = 0.75 },
                    new Trade { Ticks = 751, Price = 0.751 },
                    new Trade { Ticks = 752, Price = 0.752 },
                    new Trade { Ticks = 753, Price = 0.753 },
                    new Trade { Ticks = 754, Price = 0.754 },
                    new Trade { Ticks = 770, Price = 0.77 },
                    new Trade { Ticks = 775, Price = 0.775 },
                    new Trade { Ticks = 780, Price = 0.78 },
                    new Trade { Ticks = 800, Price = 0.8 },
                    new Trade { Ticks = 900, Price = 0.9 },
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1300, Price = 1.3 },
                    new Trade { Ticks = 1400, Price = 1.4 },
                    new Trade { Ticks = 1500, Price = 1.5 },
                    new Trade { Ticks = 1600, Price = 1.6 },
                    new Trade { Ticks = 1650, Price = 1.65 },
                    new Trade { Ticks = 1670, Price = 1.67 },
                    new Trade { Ticks = 1680, Price = 1.68 },
                    new Trade { Ticks = 1685, Price = 1.685 },
                    new Trade { Ticks = 1690, Price = 1.69 },
                    new Trade { Ticks = 1695, Price = 1.695 },
                    new Trade { Ticks = 1697, Price = 1.697 },
                    new Trade { Ticks = 1698, Price = 1.698 },
                    new Trade { Ticks = 1700, Price = 1.7 },
                    new Trade { Ticks = 1710, Price = 1.71 },
                    new Trade { Ticks = 1715, Price = 1.75 },
                    new Trade { Ticks = 1716, Price = 1.76 },
                    new Trade { Ticks = 1717, Price = 1.717 },
                    new Trade { Ticks = 1718, Price = 1.718 },
                    new Trade { Ticks = 1719, Price = 1.719 },
                    new Trade { Ticks = 1720, Price = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);

                #endregion
            }

            DeleteTestFile(fileName);

            if (0 == errorCount)
                Trace.TraceInformation("--- All tests passed ---");
            else
                Trace.TraceInformation("--- {0} tests failed ---", errorCount);
            return errorCount;
        }

        private static int TradeDataExploratoryTest03()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using TradeData data = instrument.OpenTrade(true);

                #region (1) Initial append
                Trace.TraceInformation("--- 03 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                var list = new List<Trade>();

                #region (2)
                Trace.TraceInformation("--- 03 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                list.Clear();
                status = data.FetchAll(list);
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(0));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(9000));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(0));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(9000));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(999));
                verified = Verify(new List<Trade>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1301), new DateTime(9000));
                verified = Verify(new List<Trade>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1001));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1299), new DateTime(9000));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1290), new DateTime(9000));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1009));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1110));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1270), new DateTime(1300));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1300));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 19");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1110));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 20");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1105));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1100, Price = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 21");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1103), new DateTime(1110));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1110, Price = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 22");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1101), new DateTime(1109));
                verified = Verify(new List<Trade>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 23");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1104), new DateTime(1106));
                verified = Verify(new List<Trade>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 24");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1291), new DateTime(1299));
                verified = Verify(new List<Trade>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 25");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1294), new DateTime(1296));
                verified = Verify(new List<Trade>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 26");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(1300));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 27");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1100));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1100, Price = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 28");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1205));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 29");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1200));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 30");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1110), new DateTime(1205));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 31");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1115));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 32");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1002), new DateTime(1115));
                verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion
            }

            DeleteTestFile(fileName);

            if (0 == errorCount)
                Trace.TraceInformation("--- All tests passed ---");
            else
                Trace.TraceInformation("--- {0} tests failed ---", errorCount);
            return errorCount;
        }

        private static int TradeDataExploratoryTest04()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using TradeData data = instrument.OpenTrade(true);

                #region (1) Properties
                Trace.TraceInformation("--- 04 (1) Properties ----------------");
                Trace.TraceInformation("STEP 1");
                if (0L != data.Count)
                {
                    errorCount++;
                    Trace.TraceError("FAILED: initial dataset Count is not zero.");
                }
                if (0L != data.FirstTicks)
                {
                    errorCount++;
                    Trace.TraceError("FAILED: initial dataset FirstTicks is not zero.");
                }
                if (0L != data.LastTicks)
                {
                    errorCount++;
                    Trace.TraceError("FAILED: initial dataset LastTicks is not zero.");
                }
                if (0 == errorCount)
                    Trace.TraceInformation("PASSED");
                Trace.TraceInformation("STEP 2");
                bool status = Add(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<Trade>
                {
                    new Trade { Ticks = 1000, Price = 1.0 },
                    new Trade { Ticks = 1001, Price = 1.001 },
                    new Trade { Ticks = 1002, Price = 1.002 },
                    new Trade { Ticks = 1003, Price = 1.003 },
                    new Trade { Ticks = 1004, Price = 1.004 },
                    new Trade { Ticks = 1005, Price = 1.005 },
                    new Trade { Ticks = 1006, Price = 1.006 },
                    new Trade { Ticks = 1007, Price = 1.007 },
                    new Trade { Ticks = 1008, Price = 1.008 },
                    new Trade { Ticks = 1009, Price = 1.009 },
                    new Trade { Ticks = 1100, Price = 1.1 },
                    new Trade { Ticks = 1110, Price = 1.11 },
                    new Trade { Ticks = 1120, Price = 1.12 },
                    new Trade { Ticks = 1130, Price = 1.13 },
                    new Trade { Ticks = 1140, Price = 1.14 },
                    new Trade { Ticks = 1150, Price = 1.15 },
                    new Trade { Ticks = 1160, Price = 1.16 },
                    new Trade { Ticks = 1170, Price = 1.17 },
                    new Trade { Ticks = 1180, Price = 1.18 },
                    new Trade { Ticks = 1190, Price = 1.19 },
                    new Trade { Ticks = 1200, Price = 1.2 },
                    new Trade { Ticks = 1210, Price = 1.21 },
                    new Trade { Ticks = 1220, Price = 1.22 },
                    new Trade { Ticks = 1230, Price = 1.23 },
                    new Trade { Ticks = 1240, Price = 1.24 },
                    new Trade { Ticks = 1250, Price = 1.25 },
                    new Trade { Ticks = 1260, Price = 1.26 },
                    new Trade { Ticks = 1270, Price = 1.27 },
                    new Trade { Ticks = 1280, Price = 1.28 },
                    new Trade { Ticks = 1290, Price = 1.29 },
                    new Trade { Ticks = 1300, Price = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                bool failed = false;
                if (31L != data.Count)
                {
                    errorCount++;
                    failed = true;
                    Trace.TraceError("FAILED: dataset Count is not 31.");
                }
                if (1000L != data.FirstTicks)
                {
                    errorCount++;
                    failed = true;
                    Trace.TraceError("FAILED: dataset FirstTicks is not 1000.");
                }
                if (1300L != data.LastTicks)
                {
                    errorCount++;
                    failed = true;
                    Trace.TraceError("FAILED: dataset LastTicks is not 1300.");
                }
                if (!failed)
                    Trace.TraceInformation("PASSED");
                #endregion
            }

            DeleteTestFile(fileName);

            if (0 == errorCount)
                Trace.TraceInformation("--- All tests passed ---");
            else
                Trace.TraceInformation("--- {0} tests failed ---", errorCount);
            return errorCount;
        }

        private static int TradeDataExploratoryTest05()
        {
            int errorCount = 0;

            #region (1) Spread
            Trace.TraceInformation("--- 05 (1) Spread ----------------");
            Trace.TraceInformation("STEP 1");
            var list = new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1000, Price = 1.001 },
                new Trade { Ticks = 1000, Price = 1.002 },
                new Trade { Ticks = 1000, Price = 1.003 },
                new Trade { Ticks = 1000, Price = 1.004 },
                new Trade { Ticks = 1000, Price = 1.005 },
                new Trade { Ticks = 1000, Price = 1.006 },
                new Trade { Ticks = 1000, Price = 1.007 },
                new Trade { Ticks = 1000, Price = 1.008 },
                new Trade { Ticks = 1000, Price = 1.009 }
            };
            Trade[] array = list.ToArray();
            bool status = TradeData.SpreadDuplicateTimeTicks(array, true);
            bool verified = Verify(new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 },
                new Trade { Ticks = 1002, Price = 1.002 },
                new Trade { Ticks = 1003, Price = 1.003 },
                new Trade { Ticks = 1004, Price = 1.004 },
                new Trade { Ticks = 1005, Price = 1.005 },
                new Trade { Ticks = 1006, Price = 1.006 },
                new Trade { Ticks = 1007, Price = 1.007 },
                new Trade { Ticks = 1008, Price = 1.008 },
                new Trade { Ticks = 1009, Price = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 2");
            list = new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 },
                new Trade { Ticks = 1002, Price = 1.002 },
                new Trade { Ticks = 1003, Price = 1.003 },
                new Trade { Ticks = 1004, Price = 1.004 },
                new Trade { Ticks = 1005, Price = 1.005 },
                new Trade { Ticks = 1006, Price = 1.006 },
                new Trade { Ticks = 1007, Price = 1.007 },
                new Trade { Ticks = 1008, Price = 1.008 },
                new Trade { Ticks = 1009, Price = 1.009 }
            };
            array = list.ToArray();
            status = TradeData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 },
                new Trade { Ticks = 1002, Price = 1.002 },
                new Trade { Ticks = 1003, Price = 1.003 },
                new Trade { Ticks = 1004, Price = 1.004 },
                new Trade { Ticks = 1005, Price = 1.005 },
                new Trade { Ticks = 1006, Price = 1.006 },
                new Trade { Ticks = 1007, Price = 1.007 },
                new Trade { Ticks = 1008, Price = 1.008 },
                new Trade { Ticks = 1009, Price = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 3");
            list = new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 }
            };
            array = list.ToArray();
            status = TradeData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 4");
            list = new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1000, Price = 1.001 }
            };
            array = list.ToArray();
            status = TradeData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 5");
            list = new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1000, Price = 1.001 },
                new Trade { Ticks = 1000, Price = 1.002 }
            };
            array = list.ToArray();
            status = TradeData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 },
                new Trade { Ticks = 1002, Price = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 6");
            list = new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1000, Price = 1.001 },
                new Trade { Ticks = 1002, Price = 1.002 }
            };
            array = list.ToArray();
            status = TradeData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 },
                new Trade { Ticks = 1002, Price = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 7");
            list = new List<Trade>
            {
                new Trade { Ticks = 1007, Price = 1.007 },
                new Trade { Ticks = 1008, Price = 1.008 },
                new Trade { Ticks = 1008, Price = 1.009 }
            };
            array = list.ToArray();
            status = TradeData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Trade>
            {
                new Trade { Ticks = 1007, Price = 1.007 },
                new Trade { Ticks = 1008, Price = 1.008 },
                new Trade { Ticks = 1009, Price = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 8");
            list = new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 },
                new Trade { Ticks = 1001, Price = 1.002 },
                new Trade { Ticks = 1003, Price = 1.003 }
            };
            array = list.ToArray();
            status = TradeData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 },
                new Trade { Ticks = 1002, Price = 1.002 },
                new Trade { Ticks = 1003, Price = 1.003 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 9");
            list = new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 },
                new Trade { Ticks = 1001, Price = 1.002 },
                new Trade { Ticks = 1001, Price = 1.003 },
                new Trade { Ticks = 1004, Price = 1.004 }
            };
            array = list.ToArray();
            status = TradeData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Trade>
            {
                new Trade { Ticks = 1000, Price = 1.0 },
                new Trade { Ticks = 1001, Price = 1.001 },
                new Trade { Ticks = 1002, Price = 1.002 },
                new Trade { Ticks = 1003, Price = 1.003 },
                new Trade { Ticks = 1004, Price = 1.004 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            #endregion

            if (0 == errorCount)
                Trace.TraceInformation("--- All tests passed ---");
            else
                Trace.TraceInformation("--- {0} tests failed ---", errorCount);
            return errorCount;
        }
    }
}
