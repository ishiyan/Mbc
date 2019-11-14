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
        public void H5_QuotePriceOnlyData_Exploratory_01()
        {
            int errorCount = QuotePriceOnlyDataExploratoryTest01();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_QuotePriceOnlyData_Exploratory_02()
        {
            int errorCount = QuotePriceOnlyDataExploratoryTest02();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_QuotePriceOnlyData_Exploratory_03()
        {
            int errorCount = QuotePriceOnlyDataExploratoryTest03();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_QuotePriceOnlyData_Exploratory_04()
        {
            int errorCount = QuotePriceOnlyDataExploratoryTest04();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_QuotePriceOnlyData_Exploratory_05()
        {
            int errorCount = QuotePriceOnlyDataExploratoryTest05();
            Assert.AreEqual(0, errorCount);
        }

        private static bool Add(List<QuotePriceOnly> list, QuotePriceOnlyData data, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
        {
            QuotePriceOnly[] array = list.ToArray();
            QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, verbose);
            return data.Add(array, duplicateTimeTicks, verbose);
        }

        private static bool Verify(List<QuotePriceOnly> pattern, QuotePriceOnlyData data)
        {
            var sample = new List<QuotePriceOnly>();
            if (!data.FetchAll(sample))
                return false;
            int sampleCount = sample.Count, patternCount = pattern.Count;
            if (sampleCount == patternCount)
            {
                for (int i = 0; i < sampleCount; i++)
                {
                    if (sample[i].Ticks != pattern[i].Ticks || Math.Abs(sample[i].BidPrice - pattern[i].BidPrice) > double.Epsilon)
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

        private static bool Verify(List<QuotePriceOnly> pattern, List<QuotePriceOnly> sample)
        {
            int sampleCount = sample.Count, patternCount = pattern.Count;
            if (sampleCount == patternCount)
            {
                for (int i = 0; i < sampleCount; i++)
                {
                    if (sample[i].Ticks != pattern[i].Ticks || Math.Abs(sample[i].BidPrice - pattern[i].BidPrice) > double.Epsilon)
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

        private static int QuotePriceOnlyDataExploratoryTest01()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using QuotePriceOnlyData data = instrument.OpenQuotePriceOnly(true);

                #region (1) Initial append
                Trace.TraceInformation("--- 01 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                bool verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (2) Update / Update
                Trace.TraceInformation("--- 01 (2) Update/Update ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.01 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.01 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.31 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.31 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.51 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.51 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (3) Update / Skip
                Trace.TraceInformation("--- 01 (3) Update/Skip ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.01 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.31 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.51 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (4) Update / Fail
                Trace.TraceInformation("--- 01 (4) Update/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.01 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.31 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly> {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.51 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                #endregion

                #region (5) Prepend
                Trace.TraceInformation("--- 01 (5) Prepend/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 97, BidPrice = 0.97 },
                    new QuotePriceOnly { Ticks = 98, BidPrice = 0.98 },
                    new QuotePriceOnly { Ticks = 99, BidPrice = 0.99 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 97, BidPrice = 0.97 },
                    new QuotePriceOnly { Ticks = 98, BidPrice = 0.98 },
                    new QuotePriceOnly { Ticks = 99, BidPrice = 0.99 },
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.94 },
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.95 },
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.96 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.94 },
                    new QuotePriceOnly { Ticks = 95, BidPrice = 0.95 },
                    new QuotePriceOnly { Ticks = 96, BidPrice = 0.96 },
                    new QuotePriceOnly { Ticks = 97, BidPrice = 0.97 },
                    new QuotePriceOnly { Ticks = 98, BidPrice = 0.98 },
                    new QuotePriceOnly { Ticks = 99, BidPrice = 0.99 },
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 90, BidPrice = 0.90 },
                    new QuotePriceOnly { Ticks = 90, BidPrice = 0.91 },
                    new QuotePriceOnly { Ticks = 90, BidPrice = 0.92 },
                    new QuotePriceOnly { Ticks = 93, BidPrice = 0.93 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 90, BidPrice = 0.90 },
                    new QuotePriceOnly { Ticks = 91, BidPrice = 0.91 },
                    new QuotePriceOnly { Ticks = 92, BidPrice = 0.92 },
                    new QuotePriceOnly { Ticks = 93, BidPrice = 0.93 },
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.94 },
                    new QuotePriceOnly { Ticks = 95, BidPrice = 0.95 },
                    new QuotePriceOnly { Ticks = 96, BidPrice = 0.96 },
                    new QuotePriceOnly { Ticks = 97, BidPrice = 0.97 },
                    new QuotePriceOnly { Ticks = 98, BidPrice = 0.98 },
                    new QuotePriceOnly { Ticks = 99, BidPrice = 0.99 },
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 87, BidPrice = 0.87 },
                    new QuotePriceOnly { Ticks = 88, BidPrice = 0.88 },
                    new QuotePriceOnly { Ticks = 88, BidPrice = 0.89 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 87, BidPrice = 0.87 },
                    new QuotePriceOnly { Ticks = 88, BidPrice = 0.88 },
                    new QuotePriceOnly { Ticks = 89, BidPrice = 0.89 },
                    new QuotePriceOnly { Ticks = 90, BidPrice = 0.90 },
                    new QuotePriceOnly { Ticks = 91, BidPrice = 0.91 },
                    new QuotePriceOnly { Ticks = 92, BidPrice = 0.92 },
                    new QuotePriceOnly { Ticks = 93, BidPrice = 0.93 },
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.94 },
                    new QuotePriceOnly { Ticks = 95, BidPrice = 0.95 },
                    new QuotePriceOnly { Ticks = 96, BidPrice = 0.96 },
                    new QuotePriceOnly { Ticks = 97, BidPrice = 0.97 },
                    new QuotePriceOnly { Ticks = 98, BidPrice = 0.98 },
                    new QuotePriceOnly { Ticks = 99, BidPrice = 0.99 },
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (6) Append
                Trace.TraceInformation("--- 01 (6) Append/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 151, BidPrice = 1.51 },
                    new QuotePriceOnly { Ticks = 151, BidPrice = 1.52 },
                    new QuotePriceOnly { Ticks = 151, BidPrice = 1.53 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 87, BidPrice = 0.87 },
                    new QuotePriceOnly { Ticks = 88, BidPrice = 0.88 },
                    new QuotePriceOnly { Ticks = 89, BidPrice = 0.89 },
                    new QuotePriceOnly { Ticks = 90, BidPrice = 0.90 },
                    new QuotePriceOnly { Ticks = 91, BidPrice = 0.91 },
                    new QuotePriceOnly { Ticks = 92, BidPrice = 0.92 },
                    new QuotePriceOnly { Ticks = 93, BidPrice = 0.93 },
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.94 },
                    new QuotePriceOnly { Ticks = 95, BidPrice = 0.95 },
                    new QuotePriceOnly { Ticks = 96, BidPrice = 0.96 },
                    new QuotePriceOnly { Ticks = 97, BidPrice = 0.97 },
                    new QuotePriceOnly { Ticks = 98, BidPrice = 0.98 },
                    new QuotePriceOnly { Ticks = 99, BidPrice = 0.99 },
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 151, BidPrice = 1.51 },
                    new QuotePriceOnly { Ticks = 152, BidPrice = 1.52 },
                    new QuotePriceOnly { Ticks = 153, BidPrice = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (7) Insert
                Trace.TraceInformation("--- 01 (7) Insert/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 131, BidPrice = 1.31 },
                    new QuotePriceOnly { Ticks = 132, BidPrice = 1.32 },
                    new QuotePriceOnly { Ticks = 133, BidPrice = 1.33 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 87, BidPrice = 0.87 },
                    new QuotePriceOnly { Ticks = 88, BidPrice = 0.88 },
                    new QuotePriceOnly { Ticks = 89, BidPrice = 0.89 },
                    new QuotePriceOnly { Ticks = 90, BidPrice = 0.90 },
                    new QuotePriceOnly { Ticks = 91, BidPrice = 0.91 },
                    new QuotePriceOnly { Ticks = 92, BidPrice = 0.92 },
                    new QuotePriceOnly { Ticks = 93, BidPrice = 0.93 },
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.94 },
                    new QuotePriceOnly { Ticks = 95, BidPrice = 0.95 },
                    new QuotePriceOnly { Ticks = 96, BidPrice = 0.96 },
                    new QuotePriceOnly { Ticks = 97, BidPrice = 0.97 },
                    new QuotePriceOnly { Ticks = 98, BidPrice = 0.98 },
                    new QuotePriceOnly { Ticks = 99, BidPrice = 0.99 },
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 131, BidPrice = 1.31 },
                    new QuotePriceOnly { Ticks = 132, BidPrice = 1.32 },
                    new QuotePriceOnly { Ticks = 133, BidPrice = 1.33 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 151, BidPrice = 1.51 },
                    new QuotePriceOnly { Ticks = 152, BidPrice = 1.52 },
                    new QuotePriceOnly { Ticks = 153, BidPrice = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 127, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 128, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 129, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 134, BidPrice = 1.34 },
                    new QuotePriceOnly { Ticks = 135, BidPrice = 1.35 },
                    new QuotePriceOnly { Ticks = 136, BidPrice = 1.36 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 87, BidPrice = 0.87 },
                    new QuotePriceOnly { Ticks = 88, BidPrice = 0.88 },
                    new QuotePriceOnly { Ticks = 89, BidPrice = 0.89 },
                    new QuotePriceOnly { Ticks = 90, BidPrice = 0.90 },
                    new QuotePriceOnly { Ticks = 91, BidPrice = 0.91 },
                    new QuotePriceOnly { Ticks = 92, BidPrice = 0.92 },
                    new QuotePriceOnly { Ticks = 93, BidPrice = 0.93 },
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.94 },
                    new QuotePriceOnly { Ticks = 95, BidPrice = 0.95 },
                    new QuotePriceOnly { Ticks = 96, BidPrice = 0.96 },
                    new QuotePriceOnly { Ticks = 97, BidPrice = 0.97 },
                    new QuotePriceOnly { Ticks = 98, BidPrice = 0.98 },
                    new QuotePriceOnly { Ticks = 99, BidPrice = 0.99 },
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 127, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 128, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 129, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 131, BidPrice = 1.31 },
                    new QuotePriceOnly { Ticks = 132, BidPrice = 1.32 },
                    new QuotePriceOnly { Ticks = 133, BidPrice = 1.33 },
                    new QuotePriceOnly { Ticks = 134, BidPrice = 1.34 },
                    new QuotePriceOnly { Ticks = 135, BidPrice = 1.35 },
                    new QuotePriceOnly { Ticks = 136, BidPrice = 1.36 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 151, BidPrice = 1.51 },
                    new QuotePriceOnly { Ticks = 152, BidPrice = 1.52 },
                    new QuotePriceOnly { Ticks = 153, BidPrice = 1.53 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 146, BidPrice = 1.46 },
                    new QuotePriceOnly { Ticks = 147, BidPrice = 1.47 },
                    new QuotePriceOnly { Ticks = 148, BidPrice = 1.48 },
                    new QuotePriceOnly { Ticks = 149, BidPrice = 1.49 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.50 },
                    new QuotePriceOnly { Ticks = 153, BidPrice = 1.53 },
                    new QuotePriceOnly { Ticks = 154, BidPrice = 1.54 },
                    new QuotePriceOnly { Ticks = 155, BidPrice = 1.55 },
                    new QuotePriceOnly { Ticks = 156, BidPrice = 1.56 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 87, BidPrice = 0.87 },
                    new QuotePriceOnly { Ticks = 88, BidPrice = 0.88 },
                    new QuotePriceOnly { Ticks = 89, BidPrice = 0.89 },
                    new QuotePriceOnly { Ticks = 90, BidPrice = 0.90 },
                    new QuotePriceOnly { Ticks = 91, BidPrice = 0.91 },
                    new QuotePriceOnly { Ticks = 92, BidPrice = 0.92 },
                    new QuotePriceOnly { Ticks = 93, BidPrice = 0.93 },
                    new QuotePriceOnly { Ticks = 94, BidPrice = 0.94 },
                    new QuotePriceOnly { Ticks = 95, BidPrice = 0.95 },
                    new QuotePriceOnly { Ticks = 96, BidPrice = 0.96 },
                    new QuotePriceOnly { Ticks = 97, BidPrice = 0.97 },
                    new QuotePriceOnly { Ticks = 98, BidPrice = 0.98 },
                    new QuotePriceOnly { Ticks = 99, BidPrice = 0.99 },
                    new QuotePriceOnly { Ticks = 100, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 110, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 127, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 128, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 129, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 130, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 131, BidPrice = 1.31 },
                    new QuotePriceOnly { Ticks = 132, BidPrice = 1.32 },
                    new QuotePriceOnly { Ticks = 133, BidPrice = 1.33 },
                    new QuotePriceOnly { Ticks = 134, BidPrice = 1.34 },
                    new QuotePriceOnly { Ticks = 135, BidPrice = 1.35 },
                    new QuotePriceOnly { Ticks = 136, BidPrice = 1.36 },
                    new QuotePriceOnly { Ticks = 146, BidPrice = 1.46 },
                    new QuotePriceOnly { Ticks = 147, BidPrice = 1.47 },
                    new QuotePriceOnly { Ticks = 148, BidPrice = 1.48 },
                    new QuotePriceOnly { Ticks = 149, BidPrice = 1.49 },
                    new QuotePriceOnly { Ticks = 150, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 151, BidPrice = 1.51 },
                    new QuotePriceOnly { Ticks = 152, BidPrice = 1.52 },
                    new QuotePriceOnly { Ticks = 153, BidPrice = 1.53 },
                    new QuotePriceOnly { Ticks = 154, BidPrice = 1.54 },
                    new QuotePriceOnly { Ticks = 155, BidPrice = 1.55 },
                    new QuotePriceOnly { Ticks = 156, BidPrice = 1.56 }
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

        private static int QuotePriceOnlyDataExploratoryTest02()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using QuotePriceOnlyData data = instrument.OpenQuotePriceOnly(true);

                #region (1) Initial append

                Trace.TraceInformation("--- 02 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);

                #endregion

                #region (2)

                Trace.TraceInformation("--- 02 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1690, BidPrice = 1.69 },
                    new QuotePriceOnly { Ticks = 1710, BidPrice = 1.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1690, BidPrice = 1.69 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 },
                    new QuotePriceOnly { Ticks = 1710, BidPrice = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 670, BidPrice = 0.67 },
                    new QuotePriceOnly { Ticks = 680, BidPrice = 0.68 },
                    new QuotePriceOnly { Ticks = 720, BidPrice = 0.72 },
                    new QuotePriceOnly { Ticks = 730, BidPrice = 0.73 },
                    new QuotePriceOnly { Ticks = 775, BidPrice = 0.775 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 670, BidPrice = 0.67 },
                    new QuotePriceOnly { Ticks = 680, BidPrice = 0.68 },
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 },
                    new QuotePriceOnly { Ticks = 720, BidPrice = 0.72 },
                    new QuotePriceOnly { Ticks = 730, BidPrice = 0.73 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 775, BidPrice = 0.775 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1690, BidPrice = 1.69 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 },
                    new QuotePriceOnly { Ticks = 1710, BidPrice = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1685, BidPrice = 1.685 },
                    new QuotePriceOnly { Ticks = 1695, BidPrice = 1.695 },
                    new QuotePriceOnly { Ticks = 1697, BidPrice = 1.697 },
                    new QuotePriceOnly { Ticks = 1698, BidPrice = 1.698 },
                    new QuotePriceOnly { Ticks = 1715, BidPrice = 1.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 670, BidPrice = 0.67 },
                    new QuotePriceOnly { Ticks = 680, BidPrice = 0.68 },
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 },
                    new QuotePriceOnly { Ticks = 720, BidPrice = 0.72 },
                    new QuotePriceOnly { Ticks = 730, BidPrice = 0.73 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 775, BidPrice = 0.775 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1685, BidPrice = 1.685 },
                    new QuotePriceOnly { Ticks = 1690, BidPrice = 1.69 },
                    new QuotePriceOnly { Ticks = 1695, BidPrice = 1.695 },
                    new QuotePriceOnly { Ticks = 1697, BidPrice = 1.697 },
                    new QuotePriceOnly { Ticks = 1698, BidPrice = 1.698 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 },
                    new QuotePriceOnly { Ticks = 1710, BidPrice = 1.71 },
                    new QuotePriceOnly { Ticks = 1715, BidPrice = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 669, BidPrice = 0.669 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 669, BidPrice = 0.669 },
                    new QuotePriceOnly { Ticks = 670, BidPrice = 0.67 },
                    new QuotePriceOnly { Ticks = 680, BidPrice = 0.68 },
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 },
                    new QuotePriceOnly { Ticks = 720, BidPrice = 0.72 },
                    new QuotePriceOnly { Ticks = 730, BidPrice = 0.73 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 775, BidPrice = 0.775 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1685, BidPrice = 1.685 },
                    new QuotePriceOnly { Ticks = 1690, BidPrice = 1.69 },
                    new QuotePriceOnly { Ticks = 1695, BidPrice = 1.695 },
                    new QuotePriceOnly { Ticks = 1697, BidPrice = 1.697 },
                    new QuotePriceOnly { Ticks = 1698, BidPrice = 1.698 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 },
                    new QuotePriceOnly { Ticks = 1710, BidPrice = 1.71 },
                    new QuotePriceOnly { Ticks = 1715, BidPrice = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1716, BidPrice = 1.76 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 669, BidPrice = 0.669 },
                    new QuotePriceOnly { Ticks = 670, BidPrice = 0.67 },
                    new QuotePriceOnly { Ticks = 680, BidPrice = 0.68 },
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 },
                    new QuotePriceOnly { Ticks = 720, BidPrice = 0.72 },
                    new QuotePriceOnly { Ticks = 730, BidPrice = 0.73 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 775, BidPrice = 0.775 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1685, BidPrice = 1.685 },
                    new QuotePriceOnly { Ticks = 1690, BidPrice = 1.69 },
                    new QuotePriceOnly { Ticks = 1695, BidPrice = 1.695 },
                    new QuotePriceOnly { Ticks = 1697, BidPrice = 1.697 },
                    new QuotePriceOnly { Ticks = 1698, BidPrice = 1.698 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 },
                    new QuotePriceOnly { Ticks = 1710, BidPrice = 1.71 },
                    new QuotePriceOnly { Ticks = 1715, BidPrice = 1.75 },
                    new QuotePriceOnly { Ticks = 1716, BidPrice = 1.76 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 665, BidPrice = 0.665 },
                    new QuotePriceOnly { Ticks = 1720, BidPrice = 1.72 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 665, BidPrice = 0.665 },
                    new QuotePriceOnly { Ticks = 669, BidPrice = 0.669 },
                    new QuotePriceOnly { Ticks = 670, BidPrice = 0.67 },
                    new QuotePriceOnly { Ticks = 680, BidPrice = 0.68 },
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 },
                    new QuotePriceOnly { Ticks = 720, BidPrice = 0.72 },
                    new QuotePriceOnly { Ticks = 730, BidPrice = 0.73 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 775, BidPrice = 0.775 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1685, BidPrice = 1.685 },
                    new QuotePriceOnly { Ticks = 1690, BidPrice = 1.69 },
                    new QuotePriceOnly { Ticks = 1695, BidPrice = 1.695 },
                    new QuotePriceOnly { Ticks = 1697, BidPrice = 1.697 },
                    new QuotePriceOnly { Ticks = 1698, BidPrice = 1.698 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 },
                    new QuotePriceOnly { Ticks = 1710, BidPrice = 1.71 },
                    new QuotePriceOnly { Ticks = 1715, BidPrice = 1.75 },
                    new QuotePriceOnly { Ticks = 1716, BidPrice = 1.76 },
                    new QuotePriceOnly { Ticks = 1720, BidPrice = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 668, BidPrice = 0.668 },
                    new QuotePriceOnly { Ticks = 1717, BidPrice = 1.717 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 665, BidPrice = 0.665 },
                    new QuotePriceOnly { Ticks = 668, BidPrice = 0.668 },
                    new QuotePriceOnly { Ticks = 669, BidPrice = 0.669 },
                    new QuotePriceOnly { Ticks = 670, BidPrice = 0.67 },
                    new QuotePriceOnly { Ticks = 680, BidPrice = 0.68 },
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 },
                    new QuotePriceOnly { Ticks = 720, BidPrice = 0.72 },
                    new QuotePriceOnly { Ticks = 730, BidPrice = 0.73 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 775, BidPrice = 0.775 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1685, BidPrice = 1.685 },
                    new QuotePriceOnly { Ticks = 1690, BidPrice = 1.69 },
                    new QuotePriceOnly { Ticks = 1695, BidPrice = 1.695 },
                    new QuotePriceOnly { Ticks = 1697, BidPrice = 1.697 },
                    new QuotePriceOnly { Ticks = 1698, BidPrice = 1.698 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 },
                    new QuotePriceOnly { Ticks = 1710, BidPrice = 1.71 },
                    new QuotePriceOnly { Ticks = 1715, BidPrice = 1.75 },
                    new QuotePriceOnly { Ticks = 1716, BidPrice = 1.76 },
                    new QuotePriceOnly { Ticks = 1717, BidPrice = 1.717 },
                    new QuotePriceOnly { Ticks = 1720, BidPrice = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 666, BidPrice = 0.666 },
                    new QuotePriceOnly { Ticks = 667, BidPrice = 0.667 },
                    new QuotePriceOnly { Ticks = 1718, BidPrice = 1.718 },
                    new QuotePriceOnly { Ticks = 1719, BidPrice = 1.719 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 665, BidPrice = 0.665 },
                    new QuotePriceOnly { Ticks = 666, BidPrice = 0.666 },
                    new QuotePriceOnly { Ticks = 667, BidPrice = 0.667 },
                    new QuotePriceOnly { Ticks = 668, BidPrice = 0.668 },
                    new QuotePriceOnly { Ticks = 669, BidPrice = 0.669 },
                    new QuotePriceOnly { Ticks = 670, BidPrice = 0.67 },
                    new QuotePriceOnly { Ticks = 680, BidPrice = 0.68 },
                    new QuotePriceOnly { Ticks = 690, BidPrice = 0.69 },
                    new QuotePriceOnly { Ticks = 700, BidPrice = 0.7 },
                    new QuotePriceOnly { Ticks = 710, BidPrice = 0.71 },
                    new QuotePriceOnly { Ticks = 720, BidPrice = 0.72 },
                    new QuotePriceOnly { Ticks = 730, BidPrice = 0.73 },
                    new QuotePriceOnly { Ticks = 750, BidPrice = 0.75 },
                    new QuotePriceOnly { Ticks = 751, BidPrice = 0.751 },
                    new QuotePriceOnly { Ticks = 752, BidPrice = 0.752 },
                    new QuotePriceOnly { Ticks = 753, BidPrice = 0.753 },
                    new QuotePriceOnly { Ticks = 754, BidPrice = 0.754 },
                    new QuotePriceOnly { Ticks = 770, BidPrice = 0.77 },
                    new QuotePriceOnly { Ticks = 775, BidPrice = 0.775 },
                    new QuotePriceOnly { Ticks = 780, BidPrice = 0.78 },
                    new QuotePriceOnly { Ticks = 800, BidPrice = 0.8 },
                    new QuotePriceOnly { Ticks = 900, BidPrice = 0.9 },
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 },
                    new QuotePriceOnly { Ticks = 1400, BidPrice = 1.4 },
                    new QuotePriceOnly { Ticks = 1500, BidPrice = 1.5 },
                    new QuotePriceOnly { Ticks = 1600, BidPrice = 1.6 },
                    new QuotePriceOnly { Ticks = 1650, BidPrice = 1.65 },
                    new QuotePriceOnly { Ticks = 1670, BidPrice = 1.67 },
                    new QuotePriceOnly { Ticks = 1680, BidPrice = 1.68 },
                    new QuotePriceOnly { Ticks = 1685, BidPrice = 1.685 },
                    new QuotePriceOnly { Ticks = 1690, BidPrice = 1.69 },
                    new QuotePriceOnly { Ticks = 1695, BidPrice = 1.695 },
                    new QuotePriceOnly { Ticks = 1697, BidPrice = 1.697 },
                    new QuotePriceOnly { Ticks = 1698, BidPrice = 1.698 },
                    new QuotePriceOnly { Ticks = 1700, BidPrice = 1.7 },
                    new QuotePriceOnly { Ticks = 1710, BidPrice = 1.71 },
                    new QuotePriceOnly { Ticks = 1715, BidPrice = 1.75 },
                    new QuotePriceOnly { Ticks = 1716, BidPrice = 1.76 },
                    new QuotePriceOnly { Ticks = 1717, BidPrice = 1.717 },
                    new QuotePriceOnly { Ticks = 1718, BidPrice = 1.718 },
                    new QuotePriceOnly { Ticks = 1719, BidPrice = 1.719 },
                    new QuotePriceOnly { Ticks = 1720, BidPrice = 1.72 }
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

        private static int QuotePriceOnlyDataExploratoryTest03()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using QuotePriceOnlyData data = instrument.OpenQuotePriceOnly(true);

                #region (1) Initial append
                Trace.TraceInformation("--- 03 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                var list = new List<QuotePriceOnly>();

                #region (2)
                Trace.TraceInformation("--- 03 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                list.Clear();
                status = data.FetchAll(list);
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(0));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(9000));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(0));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(9000));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(999));
                verified = Verify(new List<QuotePriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1301), new DateTime(9000));
                verified = Verify(new List<QuotePriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1001));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1299), new DateTime(9000));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1290), new DateTime(9000));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1009));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1110));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1270), new DateTime(1300));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1300));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 19");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1110));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 20");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1105));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 21");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1103), new DateTime(1110));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 22");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1101), new DateTime(1109));
                verified = Verify(new List<QuotePriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 23");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1104), new DateTime(1106));
                verified = Verify(new List<QuotePriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 24");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1291), new DateTime(1299));
                verified = Verify(new List<QuotePriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 25");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1294), new DateTime(1296));
                verified = Verify(new List<QuotePriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 26");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(1300));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 27");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1100));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 28");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1205));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 29");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1200));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 30");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1110), new DateTime(1205));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 31");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1115));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 32");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1002), new DateTime(1115));
                verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 }
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

        private static int QuotePriceOnlyDataExploratoryTest04()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using QuotePriceOnlyData data = instrument.OpenQuotePriceOnly(true);

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
                bool status = Add(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<QuotePriceOnly>
                {
                    new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                    new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                    new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                    new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                    new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                    new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                    new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                    new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                    new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                    new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 },
                    new QuotePriceOnly { Ticks = 1100, BidPrice = 1.1 },
                    new QuotePriceOnly { Ticks = 1110, BidPrice = 1.11 },
                    new QuotePriceOnly { Ticks = 1120, BidPrice = 1.12 },
                    new QuotePriceOnly { Ticks = 1130, BidPrice = 1.13 },
                    new QuotePriceOnly { Ticks = 1140, BidPrice = 1.14 },
                    new QuotePriceOnly { Ticks = 1150, BidPrice = 1.15 },
                    new QuotePriceOnly { Ticks = 1160, BidPrice = 1.16 },
                    new QuotePriceOnly { Ticks = 1170, BidPrice = 1.17 },
                    new QuotePriceOnly { Ticks = 1180, BidPrice = 1.18 },
                    new QuotePriceOnly { Ticks = 1190, BidPrice = 1.19 },
                    new QuotePriceOnly { Ticks = 1200, BidPrice = 1.2 },
                    new QuotePriceOnly { Ticks = 1210, BidPrice = 1.21 },
                    new QuotePriceOnly { Ticks = 1220, BidPrice = 1.22 },
                    new QuotePriceOnly { Ticks = 1230, BidPrice = 1.23 },
                    new QuotePriceOnly { Ticks = 1240, BidPrice = 1.24 },
                    new QuotePriceOnly { Ticks = 1250, BidPrice = 1.25 },
                    new QuotePriceOnly { Ticks = 1260, BidPrice = 1.26 },
                    new QuotePriceOnly { Ticks = 1270, BidPrice = 1.27 },
                    new QuotePriceOnly { Ticks = 1280, BidPrice = 1.28 },
                    new QuotePriceOnly { Ticks = 1290, BidPrice = 1.29 },
                    new QuotePriceOnly { Ticks = 1300, BidPrice = 1.3 }
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

        private static int QuotePriceOnlyDataExploratoryTest05()
        {
            int errorCount = 0;

            #region (1) Spread
            Trace.TraceInformation("--- 05 (1) Spread ----------------");
            Trace.TraceInformation("STEP 1");
            var list = new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.002 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.003 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.004 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.005 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.006 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.007 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.008 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.009 }
            };
            QuotePriceOnly[] array = list.ToArray();
            bool status = QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            bool verified = Verify(new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 2");
            list = new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 }
            };
            array = list.ToArray();
            status = QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 },
                new QuotePriceOnly { Ticks = 1005, BidPrice = 1.005 },
                new QuotePriceOnly { Ticks = 1006, BidPrice = 1.006 },
                new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 3");
            list = new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 }
            };
            array = list.ToArray();
            status = QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 4");
            list = new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.001 }
            };
            array = list.ToArray();
            status = QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 5");
            list = new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.002 }
            };
            array = list.ToArray();
            status = QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 6");
            list = new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 }
            };
            array = list.ToArray();
            status = QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 7");
            list = new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                new QuotePriceOnly { Ticks = 1008, BidPrice = 1.009 }
            };
            array = list.ToArray();
            status = QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1007, BidPrice = 1.007 },
                new QuotePriceOnly { Ticks = 1008, BidPrice = 1.008 },
                new QuotePriceOnly { Ticks = 1009, BidPrice = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 8");
            list = new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.002 },
                new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 }
            };
            array = list.ToArray();
            status = QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 9");
            list = new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.002 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.003 },
                new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 }
            };
            array = list.ToArray();
            status = QuotePriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<QuotePriceOnly>
            {
                new QuotePriceOnly { Ticks = 1000, BidPrice = 1.0 },
                new QuotePriceOnly { Ticks = 1001, BidPrice = 1.001 },
                new QuotePriceOnly { Ticks = 1002, BidPrice = 1.002 },
                new QuotePriceOnly { Ticks = 1003, BidPrice = 1.003 },
                new QuotePriceOnly { Ticks = 1004, BidPrice = 1.004 }
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
