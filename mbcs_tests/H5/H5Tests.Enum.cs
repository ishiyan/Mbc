using System;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Mbcs.H5;

namespace Mbcs.Tests.H5
{
    public partial class H5Tests
    {
        [TestMethod]
        public void H5_Enum_DuplicateTimeTicks_Values_AreVerified()
        {
            foreach (var v in Enum.GetValues(typeof(DuplicateTimeTicks)).Cast<DuplicateTimeTicks>().ToList())
            {
                switch (v)
                {
                    case DuplicateTimeTicks.Fail:
                    case DuplicateTimeTicks.Skip:
                    case DuplicateTimeTicks.Update:
                        break;

                    default:
                        Assert.Fail($"Enum value DuplicateTimeTicks.{v} is unknown.");
                        return;
                }
            }

            Assert.IsTrue(true);
        }

        [TestMethod]
        public void H5_Enum_FileDriver_Values_AreVerified()
        {
            foreach (var v in Enum.GetValues(typeof(FileDriver)).Cast<FileDriver>().ToList())
            {
                switch (v)
                {
                    case FileDriver.Sec2:
                    case FileDriver.StdIo:
                    case FileDriver.Core:
                    case FileDriver.Log:
                        break;

                    default:
                        Assert.Fail($"Enum value FileDriver.{v} is unknown.");
                        return;
                }
            }

            Assert.IsTrue(true);
        }

        [TestMethod]
        public void H5_Enum_DataType_Values_AreVerified()
        {
            foreach (var v in Enum.GetValues(typeof(DataType)).Cast<DataType>().ToList())
            {
                switch (v)
                {
                    case DataType.Trade:
                    case DataType.TradePriceOnly:
                    case DataType.Quote:
                    case DataType.QuotePriceOnly:
                    case DataType.Ohlcv:
                    case DataType.OhlcvPriceOnly:
                    case DataType.OhlcvAdjusted:
                    case DataType.OhlcvAdjustedPriceOnly:
                    case DataType.Scalar:
                        break;

                    default:
                        Assert.Fail($"Enum value DataType.{v} is unknown.");
                        return;
                }
            }

            Assert.IsTrue(true);
        }

        [TestMethod]
        public void H5_Enum_OhlcvKind_Values_AreVerified()
        {
            foreach (var v in Enum.GetValues(typeof(OhlcvKind)).Cast<OhlcvKind>().ToList())
            {
                switch (v)
                {
                    case OhlcvKind.Default:
                    case OhlcvKind.Ask:
                    case OhlcvKind.Bid:
                        break;

                    default:
                        Assert.Fail($"Enum value OhlcvKind.{v} is unknown.");
                        return;
                }
            }

            Assert.IsTrue(true);
        }

        [TestMethod]
        public void H5_Enum_ScalarKind_Values_AreVerified()
        {
            foreach (var v in Enum.GetValues(typeof(ScalarKind)).Cast<ScalarKind>().ToList())
            {
                switch (v)
                {
                    case ScalarKind.Default:
                        break;

                    default:
                        Assert.Fail($"Enum value ScalarKind.{v} is unknown.");
                        return;
                }
            }

            Assert.IsTrue(true);
        }

        [TestMethod]
        public void H5_Enum_TradeKind_Values_AreVerified()
        {
            foreach (var v in Enum.GetValues(typeof(TradeKind)).Cast<TradeKind>().ToList())
            {
                switch (v)
                {
                    case TradeKind.Default:
                        break;

                    default:
                        Assert.Fail($"Enum value TradeKind.{v} is unknown.");
                        return;
                }
            }

            Assert.IsTrue(true);
        }

        [TestMethod]
        public void H5_Enum_QuoteKind_Values_AreVerified()
        {
            foreach (var v in Enum.GetValues(typeof(QuoteKind)).Cast<QuoteKind>().ToList())
            {
                switch (v)
                {
                    case QuoteKind.Default:
                        break;

                    default:
                        Assert.Fail($"Enum value QuoteKind.{v} is unknown.");
                        return;
                }
            }

            Assert.IsTrue(true);
        }

