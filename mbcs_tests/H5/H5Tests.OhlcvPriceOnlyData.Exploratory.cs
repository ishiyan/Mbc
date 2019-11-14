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
        public void H5_OhlcvPriceOnlyData_Exploratory_01()
        {
            int errorCount = OhlcvPriceOnlyDataExploratoryTest01();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_OhlcvPriceOnlyData_Exploratory_02()
        {
            int errorCount = OhlcvPriceOnlyDataExploratoryTest02();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_OhlcvPriceOnlyData_Exploratory_03()
        {
            int errorCount = OhlcvPriceOnlyDataExploratoryTest03();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_OhlcvPriceOnlyData_Exploratory_04()
        {
            int errorCount = OhlcvPriceOnlyDataExploratoryTest04();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_OhlcvPriceOnlyData_Exploratory_05()
        {
            int errorCount = OhlcvPriceOnlyDataExploratoryTest05();
            Assert.AreEqual(0, errorCount);
        }

        private static bool Add(List<OhlcvPriceOnly> list, OhlcvPriceOnlyData data, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
        {
            OhlcvPriceOnly[] array = list.ToArray();
            OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, verbose);
            return data.Add(array, duplicateTimeTicks, verbose);
        }

        private static bool Verify(List<OhlcvPriceOnly> pattern, OhlcvPriceOnlyData data)
        {
            var sample = new List<OhlcvPriceOnly>();
            if (!data.FetchAll(sample))
                return false;
            int sampleCount = sample.Count, patternCount = pattern.Count;
            if (sampleCount == patternCount)
            {
                for (int i = 0; i < sampleCount; i++)
                {
                    if (sample[i].Ticks != pattern[i].Ticks || Math.Abs(sample[i].High - pattern[i].High) > double.Epsilon)
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

        private static bool Verify(List<OhlcvPriceOnly> pattern, List<OhlcvPriceOnly> sample)
        {
            int sampleCount = sample.Count, patternCount = pattern.Count;
            if (sampleCount == patternCount)
            {
                for (int i = 0; i < sampleCount; i++)
                {
                    if (sample[i].Ticks != pattern[i].Ticks || Math.Abs(sample[i].High - pattern[i].High) > double.Epsilon)
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

        private static int OhlcvPriceOnlyDataExploratoryTest01()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using OhlcvPriceOnlyData data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Default, DataTimeFrame.Minute3, true);

                #region (1) Initial append
                Trace.TraceInformation("--- 01 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                bool verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (2) Update / Update
                Trace.TraceInformation("--- 01 (2) Update/Update ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.01 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.01 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.31 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.31 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.51 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.51 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (3) Update / Skip
                Trace.TraceInformation("--- 01 (3) Update/Skip ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.01 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.31 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.51 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (4) Update / Fail
                Trace.TraceInformation("--- 01 (4) Update/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.01 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.31 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly> {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.51 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                #endregion

                #region (5) Prepend
                Trace.TraceInformation("--- 01 (5) Prepend/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 97, High = 0.97 },
                    new OhlcvPriceOnly { Ticks = 98, High = 0.98 },
                    new OhlcvPriceOnly { Ticks = 99, High = 0.99 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 97, High = 0.97 },
                    new OhlcvPriceOnly { Ticks = 98, High = 0.98 },
                    new OhlcvPriceOnly { Ticks = 99, High = 0.99 },
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 94, High = 0.94 },
                    new OhlcvPriceOnly { Ticks = 94, High = 0.95 },
                    new OhlcvPriceOnly { Ticks = 94, High = 0.96 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 94, High = 0.94 },
                    new OhlcvPriceOnly { Ticks = 95, High = 0.95 },
                    new OhlcvPriceOnly { Ticks = 96, High = 0.96 },
                    new OhlcvPriceOnly { Ticks = 97, High = 0.97 },
                    new OhlcvPriceOnly { Ticks = 98, High = 0.98 },
                    new OhlcvPriceOnly { Ticks = 99, High = 0.99 },
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 90, High = 0.90 },
                    new OhlcvPriceOnly { Ticks = 90, High = 0.91 },
                    new OhlcvPriceOnly { Ticks = 90, High = 0.92 },
                    new OhlcvPriceOnly { Ticks = 93, High = 0.93 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 90, High = 0.90 },
                    new OhlcvPriceOnly { Ticks = 91, High = 0.91 },
                    new OhlcvPriceOnly { Ticks = 92, High = 0.92 },
                    new OhlcvPriceOnly { Ticks = 93, High = 0.93 },
                    new OhlcvPriceOnly { Ticks = 94, High = 0.94 },
                    new OhlcvPriceOnly { Ticks = 95, High = 0.95 },
                    new OhlcvPriceOnly { Ticks = 96, High = 0.96 },
                    new OhlcvPriceOnly { Ticks = 97, High = 0.97 },
                    new OhlcvPriceOnly { Ticks = 98, High = 0.98 },
                    new OhlcvPriceOnly { Ticks = 99, High = 0.99 },
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 87, High = 0.87 },
                    new OhlcvPriceOnly { Ticks = 88, High = 0.88 },
                    new OhlcvPriceOnly { Ticks = 88, High = 0.89 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 87, High = 0.87 },
                    new OhlcvPriceOnly { Ticks = 88, High = 0.88 },
                    new OhlcvPriceOnly { Ticks = 89, High = 0.89 },
                    new OhlcvPriceOnly { Ticks = 90, High = 0.90 },
                    new OhlcvPriceOnly { Ticks = 91, High = 0.91 },
                    new OhlcvPriceOnly { Ticks = 92, High = 0.92 },
                    new OhlcvPriceOnly { Ticks = 93, High = 0.93 },
                    new OhlcvPriceOnly { Ticks = 94, High = 0.94 },
                    new OhlcvPriceOnly { Ticks = 95, High = 0.95 },
                    new OhlcvPriceOnly { Ticks = 96, High = 0.96 },
                    new OhlcvPriceOnly { Ticks = 97, High = 0.97 },
                    new OhlcvPriceOnly { Ticks = 98, High = 0.98 },
                    new OhlcvPriceOnly { Ticks = 99, High = 0.99 },
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (6) Append
                Trace.TraceInformation("--- 01 (6) Append/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 151, High = 1.51 },
                    new OhlcvPriceOnly { Ticks = 151, High = 1.52 },
                    new OhlcvPriceOnly { Ticks = 151, High = 1.53 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 87, High = 0.87 },
                    new OhlcvPriceOnly { Ticks = 88, High = 0.88 },
                    new OhlcvPriceOnly { Ticks = 89, High = 0.89 },
                    new OhlcvPriceOnly { Ticks = 90, High = 0.90 },
                    new OhlcvPriceOnly { Ticks = 91, High = 0.91 },
                    new OhlcvPriceOnly { Ticks = 92, High = 0.92 },
                    new OhlcvPriceOnly { Ticks = 93, High = 0.93 },
                    new OhlcvPriceOnly { Ticks = 94, High = 0.94 },
                    new OhlcvPriceOnly { Ticks = 95, High = 0.95 },
                    new OhlcvPriceOnly { Ticks = 96, High = 0.96 },
                    new OhlcvPriceOnly { Ticks = 97, High = 0.97 },
                    new OhlcvPriceOnly { Ticks = 98, High = 0.98 },
                    new OhlcvPriceOnly { Ticks = 99, High = 0.99 },
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 151, High = 1.51 },
                    new OhlcvPriceOnly { Ticks = 152, High = 1.52 },
                    new OhlcvPriceOnly { Ticks = 153, High = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (7) Insert
                Trace.TraceInformation("--- 01 (7) Insert/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 131, High = 1.31 },
                    new OhlcvPriceOnly { Ticks = 132, High = 1.32 },
                    new OhlcvPriceOnly { Ticks = 133, High = 1.33 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 87, High = 0.87 },
                    new OhlcvPriceOnly { Ticks = 88, High = 0.88 },
                    new OhlcvPriceOnly { Ticks = 89, High = 0.89 },
                    new OhlcvPriceOnly { Ticks = 90, High = 0.90 },
                    new OhlcvPriceOnly { Ticks = 91, High = 0.91 },
                    new OhlcvPriceOnly { Ticks = 92, High = 0.92 },
                    new OhlcvPriceOnly { Ticks = 93, High = 0.93 },
                    new OhlcvPriceOnly { Ticks = 94, High = 0.94 },
                    new OhlcvPriceOnly { Ticks = 95, High = 0.95 },
                    new OhlcvPriceOnly { Ticks = 96, High = 0.96 },
                    new OhlcvPriceOnly { Ticks = 97, High = 0.97 },
                    new OhlcvPriceOnly { Ticks = 98, High = 0.98 },
                    new OhlcvPriceOnly { Ticks = 99, High = 0.99 },
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 131, High = 1.31 },
                    new OhlcvPriceOnly { Ticks = 132, High = 1.32 },
                    new OhlcvPriceOnly { Ticks = 133, High = 1.33 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 151, High = 1.51 },
                    new OhlcvPriceOnly { Ticks = 152, High = 1.52 },
                    new OhlcvPriceOnly { Ticks = 153, High = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 127, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 128, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 129, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 134, High = 1.34 },
                    new OhlcvPriceOnly { Ticks = 135, High = 1.35 },
                    new OhlcvPriceOnly { Ticks = 136, High = 1.36 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 87, High = 0.87 },
                    new OhlcvPriceOnly { Ticks = 88, High = 0.88 },
                    new OhlcvPriceOnly { Ticks = 89, High = 0.89 },
                    new OhlcvPriceOnly { Ticks = 90, High = 0.90 },
                    new OhlcvPriceOnly { Ticks = 91, High = 0.91 },
                    new OhlcvPriceOnly { Ticks = 92, High = 0.92 },
                    new OhlcvPriceOnly { Ticks = 93, High = 0.93 },
                    new OhlcvPriceOnly { Ticks = 94, High = 0.94 },
                    new OhlcvPriceOnly { Ticks = 95, High = 0.95 },
                    new OhlcvPriceOnly { Ticks = 96, High = 0.96 },
                    new OhlcvPriceOnly { Ticks = 97, High = 0.97 },
                    new OhlcvPriceOnly { Ticks = 98, High = 0.98 },
                    new OhlcvPriceOnly { Ticks = 99, High = 0.99 },
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 127, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 128, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 129, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 131, High = 1.31 },
                    new OhlcvPriceOnly { Ticks = 132, High = 1.32 },
                    new OhlcvPriceOnly { Ticks = 133, High = 1.33 },
                    new OhlcvPriceOnly { Ticks = 134, High = 1.34 },
                    new OhlcvPriceOnly { Ticks = 135, High = 1.35 },
                    new OhlcvPriceOnly { Ticks = 136, High = 1.36 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 151, High = 1.51 },
                    new OhlcvPriceOnly { Ticks = 152, High = 1.52 },
                    new OhlcvPriceOnly { Ticks = 153, High = 1.53 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 146, High = 1.46 },
                    new OhlcvPriceOnly { Ticks = 147, High = 1.47 },
                    new OhlcvPriceOnly { Ticks = 148, High = 1.48 },
                    new OhlcvPriceOnly { Ticks = 149, High = 1.49 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.50 },
                    new OhlcvPriceOnly { Ticks = 153, High = 1.53 },
                    new OhlcvPriceOnly { Ticks = 154, High = 1.54 },
                    new OhlcvPriceOnly { Ticks = 155, High = 1.55 },
                    new OhlcvPriceOnly { Ticks = 156, High = 1.56 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 87, High = 0.87 },
                    new OhlcvPriceOnly { Ticks = 88, High = 0.88 },
                    new OhlcvPriceOnly { Ticks = 89, High = 0.89 },
                    new OhlcvPriceOnly { Ticks = 90, High = 0.90 },
                    new OhlcvPriceOnly { Ticks = 91, High = 0.91 },
                    new OhlcvPriceOnly { Ticks = 92, High = 0.92 },
                    new OhlcvPriceOnly { Ticks = 93, High = 0.93 },
                    new OhlcvPriceOnly { Ticks = 94, High = 0.94 },
                    new OhlcvPriceOnly { Ticks = 95, High = 0.95 },
                    new OhlcvPriceOnly { Ticks = 96, High = 0.96 },
                    new OhlcvPriceOnly { Ticks = 97, High = 0.97 },
                    new OhlcvPriceOnly { Ticks = 98, High = 0.98 },
                    new OhlcvPriceOnly { Ticks = 99, High = 0.99 },
                    new OhlcvPriceOnly { Ticks = 100, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 110, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 127, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 128, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 129, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 130, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 131, High = 1.31 },
                    new OhlcvPriceOnly { Ticks = 132, High = 1.32 },
                    new OhlcvPriceOnly { Ticks = 133, High = 1.33 },
                    new OhlcvPriceOnly { Ticks = 134, High = 1.34 },
                    new OhlcvPriceOnly { Ticks = 135, High = 1.35 },
                    new OhlcvPriceOnly { Ticks = 136, High = 1.36 },
                    new OhlcvPriceOnly { Ticks = 146, High = 1.46 },
                    new OhlcvPriceOnly { Ticks = 147, High = 1.47 },
                    new OhlcvPriceOnly { Ticks = 148, High = 1.48 },
                    new OhlcvPriceOnly { Ticks = 149, High = 1.49 },
                    new OhlcvPriceOnly { Ticks = 150, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 151, High = 1.51 },
                    new OhlcvPriceOnly { Ticks = 152, High = 1.52 },
                    new OhlcvPriceOnly { Ticks = 153, High = 1.53 },
                    new OhlcvPriceOnly { Ticks = 154, High = 1.54 },
                    new OhlcvPriceOnly { Ticks = 155, High = 1.55 },
                    new OhlcvPriceOnly { Ticks = 156, High = 1.56 }
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

        private static int OhlcvPriceOnlyDataExploratoryTest02()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using OhlcvPriceOnlyData data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Default, DataTimeFrame.Minute3, true);

                #region (1) Initial append

                Trace.TraceInformation("--- 02 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);

                #endregion

                #region (2)

                Trace.TraceInformation("--- 02 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1690, High = 1.69 },
                    new OhlcvPriceOnly { Ticks = 1710, High = 1.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1690, High = 1.69 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 },
                    new OhlcvPriceOnly { Ticks = 1710, High = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 670, High = 0.67 },
                    new OhlcvPriceOnly { Ticks = 680, High = 0.68 },
                    new OhlcvPriceOnly { Ticks = 720, High = 0.72 },
                    new OhlcvPriceOnly { Ticks = 730, High = 0.73 },
                    new OhlcvPriceOnly { Ticks = 775, High = 0.775 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 670, High = 0.67 },
                    new OhlcvPriceOnly { Ticks = 680, High = 0.68 },
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 },
                    new OhlcvPriceOnly { Ticks = 720, High = 0.72 },
                    new OhlcvPriceOnly { Ticks = 730, High = 0.73 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 775, High = 0.775 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1690, High = 1.69 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 },
                    new OhlcvPriceOnly { Ticks = 1710, High = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1685, High = 1.685 },
                    new OhlcvPriceOnly { Ticks = 1695, High = 1.695 },
                    new OhlcvPriceOnly { Ticks = 1697, High = 1.697 },
                    new OhlcvPriceOnly { Ticks = 1698, High = 1.698 },
                    new OhlcvPriceOnly { Ticks = 1715, High = 1.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 670, High = 0.67 },
                    new OhlcvPriceOnly { Ticks = 680, High = 0.68 },
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 },
                    new OhlcvPriceOnly { Ticks = 720, High = 0.72 },
                    new OhlcvPriceOnly { Ticks = 730, High = 0.73 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 775, High = 0.775 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1685, High = 1.685 },
                    new OhlcvPriceOnly { Ticks = 1690, High = 1.69 },
                    new OhlcvPriceOnly { Ticks = 1695, High = 1.695 },
                    new OhlcvPriceOnly { Ticks = 1697, High = 1.697 },
                    new OhlcvPriceOnly { Ticks = 1698, High = 1.698 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 },
                    new OhlcvPriceOnly { Ticks = 1710, High = 1.71 },
                    new OhlcvPriceOnly { Ticks = 1715, High = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 669, High = 0.669 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 669, High = 0.669 },
                    new OhlcvPriceOnly { Ticks = 670, High = 0.67 },
                    new OhlcvPriceOnly { Ticks = 680, High = 0.68 },
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 },
                    new OhlcvPriceOnly { Ticks = 720, High = 0.72 },
                    new OhlcvPriceOnly { Ticks = 730, High = 0.73 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 775, High = 0.775 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1685, High = 1.685 },
                    new OhlcvPriceOnly { Ticks = 1690, High = 1.69 },
                    new OhlcvPriceOnly { Ticks = 1695, High = 1.695 },
                    new OhlcvPriceOnly { Ticks = 1697, High = 1.697 },
                    new OhlcvPriceOnly { Ticks = 1698, High = 1.698 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 },
                    new OhlcvPriceOnly { Ticks = 1710, High = 1.71 },
                    new OhlcvPriceOnly { Ticks = 1715, High = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1716, High = 1.76 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 669, High = 0.669 },
                    new OhlcvPriceOnly { Ticks = 670, High = 0.67 },
                    new OhlcvPriceOnly { Ticks = 680, High = 0.68 },
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 },
                    new OhlcvPriceOnly { Ticks = 720, High = 0.72 },
                    new OhlcvPriceOnly { Ticks = 730, High = 0.73 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 775, High = 0.775 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1685, High = 1.685 },
                    new OhlcvPriceOnly { Ticks = 1690, High = 1.69 },
                    new OhlcvPriceOnly { Ticks = 1695, High = 1.695 },
                    new OhlcvPriceOnly { Ticks = 1697, High = 1.697 },
                    new OhlcvPriceOnly { Ticks = 1698, High = 1.698 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 },
                    new OhlcvPriceOnly { Ticks = 1710, High = 1.71 },
                    new OhlcvPriceOnly { Ticks = 1715, High = 1.75 },
                    new OhlcvPriceOnly { Ticks = 1716, High = 1.76 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 665, High = 0.665 },
                    new OhlcvPriceOnly { Ticks = 1720, High = 1.72 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 665, High = 0.665 },
                    new OhlcvPriceOnly { Ticks = 669, High = 0.669 },
                    new OhlcvPriceOnly { Ticks = 670, High = 0.67 },
                    new OhlcvPriceOnly { Ticks = 680, High = 0.68 },
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 },
                    new OhlcvPriceOnly { Ticks = 720, High = 0.72 },
                    new OhlcvPriceOnly { Ticks = 730, High = 0.73 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 775, High = 0.775 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1685, High = 1.685 },
                    new OhlcvPriceOnly { Ticks = 1690, High = 1.69 },
                    new OhlcvPriceOnly { Ticks = 1695, High = 1.695 },
                    new OhlcvPriceOnly { Ticks = 1697, High = 1.697 },
                    new OhlcvPriceOnly { Ticks = 1698, High = 1.698 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 },
                    new OhlcvPriceOnly { Ticks = 1710, High = 1.71 },
                    new OhlcvPriceOnly { Ticks = 1715, High = 1.75 },
                    new OhlcvPriceOnly { Ticks = 1716, High = 1.76 },
                    new OhlcvPriceOnly { Ticks = 1720, High = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 668, High = 0.668 },
                    new OhlcvPriceOnly { Ticks = 1717, High = 1.717 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 665, High = 0.665 },
                    new OhlcvPriceOnly { Ticks = 668, High = 0.668 },
                    new OhlcvPriceOnly { Ticks = 669, High = 0.669 },
                    new OhlcvPriceOnly { Ticks = 670, High = 0.67 },
                    new OhlcvPriceOnly { Ticks = 680, High = 0.68 },
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 },
                    new OhlcvPriceOnly { Ticks = 720, High = 0.72 },
                    new OhlcvPriceOnly { Ticks = 730, High = 0.73 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 775, High = 0.775 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1685, High = 1.685 },
                    new OhlcvPriceOnly { Ticks = 1690, High = 1.69 },
                    new OhlcvPriceOnly { Ticks = 1695, High = 1.695 },
                    new OhlcvPriceOnly { Ticks = 1697, High = 1.697 },
                    new OhlcvPriceOnly { Ticks = 1698, High = 1.698 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 },
                    new OhlcvPriceOnly { Ticks = 1710, High = 1.71 },
                    new OhlcvPriceOnly { Ticks = 1715, High = 1.75 },
                    new OhlcvPriceOnly { Ticks = 1716, High = 1.76 },
                    new OhlcvPriceOnly { Ticks = 1717, High = 1.717 },
                    new OhlcvPriceOnly { Ticks = 1720, High = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 666, High = 0.666 },
                    new OhlcvPriceOnly { Ticks = 667, High = 0.667 },
                    new OhlcvPriceOnly { Ticks = 1718, High = 1.718 },
                    new OhlcvPriceOnly { Ticks = 1719, High = 1.719 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 665, High = 0.665 },
                    new OhlcvPriceOnly { Ticks = 666, High = 0.666 },
                    new OhlcvPriceOnly { Ticks = 667, High = 0.667 },
                    new OhlcvPriceOnly { Ticks = 668, High = 0.668 },
                    new OhlcvPriceOnly { Ticks = 669, High = 0.669 },
                    new OhlcvPriceOnly { Ticks = 670, High = 0.67 },
                    new OhlcvPriceOnly { Ticks = 680, High = 0.68 },
                    new OhlcvPriceOnly { Ticks = 690, High = 0.69 },
                    new OhlcvPriceOnly { Ticks = 700, High = 0.7 },
                    new OhlcvPriceOnly { Ticks = 710, High = 0.71 },
                    new OhlcvPriceOnly { Ticks = 720, High = 0.72 },
                    new OhlcvPriceOnly { Ticks = 730, High = 0.73 },
                    new OhlcvPriceOnly { Ticks = 750, High = 0.75 },
                    new OhlcvPriceOnly { Ticks = 751, High = 0.751 },
                    new OhlcvPriceOnly { Ticks = 752, High = 0.752 },
                    new OhlcvPriceOnly { Ticks = 753, High = 0.753 },
                    new OhlcvPriceOnly { Ticks = 754, High = 0.754 },
                    new OhlcvPriceOnly { Ticks = 770, High = 0.77 },
                    new OhlcvPriceOnly { Ticks = 775, High = 0.775 },
                    new OhlcvPriceOnly { Ticks = 780, High = 0.78 },
                    new OhlcvPriceOnly { Ticks = 800, High = 0.8 },
                    new OhlcvPriceOnly { Ticks = 900, High = 0.9 },
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 },
                    new OhlcvPriceOnly { Ticks = 1400, High = 1.4 },
                    new OhlcvPriceOnly { Ticks = 1500, High = 1.5 },
                    new OhlcvPriceOnly { Ticks = 1600, High = 1.6 },
                    new OhlcvPriceOnly { Ticks = 1650, High = 1.65 },
                    new OhlcvPriceOnly { Ticks = 1670, High = 1.67 },
                    new OhlcvPriceOnly { Ticks = 1680, High = 1.68 },
                    new OhlcvPriceOnly { Ticks = 1685, High = 1.685 },
                    new OhlcvPriceOnly { Ticks = 1690, High = 1.69 },
                    new OhlcvPriceOnly { Ticks = 1695, High = 1.695 },
                    new OhlcvPriceOnly { Ticks = 1697, High = 1.697 },
                    new OhlcvPriceOnly { Ticks = 1698, High = 1.698 },
                    new OhlcvPriceOnly { Ticks = 1700, High = 1.7 },
                    new OhlcvPriceOnly { Ticks = 1710, High = 1.71 },
                    new OhlcvPriceOnly { Ticks = 1715, High = 1.75 },
                    new OhlcvPriceOnly { Ticks = 1716, High = 1.76 },
                    new OhlcvPriceOnly { Ticks = 1717, High = 1.717 },
                    new OhlcvPriceOnly { Ticks = 1718, High = 1.718 },
                    new OhlcvPriceOnly { Ticks = 1719, High = 1.719 },
                    new OhlcvPriceOnly { Ticks = 1720, High = 1.72 }
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

        private static int OhlcvPriceOnlyDataExploratoryTest03()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using OhlcvPriceOnlyData data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Default, DataTimeFrame.Minute3, true);

                #region (1) Initial append
                Trace.TraceInformation("--- 03 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                var list = new List<OhlcvPriceOnly>();

                #region (2)
                Trace.TraceInformation("--- 03 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                list.Clear();
                status = data.FetchAll(list);
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(0));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(9000));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(0));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(9000));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(999));
                verified = Verify(new List<OhlcvPriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1301), new DateTime(9000));
                verified = Verify(new List<OhlcvPriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1001));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1299), new DateTime(9000));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1290), new DateTime(9000));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1009));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1110));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1270), new DateTime(1300));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1300));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 19");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1110));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 20");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1105));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 21");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1103), new DateTime(1110));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 22");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1101), new DateTime(1109));
                verified = Verify(new List<OhlcvPriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 23");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1104), new DateTime(1106));
                verified = Verify(new List<OhlcvPriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 24");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1291), new DateTime(1299));
                verified = Verify(new List<OhlcvPriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 25");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1294), new DateTime(1296));
                verified = Verify(new List<OhlcvPriceOnly>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 26");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(1300));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 27");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1100));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 28");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1205));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 29");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1200));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 30");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1110), new DateTime(1205));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 31");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1115));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 32");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1002), new DateTime(1115));
                verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 }
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

        private static int OhlcvPriceOnlyDataExploratoryTest04()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using OhlcvPriceOnlyData data = instrument.OpenOhlcvPriceOnly(OhlcvKind.Default, DataTimeFrame.Minute3, true);

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
                bool status = Add(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<OhlcvPriceOnly>
                {
                    new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                    new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                    new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                    new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                    new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                    new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                    new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                    new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                    new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                    new OhlcvPriceOnly { Ticks = 1009, High = 1.009 },
                    new OhlcvPriceOnly { Ticks = 1100, High = 1.1 },
                    new OhlcvPriceOnly { Ticks = 1110, High = 1.11 },
                    new OhlcvPriceOnly { Ticks = 1120, High = 1.12 },
                    new OhlcvPriceOnly { Ticks = 1130, High = 1.13 },
                    new OhlcvPriceOnly { Ticks = 1140, High = 1.14 },
                    new OhlcvPriceOnly { Ticks = 1150, High = 1.15 },
                    new OhlcvPriceOnly { Ticks = 1160, High = 1.16 },
                    new OhlcvPriceOnly { Ticks = 1170, High = 1.17 },
                    new OhlcvPriceOnly { Ticks = 1180, High = 1.18 },
                    new OhlcvPriceOnly { Ticks = 1190, High = 1.19 },
                    new OhlcvPriceOnly { Ticks = 1200, High = 1.2 },
                    new OhlcvPriceOnly { Ticks = 1210, High = 1.21 },
                    new OhlcvPriceOnly { Ticks = 1220, High = 1.22 },
                    new OhlcvPriceOnly { Ticks = 1230, High = 1.23 },
                    new OhlcvPriceOnly { Ticks = 1240, High = 1.24 },
                    new OhlcvPriceOnly { Ticks = 1250, High = 1.25 },
                    new OhlcvPriceOnly { Ticks = 1260, High = 1.26 },
                    new OhlcvPriceOnly { Ticks = 1270, High = 1.27 },
                    new OhlcvPriceOnly { Ticks = 1280, High = 1.28 },
                    new OhlcvPriceOnly { Ticks = 1290, High = 1.29 },
                    new OhlcvPriceOnly { Ticks = 1300, High = 1.3 }
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

        private static int OhlcvPriceOnlyDataExploratoryTest05()
        {
            int errorCount = 0;

            #region (1) Spread
            Trace.TraceInformation("--- 05 (1) Spread ----------------");
            Trace.TraceInformation("STEP 1");
            var list = new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.002 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.003 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.004 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.005 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.006 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.007 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.008 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.009 }
            };
            OhlcvPriceOnly[] array = list.ToArray();
            bool status = OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            bool verified = Verify(new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                new OhlcvPriceOnly { Ticks = 1009, High = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 2");
            list = new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                new OhlcvPriceOnly { Ticks = 1009, High = 1.009 }
            };
            array = list.ToArray();
            status = OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                new OhlcvPriceOnly { Ticks = 1004, High = 1.004 },
                new OhlcvPriceOnly { Ticks = 1005, High = 1.005 },
                new OhlcvPriceOnly { Ticks = 1006, High = 1.006 },
                new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                new OhlcvPriceOnly { Ticks = 1009, High = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 3");
            list = new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 }
            };
            array = list.ToArray();
            status = OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 4");
            list = new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.001 }
            };
            array = list.ToArray();
            status = OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 5");
            list = new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.002 }
            };
            array = list.ToArray();
            status = OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1002, High = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 6");
            list = new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1000, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1002, High = 1.002 }
            };
            array = list.ToArray();
            status = OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1002, High = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 7");
            list = new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                new OhlcvPriceOnly { Ticks = 1008, High = 1.009 }
            };
            array = list.ToArray();
            status = OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1007, High = 1.007 },
                new OhlcvPriceOnly { Ticks = 1008, High = 1.008 },
                new OhlcvPriceOnly { Ticks = 1009, High = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 8");
            list = new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.002 },
                new OhlcvPriceOnly { Ticks = 1003, High = 1.003 }
            };
            array = list.ToArray();
            status = OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                new OhlcvPriceOnly { Ticks = 1003, High = 1.003 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 9");
            list = new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.002 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.003 },
                new OhlcvPriceOnly { Ticks = 1004, High = 1.004 }
            };
            array = list.ToArray();
            status = OhlcvPriceOnlyData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<OhlcvPriceOnly>
            {
                new OhlcvPriceOnly { Ticks = 1000, High = 1.0 },
                new OhlcvPriceOnly { Ticks = 1001, High = 1.001 },
                new OhlcvPriceOnly { Ticks = 1002, High = 1.002 },
                new OhlcvPriceOnly { Ticks = 1003, High = 1.003 },
                new OhlcvPriceOnly { Ticks = 1004, High = 1.004 }
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
