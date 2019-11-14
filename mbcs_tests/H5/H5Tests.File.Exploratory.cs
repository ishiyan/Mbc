using System.Collections.Generic;
using System.Diagnostics;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Mbcs.H5;

namespace Mbcs.Tests.H5
{
    public partial class H5Tests
    {
        [TestMethod]
        public void H5_File_Exploratory_01()
        {
            int errorCount = FileExploratoryTest01();
            Assert.AreEqual(0, errorCount);
        }

        internal class TestDataInfo
        {
            internal string Name;
            internal string Path;

            internal bool Verify(DataInfo dataInfo)
            {
                bool result = true;
                if (!string.IsNullOrEmpty(dataInfo.Name))
                {
                    if (!Name.Equals(dataInfo.Name))
                    {
                        Trace.TraceError("Verification error: name: pattern [{0}], sample [{1}]", Name, dataInfo.Name);
                        result = false;
                    }
                }
                else
                {
                    if (!string.IsNullOrEmpty(Name))
                    {
                        Trace.TraceError("Verification error: name: pattern [{0}], sample [{1}]", Name, dataInfo.Name);
                        result = false;
                    }
                }
                if (!string.IsNullOrEmpty(dataInfo.Path))
                {
                    if (!Path.Equals(dataInfo.Path))
                    {
                        Trace.TraceError("Verification error: path: pattern [{0}], sample [{1}]", Path, dataInfo.Path);
                        result = false;
                    }
                }
                else
                {
                    if (!string.IsNullOrEmpty(Path))
                    {
                        Trace.TraceError("Verification error: path: pattern [{0}], sample [{1}]", Path, dataInfo.Path);
                        result = false;
                    }
                }
                return result;
            }
        }

        internal class TestGroupInfo
        {
            internal string Name;
            internal string Path;
            internal List<TestDataInfo> Datasets;
            internal List<TestGroupInfo> Groups;

            internal bool Verify(GroupInfo groupInfo)
            {
                bool result = true;
                if (!string.IsNullOrEmpty(groupInfo.Name))
                {
                    if (!Name.Equals(groupInfo.Name))
                    {
                        Trace.TraceError("Verification error: groupInfo name: pattern [{0}], sample [{1}]", Name, groupInfo.Name);
                        result = false;
                    }
                }
                else
                {
                    if (!string.IsNullOrEmpty(Name))
                    {
                        Trace.TraceError("Verification error: groupInfo name: pattern [{0}], sample [{1}]", Name, groupInfo.Name);
                        result = false;
                    }
                }
                if (!string.IsNullOrEmpty(groupInfo.Path))
                {
                    if (!Path.Equals(groupInfo.Path))
                    {
                        Trace.TraceError("Verification error: groupInfo path: pattern [{0}], sample [{1}]", Path, groupInfo.Path);
                        result = false;
                    }
                }
                else
                {
                    if (!string.IsNullOrEmpty(Path))
                    {
                        Trace.TraceError("Verification error: groupInfo path: pattern [{0}], sample [{1}]", Path, groupInfo.Path);
                        result = false;
                    }
                }
                if (Datasets.Count != groupInfo.Datasets.Count)
                {
                    Trace.TraceError("Verification error: groupInfo dataset count: pattern [{0}], sample [{1}]", Datasets.Count, groupInfo.Datasets.Count);
                    result = false;
                }
                else
                {
                    for (int i = 0; i < Datasets.Count; i++)
                    {
                        TestDataInfo tdi = Datasets[i];
                        DataInfo di = groupInfo.Datasets[i];
                        if (!tdi.Verify(di))
                            result = false;
                    }
                }
                if (Groups.Count != groupInfo.Groups.Count)
                {
                    Trace.TraceError("Verification error: groupInfo group count: pattern [{0}], sample [{1}]", Groups.Count, groupInfo.Groups.Count);
                    result = false;
                }
                else
                {
                    for (int i = 0; i < Groups.Count; i++)
                    {
                        TestGroupInfo tgi = Groups[i];
                        GroupInfo gi = groupInfo.Groups[i];
                        if (!tgi.Verify(gi))
                            result = false;
                    }
                }
                return result;
            }
        }

        private static bool Verify(TestGroupInfo pattern, GroupInfo sample)
        {
            return pattern.Verify(sample);
        }

