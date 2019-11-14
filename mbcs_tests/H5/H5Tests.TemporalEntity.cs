using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Mbcs.H5;

namespace Mbcs.Tests.H5
{
    public partial class H5Tests
    {
        [TestMethod]
        public void H5_TemporalEntity_DateTimeStampWithFractionOfSecond_Returns_CorrectString()
        {
            const string expected = "1234/05/06 07:08:09.7890000";
            var dateTime = new DateTime(1234, 5, 6, 7, 8, 9, 789);

            var quote = new Quote {Ticks = dateTime.Ticks};
            var quotePriceOnly = new QuotePriceOnly { Ticks = dateTime.Ticks };
            var trade = new Trade { Ticks = dateTime.Ticks };
            var tradePriceOnly = new TradePriceOnly { Ticks = dateTime.Ticks };
            var ohlcv = new Ohlcv { Ticks = dateTime.Ticks };
            var ohlcvPriceOnly = new OhlcvPriceOnly { Ticks = dateTime.Ticks };
            var scalar = new Scalar { Ticks = dateTime.Ticks };

            string actualDateTime = dateTime.DateTimeStampWithFractionOfSecond();
            string actualTicks = dateTime.Ticks.DateTimeStampWithFractionOfSecond();
            string actualQuote = quote.DateTimeStampWithFractionOfSecond();
            string actualQuotePriceOnly = quotePriceOnly.DateTimeStampWithFractionOfSecond();
            string actualTrade = trade.DateTimeStampWithFractionOfSecond();
            string actualTradePriceOnly = tradePriceOnly.DateTimeStampWithFractionOfSecond();
            string actualOhlcv = ohlcv.DateTimeStampWithFractionOfSecond();
            string actualOhlcvPriceOnly = ohlcvPriceOnly.DateTimeStampWithFractionOfSecond();
            string actualScalar = scalar.DateTimeStampWithFractionOfSecond();

            Assert.AreEqual(expected, actualDateTime);
            Assert.AreEqual(expected, actualTicks);
            Assert.AreEqual(expected, actualQuote);
            Assert.AreEqual(expected, actualQuotePriceOnly);
            Assert.AreEqual(expected, actualTrade);
            Assert.AreEqual(expected, actualTradePriceOnly);
            Assert.AreEqual(expected, actualOhlcv);
            Assert.AreEqual(expected, actualOhlcvPriceOnly);
            Assert.AreEqual(expected, actualScalar);
        }

        [TestMethod]
        public void H5_TemporalEntity_DateTimeStamp_Returns_CorrectString()
        {
            const string expected = "1234/05/06 07:08:09";
            var dateTime = new DateTime(1234, 5, 6, 7, 8, 9, 789);

            var quote = new Quote { Ticks = dateTime.Ticks };
            var quotePriceOnly = new QuotePriceOnly { Ticks = dateTime.Ticks };
            var trade = new Trade { Ticks = dateTime.Ticks };
            var tradePriceOnly = new TradePriceOnly { Ticks = dateTime.Ticks };
            var ohlcv = new Ohlcv { Ticks = dateTime.Ticks };
            var ohlcvPriceOnly = new OhlcvPriceOnly { Ticks = dateTime.Ticks };
            var scalar = new Scalar { Ticks = dateTime.Ticks };

            string actualDateTime = dateTime.DateTimeStamp();
            string actualTicks = dateTime.Ticks.DateTimeStamp();
            string actualQuote = quote.DateTimeStamp();
            string actualQuotePriceOnly = quotePriceOnly.DateTimeStamp();
            string actualTrade = trade.DateTimeStamp();
            string actualTradePriceOnly = tradePriceOnly.DateTimeStamp();
            string actualOhlcv = ohlcv.DateTimeStamp();
            string actualOhlcvPriceOnly = ohlcvPriceOnly.DateTimeStamp();
            string actualScalar = scalar.DateTimeStamp();

            Assert.AreEqual(expected, actualDateTime);
            Assert.AreEqual(expected, actualTicks);
            Assert.AreEqual(expected, actualQuote);
            Assert.AreEqual(expected, actualQuotePriceOnly);
            Assert.AreEqual(expected, actualTrade);
            Assert.AreEqual(expected, actualTradePriceOnly);
            Assert.AreEqual(expected, actualOhlcv);
            Assert.AreEqual(expected, actualOhlcvPriceOnly);
            Assert.AreEqual(expected, actualScalar);
        }