        [TestMethod]
        public void H5_Enum_DataTimeFrame_Values_AreVerified()
        {
            foreach (var v in Enum.GetValues(typeof(DataTimeFrame)).Cast<DataTimeFrame>().ToList())
            {
                switch (v)
                {
                    case DataTimeFrame.Aperiodic:
                    case DataTimeFrame.Second1:
                    case DataTimeFrame.Second2:
                    case DataTimeFrame.Second3:
                    case DataTimeFrame.Second4:
                    case DataTimeFrame.Second5:
                    case DataTimeFrame.Second6:
                    case DataTimeFrame.Second10:
                    case DataTimeFrame.Second12:
                    case DataTimeFrame.Second15:
                    case DataTimeFrame.Second20:
                    case DataTimeFrame.Second30:
                    case DataTimeFrame.Minute1:
                    case DataTimeFrame.Minute2:
                    case DataTimeFrame.Minute3:
                    case DataTimeFrame.Minute4:
                    case DataTimeFrame.Minute5:
                    case DataTimeFrame.Minute6:
                    case DataTimeFrame.Minute10:
                    case DataTimeFrame.Minute12:
                    case DataTimeFrame.Minute15:
                    case DataTimeFrame.Minute20:
                    case DataTimeFrame.Minute30:
                    case DataTimeFrame.Hour1:
                    case DataTimeFrame.Hour2:
                    case DataTimeFrame.Hour3:
                    case DataTimeFrame.Hour4:
                    case DataTimeFrame.Hour6:
                    case DataTimeFrame.Hour8:
                    case DataTimeFrame.Hour12:
                    case DataTimeFrame.Day1:
                    case DataTimeFrame.Week1:
                    case DataTimeFrame.Week2:
                    case DataTimeFrame.Week3:
                    case DataTimeFrame.Month1:
                    case DataTimeFrame.Month2:
                    case DataTimeFrame.Month3:
                    case DataTimeFrame.Month4:
                    case DataTimeFrame.Month6:
                    case DataTimeFrame.Year1:
                        break;

                    default:
                        Assert.Fail($"Enum value DataTimeFrame.{v} is unknown.");
                        return;
                }
            }

            Assert.IsTrue(true);
        }

