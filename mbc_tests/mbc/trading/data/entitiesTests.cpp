#include "mbc/time/dotNetTicks.h"
#include "mbc/trading/data/entities.h"
#include "mbc/testharness.h"

namespace {

TESTGROUP("trading::temporalEntity")
{
    using DotNetTicks = mbc::time::DotNetTicks;
    using TemporalEntity = mbc::trading::TemporalEntity;
    using Scalar = mbc::trading::Scalar;
    using Trade = mbc::trading::Trade;
    using Quote = mbc::trading::Quote;
    using Ohlcv = mbc::trading::Ohlcv;

    TESTCASE("ticks() returns value passed to the constructor")
    {
        const long long expected{ 123456L };

        const Scalar scalar(expected, 0.);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        const Trade trade(expected, 0., 0.);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        const Quote quote(expected, 0., 0., 0., 0.);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        const Ohlcv ohlcv(expected, 0., 0., 0., 0., 0.);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after ticks(long long) call")
    {
        const long long expected{ 123456L };

        Scalar scalar(0L, 0.);
        scalar.ticks(expected);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(0L, 0., 0.);
        trade.ticks(expected);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(0L, 0., 0., 0., 0.);
        quote.ticks(expected);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(0L, 0., 0., 0., 0., 0.);
        ohlcv.ticks(expected);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addTicks(long long) call with a positive argument")
    {
        const long long initial{ 123000L }, added{ 456L }, expected{ 123456L };

        Scalar scalar(initial, 0.);
        scalar.addTicks(added);
        ASSERT_EQUAL(expected, scalar.ticks())  << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addTicks(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addTicks(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addTicks(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addTicks(long long) call with a negative argument")
    {
        const long long initial{ 123456L }, added{ -456L }, expected{ 123000L };

        Scalar scalar(initial, 0.);
        scalar.addTicks(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addTicks(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addTicks(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addTicks(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addMilliseconds(double) call with a positive argument")
    {
        const double added{ 123.456 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addMilliseconds(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addMilliseconds(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addMilliseconds(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addMilliseconds(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addMilliseconds(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addMilliseconds(double) call with a negative argument")
    {
        const double added{ -123.456 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addMilliseconds(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addMilliseconds(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addMilliseconds(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addMilliseconds(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addMilliseconds(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addSeconds(double) call with a positive argument")
    {
        const double added{ 1.234 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addSeconds(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addSeconds(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addSeconds(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addSeconds(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addSeconds(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addSeconds(double) call with a negative argument")
    {
        const double added{ -1.234 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addSeconds(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addSeconds(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addSeconds(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addSeconds(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addSeconds(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addMinutes(double) call with a positive argument")
    {
        const double added{ 0.123 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addMinutes(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addMinutes(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addMinutes(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addMinutes(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addMinutes(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addMinutes(double) call with a negative argument")
    {
        const double added{ -0.123 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addMinutes(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addMinutes(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addMinutes(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addMinutes(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addMinutes(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addHours(double) call with a positive argument")
    {
        const double added{ 0.012 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addHours(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addHours(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addHours(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addHours(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addHours(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addHours(double) call with a negative argument")
    {
        const double added{ -0.012 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addHours(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addHours(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addHours(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addHours(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addHours(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addDays(double) call with a positive argument")
    {
        const double added{ 0.001 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addDays(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addDays(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addDays(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addDays(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addDays(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("ticks() returns correct value after addDays(double) call with a negative argument")
    {
        const double added{ -0.001 };
        const long long initial{ 123456789L }, expected{ DotNetTicks::addDays(initial, added) };

        Scalar scalar(initial, 0.);
        scalar.addDays(added);
        ASSERT_EQUAL(expected, scalar.ticks()) << "scalar";

        Trade trade(initial, 0., 0.);
        trade.addDays(added);
        ASSERT_EQUAL(expected, trade.ticks()) << "trade";

        Quote quote(initial, 0., 0., 0., 0.);
        quote.addDays(added);
        ASSERT_EQUAL(expected, quote.ticks()) << "quote";

        Ohlcv ohlcv(initial, 0., 0., 0., 0., 0.);
        ohlcv.addDays(added);
        ASSERT_EQUAL(expected, ohlcv.ticks()) << "ohlcv";
    }

    TESTCASE("comparison and equality operators work correctly")
    {
        const long long lesser{ 123456L }, greater{ 1234567L };

        const Scalar lesserScalar(lesser, 0.), greaterScalar(greater, 0.);
        ASSERT_IS_TRUE(lesserScalar < greaterScalar) << "scalar <";
        ASSERT_IS_TRUE(lesserScalar <= lesserScalar) << "scalar <=";
        ASSERT_IS_TRUE(greaterScalar > lesserScalar) << "scalar >";
        ASSERT_IS_TRUE(greaterScalar >= greaterScalar) << "scalar >=";
        ASSERT_IS_TRUE(greaterScalar != lesserScalar) << "scalar !=";
        ASSERT_IS_TRUE(greaterScalar == greaterScalar) << "scalar ==";

        const Trade lesserTrade(lesser, 0., 0.), greaterTrade(greater, 0., 0.);
        ASSERT_IS_TRUE(lesserTrade < greaterTrade) << "trade <";
        ASSERT_IS_TRUE(lesserTrade <= lesserTrade) << "trade <=";
        ASSERT_IS_TRUE(greaterTrade > lesserTrade) << "trade >";
        ASSERT_IS_TRUE(greaterTrade >= greaterTrade) << "trade >=";
        ASSERT_IS_TRUE(greaterTrade != lesserTrade) << "trade !=";
        ASSERT_IS_TRUE(greaterTrade == greaterTrade) << "trade ==";

        const Quote lesserQuote(lesser, 0., 0., 0., 0.), greaterQuote(greater, 0., 0., 0., 0.);
        ASSERT_IS_TRUE(lesserQuote < greaterQuote) << "quote <";
        ASSERT_IS_TRUE(lesserQuote <= lesserQuote) << "quote <=";
        ASSERT_IS_TRUE(greaterQuote > lesserQuote) << "quote >";
        ASSERT_IS_TRUE(greaterQuote >= greaterQuote) << "quote >=";
        ASSERT_IS_TRUE(greaterQuote != lesserQuote) << "quote !=";
        ASSERT_IS_TRUE(greaterQuote == greaterQuote) << "quote ==";

        const Ohlcv lesserOhlcv(lesser, 0., 0., 0., 0., 0.), greaterOhlcv(greater, 0., 0., 0., 0., 0.);
        ASSERT_IS_TRUE(lesserOhlcv < greaterOhlcv) << "ohlcv <";
        ASSERT_IS_TRUE(lesserOhlcv <= lesserOhlcv) << "ohlcv <=";
        ASSERT_IS_TRUE(greaterOhlcv > lesserOhlcv) << "ohlcv >";
        ASSERT_IS_TRUE(greaterOhlcv >= greaterOhlcv) << "ohlcv >=";
        ASSERT_IS_TRUE(greaterOhlcv != lesserOhlcv) << "ohlcv !=";
        ASSERT_IS_TRUE(greaterOhlcv == greaterOhlcv) << "ohlcv ==";

        ASSERT_IS_TRUE(lesserQuote < greaterOhlcv) << "<";
        ASSERT_IS_TRUE(lesserQuote <= lesserOhlcv) << "<=";
        ASSERT_IS_TRUE(greaterQuote > lesserOhlcv) << ">";
        ASSERT_IS_TRUE(greaterQuote >= greaterOhlcv) << ">=";
        ASSERT_IS_TRUE(greaterQuote != lesserOhlcv) << "!=";
        ASSERT_IS_TRUE(greaterQuote == greaterOhlcv) << "==";
    }
}

TESTGROUP("trading::scalar")
{
    using TemporalEntity = mbc::trading::TemporalEntity;
    using Scalar = mbc::trading::Scalar;

    // How many ULPs (Units in the Last Place) we want to tolerate when comparing two numbers.
    const size_t ulp{ 0 };

    TESTCASE("returns values passed to the constructor")
    {
        const long long expectedTicks{ 12345 };
        const double expectedValue{ 123.456 };

        const Scalar scalar(expectedTicks, expectedValue);

        ASSERT_EQUAL(expectedTicks, scalar.ticks()) << "ticks()";
        ASSERT_EQUAL_FP(expectedValue, scalar.value(), ulp) << "value()";
    }

    TESTCASE("value() returns correct value after value(double) call")
    {
        const double expected{ 123.456 };
        Scalar scalar(0L, 0.);

        scalar.value(expected);

        ASSERT_EQUAL_FP(expected, scalar.value(), ulp);
    }

    TESTCASE("clone() returns an instance with correct values")
    {
        const long long expectedTicks{ 12345 };
        const double expectedValue{ 123.456 };
        const Scalar scalar(expectedTicks, expectedValue);

        const std::unique_ptr<Scalar> ptr{ static_cast<Scalar*>(scalar.clone()) };

        ASSERT_EQUAL(expectedTicks, ptr->ticks()) << "ticks()";
        ASSERT_EQUAL_FP(expectedValue, ptr->value(), ulp) << "value()";
    }
}

TESTGROUP("trading::trade")
{
    using TemporalEntity = mbc::trading::TemporalEntity;
    using Trade = mbc::trading::Trade;

    // How many ULPs (Units in the Last Place) we want to tolerate when comparing two numbers.
    const size_t ulp{ 0 };

    TESTCASE("returns values passed to the constructor")
    {
        const long long expectedTicks{ 12345 };
        const double expectedPrice{ 123.456 }, expectedVolume{ 234.567 };

        const Trade trade(expectedTicks, expectedPrice, expectedVolume);

        ASSERT_EQUAL(expectedTicks, trade.ticks()) << "ticks()";
        ASSERT_EQUAL_FP(expectedPrice, trade.price(), ulp) << "price()";
        ASSERT_EQUAL_FP(expectedVolume, trade.volume(), ulp) << "volume()";
    }

    TESTCASE("{price,volume}() returns correct value after {price,volume}(double) call")
    {
        const double expectedPrice{ 123.456 }, expectedVolume{ 234.567 };
        Trade trade(0L, 0., 0.);

        trade.price(expectedPrice);
        trade.volume(expectedVolume);

        ASSERT_EQUAL_FP(expectedPrice, trade.price(), ulp) << "price()";
        ASSERT_EQUAL_FP(expectedVolume, trade.volume(), ulp) << "volume()";
    }

    TESTCASE("clone() returns an instance with correct values")
    {
        const long long expectedTicks{ 12345 };
        const double expectedPrice{ 123.456 }, expectedVolume{ 123.456 };
        const Trade trade(expectedTicks, expectedPrice, expectedVolume);

        const std::unique_ptr<Trade> ptr{ static_cast<Trade*>(trade.clone()) };

        ASSERT_EQUAL(expectedTicks, ptr->ticks()) << "ticks()";
        ASSERT_EQUAL_FP(expectedPrice, ptr->price(), ulp) << "price()";
        ASSERT_EQUAL_FP(expectedVolume, ptr->volume(), ulp) << "volume()";
    }
}

TESTGROUP("trading::quote")
{
    using TemporalEntity = mbc::trading::TemporalEntity;
    using Quote = mbc::trading::Quote;

    // How many ULPs (Units in the Last Place) we want to tolerate when comparing two numbers.
   const size_t ulp{ 0 };

    TESTCASE("returns values passed to the constructor")
    {
        const long long expectedTicks{ 12345 };
        const double expectedBidPrice{ 123.456 }, expectedBidSize{ 234.567 };
        const double expectedAskPrice{ 345.678 }, expectedAskSize{ 456.789 };

        const Quote quote(expectedTicks, expectedBidPrice, expectedBidSize, expectedAskPrice, expectedAskSize);

        ASSERT_EQUAL(expectedTicks, quote.ticks()) << "ticks()";
        ASSERT_EQUAL_FP(expectedBidPrice, quote.bidPrice(), ulp) << "bidPrice()";
        ASSERT_EQUAL_FP(expectedBidSize, quote.bidSize(), ulp) << "bidSize()";
        ASSERT_EQUAL_FP(expectedAskPrice, quote.askPrice(), ulp) << "askPrice()";
        ASSERT_EQUAL_FP(expectedAskSize, quote.askSize(), ulp) << "askSize()";
    }

    TESTCASE("{bid,ask}{Price,Size}() returns correct value after {bid,ask}{Price,Size}(double) call")
    {
        const double expectedBidPrice{ 123.456 }, expectedBidSize{ 234.567 };
        const double expectedAskPrice{ 345.678 }, expectedAskSize{ 456.789 };
        Quote quote(0L, 0., 0., 0., 0.);

        quote.bidPrice(expectedBidPrice);
        quote.bidSize(expectedBidSize);
        quote.askPrice(expectedAskPrice);
        quote.askSize(expectedAskSize);

        ASSERT_EQUAL_FP(expectedBidPrice, quote.bidPrice(), ulp) << "bidPrice()";
        ASSERT_EQUAL_FP(expectedBidSize, quote.bidSize(), ulp) << "bidSize()";
        ASSERT_EQUAL_FP(expectedAskPrice, quote.askPrice(), ulp) << "askPrice()";
        ASSERT_EQUAL_FP(expectedAskSize, quote.askSize(), ulp) << "askSize()";
    }

    TESTCASE("clone() returns an instance with correct values")
    {
        const long long expectedTicks{ 12345 };
        const double expectedBidPrice{ 123.456 }, expectedBidSize{ 234.567 };
        const double expectedAskPrice{ 345.678 }, expectedAskSize{ 456.789 };
        const Quote quote(expectedTicks, expectedBidPrice, expectedBidSize, expectedAskPrice, expectedAskSize);

        const std::unique_ptr<Quote> ptr{ static_cast<Quote*>(quote.clone()) };

        ASSERT_EQUAL(expectedTicks, ptr->ticks()) << "ticks()";
        ASSERT_EQUAL_FP(expectedBidPrice, ptr->bidPrice(), ulp) << "bidPrice()";
        ASSERT_EQUAL_FP(expectedBidSize, ptr->bidSize(), ulp) << "bidSize()";
        ASSERT_EQUAL_FP(expectedAskPrice, ptr->askPrice(), ulp) << "askPrice()";
        ASSERT_EQUAL_FP(expectedAskSize, ptr->askSize(), ulp) << "askSize()";
    }
}

TESTGROUP("trading::ohlcv")
{
    using TemporalEntity = mbc::trading::TemporalEntity;
    using Ohlcv = mbc::trading::Ohlcv;
    using OhlcvPriceType = mbc::trading::OhlcvPriceType;
    using OhlcvComponent = mbc::trading::OhlcvComponent;

    // How many ULPs (Units in the Last Place) we want to tolerate when comparing two numbers.
    const size_t ulp{ 0 };

    TESTCASE("returns values passed to the constructor")
    {
        const long long expectedTicks{ 12345 };
        const double expectedOpen{ 123.456 }, expectedHigh{ 234.567 };
        const double expectedLow{ 345.678 }, expectedClose{ 456.789 }, expectedVolume{ 567.890 };
        const Ohlcv ohlcv(expectedTicks, expectedOpen, expectedHigh, expectedLow, expectedClose, expectedVolume);

        ASSERT_EQUAL(expectedTicks, ohlcv.ticks()) << "ticks()";
        ASSERT_EQUAL_FP(expectedOpen, ohlcv.open(), ulp) << "open()";
        ASSERT_EQUAL_FP(expectedHigh, ohlcv.high(), ulp) << "high()";
        ASSERT_EQUAL_FP(expectedLow, ohlcv.low(), ulp) << "low()";
        ASSERT_EQUAL_FP(expectedClose, ohlcv.close(), ulp) << "close()";
        ASSERT_EQUAL_FP(expectedVolume, ohlcv.volume(), ulp) << "volume()";
    }

    TESTCASE("returns values passed to the constructor without volume")
    {
        const long long expectedTicks{ 12345 };
        const double expectedOpen{ 123.456 }, expectedHigh{ 234.567 };
        const double expectedLow{ 345.678 }, expectedClose{ 456.789 };
        const Ohlcv ohlcv(expectedTicks, expectedOpen, expectedHigh, expectedLow, expectedClose);

        ASSERT_EQUAL(expectedTicks, ohlcv.ticks()) << "ticks()";
        ASSERT_EQUAL_FP(expectedOpen, ohlcv.open(), ulp) << "open()";
        ASSERT_EQUAL_FP(expectedHigh, ohlcv.high(), ulp) << "high()";
        ASSERT_EQUAL_FP(expectedLow, ohlcv.low(), ulp) << "low()";
        ASSERT_EQUAL_FP(expectedClose, ohlcv.close(), ulp) << "close()";
        ASSERT_EQUAL_FP(0., ohlcv.volume(), ulp) << "close()";
    }

    TESTCASE("{open,high,low,close,volume}() returns correct value after {open,high,low,close,volume}(double) call")
    {
        const double expectedOpen{ 123.456 }, expectedHigh{ 234.567 };
        const double expectedLow{ 345.678 }, expectedClose{ 456.789 }, expectedVolume{ 567.890 };
        Ohlcv ohlcv(0L, 0., 0., 0., 0., 0.);

        ohlcv.open(expectedOpen);
        ohlcv.high(expectedHigh);
        ohlcv.low(expectedLow);
        ohlcv.close(expectedClose);
        ohlcv.volume(expectedVolume);

        ASSERT_EQUAL_FP(expectedOpen, ohlcv.open(), ulp) << "open()";
        ASSERT_EQUAL_FP(expectedHigh, ohlcv.high(), ulp) << "high()";
        ASSERT_EQUAL_FP(expectedLow, ohlcv.low(), ulp) << "low()";
        ASSERT_EQUAL_FP(expectedClose, ohlcv.close(), ulp) << "close()";
        ASSERT_EQUAL_FP(expectedVolume, ohlcv.volume(), ulp) << "volume()";
    }

    TESTCASE("{median,typical,weighted}() returns correct value")
    {
        const double expectedOpen{ 123.456 }, expectedHigh{ 234.567 };
        const double expectedLow{ 345.678 }, expectedClose{ 456.789 };
        const double expectedMedian{ (expectedLow + expectedHigh) / 2 };
        const double expectedTypical{ (expectedLow + expectedHigh + expectedClose) / 3 };
        const double expectedWeighted{ (expectedLow + expectedHigh + expectedClose + expectedClose) / 4 };

        const Ohlcv ohlcv(0L, expectedOpen, expectedHigh, expectedLow, expectedClose, 0.);

        ASSERT_EQUAL_FP(expectedMedian, ohlcv.median(), ulp) << "median()";
        ASSERT_EQUAL_FP(expectedTypical, ohlcv.typical(), ulp) << "typical()";
        ASSERT_EQUAL_FP(expectedWeighted, ohlcv.weighted(), ulp) << "weighted()";
    }

    TESTCASE("price(OhlcvPriceType) returns correct value")
    {
        const double expectedOpen{ 123.456 }, expectedHigh{ 234.567 };
        const double expectedLow{ 345.678 }, expectedClose{ 456.789 };
        const double expectedMedian{ (expectedLow + expectedHigh) / 2 };
        const double expectedTypical{ (expectedLow + expectedHigh + expectedClose) / 3 };
        const double expectedWeighted{ (expectedLow + expectedHigh + expectedClose + expectedClose) / 4 };

        const Ohlcv ohlcv(0L, expectedOpen, expectedHigh, expectedLow, expectedClose, 0.);

        ASSERT_EQUAL_FP(expectedOpen, ohlcv.price(OhlcvPriceType::Open), ulp) << "open";
        ASSERT_EQUAL_FP(expectedHigh, ohlcv.price(OhlcvPriceType::High), ulp) << "high";
        ASSERT_EQUAL_FP(expectedLow, ohlcv.price(OhlcvPriceType::Low), ulp) << "low";
        ASSERT_EQUAL_FP(expectedClose, ohlcv.price(OhlcvPriceType::Close), ulp) << "close";
        ASSERT_EQUAL_FP(expectedMedian, ohlcv.price(OhlcvPriceType::Median), ulp) << "median";
        ASSERT_EQUAL_FP(expectedTypical, ohlcv.price(OhlcvPriceType::Typical), ulp) << "typical";
        ASSERT_EQUAL_FP(expectedWeighted, ohlcv.price(OhlcvPriceType::Weighted), ulp) << "weighted";
    }

    TESTCASE("component(OhlcvComponent) returns correct value")
    {
        const double expectedOpen{ 123.456 }, expectedHigh{ 234.567 };
        const double expectedLow{ 345.678 }, expectedClose{ 456.789 }, expectedVolume{ 567.890 };
        const double expectedMedian{ (expectedLow + expectedHigh) / 2 };
        const double expectedTypical{ (expectedLow + expectedHigh + expectedClose) / 3 };
        const double expectedWeighted{ (expectedLow + expectedHigh + expectedClose + expectedClose) / 4 };

        const Ohlcv ohlcv(0L, expectedOpen, expectedHigh, expectedLow, expectedClose, expectedVolume);

        ASSERT_EQUAL_FP(expectedOpen, ohlcv.component(OhlcvComponent::OpeningPrice), ulp) << "open";
        ASSERT_EQUAL_FP(expectedHigh, ohlcv.component(OhlcvComponent::HighPrice), ulp) << "high";
        ASSERT_EQUAL_FP(expectedLow, ohlcv.component(OhlcvComponent::LowPrice), ulp) << "low";
        ASSERT_EQUAL_FP(expectedClose, ohlcv.component(OhlcvComponent::ClosingPrice), ulp) << "close";
        ASSERT_EQUAL_FP(expectedMedian, ohlcv.component(OhlcvComponent::MedianPrice), ulp) << "median";
        ASSERT_EQUAL_FP(expectedTypical, ohlcv.component(OhlcvComponent::TypicalPrice), ulp) << "typical";
        ASSERT_EQUAL_FP(expectedWeighted, ohlcv.component(OhlcvComponent::WeightedPrice), ulp) << "weighted";
        ASSERT_EQUAL_FP(expectedVolume, ohlcv.component(OhlcvComponent::Volume), ulp) << "volume";
    }

    TESTCASE("is{Rising,Falling}() returns correct value")
    {
        Ohlcv ohlcv(0L, 0., 0., 0., 0., 0.);
        ASSERT_IS_FALSE(ohlcv.isRising()) << "rising (open == close)";
        ASSERT_IS_FALSE(ohlcv.isFalling()) << "falling (open == close)";

        ohlcv.open(1.);
        ASSERT_IS_FALSE(ohlcv.isRising()) << "rising (open > close)";
        ASSERT_IS_TRUE(ohlcv.isFalling()) << "falling (open > close)";

        ohlcv.open(0.);
        ohlcv.close(1.);
        ASSERT_IS_TRUE(ohlcv.isRising()) << "rising (open < close)";
        ASSERT_IS_FALSE(ohlcv.isFalling()) << "falling (open < close)";
    }

    TESTCASE("clone() returns an instance with correct values")
    {
        const long long expectedTicks{ 12345 };
        const double expectedOpen{ 123.456 }, expectedHigh{ 234.567 };
        const double expectedLow{ 345.678 }, expectedClose{ 456.789 }, expectedVolume{ 567.890 };
        const Ohlcv ohlcv(expectedTicks, expectedOpen, expectedHigh, expectedLow, expectedClose, expectedVolume);

        const std::unique_ptr<Ohlcv> ptr{ static_cast<Ohlcv*>(ohlcv.clone()) };

        ASSERT_EQUAL(expectedTicks, ptr->ticks()) << "ticks()";
        ASSERT_EQUAL_FP(expectedOpen, ptr->open(), ulp) << "open()";
        ASSERT_EQUAL_FP(expectedHigh, ptr->high(), ulp) << "high()";
        ASSERT_EQUAL_FP(expectedLow, ptr->low(), ulp) << "low()";
        ASSERT_EQUAL_FP(expectedClose, ptr->close(), ulp) << "close()";
        ASSERT_EQUAL_FP(expectedVolume, ptr->volume(), ulp) << "volume()";
    }

    // TODO: aggregation tests here ...
}

}