        [TestMethod]
        public void H5_TemporalEntity_DateStamp_Returns_CorrectString()
        {
            const string expected = "1234/05/06";
            var dateTime = new DateTime(1234, 5, 6, 7, 8, 9, 789);

            var quote = new Quote { Ticks = dateTime.Ticks };
            var quotePriceOnly = new QuotePriceOnly { Ticks = dateTime.Ticks };
            var trade = new Trade { Ticks = dateTime.Ticks };
            var tradePriceOnly = new TradePriceOnly { Ticks = dateTime.Ticks };
            var ohlcv = new Ohlcv { Ticks = dateTime.Ticks };
            var ohlcvPriceOnly = new OhlcvPriceOnly { Ticks = dateTime.Ticks };
            var scalar = new Scalar { Ticks = dateTime.Ticks };

            string actualDateTime = dateTime.DateStamp();
            string actualTicks = dateTime.Ticks.DateStamp();
            string actualQuote = quote.DateStamp();
            string actualQuotePriceOnly = quotePriceOnly.DateStamp();
            string actualTrade = trade.DateStamp();
            string actualTradePriceOnly = tradePriceOnly.DateStamp();
            string actualOhlcv = ohlcv.DateStamp();
            string actualOhlcvPriceOnly = ohlcvPriceOnly.DateStamp();
            string actualScalar = scalar.DateStamp();

            Assert.AreEqual(expected, actualDateTime);
            Assert.AreEqual(expected, actualTicks);
            Assert.AreEqual(expected, actualQuote);
            Assert.AreEqual(expected, actualQuotePriceOnly);
            Assert.AreEqual(expected, actualTrade);
            Assert.AreEqual(expected, actualTradePriceOnly);
            Assert.AreEqual(expected, actualOhlcv);
            Assert.AreEqual(expected, actualOhlcvPriceOnly);
            Assert.AreEqual(expected, actualScalar);
        }

        [TestMethod]
        public void H5_TemporalEntity_TimeStamp_Returns_CorrectString()
        {
            const string expected = "07:08:09";
            var dateTime = new DateTime(1234, 5, 6, 7, 8, 9, 789);

            var quote = new Quote { Ticks = dateTime.Ticks };
            var quotePriceOnly = new QuotePriceOnly { Ticks = dateTime.Ticks };
            var trade = new Trade { Ticks = dateTime.Ticks };
            var tradePriceOnly = new TradePriceOnly { Ticks = dateTime.Ticks };
            var ohlcv = new Ohlcv { Ticks = dateTime.Ticks };
            var ohlcvPriceOnly = new OhlcvPriceOnly { Ticks = dateTime.Ticks };
            var scalar = new Scalar { Ticks = dateTime.Ticks };

            string actualDateTime = dateTime.TimeStamp();
            string actualTicks = dateTime.Ticks.TimeStamp();
            string actualQuote = quote.TimeStamp();
            string actualQuotePriceOnly = quotePriceOnly.TimeStamp();
            string actualTrade = trade.TimeStamp();
            string actualTradePriceOnly = tradePriceOnly.TimeStamp();
            string actualOhlcv = ohlcv.TimeStamp();
            string actualOhlcvPriceOnly = ohlcvPriceOnly.TimeStamp();
            string actualScalar = scalar.TimeStamp();

            Assert.AreEqual(expected, actualDateTime);
            Assert.AreEqual(expected, actualTicks);
            Assert.AreEqual(expected, actualQuote);
            Assert.AreEqual(expected, actualQuotePriceOnly);
            Assert.AreEqual(expected, actualTrade);
            Assert.AreEqual(expected, actualTradePriceOnly);
            Assert.AreEqual(expected, actualOhlcv);
            Assert.AreEqual(expected, actualOhlcvPriceOnly);
            Assert.AreEqual(expected, actualScalar);
        }

        [TestMethod]
        public void H5_TemporalEntity_TimeStampWithFractionOfSecond_Returns_CorrectString()
        {
            const string expected = "07:08:09.7890000";
            var dateTime = new DateTime(1234, 5, 6, 7, 8, 9, 789);

            var quote = new Quote { Ticks = dateTime.Ticks };
            var quotePriceOnly = new QuotePriceOnly { Ticks = dateTime.Ticks };
            var trade = new Trade { Ticks = dateTime.Ticks };
            var tradePriceOnly = new TradePriceOnly { Ticks = dateTime.Ticks };
            var ohlcv = new Ohlcv { Ticks = dateTime.Ticks };
            var ohlcvPriceOnly = new OhlcvPriceOnly { Ticks = dateTime.Ticks };
            var scalar = new Scalar { Ticks = dateTime.Ticks };

            string actualDateTime = dateTime.TimeStampWithFractionOfSecond();
            string actualTicks = dateTime.Ticks.TimeStampWithFractionOfSecond();
            string actualQuote = quote.TimeStampWithFractionOfSecond();
            string actualQuotePriceOnly = quotePriceOnly.TimeStampWithFractionOfSecond();
            string actualTrade = trade.TimeStampWithFractionOfSecond();
            string actualTradePriceOnly = tradePriceOnly.TimeStampWithFractionOfSecond();
            string actualOhlcv = ohlcv.TimeStampWithFractionOfSecond();
            string actualOhlcvPriceOnly = ohlcvPriceOnly.TimeStampWithFractionOfSecond();
            string actualScalar = scalar.TimeStampWithFractionOfSecond();

            Assert.AreEqual(expected, actualDateTime);
            Assert.AreEqual(expected, actualTicks);
            Assert.AreEqual(expected, actualQuote);
            Assert.AreEqual(expected, actualQuotePriceOnly);
            Assert.AreEqual(expected, actualTrade);
            Assert.AreEqual(expected, actualTradePriceOnly);
            Assert.AreEqual(expected, actualOhlcv);
            Assert.AreEqual(expected, actualOhlcvPriceOnly);
            Assert.AreEqual(expected, actualScalar);
        }
    }
}