        private static bool Verify(List<TestDataInfo> pattern, List<DataInfo> sample)
        {
            bool result = true;
            if (pattern.Count != sample.Count)
            {
                Trace.TraceError("Verification error: pattern count [{0}], sample count [{1}]", pattern.Count, sample.Count);
                result = false;
            }
            else
            {
                for (int i = 0; i < pattern.Count; i++)
                {
                    TestDataInfo tdi = pattern[i];
                    DataInfo di = sample[i];
                    if (!tdi.Verify(di))
                        result = false;
                }
            }
            return result;
        }

        private static int FileExploratoryTest01()
        {
            string fileName = NewTestFileName;

            int errorCount = 0;
            using (File file = File.OpenReadWrite(fileName))
            {
                Instrument instrumentBar = file.OpenInstrument("/a/b/c/bar", true);
                TradeData data = instrumentBar.OpenTrade(true);
                Instrument instrumentFoo = file.OpenInstrument("/a/d/foo", true);
                OhlcvData data2 = instrumentFoo.OpenOhlcv(OhlcvKind.Default, DataTimeFrame.Day1, true);
                OhlcvData data3 = instrumentFoo.OpenOhlcv(OhlcvKind.Default, DataTimeFrame.Minute1, true);

                var tgi = new TestGroupInfo
                {
                    Name = "/",
                    Path = "",
                    Datasets = new List<TestDataInfo>(),
                    Groups = new List<TestGroupInfo>
                    {
                        new TestGroupInfo
                        {
                            Name = "a", Path = "/a", Datasets = new List<TestDataInfo>(), Groups = new List<TestGroupInfo>
                            {
                                new TestGroupInfo
                                {
                                    Name = "b", Path = "/a/b", Datasets = new List<TestDataInfo>(), Groups = new List<TestGroupInfo>
                                    {
                                        new TestGroupInfo
                                        {
                                            Name = "c", Path = "/a/b/c", Datasets = new List<TestDataInfo>(), Groups = new List<TestGroupInfo>
                                            {
                                                new TestGroupInfo
                                                {
                                                    Name = "bar", Path = "/a/b/c/bar", Groups = new List<TestGroupInfo>(), Datasets = new List<TestDataInfo>
                                                    {
                                                        new TestDataInfo
                                                        {
                                                            Name = "t____", Path = "/a/b/c/bar/t____"
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                },
                                new TestGroupInfo
                                {
                                    Name = "d", Path = "/a/d", Datasets = new List<TestDataInfo>(), Groups = new List<TestGroupInfo>
                                    {
                                        new TestGroupInfo
                                        {
                                            Name = "foo", Path = "/a/d/foo", Groups = new List<TestGroupInfo>(), Datasets = new List<TestDataInfo>
                                            {
                                                new TestDataInfo
                                                {
                                                    Name = "c_d01", Path = "/a/d/foo/c_d01"
                                                },
                                                new TestDataInfo
                                                {
                                                    Name = "c_m01", Path = "/a/d/foo/c_m01"
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                };
                var listTdi = new List<TestDataInfo>
                {
                    new TestDataInfo
                    {
                        Name = "t____", Path = "/a/b/c/bar/t____"
                    },
                    new TestDataInfo
                    {
                        Name = "c_d01", Path = "/a/d/foo/c_d01"
                    },
                    new TestDataInfo
                    {
                        Name = "c_m01", Path = "/a/d/foo/c_m01"
                    }
                };
                var listTdiSorted = new List<TestDataInfo>
                {
                    new TestDataInfo
                    {
                        Name = "t____", Path = "/a/b/c/bar/t____"
                    },
                    new TestDataInfo
                    {
                        Name = "c_d01", Path = "/a/d/foo/c_d01"
                    },
                    new TestDataInfo
                    {
                        Name = "c_m01", Path = "/a/d/foo/c_m01"
                    }
                };
                var listTdiBar = new List<TestDataInfo>
                {
                    new TestDataInfo
                    {
                        Name = "t____", Path = "/t____"
                    },
                };
                var listTdiFoo = new List<TestDataInfo>
                {
                    new TestDataInfo
                    {
                        Name = "c_d01", Path = "/c_d01"
                    },
                    new TestDataInfo
                    {
                        Name = "c_m01", Path = "/c_m01"
                    }
                };
                var listTdiFooSorted = new List<TestDataInfo>
                {
                    new TestDataInfo
                    {
                        Name = "c_d01", Path = "/c_d01"
                    },
                    new TestDataInfo
                    {
                        Name = "c_m01", Path = "/c_m01"
                    }
                };

                Trace.TraceInformation("STEP 1");
                GroupInfo gi = file.ContentTree(false);
                bool verified = Verify(tgi, gi);
                errorCount = VerifySuccess(true, verified, errorCount);

                Trace.TraceInformation("STEP 2");
                var listDi = new List<DataInfo>();
                file.EnumerateDatasets(listDi, false);
                verified = Verify(listTdi, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);
                Trace.TraceInformation("STEP 2 SORTED");
                listDi.Clear();
                file.EnumerateDatasets(listDi, true);
                verified = Verify(listTdiSorted, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);

                Trace.TraceInformation("STEP 3");
                listDi.Clear();
                instrumentBar.EnumerateDatasets(listDi, false);
                verified = Verify(listTdiBar, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);
                Trace.TraceInformation("STEP 3 SORTED");
                listDi.Clear();
                instrumentBar.EnumerateDatasets(listDi, true);
                verified = Verify(listTdiBar, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);

                Trace.TraceInformation("STEP 4");
                listDi.Clear();
                instrumentFoo.EnumerateDatasets(listDi, false);
                verified = Verify(listTdiFoo, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);
                Trace.TraceInformation("STEP 4 SORTED");
                listDi.Clear();
                instrumentFoo.EnumerateDatasets(listDi, true);
                verified = Verify(listTdiFooSorted, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);

                Trace.TraceInformation("STEP 5");
                listDi.Clear();
                file.EnumerateDatasets(listDi, "/a/b/c/bar", false);
                verified = Verify(listTdiBar, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);
                Trace.TraceInformation("STEP 5 SORTED");
                listDi.Clear();
                file.EnumerateDatasets(listDi, "/a/b/c/bar", true);
                verified = Verify(listTdiBar, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);

                Trace.TraceInformation("STEP 6");
                listDi.Clear();
                file.EnumerateDatasets(listDi, "/a/d/foo", false);
                verified = Verify(listTdiFoo, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);
                Trace.TraceInformation("STEP 6 SORTED");
                listDi.Clear();
                file.EnumerateDatasets(listDi, "/a/d/foo", true);
                verified = Verify(listTdiFooSorted, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);

                data3.Close();
                data2.Close();
                instrumentFoo.Close();
                data.Close();
                instrumentBar.Close();
                file.Close();

                Trace.TraceInformation("STEP 7");
                gi = File.ContentTree(fileName, false);
                verified = Verify(tgi, gi);
                errorCount = VerifySuccess(true, verified, errorCount);

                Trace.TraceInformation("STEP 8");
                listDi.Clear();
                File.EnumerateDatasets(fileName, listDi, false);
                verified = Verify(listTdi, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);
                Trace.TraceInformation("STEP 8 SORTED");
                listDi.Clear();
                File.EnumerateDatasets(fileName, listDi, true);
                verified = Verify(listTdiSorted, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);

                Trace.TraceInformation("STEP 9");
                listDi.Clear();
                File.EnumerateDatasets(fileName, listDi, "/a/b/c/bar", false);
                verified = Verify(listTdiBar, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);
                Trace.TraceInformation("STEP 9 SORTED");
                listDi.Clear();
                File.EnumerateDatasets(fileName, listDi, "/a/b/c/bar", true);
                verified = Verify(listTdiBar, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);

                Trace.TraceInformation("STEP 10");
                listDi.Clear();
                File.EnumerateDatasets(fileName, listDi, "/a/d/foo", false);
                verified = Verify(listTdiFoo, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);
                Trace.TraceInformation("STEP 10 SORTED");
                listDi.Clear();
                File.EnumerateDatasets(fileName, listDi, "/a/d/foo", true);
                verified = Verify(listTdiFooSorted, listDi);
                errorCount = VerifySuccess(true, verified, errorCount);
            }

            if (0 == errorCount)
                Trace.TraceInformation("--- All tests passed ---");
            else
                Trace.TraceInformation("--- {0} tests failed ---", errorCount);
            return errorCount;
        }
    }
}