        [TestMethod]
        public void H5_Enum_DataKind_Values_AreVerified()
        {
            foreach (var v in Enum.GetValues(typeof(DataKind)).Cast<DataKind>().ToList())
            {
                switch (v)
                {
                    case DataKind.TradeAperiodic:
                    case DataKind.TradePriceOnlyAperiodic:
                    case DataKind.QuoteAperiodic:
                    case DataKind.QuotePriceOnlyAperiodic:

                    case DataKind.OhlcvAperiodic:
                    case DataKind.OhlcvSecond1:
                    case DataKind.OhlcvSecond2:
                    case DataKind.OhlcvSecond3:
                    case DataKind.OhlcvSecond4:
                    case DataKind.OhlcvSecond5:
                    case DataKind.OhlcvSecond6:
                    case DataKind.OhlcvSecond10:
                    case DataKind.OhlcvSecond12:
                    case DataKind.OhlcvSecond15:
                    case DataKind.OhlcvSecond20:
                    case DataKind.OhlcvSecond30:
                    case DataKind.OhlcvMinute1:
                    case DataKind.OhlcvMinute2:
                    case DataKind.OhlcvMinute3:
                    case DataKind.OhlcvMinute4:
                    case DataKind.OhlcvMinute5:
                    case DataKind.OhlcvMinute6:
                    case DataKind.OhlcvMinute10:
                    case DataKind.OhlcvMinute12:
                    case DataKind.OhlcvMinute15:
                    case DataKind.OhlcvMinute20:
                    case DataKind.OhlcvMinute30:
                    case DataKind.OhlcvHour1:
                    case DataKind.OhlcvHour2:
                    case DataKind.OhlcvHour3:
                    case DataKind.OhlcvHour4:
                    case DataKind.OhlcvHour6:
                    case DataKind.OhlcvHour8:
                    case DataKind.OhlcvHour12:
                    case DataKind.OhlcvDay1:
                    case DataKind.OhlcvWeek1:
                    case DataKind.OhlcvWeek2:
                    case DataKind.OhlcvWeek3:
                    case DataKind.OhlcvMonth1:
                    case DataKind.OhlcvMonth2:
                    case DataKind.OhlcvMonth3:
                    case DataKind.OhlcvMonth4:
                    case DataKind.OhlcvMonth6:
                    case DataKind.OhlcvYear1:

                    case DataKind.OhlcvAskAperiodic:
                    case DataKind.OhlcvAskSecond1:
                    case DataKind.OhlcvAskSecond2:
                    case DataKind.OhlcvAskSecond3:
                    case DataKind.OhlcvAskSecond4:
                    case DataKind.OhlcvAskSecond5:
                    case DataKind.OhlcvAskSecond6:
                    case DataKind.OhlcvAskSecond10:
                    case DataKind.OhlcvAskSecond12:
                    case DataKind.OhlcvAskSecond15:
                    case DataKind.OhlcvAskSecond20:
                    case DataKind.OhlcvAskSecond30:
                    case DataKind.OhlcvAskMinute1:
                    case DataKind.OhlcvAskMinute2:
                    case DataKind.OhlcvAskMinute3:
                    case DataKind.OhlcvAskMinute4:
                    case DataKind.OhlcvAskMinute5:
                    case DataKind.OhlcvAskMinute6:
                    case DataKind.OhlcvAskMinute10:
                    case DataKind.OhlcvAskMinute12:
                    case DataKind.OhlcvAskMinute15:
                    case DataKind.OhlcvAskMinute20:
                    case DataKind.OhlcvAskMinute30:
                    case DataKind.OhlcvAskHour1:
                    case DataKind.OhlcvAskHour2:
                    case DataKind.OhlcvAskHour3:
                    case DataKind.OhlcvAskHour4:
                    case DataKind.OhlcvAskHour6:
                    case DataKind.OhlcvAskHour8:
                    case DataKind.OhlcvAskHour12:
                    case DataKind.OhlcvAskDay1:
                    case DataKind.OhlcvAskWeek1:
                    case DataKind.OhlcvAskWeek2:
                    case DataKind.OhlcvAskWeek3:
                    case DataKind.OhlcvAskMonth1:
                    case DataKind.OhlcvAskMonth2:
                    case DataKind.OhlcvAskMonth3:
                    case DataKind.OhlcvAskMonth4:
                    case DataKind.OhlcvAskMonth6:
                    case DataKind.OhlcvAskYear1:

                    case DataKind.OhlcvBidAperiodic:
                    case DataKind.OhlcvBidSecond1:
                    case DataKind.OhlcvBidSecond2:
                    case DataKind.OhlcvBidSecond3:
                    case DataKind.OhlcvBidSecond4:
                    case DataKind.OhlcvBidSecond5:
                    case DataKind.OhlcvBidSecond6:
                    case DataKind.OhlcvBidSecond10:
                    case DataKind.OhlcvBidSecond12:
                    case DataKind.OhlcvBidSecond15:
                    case DataKind.OhlcvBidSecond20:
                    case DataKind.OhlcvBidSecond30:
                    case DataKind.OhlcvBidMinute1:
                    case DataKind.OhlcvBidMinute2:
                    case DataKind.OhlcvBidMinute3:
                    case DataKind.OhlcvBidMinute4:
                    case DataKind.OhlcvBidMinute5:
                    case DataKind.OhlcvBidMinute6:
                    case DataKind.OhlcvBidMinute10:
                    case DataKind.OhlcvBidMinute12:
                    case DataKind.OhlcvBidMinute15:
                    case DataKind.OhlcvBidMinute20:
                    case DataKind.OhlcvBidMinute30:
                    case DataKind.OhlcvBidHour1:
                    case DataKind.OhlcvBidHour2:
                    case DataKind.OhlcvBidHour3:
                    case DataKind.OhlcvBidHour4:
                    case DataKind.OhlcvBidHour6:
                    case DataKind.OhlcvBidHour8:
                    case DataKind.OhlcvBidHour12:
                    case DataKind.OhlcvBidDay1:
                    case DataKind.OhlcvBidWeek1:
                    case DataKind.OhlcvBidWeek2:
                    case DataKind.OhlcvBidWeek3:
                    case DataKind.OhlcvBidMonth1:
                    case DataKind.OhlcvBidMonth2:
                    case DataKind.OhlcvBidMonth3:
                    case DataKind.OhlcvBidMonth4:
                    case DataKind.OhlcvBidMonth6:
                    case DataKind.OhlcvBidYear1:

                    case DataKind.OhlcvPriceOnlyAperiodic:
                    case DataKind.OhlcvPriceOnlySecond1:
                    case DataKind.OhlcvPriceOnlySecond2:
                    case DataKind.OhlcvPriceOnlySecond3:
                    case DataKind.OhlcvPriceOnlySecond4:
                    case DataKind.OhlcvPriceOnlySecond5:
                    case DataKind.OhlcvPriceOnlySecond6:
                    case DataKind.OhlcvPriceOnlySecond10:
                    case DataKind.OhlcvPriceOnlySecond12:
                    case DataKind.OhlcvPriceOnlySecond15:
                    case DataKind.OhlcvPriceOnlySecond20:
                    case DataKind.OhlcvPriceOnlySecond30:
                    case DataKind.OhlcvPriceOnlyMinute1:
                    case DataKind.OhlcvPriceOnlyMinute2:
                    case DataKind.OhlcvPriceOnlyMinute3:
                    case DataKind.OhlcvPriceOnlyMinute4:
                    case DataKind.OhlcvPriceOnlyMinute5:
                    case DataKind.OhlcvPriceOnlyMinute6:
                    case DataKind.OhlcvPriceOnlyMinute10:
                    case DataKind.OhlcvPriceOnlyMinute12:
                    case DataKind.OhlcvPriceOnlyMinute15:
                    case DataKind.OhlcvPriceOnlyMinute20:
                    case DataKind.OhlcvPriceOnlyMinute30:
                    case DataKind.OhlcvPriceOnlyHour1:
                    case DataKind.OhlcvPriceOnlyHour2:
                    case DataKind.OhlcvPriceOnlyHour3:
                    case DataKind.OhlcvPriceOnlyHour4:
                    case DataKind.OhlcvPriceOnlyHour6:
                    case DataKind.OhlcvPriceOnlyHour8:
                    case DataKind.OhlcvPriceOnlyHour12:
                    case DataKind.OhlcvPriceOnlyDay1:
                    case DataKind.OhlcvPriceOnlyWeek1:
                    case DataKind.OhlcvPriceOnlyWeek2:
                    case DataKind.OhlcvPriceOnlyWeek3:
                    case DataKind.OhlcvPriceOnlyMonth1:
                    case DataKind.OhlcvPriceOnlyMonth2:
                    case DataKind.OhlcvPriceOnlyMonth3:
                    case DataKind.OhlcvPriceOnlyMonth4:
                    case DataKind.OhlcvPriceOnlyMonth6:
                    case DataKind.OhlcvPriceOnlyYear1:

                    case DataKind.OhlcvPriceOnlyAskAperiodic:
                    case DataKind.OhlcvPriceOnlyAskSecond1:
                    case DataKind.OhlcvPriceOnlyAskSecond2:
                    case DataKind.OhlcvPriceOnlyAskSecond3:
                    case DataKind.OhlcvPriceOnlyAskSecond4:
                    case DataKind.OhlcvPriceOnlyAskSecond5:
                    case DataKind.OhlcvPriceOnlyAskSecond6:
                    case DataKind.OhlcvPriceOnlyAskSecond10:
                    case DataKind.OhlcvPriceOnlyAskSecond12:
                    case DataKind.OhlcvPriceOnlyAskSecond15:
                    case DataKind.OhlcvPriceOnlyAskSecond20:
                    case DataKind.OhlcvPriceOnlyAskSecond30:
                    case DataKind.OhlcvPriceOnlyAskMinute1:
                    case DataKind.OhlcvPriceOnlyAskMinute2:
                    case DataKind.OhlcvPriceOnlyAskMinute3:
                    case DataKind.OhlcvPriceOnlyAskMinute4:
                    case DataKind.OhlcvPriceOnlyAskMinute5:
                    case DataKind.OhlcvPriceOnlyAskMinute6:
                    case DataKind.OhlcvPriceOnlyAskMinute10:
                    case DataKind.OhlcvPriceOnlyAskMinute12:
                    case DataKind.OhlcvPriceOnlyAskMinute15:
                    case DataKind.OhlcvPriceOnlyAskMinute20:
                    case DataKind.OhlcvPriceOnlyAskMinute30:
                    case DataKind.OhlcvPriceOnlyAskHour1:
                    case DataKind.OhlcvPriceOnlyAskHour2:
                    case DataKind.OhlcvPriceOnlyAskHour3:
                    case DataKind.OhlcvPriceOnlyAskHour4:
                    case DataKind.OhlcvPriceOnlyAskHour6:
                    case DataKind.OhlcvPriceOnlyAskHour8:
                    case DataKind.OhlcvPriceOnlyAskHour12:
                    case DataKind.OhlcvPriceOnlyAskDay1:
                    case DataKind.OhlcvPriceOnlyAskWeek1:
                    case DataKind.OhlcvPriceOnlyAskWeek2:
                    case DataKind.OhlcvPriceOnlyAskWeek3:
                    case DataKind.OhlcvPriceOnlyAskMonth1:
                    case DataKind.OhlcvPriceOnlyAskMonth2:
                    case DataKind.OhlcvPriceOnlyAskMonth3:
                    case DataKind.OhlcvPriceOnlyAskMonth4:
                    case DataKind.OhlcvPriceOnlyAskMonth6:
                    case DataKind.OhlcvPriceOnlyAskYear1:

                    case DataKind.OhlcvPriceOnlyBidAperiodic:
                    case DataKind.OhlcvPriceOnlyBidSecond1:
                    case DataKind.OhlcvPriceOnlyBidSecond2:
                    case DataKind.OhlcvPriceOnlyBidSecond3:
                    case DataKind.OhlcvPriceOnlyBidSecond4:
                    case DataKind.OhlcvPriceOnlyBidSecond5:
                    case DataKind.OhlcvPriceOnlyBidSecond6:
                    case DataKind.OhlcvPriceOnlyBidSecond10:
                    case DataKind.OhlcvPriceOnlyBidSecond12:
                    case DataKind.OhlcvPriceOnlyBidSecond15:
                    case DataKind.OhlcvPriceOnlyBidSecond20:
                    case DataKind.OhlcvPriceOnlyBidSecond30:
                    case DataKind.OhlcvPriceOnlyBidMinute1:
                    case DataKind.OhlcvPriceOnlyBidMinute2:
                    case DataKind.OhlcvPriceOnlyBidMinute3:
                    case DataKind.OhlcvPriceOnlyBidMinute4:
                    case DataKind.OhlcvPriceOnlyBidMinute5:
                    case DataKind.OhlcvPriceOnlyBidMinute6:
                    case DataKind.OhlcvPriceOnlyBidMinute10:
                    case DataKind.OhlcvPriceOnlyBidMinute12:
                    case DataKind.OhlcvPriceOnlyBidMinute15:
                    case DataKind.OhlcvPriceOnlyBidMinute20:
                    case DataKind.OhlcvPriceOnlyBidMinute30:
                    case DataKind.OhlcvPriceOnlyBidHour1:
                    case DataKind.OhlcvPriceOnlyBidHour2:
                    case DataKind.OhlcvPriceOnlyBidHour3:
                    case DataKind.OhlcvPriceOnlyBidHour4:
                    case DataKind.OhlcvPriceOnlyBidHour6:
                    case DataKind.OhlcvPriceOnlyBidHour8:
                    case DataKind.OhlcvPriceOnlyBidHour12:
                    case DataKind.OhlcvPriceOnlyBidDay1:
                    case DataKind.OhlcvPriceOnlyBidWeek1:
                    case DataKind.OhlcvPriceOnlyBidWeek2:
                    case DataKind.OhlcvPriceOnlyBidWeek3:
                    case DataKind.OhlcvPriceOnlyBidMonth1:
                    case DataKind.OhlcvPriceOnlyBidMonth2:
                    case DataKind.OhlcvPriceOnlyBidMonth3:
                    case DataKind.OhlcvPriceOnlyBidMonth4:
                    case DataKind.OhlcvPriceOnlyBidMonth6:
                    case DataKind.OhlcvPriceOnlyBidYear1:

                    case DataKind.ScalarAperiodic:
                    case DataKind.ScalarSecond1:
                    case DataKind.ScalarSecond2:
                    case DataKind.ScalarSecond3:
                    case DataKind.ScalarSecond4:
                    case DataKind.ScalarSecond5:
                    case DataKind.ScalarSecond6:
                    case DataKind.ScalarSecond10:
                    case DataKind.ScalarSecond12:
                    case DataKind.ScalarSecond15:
                    case DataKind.ScalarSecond20:
                    case DataKind.ScalarSecond30:
                    case DataKind.ScalarMinute1:
                    case DataKind.ScalarMinute2:
                    case DataKind.ScalarMinute3:
                    case DataKind.ScalarMinute4:
                    case DataKind.ScalarMinute5:
                    case DataKind.ScalarMinute6:
                    case DataKind.ScalarMinute10:
                    case DataKind.ScalarMinute12:
                    case DataKind.ScalarMinute15:
                    case DataKind.ScalarMinute20:
                    case DataKind.ScalarMinute30:
                    case DataKind.ScalarHour1:
                    case DataKind.ScalarHour2:
                    case DataKind.ScalarHour3:
                    case DataKind.ScalarHour4:
                    case DataKind.ScalarHour6:
                    case DataKind.ScalarHour8:
                    case DataKind.ScalarHour12:
                    case DataKind.ScalarDay1:
                    case DataKind.ScalarWeek1:
                    case DataKind.ScalarWeek2:
                    case DataKind.ScalarWeek3:
                    case DataKind.ScalarMonth1:
                    case DataKind.ScalarMonth2:
                    case DataKind.ScalarMonth3:
                    case DataKind.ScalarMonth4:
                    case DataKind.ScalarMonth6:
                    case DataKind.ScalarYear1:

                    case DataKind.OhlcvAdjustedAperiodic:
                    case DataKind.OhlcvAdjustedSecond1:
                    case DataKind.OhlcvAdjustedSecond2:
                    case DataKind.OhlcvAdjustedSecond3:
                    case DataKind.OhlcvAdjustedSecond4:
                    case DataKind.OhlcvAdjustedSecond5:
                    case DataKind.OhlcvAdjustedSecond6:
                    case DataKind.OhlcvAdjustedSecond10:
                    case DataKind.OhlcvAdjustedSecond12:
                    case DataKind.OhlcvAdjustedSecond15:
                    case DataKind.OhlcvAdjustedSecond20:
                    case DataKind.OhlcvAdjustedSecond30:
                    case DataKind.OhlcvAdjustedMinute1:
                    case DataKind.OhlcvAdjustedMinute2:
                    case DataKind.OhlcvAdjustedMinute3:
                    case DataKind.OhlcvAdjustedMinute4:
                    case DataKind.OhlcvAdjustedMinute5:
                    case DataKind.OhlcvAdjustedMinute6:
                    case DataKind.OhlcvAdjustedMinute10:
                    case DataKind.OhlcvAdjustedMinute12:
                    case DataKind.OhlcvAdjustedMinute15:
                    case DataKind.OhlcvAdjustedMinute20:
                    case DataKind.OhlcvAdjustedMinute30:
                    case DataKind.OhlcvAdjustedHour1:
                    case DataKind.OhlcvAdjustedHour2:
                    case DataKind.OhlcvAdjustedHour3:
                    case DataKind.OhlcvAdjustedHour4:
                    case DataKind.OhlcvAdjustedHour6:
                    case DataKind.OhlcvAdjustedHour8:
                    case DataKind.OhlcvAdjustedHour12:
                    case DataKind.OhlcvAdjustedDay1:
                    case DataKind.OhlcvAdjustedWeek1:
                    case DataKind.OhlcvAdjustedWeek2:
                    case DataKind.OhlcvAdjustedWeek3:
                    case DataKind.OhlcvAdjustedMonth1:
                    case DataKind.OhlcvAdjustedMonth2:
                    case DataKind.OhlcvAdjustedMonth3:
                    case DataKind.OhlcvAdjustedMonth4:
                    case DataKind.OhlcvAdjustedMonth6:
                    case DataKind.OhlcvAdjustedYear1:

                    case DataKind.OhlcvAdjustedPriceOnlyAperiodic:
                    case DataKind.OhlcvAdjustedPriceOnlySecond1:
                    case DataKind.OhlcvAdjustedPriceOnlySecond2:
                    case DataKind.OhlcvAdjustedPriceOnlySecond3:
                    case DataKind.OhlcvAdjustedPriceOnlySecond4:
                    case DataKind.OhlcvAdjustedPriceOnlySecond5:
                    case DataKind.OhlcvAdjustedPriceOnlySecond6:
                    case DataKind.OhlcvAdjustedPriceOnlySecond10:
                    case DataKind.OhlcvAdjustedPriceOnlySecond12:
                    case DataKind.OhlcvAdjustedPriceOnlySecond15:
                    case DataKind.OhlcvAdjustedPriceOnlySecond20:
                    case DataKind.OhlcvAdjustedPriceOnlySecond30:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute1:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute2:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute3:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute4:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute5:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute6:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute10:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute12:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute15:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute20:
                    case DataKind.OhlcvAdjustedPriceOnlyMinute30:
                    case DataKind.OhlcvAdjustedPriceOnlyHour1:
                    case DataKind.OhlcvAdjustedPriceOnlyHour2:
                    case DataKind.OhlcvAdjustedPriceOnlyHour3:
                    case DataKind.OhlcvAdjustedPriceOnlyHour4:
                    case DataKind.OhlcvAdjustedPriceOnlyHour6:
                    case DataKind.OhlcvAdjustedPriceOnlyHour8:
                    case DataKind.OhlcvAdjustedPriceOnlyHour12:
                    case DataKind.OhlcvAdjustedPriceOnlyDay1:
                    case DataKind.OhlcvAdjustedPriceOnlyWeek1:
                    case DataKind.OhlcvAdjustedPriceOnlyWeek2:
                    case DataKind.OhlcvAdjustedPriceOnlyWeek3:
                    case DataKind.OhlcvAdjustedPriceOnlyMonth1:
                    case DataKind.OhlcvAdjustedPriceOnlyMonth2:
                    case DataKind.OhlcvAdjustedPriceOnlyMonth3:
                    case DataKind.OhlcvAdjustedPriceOnlyMonth4:
                    case DataKind.OhlcvAdjustedPriceOnlyMonth6:
                    case DataKind.OhlcvAdjustedPriceOnlyYear1:
                        break;

                    default:
                        Assert.Fail($"Enum value DataKind.{v} is unknown.");
                        return;
                }
            }

            Assert.IsTrue(true);
        }
    }
}
