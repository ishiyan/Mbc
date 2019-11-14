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
        public void H5_ScalarData_Exploratory_01()
        {
            int errorCount = ScalarDataExploratoryTest01();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_ScalarData_Exploratory_02()
        {
            int errorCount = ScalarDataExploratoryTest02();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_ScalarData_Exploratory_03()
        {
            int errorCount = ScalarDataExploratoryTest03();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_ScalarData_Exploratory_04()
        {
            int errorCount = ScalarDataExploratoryTest04();
            Assert.AreEqual(0, errorCount);
        }

        [TestMethod]
        public void H5_ScalarData_Exploratory_05()
        {
            int errorCount = ScalarDataExploratoryTest05();
            Assert.AreEqual(0, errorCount);
        }

        private static bool Add(List<Scalar> list, ScalarData data, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
        {
            Scalar[] array = list.ToArray();
            ScalarData.SpreadDuplicateTimeTicks(array, verbose);
            return data.Add(array, duplicateTimeTicks, verbose);
        }

        private static bool Verify(List<Scalar> pattern, ScalarData data)
        {
            var sample = new List<Scalar>();
            if (!data.FetchAll(sample))
                return false;
            int sampleCount = sample.Count, patternCount = pattern.Count;
            if (sampleCount == patternCount)
            {
                for (int i = 0; i < sampleCount; i++)
                {
                    if (sample[i].Ticks != pattern[i].Ticks || Math.Abs(sample[i].Value - pattern[i].Value) > double.Epsilon)
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

        private static bool Verify(List<Scalar> pattern, List<Scalar> sample)
        {
            int sampleCount = sample.Count, patternCount = pattern.Count;
            if (sampleCount == patternCount)
            {
                for (int i = 0; i < sampleCount; i++)
                {
                    if (sample[i].Ticks != pattern[i].Ticks || Math.Abs(sample[i].Value - pattern[i].Value) > double.Epsilon)
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

        private static int ScalarDataExploratoryTest01()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using ScalarData data = instrument.OpenScalar(DataTimeFrame.Aperiodic, true);

                #region (1) Initial append
                Trace.TraceInformation("--- 01 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                bool verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (2) Update / Update
                Trace.TraceInformation("--- 01 (2) Update/Update ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.01 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.01 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 130, Value = 1.31 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.31 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.51 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.51 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data, DuplicateTimeTicks.Update, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (3) Update / Skip
                Trace.TraceInformation("--- 01 (3) Update/Skip ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.01 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 130, Value = 1.31 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.51 }
                }, data, DuplicateTimeTicks.Skip, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (4) Update / Fail
                Trace.TraceInformation("--- 01 (4) Update/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.01 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 130, Value = 1.31 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar> {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.51 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifyFailure(status, verified, errorCount);
                #endregion

                #region (5) Prepend
                Trace.TraceInformation("--- 01 (5) Prepend/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 97, Value = 0.97 },
                    new Scalar { Ticks = 98, Value = 0.98 },
                    new Scalar { Ticks = 99, Value = 0.99 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 97, Value = 0.97 },
                    new Scalar { Ticks = 98, Value = 0.98 },
                    new Scalar { Ticks = 99, Value = 0.99 },
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 94, Value = 0.94 },
                    new Scalar { Ticks = 94, Value = 0.95 },
                    new Scalar { Ticks = 94, Value = 0.96 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 94, Value = 0.94 },
                    new Scalar { Ticks = 95, Value = 0.95 },
                    new Scalar { Ticks = 96, Value = 0.96 },
                    new Scalar { Ticks = 97, Value = 0.97 },
                    new Scalar { Ticks = 98, Value = 0.98 },
                    new Scalar { Ticks = 99, Value = 0.99 },
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 90, Value = 0.90 },
                    new Scalar { Ticks = 90, Value = 0.91 },
                    new Scalar { Ticks = 90, Value = 0.92 },
                    new Scalar { Ticks = 93, Value = 0.93 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 90, Value = 0.90 },
                    new Scalar { Ticks = 91, Value = 0.91 },
                    new Scalar { Ticks = 92, Value = 0.92 },
                    new Scalar { Ticks = 93, Value = 0.93 },
                    new Scalar { Ticks = 94, Value = 0.94 },
                    new Scalar { Ticks = 95, Value = 0.95 },
                    new Scalar { Ticks = 96, Value = 0.96 },
                    new Scalar { Ticks = 97, Value = 0.97 },
                    new Scalar { Ticks = 98, Value = 0.98 },
                    new Scalar { Ticks = 99, Value = 0.99 },
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 87, Value = 0.87 },
                    new Scalar { Ticks = 88, Value = 0.88 },
                    new Scalar { Ticks = 88, Value = 0.89 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 87, Value = 0.87 },
                    new Scalar { Ticks = 88, Value = 0.88 },
                    new Scalar { Ticks = 89, Value = 0.89 },
                    new Scalar { Ticks = 90, Value = 0.90 },
                    new Scalar { Ticks = 91, Value = 0.91 },
                    new Scalar { Ticks = 92, Value = 0.92 },
                    new Scalar { Ticks = 93, Value = 0.93 },
                    new Scalar { Ticks = 94, Value = 0.94 },
                    new Scalar { Ticks = 95, Value = 0.95 },
                    new Scalar { Ticks = 96, Value = 0.96 },
                    new Scalar { Ticks = 97, Value = 0.97 },
                    new Scalar { Ticks = 98, Value = 0.98 },
                    new Scalar { Ticks = 99, Value = 0.99 },
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (6) Append
                Trace.TraceInformation("--- 01 (6) Append/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 151, Value = 1.51 },
                    new Scalar { Ticks = 151, Value = 1.52 },
                    new Scalar { Ticks = 151, Value = 1.53 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 87, Value = 0.87 },
                    new Scalar { Ticks = 88, Value = 0.88 },
                    new Scalar { Ticks = 89, Value = 0.89 },
                    new Scalar { Ticks = 90, Value = 0.90 },
                    new Scalar { Ticks = 91, Value = 0.91 },
                    new Scalar { Ticks = 92, Value = 0.92 },
                    new Scalar { Ticks = 93, Value = 0.93 },
                    new Scalar { Ticks = 94, Value = 0.94 },
                    new Scalar { Ticks = 95, Value = 0.95 },
                    new Scalar { Ticks = 96, Value = 0.96 },
                    new Scalar { Ticks = 97, Value = 0.97 },
                    new Scalar { Ticks = 98, Value = 0.98 },
                    new Scalar { Ticks = 99, Value = 0.99 },
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 150, Value = 1.5 },
                    new Scalar { Ticks = 151, Value = 1.51 },
                    new Scalar { Ticks = 152, Value = 1.52 },
                    new Scalar { Ticks = 153, Value = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                #region (7) Insert
                Trace.TraceInformation("--- 01 (7) Insert/Fail ----------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 131, Value = 1.31 },
                    new Scalar { Ticks = 132, Value = 1.32 },
                    new Scalar { Ticks = 133, Value = 1.33 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 87, Value = 0.87 },
                    new Scalar { Ticks = 88, Value = 0.88 },
                    new Scalar { Ticks = 89, Value = 0.89 },
                    new Scalar { Ticks = 90, Value = 0.90 },
                    new Scalar { Ticks = 91, Value = 0.91 },
                    new Scalar { Ticks = 92, Value = 0.92 },
                    new Scalar { Ticks = 93, Value = 0.93 },
                    new Scalar { Ticks = 94, Value = 0.94 },
                    new Scalar { Ticks = 95, Value = 0.95 },
                    new Scalar { Ticks = 96, Value = 0.96 },
                    new Scalar { Ticks = 97, Value = 0.97 },
                    new Scalar { Ticks = 98, Value = 0.98 },
                    new Scalar { Ticks = 99, Value = 0.99 },
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 131, Value = 1.31 },
                    new Scalar { Ticks = 132, Value = 1.32 },
                    new Scalar { Ticks = 133, Value = 1.33 },
                    new Scalar { Ticks = 150, Value = 1.5 },
                    new Scalar { Ticks = 151, Value = 1.51 },
                    new Scalar { Ticks = 152, Value = 1.52 },
                    new Scalar { Ticks = 153, Value = 1.53 },
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 127, Value = 1.27 },
                    new Scalar { Ticks = 128, Value = 1.28 },
                    new Scalar { Ticks = 129, Value = 1.29 },
                    new Scalar { Ticks = 134, Value = 1.34 },
                    new Scalar { Ticks = 135, Value = 1.35 },
                    new Scalar { Ticks = 136, Value = 1.36 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 87, Value = 0.87 },
                    new Scalar { Ticks = 88, Value = 0.88 },
                    new Scalar { Ticks = 89, Value = 0.89 },
                    new Scalar { Ticks = 90, Value = 0.90 },
                    new Scalar { Ticks = 91, Value = 0.91 },
                    new Scalar { Ticks = 92, Value = 0.92 },
                    new Scalar { Ticks = 93, Value = 0.93 },
                    new Scalar { Ticks = 94, Value = 0.94 },
                    new Scalar { Ticks = 95, Value = 0.95 },
                    new Scalar { Ticks = 96, Value = 0.96 },
                    new Scalar { Ticks = 97, Value = 0.97 },
                    new Scalar { Ticks = 98, Value = 0.98 },
                    new Scalar { Ticks = 99, Value = 0.99 },
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 127, Value = 1.27 },
                    new Scalar { Ticks = 128, Value = 1.28 },
                    new Scalar { Ticks = 129, Value = 1.29 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 131, Value = 1.31 },
                    new Scalar { Ticks = 132, Value = 1.32 },
                    new Scalar { Ticks = 133, Value = 1.33 },
                    new Scalar { Ticks = 134, Value = 1.34 },
                    new Scalar { Ticks = 135, Value = 1.35 },
                    new Scalar { Ticks = 136, Value = 1.36 },
                    new Scalar { Ticks = 150, Value = 1.5 },
                    new Scalar { Ticks = 151, Value = 1.51 },
                    new Scalar { Ticks = 152, Value = 1.52 },
                    new Scalar { Ticks = 153, Value = 1.53 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 146, Value = 1.46 },
                    new Scalar { Ticks = 147, Value = 1.47 },
                    new Scalar { Ticks = 148, Value = 1.48 },
                    new Scalar { Ticks = 149, Value = 1.49 },
                    new Scalar { Ticks = 150, Value = 1.50 },
                    new Scalar { Ticks = 153, Value = 1.53 },
                    new Scalar { Ticks = 154, Value = 1.54 },
                    new Scalar { Ticks = 155, Value = 1.55 },
                    new Scalar { Ticks = 156, Value = 1.56 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 87, Value = 0.87 },
                    new Scalar { Ticks = 88, Value = 0.88 },
                    new Scalar { Ticks = 89, Value = 0.89 },
                    new Scalar { Ticks = 90, Value = 0.90 },
                    new Scalar { Ticks = 91, Value = 0.91 },
                    new Scalar { Ticks = 92, Value = 0.92 },
                    new Scalar { Ticks = 93, Value = 0.93 },
                    new Scalar { Ticks = 94, Value = 0.94 },
                    new Scalar { Ticks = 95, Value = 0.95 },
                    new Scalar { Ticks = 96, Value = 0.96 },
                    new Scalar { Ticks = 97, Value = 0.97 },
                    new Scalar { Ticks = 98, Value = 0.98 },
                    new Scalar { Ticks = 99, Value = 0.99 },
                    new Scalar { Ticks = 100, Value = 1.0 },
                    new Scalar { Ticks = 110, Value = 1.1 },
                    new Scalar { Ticks = 127, Value = 1.27 },
                    new Scalar { Ticks = 128, Value = 1.28 },
                    new Scalar { Ticks = 129, Value = 1.29 },
                    new Scalar { Ticks = 130, Value = 1.3 },
                    new Scalar { Ticks = 131, Value = 1.31 },
                    new Scalar { Ticks = 132, Value = 1.32 },
                    new Scalar { Ticks = 133, Value = 1.33 },
                    new Scalar { Ticks = 134, Value = 1.34 },
                    new Scalar { Ticks = 135, Value = 1.35 },
                    new Scalar { Ticks = 136, Value = 1.36 },
                    new Scalar { Ticks = 146, Value = 1.46 },
                    new Scalar { Ticks = 147, Value = 1.47 },
                    new Scalar { Ticks = 148, Value = 1.48 },
                    new Scalar { Ticks = 149, Value = 1.49 },
                    new Scalar { Ticks = 150, Value = 1.5 },
                    new Scalar { Ticks = 151, Value = 1.51 },
                    new Scalar { Ticks = 152, Value = 1.52 },
                    new Scalar { Ticks = 153, Value = 1.53 },
                    new Scalar { Ticks = 154, Value = 1.54 },
                    new Scalar { Ticks = 155, Value = 1.55 },
                    new Scalar { Ticks = 156, Value = 1.56 }
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

        private static int ScalarDataExploratoryTest02()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using ScalarData data = instrument.OpenScalar(DataTimeFrame.Aperiodic, true);

                #region (1) Initial append

                Trace.TraceInformation("--- 02 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);

                #endregion

                #region (2)

                Trace.TraceInformation("--- 02 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 700, Value = 0.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1700, Value = 1.7 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1700, Value = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1650, Value = 1.65 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1700, Value = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1700, Value = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 780, Value = 0.78 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1700, Value = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 750, Value = 0.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1700, Value = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1700, Value = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 710, Value = 0.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 710, Value = 0.71 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1700, Value = 1.7 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1690, Value = 1.69 },
                    new Scalar { Ticks = 1710, Value = 1.71 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 710, Value = 0.71 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1690, Value = 1.69 },
                    new Scalar { Ticks = 1700, Value = 1.7 },
                    new Scalar { Ticks = 1710, Value = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 670, Value = 0.67 },
                    new Scalar { Ticks = 680, Value = 0.68 },
                    new Scalar { Ticks = 720, Value = 0.72 },
                    new Scalar { Ticks = 730, Value = 0.73 },
                    new Scalar { Ticks = 775, Value = 0.775 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 670, Value = 0.67 },
                    new Scalar { Ticks = 680, Value = 0.68 },
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 710, Value = 0.71 },
                    new Scalar { Ticks = 720, Value = 0.72 },
                    new Scalar { Ticks = 730, Value = 0.73 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 775, Value = 0.775 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1690, Value = 1.69 },
                    new Scalar { Ticks = 1700, Value = 1.7 },
                    new Scalar { Ticks = 1710, Value = 1.71 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1685, Value = 1.685 },
                    new Scalar { Ticks = 1695, Value = 1.695 },
                    new Scalar { Ticks = 1697, Value = 1.697 },
                    new Scalar { Ticks = 1698, Value = 1.698 },
                    new Scalar { Ticks = 1715, Value = 1.75 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 670, Value = 0.67 },
                    new Scalar { Ticks = 680, Value = 0.68 },
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 710, Value = 0.71 },
                    new Scalar { Ticks = 720, Value = 0.72 },
                    new Scalar { Ticks = 730, Value = 0.73 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 775, Value = 0.775 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1685, Value = 1.685 },
                    new Scalar { Ticks = 1690, Value = 1.69 },
                    new Scalar { Ticks = 1695, Value = 1.695 },
                    new Scalar { Ticks = 1697, Value = 1.697 },
                    new Scalar { Ticks = 1698, Value = 1.698 },
                    new Scalar { Ticks = 1700, Value = 1.7 },
                    new Scalar { Ticks = 1710, Value = 1.71 },
                    new Scalar { Ticks = 1715, Value = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 669, Value = 0.669 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 669, Value = 0.669 },
                    new Scalar { Ticks = 670, Value = 0.67 },
                    new Scalar { Ticks = 680, Value = 0.68 },
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 710, Value = 0.71 },
                    new Scalar { Ticks = 720, Value = 0.72 },
                    new Scalar { Ticks = 730, Value = 0.73 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 775, Value = 0.775 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1685, Value = 1.685 },
                    new Scalar { Ticks = 1690, Value = 1.69 },
                    new Scalar { Ticks = 1695, Value = 1.695 },
                    new Scalar { Ticks = 1697, Value = 1.697 },
                    new Scalar { Ticks = 1698, Value = 1.698 },
                    new Scalar { Ticks = 1700, Value = 1.7 },
                    new Scalar { Ticks = 1710, Value = 1.71 },
                    new Scalar { Ticks = 1715, Value = 1.75 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1716, Value = 1.76 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 669, Value = 0.669 },
                    new Scalar { Ticks = 670, Value = 0.67 },
                    new Scalar { Ticks = 680, Value = 0.68 },
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 710, Value = 0.71 },
                    new Scalar { Ticks = 720, Value = 0.72 },
                    new Scalar { Ticks = 730, Value = 0.73 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 775, Value = 0.775 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1685, Value = 1.685 },
                    new Scalar { Ticks = 1690, Value = 1.69 },
                    new Scalar { Ticks = 1695, Value = 1.695 },
                    new Scalar { Ticks = 1697, Value = 1.697 },
                    new Scalar { Ticks = 1698, Value = 1.698 },
                    new Scalar { Ticks = 1700, Value = 1.7 },
                    new Scalar { Ticks = 1710, Value = 1.71 },
                    new Scalar { Ticks = 1715, Value = 1.75 },
                    new Scalar { Ticks = 1716, Value = 1.76 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 665, Value = 0.665 },
                    new Scalar { Ticks = 1720, Value = 1.72 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 665, Value = 0.665 },
                    new Scalar { Ticks = 669, Value = 0.669 },
                    new Scalar { Ticks = 670, Value = 0.67 },
                    new Scalar { Ticks = 680, Value = 0.68 },
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 710, Value = 0.71 },
                    new Scalar { Ticks = 720, Value = 0.72 },
                    new Scalar { Ticks = 730, Value = 0.73 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 775, Value = 0.775 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1685, Value = 1.685 },
                    new Scalar { Ticks = 1690, Value = 1.69 },
                    new Scalar { Ticks = 1695, Value = 1.695 },
                    new Scalar { Ticks = 1697, Value = 1.697 },
                    new Scalar { Ticks = 1698, Value = 1.698 },
                    new Scalar { Ticks = 1700, Value = 1.7 },
                    new Scalar { Ticks = 1710, Value = 1.71 },
                    new Scalar { Ticks = 1715, Value = 1.75 },
                    new Scalar { Ticks = 1716, Value = 1.76 },
                    new Scalar { Ticks = 1720, Value = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 668, Value = 0.668 },
                    new Scalar { Ticks = 1717, Value = 1.717 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 665, Value = 0.665 },
                    new Scalar { Ticks = 668, Value = 0.668 },
                    new Scalar { Ticks = 669, Value = 0.669 },
                    new Scalar { Ticks = 670, Value = 0.67 },
                    new Scalar { Ticks = 680, Value = 0.68 },
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 710, Value = 0.71 },
                    new Scalar { Ticks = 720, Value = 0.72 },
                    new Scalar { Ticks = 730, Value = 0.73 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 775, Value = 0.775 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1685, Value = 1.685 },
                    new Scalar { Ticks = 1690, Value = 1.69 },
                    new Scalar { Ticks = 1695, Value = 1.695 },
                    new Scalar { Ticks = 1697, Value = 1.697 },
                    new Scalar { Ticks = 1698, Value = 1.698 },
                    new Scalar { Ticks = 1700, Value = 1.7 },
                    new Scalar { Ticks = 1710, Value = 1.71 },
                    new Scalar { Ticks = 1715, Value = 1.75 },
                    new Scalar { Ticks = 1716, Value = 1.76 },
                    new Scalar { Ticks = 1717, Value = 1.717 },
                    new Scalar { Ticks = 1720, Value = 1.72 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 666, Value = 0.666 },
                    new Scalar { Ticks = 667, Value = 0.667 },
                    new Scalar { Ticks = 1718, Value = 1.718 },
                    new Scalar { Ticks = 1719, Value = 1.719 }
                }, data, DuplicateTimeTicks.Fail, true);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 665, Value = 0.665 },
                    new Scalar { Ticks = 666, Value = 0.666 },
                    new Scalar { Ticks = 667, Value = 0.667 },
                    new Scalar { Ticks = 668, Value = 0.668 },
                    new Scalar { Ticks = 669, Value = 0.669 },
                    new Scalar { Ticks = 670, Value = 0.67 },
                    new Scalar { Ticks = 680, Value = 0.68 },
                    new Scalar { Ticks = 690, Value = 0.69 },
                    new Scalar { Ticks = 700, Value = 0.7 },
                    new Scalar { Ticks = 710, Value = 0.71 },
                    new Scalar { Ticks = 720, Value = 0.72 },
                    new Scalar { Ticks = 730, Value = 0.73 },
                    new Scalar { Ticks = 750, Value = 0.75 },
                    new Scalar { Ticks = 751, Value = 0.751 },
                    new Scalar { Ticks = 752, Value = 0.752 },
                    new Scalar { Ticks = 753, Value = 0.753 },
                    new Scalar { Ticks = 754, Value = 0.754 },
                    new Scalar { Ticks = 770, Value = 0.77 },
                    new Scalar { Ticks = 775, Value = 0.775 },
                    new Scalar { Ticks = 780, Value = 0.78 },
                    new Scalar { Ticks = 800, Value = 0.8 },
                    new Scalar { Ticks = 900, Value = 0.9 },
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1300, Value = 1.3 },
                    new Scalar { Ticks = 1400, Value = 1.4 },
                    new Scalar { Ticks = 1500, Value = 1.5 },
                    new Scalar { Ticks = 1600, Value = 1.6 },
                    new Scalar { Ticks = 1650, Value = 1.65 },
                    new Scalar { Ticks = 1670, Value = 1.67 },
                    new Scalar { Ticks = 1680, Value = 1.68 },
                    new Scalar { Ticks = 1685, Value = 1.685 },
                    new Scalar { Ticks = 1690, Value = 1.69 },
                    new Scalar { Ticks = 1695, Value = 1.695 },
                    new Scalar { Ticks = 1697, Value = 1.697 },
                    new Scalar { Ticks = 1698, Value = 1.698 },
                    new Scalar { Ticks = 1700, Value = 1.7 },
                    new Scalar { Ticks = 1710, Value = 1.71 },
                    new Scalar { Ticks = 1715, Value = 1.75 },
                    new Scalar { Ticks = 1716, Value = 1.76 },
                    new Scalar { Ticks = 1717, Value = 1.717 },
                    new Scalar { Ticks = 1718, Value = 1.718 },
                    new Scalar { Ticks = 1719, Value = 1.719 },
                    new Scalar { Ticks = 1720, Value = 1.72 }
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

        private static int ScalarDataExploratoryTest03()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using ScalarData data = instrument.OpenScalar(DataTimeFrame.Aperiodic, true);

                #region (1) Initial append
                Trace.TraceInformation("--- 03 (1) Initial append ----------------");
                Trace.TraceInformation("STEP 1");
                bool status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, data);
                errorCount = VerifySuccess(status, verified, errorCount);
                #endregion

                var list = new List<Scalar>();

                #region (2)
                Trace.TraceInformation("--- 03 (2) -------------------------------");
                Trace.TraceInformation("STEP 1");
                list.Clear();
                status = data.FetchAll(list);
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 2");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(0));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 3");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(0), new DateTime(9000));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 4");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(0));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 5");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(9000));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 6");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 7");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 8");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(999));
                verified = Verify(new List<Scalar>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 9");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1301), new DateTime(9000));
                verified = Verify(new List<Scalar>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 10");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1000));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 11");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(9000));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 12");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1), new DateTime(1001));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 13");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1299), new DateTime(9000));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 14");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1290), new DateTime(9000));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 15");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1009));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 16");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1110));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 17");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1270), new DateTime(1300));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 18");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1300));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 19");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1110));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 20");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1105));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1100, Value = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 21");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1103), new DateTime(1110));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1110, Value = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 22");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1101), new DateTime(1109));
                verified = Verify(new List<Scalar>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 23");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1104), new DateTime(1106));
                verified = Verify(new List<Scalar>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 24");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1291), new DateTime(1299));
                verified = Verify(new List<Scalar>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 25");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1294), new DateTime(1296));
                verified = Verify(new List<Scalar>(), list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 26");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1300), new DateTime(1300));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 27");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1100), new DateTime(1100));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1100, Value = 1.1 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 28");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1205));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 29");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1105), new DateTime(1200));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 30");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1110), new DateTime(1205));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 31");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1000), new DateTime(1115));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 }
                }, list);
                errorCount = VerifySuccess(status, verified, errorCount);
                Trace.TraceInformation("STEP 32");
                list.Clear();
                status = data.FetchTickRange(list, new DateTime(1002), new DateTime(1115));
                verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 }
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

        private static int ScalarDataExploratoryTest04()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                using Instrument instrument = file.OpenInstrument("/a/b/c/bar", true);
                using ScalarData data = instrument.OpenScalar(DataTimeFrame.Aperiodic, true);

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
                bool status = Add(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
                }, data, DuplicateTimeTicks.Fail, true);
                bool verified = Verify(new List<Scalar>
                {
                    new Scalar { Ticks = 1000, Value = 1.0 },
                    new Scalar { Ticks = 1001, Value = 1.001 },
                    new Scalar { Ticks = 1002, Value = 1.002 },
                    new Scalar { Ticks = 1003, Value = 1.003 },
                    new Scalar { Ticks = 1004, Value = 1.004 },
                    new Scalar { Ticks = 1005, Value = 1.005 },
                    new Scalar { Ticks = 1006, Value = 1.006 },
                    new Scalar { Ticks = 1007, Value = 1.007 },
                    new Scalar { Ticks = 1008, Value = 1.008 },
                    new Scalar { Ticks = 1009, Value = 1.009 },
                    new Scalar { Ticks = 1100, Value = 1.1 },
                    new Scalar { Ticks = 1110, Value = 1.11 },
                    new Scalar { Ticks = 1120, Value = 1.12 },
                    new Scalar { Ticks = 1130, Value = 1.13 },
                    new Scalar { Ticks = 1140, Value = 1.14 },
                    new Scalar { Ticks = 1150, Value = 1.15 },
                    new Scalar { Ticks = 1160, Value = 1.16 },
                    new Scalar { Ticks = 1170, Value = 1.17 },
                    new Scalar { Ticks = 1180, Value = 1.18 },
                    new Scalar { Ticks = 1190, Value = 1.19 },
                    new Scalar { Ticks = 1200, Value = 1.2 },
                    new Scalar { Ticks = 1210, Value = 1.21 },
                    new Scalar { Ticks = 1220, Value = 1.22 },
                    new Scalar { Ticks = 1230, Value = 1.23 },
                    new Scalar { Ticks = 1240, Value = 1.24 },
                    new Scalar { Ticks = 1250, Value = 1.25 },
                    new Scalar { Ticks = 1260, Value = 1.26 },
                    new Scalar { Ticks = 1270, Value = 1.27 },
                    new Scalar { Ticks = 1280, Value = 1.28 },
                    new Scalar { Ticks = 1290, Value = 1.29 },
                    new Scalar { Ticks = 1300, Value = 1.3 }
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

        private static int ScalarDataExploratoryTest05()
        {
            int errorCount = 0;

            #region (1) Spread
            Trace.TraceInformation("--- 05 (1) Spread ----------------");
            Trace.TraceInformation("STEP 1");
            var list = new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1000, Value = 1.001 },
                new Scalar { Ticks = 1000, Value = 1.002 },
                new Scalar { Ticks = 1000, Value = 1.003 },
                new Scalar { Ticks = 1000, Value = 1.004 },
                new Scalar { Ticks = 1000, Value = 1.005 },
                new Scalar { Ticks = 1000, Value = 1.006 },
                new Scalar { Ticks = 1000, Value = 1.007 },
                new Scalar { Ticks = 1000, Value = 1.008 },
                new Scalar { Ticks = 1000, Value = 1.009 }
            };
            Scalar[] array = list.ToArray();
            bool status = ScalarData.SpreadDuplicateTimeTicks(array, true);
            bool verified = Verify(new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 },
                new Scalar { Ticks = 1002, Value = 1.002 },
                new Scalar { Ticks = 1003, Value = 1.003 },
                new Scalar { Ticks = 1004, Value = 1.004 },
                new Scalar { Ticks = 1005, Value = 1.005 },
                new Scalar { Ticks = 1006, Value = 1.006 },
                new Scalar { Ticks = 1007, Value = 1.007 },
                new Scalar { Ticks = 1008, Value = 1.008 },
                new Scalar { Ticks = 1009, Value = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 2");
            list = new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 },
                new Scalar { Ticks = 1002, Value = 1.002 },
                new Scalar { Ticks = 1003, Value = 1.003 },
                new Scalar { Ticks = 1004, Value = 1.004 },
                new Scalar { Ticks = 1005, Value = 1.005 },
                new Scalar { Ticks = 1006, Value = 1.006 },
                new Scalar { Ticks = 1007, Value = 1.007 },
                new Scalar { Ticks = 1008, Value = 1.008 },
                new Scalar { Ticks = 1009, Value = 1.009 }
            };
            array = list.ToArray();
            status = ScalarData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 },
                new Scalar { Ticks = 1002, Value = 1.002 },
                new Scalar { Ticks = 1003, Value = 1.003 },
                new Scalar { Ticks = 1004, Value = 1.004 },
                new Scalar { Ticks = 1005, Value = 1.005 },
                new Scalar { Ticks = 1006, Value = 1.006 },
                new Scalar { Ticks = 1007, Value = 1.007 },
                new Scalar { Ticks = 1008, Value = 1.008 },
                new Scalar { Ticks = 1009, Value = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 3");
            list = new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 }
            };
            array = list.ToArray();
            status = ScalarData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 }
            }, array.ToList());
            errorCount = VerifySuccess(!status, verified, errorCount);
            Trace.TraceInformation("STEP 4");
            list = new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1000, Value = 1.001 }
            };
            array = list.ToArray();
            status = ScalarData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 5");
            list = new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1000, Value = 1.001 },
                new Scalar { Ticks = 1000, Value = 1.002 }
            };
            array = list.ToArray();
            status = ScalarData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 },
                new Scalar { Ticks = 1002, Value = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 6");
            list = new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1000, Value = 1.001 },
                new Scalar { Ticks = 1002, Value = 1.002 }
            };
            array = list.ToArray();
            status = ScalarData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 },
                new Scalar { Ticks = 1002, Value = 1.002 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 7");
            list = new List<Scalar>
            {
                new Scalar { Ticks = 1007, Value = 1.007 },
                new Scalar { Ticks = 1008, Value = 1.008 },
                new Scalar { Ticks = 1008, Value = 1.009 }
            };
            array = list.ToArray();
            status = ScalarData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Scalar>
            {
                new Scalar { Ticks = 1007, Value = 1.007 },
                new Scalar { Ticks = 1008, Value = 1.008 },
                new Scalar { Ticks = 1009, Value = 1.009 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 8");
            list = new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 },
                new Scalar { Ticks = 1001, Value = 1.002 },
                new Scalar { Ticks = 1003, Value = 1.003 }
            };
            array = list.ToArray();
            status = ScalarData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 },
                new Scalar { Ticks = 1002, Value = 1.002 },
                new Scalar { Ticks = 1003, Value = 1.003 }
            }, array.ToList());
            errorCount = VerifySuccess(status, verified, errorCount);
            Trace.TraceInformation("STEP 9");
            list = new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 },
                new Scalar { Ticks = 1001, Value = 1.002 },
                new Scalar { Ticks = 1001, Value = 1.003 },
                new Scalar { Ticks = 1004, Value = 1.004 }
            };
            array = list.ToArray();
            status = ScalarData.SpreadDuplicateTimeTicks(array, true);
            verified = Verify(new List<Scalar>
            {
                new Scalar { Ticks = 1000, Value = 1.0 },
                new Scalar { Ticks = 1001, Value = 1.001 },
                new Scalar { Ticks = 1002, Value = 1.002 },
                new Scalar { Ticks = 1003, Value = 1.003 },
                new Scalar { Ticks = 1004, Value = 1.004 }
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
