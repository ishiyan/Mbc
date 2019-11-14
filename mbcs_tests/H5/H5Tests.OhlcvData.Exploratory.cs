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
        public void H5_OhlcvData_Exploratory_01()
        {
            int errorCount = OhlcvDataExploratoryTest01();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_OhlcvData_Exploratory_02()
        {
            int errorCount = OhlcvDataExploratoryTest02();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_OhlcvData_Exploratory_03()
        {
            int errorCount = OhlcvDataExploratoryTest03();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_OhlcvData_Exploratory_04()
        {
            int errorCount = OhlcvDataExploratoryTest04();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_OhlcvData_Exploratory_05()
        {
            int errorCount = OhlcvDataExploratoryTest05();
            Assert.AreEqual(0, errorCount);
        }

        private static bool Add(List<Ohlcv> list, OhlcvData data, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
        {
            Ohlcv[] array = list.ToArray();
            OhlcvData.SpreadDuplicateTimeTicks(array, verbose);
            return data.Add(array, duplicateTimeTicks, verbose);
        }

        private static bool Verify(List<Ohlcv> pattern, OhlcvData data)
        {
            var sample = new List<Ohlcv>();
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

        private static bool Verify(List<Ohlcv> pattern, List<Ohlcv> sample)
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

        private static int OhlcvDataExploratoryTest01()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using OhlcvData data = instrument.OpenOhlcv(OhlcvKind.Default, DataTimeFrame.Minute3, true);

                #region (1) Initial append
                Trace.TraceInformation("--- 01 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                bool verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (2) Update / Update
                Trace.TraceInformation("--- 01 (2) Update/Update ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.01 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.01 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 130, High = 1.31 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.31 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.51 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.51 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (3) Update / Skip
                Trace.TraceInformation("--- 01 (3) Update/Skip ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.01 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 130, High = 1.31 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.51 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (4) Update / Fail
                Trace.TraceInformation("--- 01 (4) Update/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.01 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 130, High = 1.31 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv> {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.51 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                #endregion

                #region (5) Prepend
                Trace.TraceInformation("--- 01 (5) Prepend/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 97, High = 0.97 },
                    new Ohlcv { Ticks = 98, High = 0.98 },
                    new Ohlcv { Ticks = 99, High = 0.99 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 97, High = 0.97 },
                    new Ohlcv { Ticks = 98, High = 0.98 },
                    new Ohlcv { Ticks = 99, High = 0.99 },
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 94, High = 0.94 },
                    new Ohlcv { Ticks = 94, High = 0.95 },
                    new Ohlcv { Ticks = 94, High = 0.96 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 94, High = 0.94 },
                    new Ohlcv { Ticks = 95, High = 0.95 },
                    new Ohlcv { Ticks = 96, High = 0.96 },
                    new Ohlcv { Ticks = 97, High = 0.97 },
                    new Ohlcv { Ticks = 98, High = 0.98 },
                    new Ohlcv { Ticks = 99, High = 0.99 },
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 90, High = 0.90 },
                    new Ohlcv { Ticks = 90, High = 0.91 },
                    new Ohlcv { Ticks = 90, High = 0.92 },
                    new Ohlcv { Ticks = 93, High = 0.93 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 90, High = 0.90 },
                    new Ohlcv { Ticks = 91, High = 0.91 },
                    new Ohlcv { Ticks = 92, High = 0.92 },
                    new Ohlcv { Ticks = 93, High = 0.93 },
                    new Ohlcv { Ticks = 94, High = 0.94 },
                    new Ohlcv { Ticks = 95, High = 0.95 },
                    new Ohlcv { Ticks = 96, High = 0.96 },
                    new Ohlcv { Ticks = 97, High = 0.97 },
                    new Ohlcv { Ticks = 98, High = 0.98 },
                    new Ohlcv { Ticks = 99, High = 0.99 },
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 87, High = 0.87 },
                    new Ohlcv { Ticks = 88, High = 0.88 },
                    new Ohlcv { Ticks = 88, High = 0.89 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 87, High = 0.87 },
                    new Ohlcv { Ticks = 88, High = 0.88 },
                    new Ohlcv { Ticks = 89, High = 0.89 },
                    new Ohlcv { Ticks = 90, High = 0.90 },
                    new Ohlcv { Ticks = 91, High = 0.91 },
                    new Ohlcv { Ticks = 92, High = 0.92 },
                    new Ohlcv { Ticks = 93, High = 0.93 },
                    new Ohlcv { Ticks = 94, High = 0.94 },
                    new Ohlcv { Ticks = 95, High = 0.95 },
                    new Ohlcv { Ticks = 96, High = 0.96 },
                    new Ohlcv { Ticks = 97, High = 0.97 },
                    new Ohlcv { Ticks = 98, High = 0.98 },
                    new Ohlcv { Ticks = 99, High = 0.99 },
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (6) Append
                Trace.TraceInformation("--- 01 (6) Append/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 151, High = 1.51 },
                    new Ohlcv { Ticks = 151, High = 1.52 },
                    new Ohlcv { Ticks = 151, High = 1.53 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 87, High = 0.87 },
                    new Ohlcv { Ticks = 88, High = 0.88 },
                    new Ohlcv { Ticks = 89, High = 0.89 },
                    new Ohlcv { Ticks = 90, High = 0.90 },
                    new Ohlcv { Ticks = 91, High = 0.91 },
                    new Ohlcv { Ticks = 92, High = 0.92 },
                    new Ohlcv { Ticks = 93, High = 0.93 },
                    new Ohlcv { Ticks = 94, High = 0.94 },
                    new Ohlcv { Ticks = 95, High = 0.95 },
                    new Ohlcv { Ticks = 96, High = 0.96 },
                    new Ohlcv { Ticks = 97, High = 0.97 },
                    new Ohlcv { Ticks = 98, High = 0.98 },
                    new Ohlcv { Ticks = 99, High = 0.99 },
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 150, High = 1.5 },
                    new Ohlcv { Ticks = 151, High = 1.51 },
                    new Ohlcv { Ticks = 152, High = 1.52 },
                    new Ohlcv { Ticks = 153, High = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (7) Insert
                Trace.TraceInformation("--- 01 (7) Insert/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 131, High = 1.31 },
                    new Ohlcv { Ticks = 132, High = 1.32 },
                    new Ohlcv { Ticks = 133, High = 1.33 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 87, High = 0.87 },
                    new Ohlcv { Ticks = 88, High = 0.88 },
                    new Ohlcv { Ticks = 89, High = 0.89 },
                    new Ohlcv { Ticks = 90, High = 0.90 },
                    new Ohlcv { Ticks = 91, High = 0.91 },
                    new Ohlcv { Ticks = 92, High = 0.92 },
                    new Ohlcv { Ticks = 93, High = 0.93 },
                    new Ohlcv { Ticks = 94, High = 0.94 },
                    new Ohlcv { Ticks = 95, High = 0.95 },
                    new Ohlcv { Ticks = 96, High = 0.96 },
                    new Ohlcv { Ticks = 97, High = 0.97 },
                    new Ohlcv { Ticks = 98, High = 0.98 },
                    new Ohlcv { Ticks = 99, High = 0.99 },
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 131, High = 1.31 },
                    new Ohlcv { Ticks = 132, High = 1.32 },
                    new Ohlcv { Ticks = 133, High = 1.33 },
                    new Ohlcv { Ticks = 150, High = 1.5 },
                    new Ohlcv { Ticks = 151, High = 1.51 },
                    new Ohlcv { Ticks = 152, High = 1.52 },
                    new Ohlcv { Ticks = 153, High = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 127, High = 1.27 },
                    new Ohlcv { Ticks = 128, High = 1.28 },
                    new Ohlcv { Ticks = 129, High = 1.29 },
                    new Ohlcv { Ticks = 134, High = 1.34 },
                    new Ohlcv { Ticks = 135, High = 1.35 },
                    new Ohlcv { Ticks = 136, High = 1.36 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 87, High = 0.87 },
                    new Ohlcv { Ticks = 88, High = 0.88 },
                    new Ohlcv { Ticks = 89, High = 0.89 },
                    new Ohlcv { Ticks = 90, High = 0.90 },
                    new Ohlcv { Ticks = 91, High = 0.91 },
                    new Ohlcv { Ticks = 92, High = 0.92 },
                    new Ohlcv { Ticks = 93, High = 0.93 },
                    new Ohlcv { Ticks = 94, High = 0.94 },
                    new Ohlcv { Ticks = 95, High = 0.95 },
                    new Ohlcv { Ticks = 96, High = 0.96 },
                    new Ohlcv { Ticks = 97, High = 0.97 },
                    new Ohlcv { Ticks = 98, High = 0.98 },
                    new Ohlcv { Ticks = 99, High = 0.99 },
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 127, High = 1.27 },
                    new Ohlcv { Ticks = 128, High = 1.28 },
                    new Ohlcv { Ticks = 129, High = 1.29 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 131, High = 1.31 },
                    new Ohlcv { Ticks = 132, High = 1.32 },
                    new Ohlcv { Ticks = 133, High = 1.33 },
                    new Ohlcv { Ticks = 134, High = 1.34 },
                    new Ohlcv { Ticks = 135, High = 1.35 },
                    new Ohlcv { Ticks = 136, High = 1.36 },
                    new Ohlcv { Ticks = 150, High = 1.5 },
                    new Ohlcv { Ticks = 151, High = 1.51 },
                    new Ohlcv { Ticks = 152, High = 1.52 },
                    new Ohlcv { Ticks = 153, High = 1.53 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 146, High = 1.46 },
                    new Ohlcv { Ticks = 147, High = 1.47 },
                    new Ohlcv { Ticks = 148, High = 1.48 },
                    new Ohlcv { Ticks = 149, High = 1.49 },
                    new Ohlcv { Ticks = 150, High = 1.50 },
                    new Ohlcv { Ticks = 153, High = 1.53 },
                    new Ohlcv { Ticks = 154, High = 1.54 },
                    new Ohlcv { Ticks = 155, High = 1.55 },
                    new Ohlcv { Ticks = 156, High = 1.56 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 87, High = 0.87 },
                    new Ohlcv { Ticks = 88, High = 0.88 },
                    new Ohlcv { Ticks = 89, High = 0.89 },
                    new Ohlcv { Ticks = 90, High = 0.90 },
                    new Ohlcv { Ticks = 91, High = 0.91 },
                    new Ohlcv { Ticks = 92, High = 0.92 },
                    new Ohlcv { Ticks = 93, High = 0.93 },
                    new Ohlcv { Ticks = 94, High = 0.94 },
                    new Ohlcv { Ticks = 95, High = 0.95 },
                    new Ohlcv { Ticks = 96, High = 0.96 },
                    new Ohlcv { Ticks = 97, High = 0.97 },
                    new Ohlcv { Ticks = 98, High = 0.98 },
                    new Ohlcv { Ticks = 99, High = 0.99 },
                    new Ohlcv { Ticks = 100, High = 1.0 },
                    new Ohlcv { Ticks = 110, High = 1.1 },
                    new Ohlcv { Ticks = 127, High = 1.27 },
                    new Ohlcv { Ticks = 128, High = 1.28 },
                    new Ohlcv { Ticks = 129, High = 1.29 },
                    new Ohlcv { Ticks = 130, High = 1.3 },
                    new Ohlcv { Ticks = 131, High = 1.31 },
                    new Ohlcv { Ticks = 132, High = 1.32 },
                    new Ohlcv { Ticks = 133, High = 1.33 },
                    new Ohlcv { Ticks = 134, High = 1.34 },
                    new Ohlcv { Ticks = 135, High = 1.35 },
                    new Ohlcv { Ticks = 136, High = 1.36 },
                    new Ohlcv { Ticks = 146, High = 1.46 },
                    new Ohlcv { Ticks = 147, High = 1.47 },
                    new Ohlcv { Ticks = 148, High = 1.48 },
                    new Ohlcv { Ticks = 149, High = 1.49 },
                    new Ohlcv { Ticks = 150, High = 1.5 },
                    new Ohlcv { Ticks = 151, High = 1.51 },
                    new Ohlcv { Ticks = 152, High = 1.52 },
                    new Ohlcv { Ticks = 153, High = 1.53 },
                    new Ohlcv { Ticks = 154, High = 1.54 },
                    new Ohlcv { Ticks = 155, High = 1.55 },
                    new Ohlcv { Ticks = 156, High = 1.56 }
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

        private static int OhlcvDataExploratoryTest02()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using OhlcvData data = instrument.OpenOhlcv(OhlcvKind.Default, DataTimeFrame.Minute3, true);

                #region (1) Initial append

                Trace.TraceInformation("--- 02 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);

                #endregion

                #region (2)

                Trace.TraceInformation("--- 02 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 700, High = 0.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1700, High = 1.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1650, High = 1.65 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 780, High = 0.78 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 750, High = 0.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 710, High = 0.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 710, High = 0.71 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1700, High = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1690, High = 1.69 },
                    new Ohlcv { Ticks = 1710, High = 1.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 710, High = 0.71 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1690, High = 1.69 },
                    new Ohlcv { Ticks = 1700, High = 1.7 },
                    new Ohlcv { Ticks = 1710, High = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 670, High = 0.67 },
                    new Ohlcv { Ticks = 680, High = 0.68 },
                    new Ohlcv { Ticks = 720, High = 0.72 },
                    new Ohlcv { Ticks = 730, High = 0.73 },
                    new Ohlcv { Ticks = 775, High = 0.775 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 670, High = 0.67 },
                    new Ohlcv { Ticks = 680, High = 0.68 },
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 710, High = 0.71 },
                    new Ohlcv { Ticks = 720, High = 0.72 },
                    new Ohlcv { Ticks = 730, High = 0.73 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 775, High = 0.775 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1690, High = 1.69 },
                    new Ohlcv { Ticks = 1700, High = 1.7 },
                    new Ohlcv { Ticks = 1710, High = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1685, High = 1.685 },
                    new Ohlcv { Ticks = 1695, High = 1.695 },
                    new Ohlcv { Ticks = 1697, High = 1.697 },
                    new Ohlcv { Ticks = 1698, High = 1.698 },
                    new Ohlcv { Ticks = 1715, High = 1.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 670, High = 0.67 },
                    new Ohlcv { Ticks = 680, High = 0.68 },
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 710, High = 0.71 },
                    new Ohlcv { Ticks = 720, High = 0.72 },
                    new Ohlcv { Ticks = 730, High = 0.73 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 775, High = 0.775 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1685, High = 1.685 },
                    new Ohlcv { Ticks = 1690, High = 1.69 },
                    new Ohlcv { Ticks = 1695, High = 1.695 },
                    new Ohlcv { Ticks = 1697, High = 1.697 },
                    new Ohlcv { Ticks = 1698, High = 1.698 },
                    new Ohlcv { Ticks = 1700, High = 1.7 },
                    new Ohlcv { Ticks = 1710, High = 1.71 },
                    new Ohlcv { Ticks = 1715, High = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 669, High = 0.669 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 669, High = 0.669 },
                    new Ohlcv { Ticks = 670, High = 0.67 },
                    new Ohlcv { Ticks = 680, High = 0.68 },
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 710, High = 0.71 },
                    new Ohlcv { Ticks = 720, High = 0.72 },
                    new Ohlcv { Ticks = 730, High = 0.73 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 775, High = 0.775 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1685, High = 1.685 },
                    new Ohlcv { Ticks = 1690, High = 1.69 },
                    new Ohlcv { Ticks = 1695, High = 1.695 },
                    new Ohlcv { Ticks = 1697, High = 1.697 },
                    new Ohlcv { Ticks = 1698, High = 1.698 },
                    new Ohlcv { Ticks = 1700, High = 1.7 },
                    new Ohlcv { Ticks = 1710, High = 1.71 },
                    new Ohlcv { Ticks = 1715, High = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1716, High = 1.76 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 669, High = 0.669 },
                    new Ohlcv { Ticks = 670, High = 0.67 },
                    new Ohlcv { Ticks = 680, High = 0.68 },
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 710, High = 0.71 },
                    new Ohlcv { Ticks = 720, High = 0.72 },
                    new Ohlcv { Ticks = 730, High = 0.73 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 775, High = 0.775 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1685, High = 1.685 },
                    new Ohlcv { Ticks = 1690, High = 1.69 },
                    new Ohlcv { Ticks = 1695, High = 1.695 },
                    new Ohlcv { Ticks = 1697, High = 1.697 },
                    new Ohlcv { Ticks = 1698, High = 1.698 },
                    new Ohlcv { Ticks = 1700, High = 1.7 },
                    new Ohlcv { Ticks = 1710, High = 1.71 },
                    new Ohlcv { Ticks = 1715, High = 1.75 },
                    new Ohlcv { Ticks = 1716, High = 1.76 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 665, High = 0.665 },
                    new Ohlcv { Ticks = 1720, High = 1.72 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 665, High = 0.665 },
                    new Ohlcv { Ticks = 669, High = 0.669 },
                    new Ohlcv { Ticks = 670, High = 0.67 },
                    new Ohlcv { Ticks = 680, High = 0.68 },
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 710, High = 0.71 },
                    new Ohlcv { Ticks = 720, High = 0.72 },
                    new Ohlcv { Ticks = 730, High = 0.73 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 775, High = 0.775 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1685, High = 1.685 },
                    new Ohlcv { Ticks = 1690, High = 1.69 },
                    new Ohlcv { Ticks = 1695, High = 1.695 },
                    new Ohlcv { Ticks = 1697, High = 1.697 },
                    new Ohlcv { Ticks = 1698, High = 1.698 },
                    new Ohlcv { Ticks = 1700, High = 1.7 },
                    new Ohlcv { Ticks = 1710, High = 1.71 },
                    new Ohlcv { Ticks = 1715, High = 1.75 },
                    new Ohlcv { Ticks = 1716, High = 1.76 },
                    new Ohlcv { Ticks = 1720, High = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 668, High = 0.668 },
                    new Ohlcv { Ticks = 1717, High = 1.717 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 665, High = 0.665 },
                    new Ohlcv { Ticks = 668, High = 0.668 },
                    new Ohlcv { Ticks = 669, High = 0.669 },
                    new Ohlcv { Ticks = 670, High = 0.67 },
                    new Ohlcv { Ticks = 680, High = 0.68 },
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 710, High = 0.71 },
                    new Ohlcv { Ticks = 720, High = 0.72 },
                    new Ohlcv { Ticks = 730, High = 0.73 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 775, High = 0.775 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1685, High = 1.685 },
                    new Ohlcv { Ticks = 1690, High = 1.69 },
                    new Ohlcv { Ticks = 1695, High = 1.695 },
                    new Ohlcv { Ticks = 1697, High = 1.697 },
                    new Ohlcv { Ticks = 1698, High = 1.698 },
                    new Ohlcv { Ticks = 1700, High = 1.7 },
                    new Ohlcv { Ticks = 1710, High = 1.71 },
                    new Ohlcv { Ticks = 1715, High = 1.75 },
                    new Ohlcv { Ticks = 1716, High = 1.76 },
                    new Ohlcv { Ticks = 1717, High = 1.717 },
                    new Ohlcv { Ticks = 1720, High = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 666, High = 0.666 },
                    new Ohlcv { Ticks = 667, High = 0.667 },
                    new Ohlcv { Ticks = 1718, High = 1.718 },
                    new Ohlcv { Ticks = 1719, High = 1.719 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 665, High = 0.665 },
                    new Ohlcv { Ticks = 666, High = 0.666 },
                    new Ohlcv { Ticks = 667, High = 0.667 },
                    new Ohlcv { Ticks = 668, High = 0.668 },
                    new Ohlcv { Ticks = 669, High = 0.669 },
                    new Ohlcv { Ticks = 670, High = 0.67 },
                    new Ohlcv { Ticks = 680, High = 0.68 },
                    new Ohlcv { Ticks = 690, High = 0.69 },
                    new Ohlcv { Ticks = 700, High = 0.7 },
                    new Ohlcv { Ticks = 710, High = 0.71 },
                    new Ohlcv { Ticks = 720, High = 0.72 },
                    new Ohlcv { Ticks = 730, High = 0.73 },
                    new Ohlcv { Ticks = 750, High = 0.75 },
                    new Ohlcv { Ticks = 751, High = 0.751 },
                    new Ohlcv { Ticks = 752, High = 0.752 },
                    new Ohlcv { Ticks = 753, High = 0.753 },
                    new Ohlcv { Ticks = 754, High = 0.754 },
                    new Ohlcv { Ticks = 770, High = 0.77 },
                    new Ohlcv { Ticks = 775, High = 0.775 },
                    new Ohlcv { Ticks = 780, High = 0.78 },
                    new Ohlcv { Ticks = 800, High = 0.8 },
                    new Ohlcv { Ticks = 900, High = 0.9 },
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1300, High = 1.3 },
                    new Ohlcv { Ticks = 1400, High = 1.4 },
                    new Ohlcv { Ticks = 1500, High = 1.5 },
                    new Ohlcv { Ticks = 1600, High = 1.6 },
                    new Ohlcv { Ticks = 1650, High = 1.65 },
                    new Ohlcv { Ticks = 1670, High = 1.67 },
                    new Ohlcv { Ticks = 1680, High = 1.68 },
                    new Ohlcv { Ticks = 1685, High = 1.685 },
                    new Ohlcv { Ticks = 1690, High = 1.69 },
                    new Ohlcv { Ticks = 1695, High = 1.695 },
                    new Ohlcv { Ticks = 1697, High = 1.697 },
                    new Ohlcv { Ticks = 1698, High = 1.698 },
                    new Ohlcv { Ticks = 1700, High = 1.7 },
                    new Ohlcv { Ticks = 1710, High = 1.71 },
                    new Ohlcv { Ticks = 1715, High = 1.75 },
                    new Ohlcv { Ticks = 1716, High = 1.76 },
                    new Ohlcv { Ticks = 1717, High = 1.717 },
                    new Ohlcv { Ticks = 1718, High = 1.718 },
                    new Ohlcv { Ticks = 1719, High = 1.719 },
                    new Ohlcv { Ticks = 1720, High = 1.72 }
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

        private static int OhlcvDataExploratoryTest03()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using OhlcvData data = instrument.OpenOhlcv(OhlcvKind.Default, DataTimeFrame.Minute3, true);

                #region (1) Initial append
                Trace.TraceInformation("--- 03 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                var list = new List<Ohlcv>();

                #region (2)
                Trace.TraceInformation("--- 03 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                list.Clear();
                status = data.FetchAll(list);
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(0));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(9000));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(0));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(9000));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(999));
                verified = Verify(new List<Ohlcv>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1301), new DateTime(9000));
                verified = Verify(new List<Ohlcv>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1001));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1299), new DateTime(9000));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1290), new DateTime(9000));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1009));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1110));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1270), new DateTime(1300));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1300));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 19");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1110));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 20");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1105));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1100, High = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 21");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1103), new DateTime(1110));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1110, High = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 22");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1101), new DateTime(1109));
                verified = Verify(new List<Ohlcv>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 23");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1104), new DateTime(1106));
                verified = Verify(new List<Ohlcv>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 24");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1291), new DateTime(1299));
                verified = Verify(new List<Ohlcv>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 25");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1294), new DateTime(1296));
                verified = Verify(new List<Ohlcv>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 26");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(1300));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 27");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1100));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1100, High = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 28");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1205));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 29");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1200));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 30");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1110), new DateTime(1205));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 31");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1115));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 32");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1002), new DateTime(1115));
                verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 }
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

        private static int OhlcvDataExploratoryTest04()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using OhlcvData data = instrument.OpenOhlcv(OhlcvKind.Default, DataTimeFrame.Minute3, true);

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
                bool status = Add(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<Ohlcv>
                {
                    new Ohlcv { Ticks = 1000, High = 1.0 },
                    new Ohlcv { Ticks = 1001, High = 1.001 },
                    new Ohlcv { Ticks = 1002, High = 1.002 },
                    new Ohlcv { Ticks = 1003, High = 1.003 },
                    new Ohlcv { Ticks = 1004, High = 1.004 },
                    new Ohlcv { Ticks = 1005, High = 1.005 },
                    new Ohlcv { Ticks = 1006, High = 1.006 },
                    new Ohlcv { Ticks = 1007, High = 1.007 },
                    new Ohlcv { Ticks = 1008, High = 1.008 },
                    new Ohlcv { Ticks = 1009, High = 1.009 },
                    new Ohlcv { Ticks = 1100, High = 1.1 },
                    new Ohlcv { Ticks = 1110, High = 1.11 },
                    new Ohlcv { Ticks = 1120, High = 1.12 },
                    new Ohlcv { Ticks = 1130, High = 1.13 },
                    new Ohlcv { Ticks = 1140, High = 1.14 },
                    new Ohlcv { Ticks = 1150, High = 1.15 },
                    new Ohlcv { Ticks = 1160, High = 1.16 },
                    new Ohlcv { Ticks = 1170, High = 1.17 },
                    new Ohlcv { Ticks = 1180, High = 1.18 },
                    new Ohlcv { Ticks = 1190, High = 1.19 },
                    new Ohlcv { Ticks = 1200, High = 1.2 },
                    new Ohlcv { Ticks = 1210, High = 1.21 },
                    new Ohlcv { Ticks = 1220, High = 1.22 },
                    new Ohlcv { Ticks = 1230, High = 1.23 },
                    new Ohlcv { Ticks = 1240, High = 1.24 },
                    new Ohlcv { Ticks = 1250, High = 1.25 },
                    new Ohlcv { Ticks = 1260, High = 1.26 },
                    new Ohlcv { Ticks = 1270, High = 1.27 },
                    new Ohlcv { Ticks = 1280, High = 1.28 },
                    new Ohlcv { Ticks = 1290, High = 1.29 },
                    new Ohlcv { Ticks = 1300, High = 1.3 }
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

        private static int OhlcvDataExploratoryTest05()
        {
            int errorCount = 0;

            #region (1) Spread
            Trace.TraceInformation("--- 05 (1) Spread ----------------");
            Trace.TraceInformation("STEP 1");
            var list = new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1000, High = 1.001 },
                new Ohlcv { Ticks = 1000, High = 1.002 },
                new Ohlcv { Ticks = 1000, High = 1.003 },
                new Ohlcv { Ticks = 1000, High = 1.004 },
                new Ohlcv { Ticks = 1000, High = 1.005 },
                new Ohlcv { Ticks = 1000, High = 1.006 },
                new Ohlcv { Ticks = 1000, High = 1.007 },
                new Ohlcv { Ticks = 1000, High = 1.008 },
                new Ohlcv { Ticks = 1000, High = 1.009 }
            };
            Ohlcv[] array = list.ToArray();
            bool status = OhlcvData.SpreadDuplicateTimeTicks(array, true);
            bool verified = Verify(new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 },
                new Ohlcv { Ticks = 1002, High = 1.002 },
                new Ohlcv { Ticks = 1003, High = 1.003 },
                new Ohlcv { Ticks = 1004, High = 1.004 },
                new Ohlcv { Ticks = 1005, High = 1.005 },
                new Ohlcv { Ticks = 1006, High = 1.006 },
                new Ohlcv { Ticks = 1007, High = 1.007 },
                new Ohlcv { Ticks = 1008, High = 1.008 },
                new Ohlcv { Ticks = 1009, High = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 2");
            list = new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 },
                new Ohlcv { Ticks = 1002, High = 1.002 },
                new Ohlcv { Ticks = 1003, High = 1.003 },
                new Ohlcv { Ticks = 1004, High = 1.004 },
                new Ohlcv { Ticks = 1005, High = 1.005 },
                new Ohlcv { Ticks = 1006, High = 1.006 },
                new Ohlcv { Ticks = 1007, High = 1.007 },
                new Ohlcv { Ticks = 1008, High = 1.008 },
                new Ohlcv { Ticks = 1009, High = 1.009 }
            };
            array = list.ToArray();
            status = OhlcvData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 },
                new Ohlcv { Ticks = 1002, High = 1.002 },
                new Ohlcv { Ticks = 1003, High = 1.003 },
                new Ohlcv { Ticks = 1004, High = 1.004 },
                new Ohlcv { Ticks = 1005, High = 1.005 },
                new Ohlcv { Ticks = 1006, High = 1.006 },
                new Ohlcv { Ticks = 1007, High = 1.007 },
                new Ohlcv { Ticks = 1008, High = 1.008 },
                new Ohlcv { Ticks = 1009, High = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 3");
            list = new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 }
            };
            array = list.ToArray();
            status = OhlcvData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 4");
            list = new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1000, High = 1.001 }
            };
            array = list.ToArray();
            status = OhlcvData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 5");
            list = new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1000, High = 1.001 },
                new Ohlcv { Ticks = 1000, High = 1.002 }
            };
            array = list.ToArray();
            status = OhlcvData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 },
                new Ohlcv { Ticks = 1002, High = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 6");
            list = new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1000, High = 1.001 },
                new Ohlcv { Ticks = 1002, High = 1.002 }
            };
            array = list.ToArray();
            status = OhlcvData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 },
                new Ohlcv { Ticks = 1002, High = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 7");
            list = new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1007, High = 1.007 },
                new Ohlcv { Ticks = 1008, High = 1.008 },
                new Ohlcv { Ticks = 1008, High = 1.009 }
            };
            array = list.ToArray();
            status = OhlcvData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1007, High = 1.007 },
                new Ohlcv { Ticks = 1008, High = 1.008 },
                new Ohlcv { Ticks = 1009, High = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 8");
            list = new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 },
                new Ohlcv { Ticks = 1001, High = 1.002 },
                new Ohlcv { Ticks = 1003, High = 1.003 }
            };
            array = list.ToArray();
            status = OhlcvData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 },
                new Ohlcv { Ticks = 1002, High = 1.002 },
                new Ohlcv { Ticks = 1003, High = 1.003 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 9");
            list = new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 },
                new Ohlcv { Ticks = 1001, High = 1.002 },
                new Ohlcv { Ticks = 1001, High = 1.003 },
                new Ohlcv { Ticks = 1004, High = 1.004 }
            };
            array = list.ToArray();
            status = OhlcvData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Ohlcv>
            {
                new Ohlcv { Ticks = 1000, High = 1.0 },
                new Ohlcv { Ticks = 1001, High = 1.001 },
                new Ohlcv { Ticks = 1002, High = 1.002 },
                new Ohlcv { Ticks = 1003, High = 1.003 },
                new Ohlcv { Ticks = 1004, High = 1.004 }
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
