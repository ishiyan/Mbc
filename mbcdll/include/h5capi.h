#pragma once

#include "visibility.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _HDF5_H
    typedef unsigned long long hsize_t;
    typedef long long hid_t;
#endif

    typedef const void* H5FileHandle;
    typedef const void* H5InstrumentHandle;
    typedef const void* H5DataHandle;
    typedef const void* H5QuoteDataHandle;
    typedef const void* H5QuotePriceOnlyDataHandle;
    typedef const void* H5TradeDataHandle;
    typedef const void* H5TradePriceOnlyDataHandle;
    typedef const void* H5OhlcvDataHandle;
    typedef const void* H5OhlcvPriceOnlyDataHandle;
    typedef const void* H5ScalarDataHandle;

    // ----------------------------------------------------------------------
    // Enumerations
    // ----------------------------------------------------------------------

    //! The data type enumeration.
    enum class DataType
    {
        //! The trade (time, price, volume).
        Trade = 0,
        //! The trade without volume (time, price).
        TradePriceOnly = 1,
        //! The quote (time, ask price, ask size, bid price, bid size).
        Quote = 2,
        //! The quote without size (time, ask price, bid price).
        QuotePriceOnly = 3,
        //! The ohlcv (time, opening price, highest price, lowest price, closing price, volume).
        Ohlcv = 4,
        //! The ohlcv without volume (time, opening price, highest price, lowest price, closing price).
        OhlcvPriceOnly = 5,
        //! The scalar (time, value).
        Scalar = 6,
        //! The adjusted ohlcv (time, opening price, highest price, lowest price, closing price, volume).
        OhlcvAdjusted = 7,
        //! The adjusted ohlcv without volume (time, opening price, highest price, lowest price, closing price).
        OhlcvAdjustedPriceOnly = 8
    };

    //! The ohlcv kind enumeration.
    enum class OhlcvKind
    {
        //! Has no specific meaning.
        Default = 0,
        //! Ask price(/size) ohlc(v).
        Ask = 1,
        //! Bid price(/size) ohlc(v).
        Bid = 2
    };

    //! The scalar kind enumeration.
    enum class ScalarKind
    {
        //! Has no specific meaning.
        Default = 0
    };

    //! The trade kind enumeration.
    enum class TradeKind
    {
        //! Has no specific meaning.
        Default = 0
    };

    //! The quote kind enumeration.
    enum class QuoteKind
    {
        //! Has no specific meaning.
        Default = 0
    };

    //! The data kind enumeration.
    enum class DataKind
    {
        //! The trade price/volume.
        TradeAperiodic = 0x0000000,
        //! The trade price (no volume).
        TradePriceOnlyAperiodic = 0x1000000,
        //! The ask/bid price/size quote.
        QuoteAperiodic = 0x2000000,
        //! The ask/bid price (no size) quote.
        QuotePriceOnlyAperiodic = 0x3000000,

        //! The open/high/low/close/volume without any specific time frame.
        OhlcvAperiodic = 0x4000000,
        //! The 1-second open/high/low/close/volume.
        OhlcvSecond1 = 0x4000101,
        //! The 2-second open/high/low/close/volume.
        OhlcvSecond2 = 0x4000102,
        //! The 3-second open/high/low/close/volume.
        OhlcvSecond3 = 0x4000103,
        //! The 4-second open/high/low/close/volume.
        OhlcvSecond4 = 0x4000104,
        //! The 5-second open/high/low/close/volume.
        OhlcvSecond5 = 0x4000105,
        //! The 6-second open/high/low/close/volume.
        OhlcvSecond6 = 0x4000106,
        //! The 10-second open/high/low/close/volume.
        OhlcvSecond10 = 0x400010a,
        //! The 12-second open/high/low/close/volume.
        OhlcvSecond12 = 0x400010c,
        //! The 15-second open/high/low/close/volume.
        OhlcvSecond15 = 0x400010f,
        //! The 20-second open/high/low/close/volume.
        OhlcvSecond20 = 0x4000114,
        //! The 30-second open/high/low/close/volume.
        OhlcvSecond30 = 0x400011e,
        //! The 1-minute open/high/low/close/volume.
        OhlcvMinute1 = 0x4000201,
        //! The 2-minute open/high/low/close/volume.
        OhlcvMinute2 = 0x4000202,
        //! The 3-minute open/high/low/close/volume.
        OhlcvMinute3 = 0x4000203,
        //! The 4-minute open/high/low/close/volume.
        OhlcvMinute4 = 0x4000204,
        //! The 5-minute open/high/low/close/volume.
        OhlcvMinute5 = 0x4000205,
        //! The 6-minute open/high/low/close/volume.
        OhlcvMinute6 = 0x4000206,
        //! The 10-minute open/high/low/close/volume.
        OhlcvMinute10 = 0x400020a,
        //! The 12-minute open/high/low/close/volume.
        OhlcvMinute12 = 0x400020c,
        //! The 15-minute open/high/low/close/volume.
        OhlcvMinute15 = 0x400020f,
        //! The 20-minute open/high/low/close/volume.
        OhlcvMinute20 = 0x4000214,
        //! The 30-minute open/high/low/close/volume.
        OhlcvMinute30 = 0x400021e,
        //! The 1-hour open/high/low/close/volume.
        OhlcvHour1 = 0x4000301,
        //! The 2-hour open/high/low/close/volume.
        OhlcvHour2 = 0x4000302,
        //! The 3-hour open/high/low/close/volume.
        OhlcvHour3 = 0x4000303,
        //! The 4-hour open/high/low/close/volume.
        OhlcvHour4 = 0x4000304,
        //! The 6-hour open/high/low/close/volume.
        OhlcvHour6 = 0x4000306,
        //! The 8-hour open/high/low/close/volume.
        OhlcvHour8 = 0x4000308,
        //! The 12-hour open/high/low/close/volume.
        OhlcvHour12 = 0x400030c,
        //! The 1-day open/high/low/close/volume.
        OhlcvDay1 = 0x4000401,
        //! The 1-week open/high/low/close/volume.
        OhlcvWeek1 = 0x4000501,
        //! The 2-week open/high/low/close/volume.
        OhlcvWeek2 = 0x4000502,
        //! The 3-week open/high/low/close/volume.
        OhlcvWeek3 = 0x4000503,
        //! The 1-month open/high/low/close/volume.
        OhlcvMonth1 = 0x4000601,
        //! The 2-month open/high/low/close/volume.
        OhlcvMonth2 = 0x4000602,
        //! The 3-month open/high/low/close/volume.
        OhlcvMonth3 = 0x4000603,
        //! The 4-month open/high/low/close/volume.
        OhlcvMonth4 = 0x4000604,
        //! The 6-month open/high/low/close/volume.
        OhlcvMonth6 = 0x4000606,
        //! The 1-year open/high/low/close/volume.
        OhlcvYear1 = 0x4000701,

        //! The open/high/low/close/volume ask price/size without any specific time frame.
        OhlcvAskAperiodic = 0x4010000,
        //! The 1-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond1 = 0x4010101,
        //! The 2-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond2 = 0x4010102,
        //! The 3-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond3 = 0x4010103,
        //! The 4-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond4 = 0x4010104,
        //! The 5-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond5 = 0x4010105,
        //! The 6-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond6 = 0x4010106,
        //! The 10-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond10 = 0x401010a,
        //! The 12-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond12 = 0x401010c,
        //! The 15-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond15 = 0x401010f,
        //! The 20-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond20 = 0x4010114,
        //! The 30-second open/high/low/close/volume ask price/size.
        OhlcvAskSecond30 = 0x401011e,
        //! The 1-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute1 = 0x4010201,
        //! The 2-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute2 = 0x4010202,
        //! The 3-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute3 = 0x4010203,
        //! The 4-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute4 = 0x4010204,
        //! The 5-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute5 = 0x4010205,
        //! The 6-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute6 = 0x4010206,
        //! The 10-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute10 = 0x401020a,
        //! The 12-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute12 = 0x401020c,
        //! The 15-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute15 = 0x401020f,
        //! The 20-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute20 = 0x4010214,
        //! The 30-minute open/high/low/close/volume ask price/size.
        OhlcvAskMinute30 = 0x401021e,
        //! The 1-hour open/high/low/close/volume ask price/size.
        OhlcvAskHour1 = 0x4010301,
        //! The 2-hour open/high/low/close/volume ask price/size.
        OhlcvAskHour2 = 0x4010302,
        //! The 3-hour open/high/low/close/volume ask price/size.
        OhlcvAskHour3 = 0x4010303,
        //! The 4-hour open/high/low/close/volume ask price/size.
        OhlcvAskHour4 = 0x4010304,
        //! The 6-hour open/high/low/close/volume ask price/size.
        OhlcvAskHour6 = 0x4010306,
        //! The 8-hour open/high/low/close/volume ask price/size.
        OhlcvAskHour8 = 0x4010308,
        //! The 12-hour open/high/low/close/volume ask price/size.
        OhlcvAskHour12 = 0x401030c,
        //! The 1-day open/high/low/close/volume ask price/size.
        OhlcvAskDay1 = 0x4010401,
        //! The 1-week open/high/low/close/volume ask price/size.
        OhlcvAskWeek1 = 0x4010501,
        //! The 2-week open/high/low/close/volume ask price/size.
        OhlcvAskWeek2 = 0x4010502,
        //! The 3-week open/high/low/close/volume ask price/size.
        OhlcvAskWeek3 = 0x4010503,
        //! The 1-month open/high/low/close/volume ask price/size.
        OhlcvAskMonth1 = 0x4010601,
        //! The 2-month open/high/low/close/volume ask price/size.
        OhlcvAskMonth2 = 0x4010602,
        //! The 3-month open/high/low/close/volume ask price/size.
        OhlcvAskMonth3 = 0x4010603,
        //! The 4-month open/high/low/close/volume ask price/size.
        OhlcvAskMonth4 = 0x4010604,
        //! The 6-month open/high/low/close/volume ask price/size.
        OhlcvAskMonth6 = 0x4010606,
        //! The 1-year open/high/low/close/volume ask price/size.
        OhlcvAskYear1 = 0x4010701,

        //! The open/high/low/close/volume bid price/size without any specific time frame.
        OhlcvBidAperiodic = 0x4020000,
        //! The 1-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond1 = 0x4020101,
        //! The 2-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond2 = 0x4020102,
        //! The 3-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond3 = 0x4020103,
        //! The 4-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond4 = 0x4020104,
        //! The 5-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond5 = 0x4020105,
        //! The 6-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond6 = 0x4020106,
        //! The 10-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond10 = 0x402010a,
        //! The 12-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond12 = 0x402010c,
        //! The 15-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond15 = 0x402010f,
        //! The 20-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond20 = 0x4020114,
        //! The 30-second open/high/low/close/volume bid price/size.
        OhlcvBidSecond30 = 0x402011e,
        //! The 1-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute1 = 0x4020201,
        //! The 2-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute2 = 0x4020202,
        //! The 3-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute3 = 0x4020203,
        //! The 4-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute4 = 0x4020204,
        //! The 5-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute5 = 0x4020205,
        //! The 6-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute6 = 0x4020206,
        //! The 10-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute10 = 0x402020a,
        //! The 12-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute12 = 0x402020c,
        //! The 15-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute15 = 0x402020f,
        //! The 20-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute20 = 0x4020214,
        //! The 30-minute open/high/low/close/volume bid price/size.
        OhlcvBidMinute30 = 0x402021e,
        //! The 1-hour open/high/low/close/volume bid price/size.
        OhlcvBidHour1 = 0x4020301,
        //! The 2-hour open/high/low/close/volume bid price/size.
        OhlcvBidHour2 = 0x4020302,
        //! The 3-hour open/high/low/close/volume bid price/size.
        OhlcvBidHour3 = 0x4020303,
        //! The 4-hour open/high/low/close/volume bid price/size.
        OhlcvBidHour4 = 0x4020304,
        //! The 6-hour open/high/low/close/volume bid price/size.
        OhlcvBidHour6 = 0x4020306,
        //! The 8-hour open/high/low/close/volume bid price/size.
        OhlcvBidHour8 = 0x4020308,
        //! The 12-hour open/high/low/close/volume bid price/size.
        OhlcvBidHour12 = 0x402030c,
        //! The 1-day open/high/low/close/volume bid price/size.
        OhlcvBidDay1 = 0x4020401,
        //! The 1-week open/high/low/close/volume bid price/size.
        OhlcvBidWeek1 = 0x4020501,
        //! The 2-week open/high/low/close/volume bid price/size.
        OhlcvBidWeek2 = 0x4020502,
        //! The 3-week open/high/low/close/volume bid price/size.
        OhlcvBidWeek3 = 0x4020503,
        //! The 1-month open/high/low/close/volume bid price/size.
        OhlcvBidMonth1 = 0x4020601,
        //! The 2-month open/high/low/close/volume bid price/size.
        OhlcvBidMonth2 = 0x4020602,
        //! The 3-month open/high/low/close/volume bid price/size.
        OhlcvBidMonth3 = 0x4020603,
        //! The 4-month open/high/low/close/volume bid price/size.
        OhlcvBidMonth4 = 0x4020604,
        //! The 6-month open/high/low/close/volume bid price/size.
        OhlcvBidMonth6 = 0x4020606,
        //! The 1-year open/high/low/close/volume bid price/size.
        OhlcvBidYear1 = 0x4020701,

        //! The open/high/low/close without any specific time frame.
        OhlcvPriceOnlyAperiodic = 0x5000000,
        //! The 1-second open/high/low/close.
        OhlcvPriceOnlySecond1 = 0x5000101,
        //! The 2-second open/high/low/close.
        OhlcvPriceOnlySecond2 = 0x5000102,
        //! The 3-second open/high/low/close.
        OhlcvPriceOnlySecond3 = 0x5000103,
        //! The 4-second open/high/low/close.
        OhlcvPriceOnlySecond4 = 0x5000104,
        //! The 5-second open/high/low/close.
        OhlcvPriceOnlySecond5 = 0x5000105,
        //! The 6-second open/high/low/close.
        OhlcvPriceOnlySecond6 = 0x5000106,
        //! The 10-second open/high/low/close.
        OhlcvPriceOnlySecond10 = 0x500010a,
        //! The 12-second open/high/low/close.
        OhlcvPriceOnlySecond12 = 0x500010c,
        //! The 15-second open/high/low/close.
        OhlcvPriceOnlySecond15 = 0x500010f,
        //! The 20-second open/high/low/close.
        OhlcvPriceOnlySecond20 = 0x5000114,
        //! The 30-second open/high/low/close.
        OhlcvPriceOnlySecond30 = 0x500011e,
        //! The 1-minute open/high/low/close.
        OhlcvPriceOnlyMinute1 = 0x5000201,
        //! The 2-minute open/high/low/close.
        OhlcvPriceOnlyMinute2 = 0x5000202,
        //! The 3-minute open/high/low/close.
        OhlcvPriceOnlyMinute3 = 0x5000203,
        //! The 4-minute open/high/low/close.
        OhlcvPriceOnlyMinute4 = 0x5000204,
        //! The 5-minute open/high/low/close.
        OhlcvPriceOnlyMinute5 = 0x5000205,
        //! The 6-minute open/high/low/close.
        OhlcvPriceOnlyMinute6 = 0x5000206,
        //! The 10-minute open/high/low/close.
        OhlcvPriceOnlyMinute10 = 0x500020a,
        //! The 12-minute open/high/low/close.
        OhlcvPriceOnlyMinute12 = 0x500020c,
        //! The 15-minute open/high/low/close.
        OhlcvPriceOnlyMinute15 = 0x500020f,
        //! The 20-minute open/high/low/close.
        OhlcvPriceOnlyMinute20 = 0x5000214,
        //! The 30-minute open/high/low/close.
        OhlcvPriceOnlyMinute30 = 0x500021e,
        //! The 1-hour open/high/low/close.
        OhlcvPriceOnlyHour1 = 0x5000301,
        //! The 2-hour open/high/low/close.
        OhlcvPriceOnlyHour2 = 0x5000302,
        //! The 3-hour open/high/low/close.
        OhlcvPriceOnlyHour3 = 0x5000303,
        //! The 4-hour open/high/low/close.
        OhlcvPriceOnlyHour4 = 0x5000304,
        //! The 6-hour open/high/low/close.
        OhlcvPriceOnlyHour6 = 0x5000306,
        //! The 8-hour open/high/low/close.
        OhlcvPriceOnlyHour8 = 0x5000308,
        //! The 12-hour open/high/low/close.
        OhlcvPriceOnlyHour12 = 0x500030c,
        //! The 1-day open/high/low/close.
        OhlcvPriceOnlyDay1 = 0x5000401,
        //! The 1-week open/high/low/close.
        OhlcvPriceOnlyWeek1 = 0x5000501,
        //! The 2-week open/high/low/close.
        OhlcvPriceOnlyWeek2 = 0x5000502,
        //! The 3-week open/high/low/close.
        OhlcvPriceOnlyWeek3 = 0x5000503,
        //! The 1-month open/high/low/close.
        OhlcvPriceOnlyMonth1 = 0x5000601,
        //! The 2-month open/high/low/close.
        OhlcvPriceOnlyMonth2 = 0x5000602,
        //! The 3-month open/high/low/close.
        OhlcvPriceOnlyMonth3 = 0x5000603,
        //! The 4-month open/high/low/close.
        OhlcvPriceOnlyMonth4 = 0x5000604,
        //! The 6-month open/high/low/close.
        OhlcvPriceOnlyMonth6 = 0x5000606,
        //! The 1-year open/high/low/close.
        OhlcvPriceOnlyYear1 = 0x5000701,

        //! The open/high/low/close ask price without any specific time frame.
        OhlcvPriceOnlyAskAperiodic = 0x5010000,
        //! The 1-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond1 = 0x5010101,
        //! The 2-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond2 = 0x5010102,
        //! The 3-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond3 = 0x5010103,
        //! The 4-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond4 = 0x5010104,
        //! The 5-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond5 = 0x5010105,
        //! The 6-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond6 = 0x5010106,
        //! The 10-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond10 = 0x501010a,
        //! The 12-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond12 = 0x501010c,
        //! The 15-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond15 = 0x501010f,
        //! The 20-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond20 = 0x5010114,
        //! The 30-second open/high/low/close ask price.
        OhlcvPriceOnlyAskSecond30 = 0x501011e,
        //! The 1-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute1 = 0x5010201,
        //! The 2-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute2 = 0x5010202,
        //! The 3-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute3 = 0x5010203,
        //! The 4-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute4 = 0x5010204,
        //! The 5-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute5 = 0x5010205,
        //! The 6-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute6 = 0x5010206,
        //! The 10-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute10 = 0x501020a,
        //! The 12-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute12 = 0x501020c,
        //! The 15-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute15 = 0x501020f,
        //! The 20-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute20 = 0x5010214,
        //! The 30-minute open/high/low/close ask price.
        OhlcvPriceOnlyAskMinute30 = 0x501021e,
        //! The 1-hour open/high/low/close ask price.
        OhlcvPriceOnlyAskHour1 = 0x5010301,
        //! The 2-hour open/high/low/close ask price.
        OhlcvPriceOnlyAskHour2 = 0x5010302,
        //! The 3-hour open/high/low/close ask price.
        OhlcvPriceOnlyAskHour3 = 0x5010303,
        //! The 4-hour open/high/low/close ask price.
        OhlcvPriceOnlyAskHour4 = 0x5010304,
        //! The 6-hour open/high/low/close ask price.
        OhlcvPriceOnlyAskHour6 = 0x5010306,
        //! The 8-hour open/high/low/close ask price.
        OhlcvPriceOnlyAskHour8 = 0x5010308,
        //! The 12-hour open/high/low/close ask price.
        OhlcvPriceOnlyAskHour12 = 0x501030c,
        //! The 1-day open/high/low/close ask price.
        OhlcvPriceOnlyAskDay1 = 0x5010401,
        //! The 1-week open/high/low/close ask price.
        OhlcvPriceOnlyAskWeek1 = 0x5010501,
        //! The 2-week open/high/low/close ask price.
        OhlcvPriceOnlyAskWeek2 = 0x5010502,
        //! The 3-week open/high/low/close ask price.
        OhlcvPriceOnlyAskWeek3 = 0x5010503,
        //! The 1-month open/high/low/close ask price.
        OhlcvPriceOnlyAskMonth1 = 0x5010601,
        //! The 2-month open/high/low/close ask price.
        OhlcvPriceOnlyAskMonth2 = 0x5010602,
        //! The 3-month open/high/low/close ask price.
        OhlcvPriceOnlyAskMonth3 = 0x5010603,
        //! The 4-month open/high/low/close ask price.
        OhlcvPriceOnlyAskMonth4 = 0x5010604,
        //! The 6-month open/high/low/close ask price.
        OhlcvPriceOnlyAskMonth6 = 0x5010606,
        //! The 1-year open/high/low/close ask price.
        OhlcvPriceOnlyAskYear1 = 0x5010701,

        //! The open/high/low/close bid price without any specific time frame.
        OhlcvPriceOnlyBidAperiodic = 0x5020000,
        //! The 1-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond1 = 0x5020101,
        //! The 2-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond2 = 0x5020102,
        //! The 3-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond3 = 0x5020103,
        //! The 4-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond4 = 0x5020104,
        //! The 5-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond5 = 0x5020105,
        //! The 6-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond6 = 0x5020106,
        //! The 10-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond10 = 0x502010a,
        //! The 12-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond12 = 0x502010c,
        //! The 15-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond15 = 0x502010f,
        //! The 20-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond20 = 0x5020114,
        //! The 30-second open/high/low/close bid price.
        OhlcvPriceOnlyBidSecond30 = 0x502011e,
        //! The 1-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute1 = 0x5020201,
        //! The 2-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute2 = 0x5020202,
        //! The 3-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute3 = 0x5020203,
        //! The 4-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute4 = 0x5020204,
        //! The 5-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute5 = 0x5020205,
        //! The 6-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute6 = 0x5020206,
        //! The 10-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute10 = 0x502020a,
        //! The 12-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute12 = 0x502020c,
        //! The 15-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute15 = 0x502020f,
        //! The 20-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute20 = 0x5020214,
        //! The 30-minute open/high/low/close bid price.
        OhlcvPriceOnlyBidMinute30 = 0x502021e,
        //! The 1-hour open/high/low/close bid price.
        OhlcvPriceOnlyBidHour1 = 0x5020301,
        //! The 2-hour open/high/low/close bid price.
        OhlcvPriceOnlyBidHour2 = 0x5020302,
        //! The 3-hour open/high/low/close bid price.
        OhlcvPriceOnlyBidHour3 = 0x5020303,
        //! The 4-hour open/high/low/close bid price.
        OhlcvPriceOnlyBidHour4 = 0x5020304,
        //! The 6-hour open/high/low/close bid price.
        OhlcvPriceOnlyBidHour6 = 0x5020306,
        //! The 8-hour open/high/low/close bid price.
        OhlcvPriceOnlyBidHour8 = 0x5020308,
        //! The 12-hour open/high/low/close bid price.
        OhlcvPriceOnlyBidHour12 = 0x502030c,
        //! The 1-day open/high/low/close bid price.
        OhlcvPriceOnlyBidDay1 = 0x5020401,
        //! The 1-week open/high/low/close bid price.
        OhlcvPriceOnlyBidWeek1 = 0x5020501,
        //! The 2-week open/high/low/close bid price.
        OhlcvPriceOnlyBidWeek2 = 0x5020502,
        //! The 3-week open/high/low/close bid price.
        OhlcvPriceOnlyBidWeek3 = 0x5020503,
        //! The 1-month open/high/low/close bid price.
        OhlcvPriceOnlyBidMonth1 = 0x5020601,
        //! The 2-month open/high/low/close bid price.
        OhlcvPriceOnlyBidMonth2 = 0x5020602,
        //! The 3-month open/high/low/close bid price.
        OhlcvPriceOnlyBidMonth3 = 0x5020603,
        //! The 4-month open/high/low/close bid price.
        OhlcvPriceOnlyBidMonth4 = 0x5020604,
        //! The 6-month open/high/low/close bid price.
        OhlcvPriceOnlyBidMonth6 = 0x5020606,
        //! The 1-year open/high/low/close bid price.
        OhlcvPriceOnlyBidYear1 = 0x5020701,

        //! The scalar without any specific time frame.
        ScalarAperiodic = 0x6000000,
        //! The 1-second scalar.
        ScalarSecond1 = 0x6000101,
        //! The 2-second scalar.
        ScalarSecond2 = 0x6000102,
        //! The 3-second scalar.
        ScalarSecond3 = 0x6000103,
        //! The 4-second scalar.
        ScalarSecond4 = 0x6000104,
        //! The 5-second scalar.
        ScalarSecond5 = 0x6000105,
        //! The 6-second scalar.
        ScalarSecond6 = 0x6000106,
        //! The 10-second scalar.
        ScalarSecond10 = 0x600010a,
        //! The 12-second scalar.
        ScalarSecond12 = 0x600010c,
        //! The 15-second scalar.
        ScalarSecond15 = 0x600010f,
        //! The 20-second scalar.
        ScalarSecond20 = 0x6000114,
        //! The 30-second scalar.
        ScalarSecond30 = 0x600011e,
        //! The 1-minute scalar.
        ScalarMinute1 = 0x6000201,
        //! The 2-minute scalar.
        ScalarMinute2 = 0x6000202,
        //! The 3-minute scalar.
        ScalarMinute3 = 0x6000203,
        //! The 4-minute scalar.
        ScalarMinute4 = 0x6000204,
        //! The 5-minute scalar.
        ScalarMinute5 = 0x6000205,
        //! The 6-minute scalar.
        ScalarMinute6 = 0x6000206,
        //! The 10-minute scalar.
        ScalarMinute10 = 0x600020a,
        //! The 12-minute scalar.
        ScalarMinute12 = 0x600020c,
        //! The 15-minute scalar.
        ScalarMinute15 = 0x600020f,
        //! The 20-minute scalar.
        ScalarMinute20 = 0x6000214,
        //! The 30-minute scalar.
        ScalarMinute30 = 0x600021e,
        //! The 1-hour scalar.
        ScalarHour1 = 0x6000301,
        //! The 2-hour scalar.
        ScalarHour2 = 0x6000302,
        //! The 3-hour scalar.
        ScalarHour3 = 0x6000303,
        //! The 4-hour scalar.
        ScalarHour4 = 0x6000304,
        //! The 6-hour scalar.
        ScalarHour6 = 0x6000306,
        //! The 8-hour scalar.
        ScalarHour8 = 0x6000308,
        //! The 12-hour scalar.
        ScalarHour12 = 0x600030c,
        //! The 1-day scalar.
        ScalarDay1 = 0x6000401,
        //! The 1-week scalar.
        ScalarWeek1 = 0x6000501,
        //! The 2-week scalar.
        ScalarWeek2 = 0x6000502,
        //! The 3-week scalar.
        ScalarWeek3 = 0x6000503,
        //! The 1-month scalar.
        ScalarMonth1 = 0x6000601,
        //! The 2-month scalar.
        ScalarMonth2 = 0x6000602,
        //! The 3-month scalar.
        ScalarMonth3 = 0x6000603,
        //! The 4-month scalar.
        ScalarMonth4 = 0x6000604,
        //! The 6-month scalar.
        ScalarMonth6 = 0x6000606,
        //! The 1-year scalar.
        ScalarYear1 = 0x6000701,

        //! The adjusted open/high/low/close/volume without any specific time frame.
        OhlcvAdjustedAperiodic = 0x7000000,
        //! The 1-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond1 = 0x7000101,
        //! The 2-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond2 = 0x7000102,
        //! The 3-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond3 = 0x7000103,
        //! The 4-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond4 = 0x7000104,
        //! The 5-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond5 = 0x7000105,
        //! The 6-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond6 = 0x7000106,
        //! The 10-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond10 = 0x700010a,
        //! The 12-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond12 = 0x700010c,
        //! The 15-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond15 = 0x700010f,
        //! The 20-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond20 = 0x7000114,
        //! The 30-second adjusted open/high/low/close/volume.
        OhlcvAdjustedSecond30 = 0x700011e,
        //! The 1-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute1 = 0x7000201,
        //! The 2-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute2 = 0x7000202,
        //! The 3-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute3 = 0x7000203,
        //! The 4-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute4 = 0x7000204,
        //! The 5-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute5 = 0x7000205,
        //! The 6-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute6 = 0x7000206,
        //! The 10-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute10 = 0x700020a,
        //! The 12-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute12 = 0x700020c,
        //! The 15-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute15 = 0x700020f,
        //! The 20-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute20 = 0x7000214,
        //! The 30-minute adjusted open/high/low/close/volume.
        OhlcvAdjustedMinute30 = 0x700021e,
        //! The 1-hour adjusted open/high/low/close/volume.
        OhlcvAdjustedHour1 = 0x7000301,
        //! The 2-hour adjusted open/high/low/close/volume.
        OhlcvAdjustedHour2 = 0x7000302,
        //! The 3-hour adjusted open/high/low/close/volume.
        OhlcvAdjustedHour3 = 0x7000303,
        //! The 4-hour adjusted open/high/low/close/volume.
        OhlcvAdjustedHour4 = 0x7000304,
        //! The 6-hour adjusted open/high/low/close/volume.
        OhlcvAdjustedHour6 = 0x7000306,
        //! The 8-hour adjusted open/high/low/close/volume.
        OhlcvAdjustedHour8 = 0x7000308,
        //! The 12-hour adjusted open/high/low/close/volume.
        OhlcvAdjustedHour12 = 0x700030c,
        //! The 1-day adjusted open/high/low/close/volume.
        OhlcvAdjustedDay1 = 0x7000401,
        //! The 1-week adjusted open/high/low/close/volume.
        OhlcvAdjustedWeek1 = 0x7000501,
        //! The 2-week adjusted open/high/low/close/volume.
        OhlcvAdjustedWeek2 = 0x7000502,
        //! The 3-week adjusted open/high/low/close/volume.
        OhlcvAdjustedWeek3 = 0x7000503,
        //! The 1-month adjusted open/high/low/close/volume.
        OhlcvAdjustedMonth1 = 0x7000601,
        //! The 2-month adjusted open/high/low/close/volume.
        OhlcvAdjustedMonth2 = 0x7000602,
        //! The 3-month adjusted open/high/low/close/volume.
        OhlcvAdjustedMonth3 = 0x7000603,
        //! The 4-month adjusted open/high/low/close/volume.
        OhlcvAdjustedMonth4 = 0x7000604,
        //! The 6-month adjusted open/high/low/close/volume.
        OhlcvAdjustedMonth6 = 0x7000606,
        //! The 1-year adjusted open/high/low/close/volume.
        OhlcvAdjustedYear1 = 0x7000701,

        //! The adjusted open/high/low/close without any specific time frame.
        OhlcvAdjustedPriceOnlyAperiodic = 0x8000000,
        //! The 1-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond1 = 0x8000101,
        //! The 2-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond2 = 0x8000102,
        //! The 3-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond3 = 0x8000103,
        //! The 4-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond4 = 0x8000104,
        //! The 5-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond5 = 0x8000105,
        //! The 6-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond6 = 0x8000106,
        //! The 10-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond10 = 0x800010a,
        //! The 12-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond12 = 0x800010c,
        //! The 15-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond15 = 0x800010f,
        //! The 20-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond20 = 0x8000114,
        //! The 30-second adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlySecond30 = 0x800011e,
        //! The 1-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute1 = 0x8000201,
        //! The 2-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute2 = 0x8000202,
        //! The 3-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute3 = 0x8000203,
        //! The 4-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute4 = 0x8000204,
        //! The 5-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute5 = 0x8000205,
        //! The 6-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute6 = 0x8000206,
        //! The 10-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute10 = 0x800020a,
        //! The 12-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute12 = 0x800020c,
        //! The 15-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute15 = 0x800020f,
        //! The 20-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute20 = 0x8000214,
        //! The 30-minute adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMinute30 = 0x800021e,
        //! The 1-hour adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyHour1 = 0x8000301,
        //! The 2-hour adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyHour2 = 0x8000302,
        //! The 3-hour adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyHour3 = 0x8000303,
        //! The 4-hour adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyHour4 = 0x8000304,
        //! The 6-hour adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyHour6 = 0x8000306,
        //! The 8-hour adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyHour8 = 0x8000308,
        //! The 12-hour adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyHour12 = 0x800030c,
        //! The 1-day adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyDay1 = 0x8000401,
        //! The 1-week adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyWeek1 = 0x8000501,
        //! The 2-week adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyWeek2 = 0x8000502,
        //! The 3-week adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyWeek3 = 0x8000503,
        //! The 1-month adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMonth1 = 0x8000601,
        //! The 2-month adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMonth2 = 0x8000602,
        //! The 3-month adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMonth3 = 0x8000603,
        //! The 4-month adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMonth4 = 0x8000604,
        //! The 6-month adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyMonth6 = 0x8000606,
        //! The 1-year adjusted open/high/low/close.
        OhlcvAdjustedPriceOnlyYear1 = 0x8000701
    };

    //! The data time frame enumeration.
    enum class DataTimeFrame
    {
        //! Without any specific time frame.
        Aperiodic = 0x0000,
        //! The 1-second time frame.
        Second1 = 0x0101,
        //! The 2-second time frame.
        Second2 = 0x0102,
        //! The 3-second time frame.
        Second3 = 0x0103,
        //! The 4-second time frame.
        Second4 = 0x0104,
        //! The 5-second time frame.
        Second5 = 0x0105,
        //! The 6-second time frame.
        Second6 = 0x0106,
        //! The 10-second time frame.
        Second10 = 0x010a,
        //! The 12-second time frame.
        Second12 = 0x010c,
        //! The 15-second time frame.
        Second15 = 0x010f,
        //! The 20-second time frame.
        Second20 = 0x0114,
        //! The 30-second time frame.
        Second30 = 0x011e,
        //! The 1-minute time frame.
        Minute1 = 0x0201,
        //! The 2-minute time frame.
        Minute2 = 0x0202,
        //! The 3-minute time frame.
        Minute3 = 0x0203,
        //! The 4-minute time frame.
        Minute4 = 0x0204,
        //! The 5-minute time frame.
        Minute5 = 0x0205,
        //! The 6-minute time frame.
        Minute6 = 0x0206,
        //! The 10-minute time frame.
        Minute10 = 0x020a,
        //! The 12-minute time frame.
        Minute12 = 0x020c,
        //! The 15-minute time frame.
        Minute15 = 0x020f,
        //! The 20-minute time frame.
        Minute20 = 0x0214,
        //! The 30-minute time frame.
        Minute30 = 0x021e,
        //! The 1-hour time frame.
        Hour1 = 0x0301,
        //! The 2-hour time frame.
        Hour2 = 0x0302,
        //! The 3-hour time frame.
        Hour3 = 0x0303,
        //! The 4-hour time frame.
        Hour4 = 0x0304,
        //! The 6-hour time frame.
        Hour6 = 0x0306,
        //! The 8-hour time frame.
        Hour8 = 0x0308,
        //! The 12-hour time frame.
        Hour12 = 0x030c,
        //! The 1-day time frame.
        Day1 = 0x0401,
        //! The 1-week time frame.
        Week1 = 0x0501,
        //! The 2-week time frame.
        Week2 = 0x0502,
        //! The 3-week time frame.
        Week3 = 0x0503,
        //! The 1-month time frame.
        Month1 = 0x0601,
        //! The 2-month time frame.
        Month2 = 0x0602,
        //! The 3-month time frame.
        Month3 = 0x0603,
        //! The 4-month time frame.
        Month4 = 0x0604,
        //! The 6-month time frame.
        Month6 = 0x0606,
        //! The 1-year time frame.
        Year1 = 0x0701
    };

    // ----------------------------------------------------------------------
    // Callbacks
    // ----------------------------------------------------------------------

    //! Callback function to enumerate datasets.
    typedef void (*DatasetEnumerator)(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
        DataTimeFrame timeFrame, bool isValidName, const char* name, const char* path) noexcept;

    //! Callback function to add child data info to the group info.
    typedef void (*GroupInfoAddDataInfo)(const void* groupInfoHandle, DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind,
        TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, const char* name, const char* path) noexcept;

    //! Callback function to add child group info to the group info.
    typedef const void* (*GroupInfoAddGroupInfo)(const void* groupInfoHandle, const char* name, const char* path) noexcept;

    //! Callback function to redirects the HDF5 error messages.
    //! The first argument specifies the message.
    //! If the second argument is \c true, the message is an error.
    typedef void(*ErrorLogger)(const char*, bool) noexcept;

    // ----------------------------------------------------------------------
    // File
    // ----------------------------------------------------------------------

    //! The file driver enumeration.
    enum class FileDriver
    {
        //! The default Section 2 POSIX driver.
        Sec2 = 0,
        //! The standard I/O driver.
        StdIo = 1,
        //! File contents are stored only in memory until the file is closed and is written to disk.
        Core = 2,
        //! The logging driver. Logs the file access information and statistics to the [filename].fapl_log file.
        Log = 3
    };

    //! Redirects the HDF5 error messages to the caller-supplied callback function; otherwise, the HDF5 library writes them to the console.
    //! Should be called once at the very beginning.
    //! \param errorLogger The error logging callback.
    DLL_PUBLIC void DLL_CC h5InterceptErrorStack(ErrorLogger errorLogger);

    //! Scans a hierarchical tree structure of the content of the opened h5 file.
    //! \param fileHandle The h5 file handle.
    //! \param groupInfoHandle The root group info handle.
    //! \param addDataInfo Callback function to add child data info to the group info.
    //! \param addGroupInfo Callback function to add child group info to the group info.
    //! \param sort Specifies whether to sort the hierarchical tree structure.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5FileEnumerateContentTree(H5FileHandle fileHandle, const void* groupInfoHandle, GroupInfoAddDataInfo addDataInfo, GroupInfoAddGroupInfo addGroupInfo, bool sort);

    //! Scans a hierarchical tree structure of the content of the opened h5 file.
    //! \param filePath The h5 file path to scan.
    //! \param fileDriver Specifies a file driver to be used to open the file.
    //! \param groupInfoHandle The root group info handle.
    //! \param addDataInfo Callback function to add child data info to the group info.
    //! \param addGroupInfo Callback function to add child group info to the group info.
    //! \param sort Specifies whether to sort the hierarchical tree structure.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5FileEnumerateContentTreeStatic(const char* filePath, FileDriver fileDriver, const void* groupInfoHandle, GroupInfoAddDataInfo addDataInfo, GroupInfoAddGroupInfo addGroupInfo, bool sort);

    //! Enumerates datasets of the h5 file.
    //! \param fileHandle The h5 file handle.
    //! \param enumerate A \c DatasetEnumerator callback function.
    //! \param sort Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5FileEnumerateDatasets(H5FileHandle fileHandle, DatasetEnumerator enumerate, bool sort);

    //! Enumerates datasets from the specified instrument in the h5 file.
    //! \param fileHandle The h5 file handle.
    //! \param enumerate A \c DatasetEnumerator callback function.
    //! \param instrumentPath The instrument to scan.
    //! \param sort Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5FileEnumerateInstrumentDatasets(H5FileHandle fileHandle, DatasetEnumerator enumerate, const char* instrumentPath, bool sort);

    //! Enumerates datasets from the specified h5 file.
    //! \param enumerate A callback to enumerate datasets.
    //! \param filePath A path to the h5 file.
    //! \param fileDriver Specifies a file driver to be used to open the file.
    //! \param sort Specifies whether to sort the data content.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5FileEnumerateDatasetsStatic(DatasetEnumerator enumerate, const char* filePath, FileDriver fileDriver, bool sort);

    //! Enumerates datasets from the specified instrument within the specified h5 file.
    //! \param enumerate A callback to enumerate datasets.
    //! \param filePath A path to the h5 file.
    //! \param fileDriver Specifies a file driver to be used to open the file.
    //! \param instrumentPath A path to the instrument to scan.
    //! \param sort Specifies whether to sort the data content.
    DLL_PUBLIC bool DLL_CC h5FileEnumerateInstrumentDatasetsStatic(DatasetEnumerator enumerate, const char* filePath, FileDriver fileDriver, const char* instrumentPath, bool sort);

    //! Opens a h5 file for reading.
    //! \param filePath The h5 file path to open.
    //! \param fileDriver The file driver to use.
    //! \return A handle to a read-only h5 file object or \c nullptr in case of error.
    DLL_PUBLIC H5FileHandle DLL_CC h5FileOpenReadOnly(const char* filePath, FileDriver fileDriver = FileDriver::Sec2);

    //! Opens a h5 file for both reading and writing.
    //! \param filePath The h5 file path to open.
    //! \param createNonexistent Create file if it does not exist.
    //! \param fileDriver The file driver to use.
    //! \param corkTheCache If you have memory to spare, you could "cork the cache" until you reached a suitable point to update the metadata in the file, and call flush.
    //! \return A handle to a writable h5 file object or \c nullptr in case of error.
    DLL_PUBLIC H5FileHandle DLL_CC h5FileOpenReadWrite(const char* filePath, bool createNonexistent, FileDriver fileDriver = FileDriver::Sec2, bool corkTheCache = false);

    //! Flushes the data and closes an access to the h5 file. The \c fileHandle is not valid after this call.
    //! \param fileHandle The file handle.
    DLL_PUBLIC void DLL_CC h5FileClose(H5FileHandle fileHandle);

    //! Flushes the h5 file.
    //! \param fileHandle The file handle.
    DLL_PUBLIC void DLL_CC h5FileFlush(H5FileHandle fileHandle);

    //! Deletes an object with all descending objects from the h5 file.
    //! \param fileHandle The file handle.
    //! \param objectPath The object path to delete.
    //! \param deleteEmptyParentPath Delete the parent path if empty.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5FileDeleteObject(H5FileHandle fileHandle, const char* objectPath, bool deleteEmptyParentPath) noexcept;

    //! Moves an instrument from one path to another within the same h5 file.
    //! \param fileHandle The file handle.
    //! \param instrumentPathOld The old instrument path.
    //! \param instrumentPathNew The new moved instrument path.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5FileMoveInstrument(H5FileHandle fileHandle, const char* instrumentPathOld, const char* instrumentPathNew) noexcept;

    //! Renames an instrument. The path to the instrument remains the same, only the name will be changed.
    //! \param fileHandle The file handle.
    //! \param instrumentPath The instrument path including the name.
    //! \param instrumentNewName The new name of the instrument.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5FileRenameInstrument(H5FileHandle fileHandle, const char* instrumentPath, const char* instrumentNewName) noexcept;

    //! Opens an instrument.
    //! \param fileHandle The file handle.
    //! \param instrumentPath The instrument path to open.
    //! \param createNonexistent Create the instrument if it does not exist; works for writable h5 files only.
    //! \return A handle to an instrument object or \c nullptr in case of error.
    DLL_PUBLIC H5InstrumentHandle DLL_CC h5FileOpenInstrument(H5FileHandle fileHandle, const char* instrumentPath, bool createNonexistent = false);

    // ----------------------------------------------------------------------
    // Instrument
    // ----------------------------------------------------------------------

    //! Flushes the data and closes an access to the instrument. The \c instrumentHandle is not valid after this call.
    //! \param instrumentHandle The instrument handle.
    DLL_PUBLIC void DLL_CC h5InstrumentClose(H5InstrumentHandle instrumentHandle);

    //! Flushes the instrument.
    //! \param instrumentHandle The instrument handle.
    DLL_PUBLIC void DLL_CC h5InstrumentFlush(H5InstrumentHandle instrumentHandle);

    //! Enumerates a flat non-recursive list of datasets of the instrument's group.
    //! \param instrumentHandle The instrument's handle.
    //! \param enumerate A \c DatasetEnumerator callback function.
    //! \param sort Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5InstrumentEnumerateDatasets(H5InstrumentHandle instrumentHandle, DatasetEnumerator enumerate, bool sort);

    //! Opens an instrument's scalar data.
    //! \param instrumentHandle The instrument's handle.
    //! \param timeFrame A time frame of the data to open.
    //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
    //! \return A handle to a data object or \c nullptr in case of error.
    DLL_PUBLIC H5ScalarDataHandle DLL_CC h5InstrumentOpenScalarData(H5InstrumentHandle instrumentHandle, DataTimeFrame timeFrame, bool createNonexistent = false);

    //! Opens an instrument's trade data.
    //! \param instrumentHandle The instrument's handle.
    //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
    //! \return A handle to a data object or \c nullptr in case of error.
    DLL_PUBLIC H5TradeDataHandle DLL_CC h5InstrumentOpenTradeData(H5InstrumentHandle instrumentHandle, bool createNonexistent = false);

    //! Opens an instrument's price-only trade data.
    //! \param instrumentHandle The instrument's handle.
    //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
    //! \return A handle to a data object or \c nullptr in case of error.
    DLL_PUBLIC H5TradePriceOnlyDataHandle DLL_CC h5InstrumentOpenTradePriceOnlyData(H5InstrumentHandle instrumentHandle, bool createNonexistent = false);

    //! Opens an instrument's ohlcv data.
    //! \param instrumentHandle The instrument's handle.
    //! \param kind A kind of the data to open.
    //! \param timeFrame A time frame of the data to open.
    //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
    //! \return A handle to a data object or \c nullptr in case of error.
    DLL_PUBLIC H5OhlcvDataHandle DLL_CC h5InstrumentOpenOhlcvData(H5InstrumentHandle instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent = false);

    //! Opens an instrument's price-only ohlcv data.
    //! \param instrumentHandle The instrument's handle.
    //! \param kind A kind of the data to open.
    //! \param timeFrame A time frame of the data to open.
    //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
    //! \return A handle to a data object or \c nullptr in case of error.
    DLL_PUBLIC H5OhlcvPriceOnlyDataHandle DLL_CC h5InstrumentOpenOhlcvPriceOnlyData(H5InstrumentHandle instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent = false);

    //! Opens an adjusted instrument's ohlcv data.
    //! \param instrumentHandle The instrument's handle.
    //! \param kind A kind of the data to open.
    //! \param timeFrame A time frame of the data to open.
    //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
    //! \return A handle to a data object or \c nullptr in case of error.
    DLL_PUBLIC H5OhlcvDataHandle DLL_CC h5InstrumentOpenOhlcvAdjustedData(H5InstrumentHandle instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent = false);

    //! Opens an adjusted instrument's price-only ohlcv data.
    //! \param instrumentHandle The instrument's handle.
    //! \param kind A kind of the data to open.
    //! \param timeFrame A time frame of the data to open.
    //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
    //! \return A handle to a data object or \c nullptr in case of error.
    DLL_PUBLIC H5OhlcvPriceOnlyDataHandle DLL_CC h5InstrumentOpenOhlcvAdjustedPriceOnlyData(H5InstrumentHandle instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent = false);

    //! Opens an instrument's quote data.
    //! \param instrumentHandle The instrument's handle.
    //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
    //! \return A handle to a data object or \c nullptr in case of error.
    DLL_PUBLIC H5QuoteDataHandle DLL_CC h5InstrumentOpenQuoteData(H5InstrumentHandle instrumentHandle, bool createNonexistent = false);

    //! Opens an instrument's price-only quote data.
    //! \param instrumentHandle The instrument's handle.
    //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
    //! \return A handle to a data object or \c nullptr in case of error.
    DLL_PUBLIC H5QuotePriceOnlyDataHandle DLL_CC h5InstrumentOpenQuotePriceOnlyData(H5InstrumentHandle instrumentHandle, bool createNonexistent = false);

    // ----------------------------------------------------------------------
    // Data
    // ----------------------------------------------------------------------

    //! The duplicate time tick policy enumeration.
    enum class DuplicateTimeTicks
    {
        //! Fail the execution.
        Fail = 0,
        //! Drop the sample.
        Skip = 1,
        //! Update the sample.
        Update = 2
    };

    //! Gets the default maximal size in bytes of the read buffer, 0 means unlimited.
    DLL_PUBLIC unsigned long long DLL_CC h5DataGetDefaultMaximumReadBufferBytes();

    //! Sets the default maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param value The new value.
    DLL_PUBLIC void DLL_CC h5DataSetDefaultMaximumReadBufferBytes(unsigned long long value);

    // ----------------------------------------------------------------------
    // ScalarData
    // ----------------------------------------------------------------------

    //! Represents a scalar (time and value pair).
    struct Scalar
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The value.
        double value_;
    };

    //! Callback function to append scalar data item.
    //! The \c ticks parameter represents the number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
    typedef void(*ScalarDataAppender)(long long ticks, double value) noexcept;
	
    //! Flushes and closes an access to the scalar data. The \c dataHandle is not valid after this call.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5ScalarDataClose(H5ScalarDataHandle dataHandle);

    //! Flushes the scalar data.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5ScalarDataFlush(H5ScalarDataHandle dataHandle);

    //! The date and time ticks of the first scalar data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5ScalarDataFirstTicks(H5ScalarDataHandle dataHandle);

    //! The date and time ticks of the last scalar data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5ScalarDataLastTicks(H5ScalarDataHandle dataHandle);

    //! The number of the scalar data items.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5ScalarDataCount(H5ScalarDataHandle dataHandle);

    //! Gets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5ScalarDataGetMaximumReadBufferBytes(H5ScalarDataHandle dataHandle);

    //! Sets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    //! \param value The new value.
    DLL_PUBLIC void DLL_CC h5ScalarDataSetMaximumReadBufferBytes(H5ScalarDataHandle dataHandle, unsigned long long value);

    //! Converts ticks to a nearest index.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks to convert.
    //! \param index The index of the ticks.
    //! \param match If the index matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataTicksIndex(H5ScalarDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match);

    //! Converts a ticks range to an index range.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range.
    //! \param ticksTo The last ticks of the range.
    //! \param indexFrom The index of the first item in the range.
    //! \param indexTo The index of the last item in the range.
    //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
    //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataTicksIndexRange(H5ScalarDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo);

    //! Fetches all stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataFetchAll(H5ScalarDataHandle dataHandle, ScalarDataAppender collect);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param from A beginning date and time.
    //! \param to An ending date and time.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataFetchTickRange(H5ScalarDataHandle dataHandle, ScalarDataAppender collect, long long from, long long to);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param index The index of the first item in the range.
    //! \param count The number of items in the range.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataFetchIndexRange(H5ScalarDataHandle dataHandle, ScalarDataAppender collect, unsigned long long index, unsigned long long count);

    //! Adds an array of scalar items to the data.
    //! \param dataHandle The data handle.
    //! \param input A pointer to an array of input scalar items.
    //! \param inputCount A number of scalar items in the array.
    //! \param duplicateTimeTicks The duplicate time tick policy.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataAdd(H5ScalarDataHandle dataHandle, const Scalar* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose);

    //! Spreads the duplicate time ticks making one-tick increments.
    //! \param input A pointer to the input data array.
    //! \param inputCount The number of the data items in the \c input array.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating if the time ticks were spread.
    DLL_PUBLIC bool DLL_CC h5ScalarDataSpreadDuplicateTimeTicks(Scalar* input, size_t inputCount, bool verbose);

    //! Deletes a range of indices from the dataset.
    //! \param dataHandle The data handle.
    //! \param indexFrom The first index of the range (inclusive).
    //! \param indexTo The last index of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteIndexRange(H5ScalarDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo);

    //! Deletes a range of ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range (inclusive).
    //! \param ticksTo The last ticks of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteTickRange(H5ScalarDataHandle dataHandle, long long ticksFrom, long long ticksTo);

    //! Deletes all ticks before a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteAllBeforeTick(H5ScalarDataHandle dataHandle, long long ticks);

    //! Deletes all ticks before a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteAllBeforeIndex(H5ScalarDataHandle dataHandle, hsize_t index);

    //! Deletes all ticks after a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteAllAfterTick(H5ScalarDataHandle dataHandle, long long ticks);

    //! Deletes all ticks after a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteAllAfterIndex(H5ScalarDataHandle dataHandle, hsize_t index);

    // ----------------------------------------------------------------------
    // TradeData
    // ----------------------------------------------------------------------

    //! Represents a trade (price and volume).
    struct Trade
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The trade price.
        double price_;

        //! The trade volume.
        double volume_;
    };

    //! Callback function to append trade data item.
    //! The \c ticks parameter represents the number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
    typedef void(*TradeDataAppender)(long long ticks, double price, double volume) noexcept;

    //! Flushes and closes an access to the trade data. The \c dataHandle is not valid after this call.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5TradeDataClose(H5TradeDataHandle dataHandle);

    //! Flushes the trade data.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5TradeDataFlush(H5TradeDataHandle dataHandle);

    //! The date and time ticks of the first trade data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5TradeDataFirstTicks(H5TradeDataHandle dataHandle);

    //! The date and time ticks of the last trade data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5TradeDataLastTicks(H5TradeDataHandle dataHandle);

    //! The number of the trade data items.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5TradeDataCount(H5TradeDataHandle dataHandle);

    //! Gets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5TradeDataGetMaximumReadBufferBytes(H5TradeDataHandle dataHandle);

    //! Sets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    //! \param value The new value.
    DLL_PUBLIC void DLL_CC h5TradeDataSetMaximumReadBufferBytes(H5TradeDataHandle dataHandle, unsigned long long value);

    //! Converts ticks to a nearest index.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks to convert.
    //! \param index The index of the ticks.
    //! \param match If the index matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataTicksIndex(H5TradeDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match);

    //! Converts a ticks range to an index range.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range.
    //! \param ticksTo The last ticks of the range.
    //! \param indexFrom The index of the first item in the range.
    //! \param indexTo The index of the last item in the range.
    //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
    //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataTicksIndexRange(H5TradeDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo);

    //! Fetches all stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataFetchAll(H5TradeDataHandle dataHandle, TradeDataAppender collect);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param from A beginning date and time.
    //! \param to An ending date and time.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataFetchTickRange(H5TradeDataHandle dataHandle, TradeDataAppender collect, long long from, long long to);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param index The index of the first item in the range.
    //! \param count The number of items in the range.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataFetchIndexRange(H5TradeDataHandle dataHandle, TradeDataAppender collect, unsigned long long index, unsigned long long count);

    //! Adds an array of trade items to the data.
    //! \param dataHandle The data handle.
    //! \param input A pointer to an array of input trade items.
    //! \param inputCount A number of trade items in the array.
    //! \param duplicateTimeTicks The duplicate time tick policy.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataAdd(H5TradeDataHandle dataHandle, const Trade* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose);

    //! Spreads the duplicate time ticks making one-tick increments.
    //! \param input A pointer to the input data array.
    //! \param inputCount The number of the data items in the \c input array.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating if the time ticks were spread.
    DLL_PUBLIC bool DLL_CC h5TradeDataSpreadDuplicateTimeTicks(Trade* input, size_t inputCount, bool verbose);

    //! Deletes a range of indices from the dataset.
    //! \param dataHandle The data handle.
    //! \param indexFrom The first index of the range (inclusive).
    //! \param indexTo The last index of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteIndexRange(H5TradeDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo);

    //! Deletes a range of ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range (inclusive).
    //! \param ticksTo The last ticks of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteTickRange(H5TradeDataHandle dataHandle, long long ticksFrom, long long ticksTo);

    //! Deletes all ticks before a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteAllBeforeTick(H5TradeDataHandle dataHandle, long long ticks);

    //! Deletes all ticks before a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteAllBeforeIndex(H5TradeDataHandle dataHandle, hsize_t index);

    //! Deletes all ticks after a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteAllAfterTick(H5TradeDataHandle dataHandle, long long ticks);

    //! Deletes all ticks after a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteAllAfterIndex(H5TradeDataHandle dataHandle, hsize_t index);

    // ----------------------------------------------------------------------
    // TradePriceOnlyData
    // ----------------------------------------------------------------------

    //! Represents a trade without volume (price only).
    struct TradePriceOnly
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The trade price.
        double price_;
    };

    //! Callback function to append price-only trade data item.
    //! The \c ticks parameter represents the number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
    typedef void(*TradePriceOnlyDataAppender)(long long ticks, double price) noexcept;

    //! Flushes and closes an access to the price-only trade data. The \c dataHandle is not valid after this call.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5TradePriceOnlyDataClose(H5TradePriceOnlyDataHandle dataHandle);

    //! Flushes the price-only trade data.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5TradePriceOnlyDataFlush(H5TradePriceOnlyDataHandle dataHandle);

    //! The date and time ticks of the first price-only trade data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5TradePriceOnlyDataFirstTicks(H5TradePriceOnlyDataHandle dataHandle);

    //! The date and time ticks of the last price-only trade data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5TradePriceOnlyDataLastTicks(H5TradePriceOnlyDataHandle dataHandle);

    //! The number of the price-only trade data items.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5TradePriceOnlyDataCount(H5TradePriceOnlyDataHandle dataHandle);

    //! Gets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5TradePriceOnlyDataGetMaximumReadBufferBytes(H5TradePriceOnlyDataHandle dataHandle);

    //! Sets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    //! \param value The new value.
    DLL_PUBLIC void DLL_CC h5TradePriceOnlyDataSetMaximumReadBufferBytes(H5TradePriceOnlyDataHandle dataHandle, unsigned long long value);

    //! Converts ticks to a nearest index.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks to convert.
    //! \param index The index of the ticks.
    //! \param match If the index matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataTicksIndex(H5TradePriceOnlyDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match);

    //! Converts a ticks range to an index range.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range.
    //! \param ticksTo The last ticks of the range.
    //! \param indexFrom The index of the first item in the range.
    //! \param indexTo The index of the last item in the range.
    //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
    //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataTicksIndexRange(H5TradePriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo);

    //! Fetches all stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataFetchAll(H5TradePriceOnlyDataHandle dataHandle, TradePriceOnlyDataAppender collect);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param from A beginning date and time.
    //! \param to An ending date and time.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataFetchTickRange(H5TradePriceOnlyDataHandle dataHandle, TradePriceOnlyDataAppender collect, long long from, long long to);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param index The index of the first item in the range.
    //! \param count The number of items in the range.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataFetchIndexRange(H5TradePriceOnlyDataHandle dataHandle, TradePriceOnlyDataAppender collect, unsigned long long index, unsigned long long count);

    //! Adds an array of price-only trade items to the data.
    //! \param dataHandle The data handle.
    //! \param input A pointer to an array of input price-only trade items.
    //! \param inputCount A number of price-only trade items in the array.
    //! \param duplicateTimeTicks The duplicate time tick policy.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataAdd(H5TradePriceOnlyDataHandle dataHandle, const TradePriceOnly* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose);

    //! Spreads the duplicate time ticks making one-tick increments.
    //! \param input A pointer to the input data array.
    //! \param inputCount The number of the data items in the \c input array.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating if the time ticks were spread.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataSpreadDuplicateTimeTicks(TradePriceOnly* input, size_t inputCount, bool verbose);

    //! Deletes a range of indices from the dataset.
    //! \param dataHandle The data handle.
    //! \param indexFrom The first index of the range (inclusive).
    //! \param indexTo The last index of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteIndexRange(H5TradePriceOnlyDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo);

    //! Deletes a range of ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range (inclusive).
    //! \param ticksTo The last ticks of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteTickRange(H5TradePriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo);

    //! Deletes all ticks before a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteAllBeforeTick(H5TradePriceOnlyDataHandle dataHandle, long long ticks);

    //! Deletes all ticks before a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteAllBeforeIndex(H5TradePriceOnlyDataHandle dataHandle, hsize_t index);

    //! Deletes all ticks after a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteAllAfterTick(H5TradePriceOnlyDataHandle dataHandle, long long ticks);

    //! Deletes all ticks after a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteAllAfterIndex(H5TradePriceOnlyDataHandle dataHandle, hsize_t index);

    // ----------------------------------------------------------------------
    // OhlcvData
    // ----------------------------------------------------------------------

    //! Represents an ohlcv (open, high, low, close, volume) entity on a certain time interval.
    struct Ohlcv
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The opening price.
        double open_;

        //! The highest price.
        double high_;

        //! The lowest price.
        double low_;

        //! The closing price.
        double close_;

        //! The volume.
        double volume_;
    };

    //! Callback function to append ohlcv data item.
    //! The \c ticks parameter represents the number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
    typedef void(*OhlcvDataAppender)(long long ticks, double open, double high, double low, double close, double volume) noexcept;

    //! Flushes and closes an access to the ohlcv data. The \c dataHandle is not valid after this call.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5OhlcvDataClose(H5OhlcvDataHandle dataHandle);

    //! Flushes the ohlcv data.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5OhlcvDataFlush(H5OhlcvDataHandle dataHandle);

    //! The date and time ticks of the first ohlcv data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5OhlcvDataFirstTicks(H5OhlcvDataHandle dataHandle);

    //! The date and time ticks of the last ohlcv data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5OhlcvDataLastTicks(H5OhlcvDataHandle dataHandle);

    //! The number of the ohlcv data items.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5OhlcvDataCount(H5OhlcvDataHandle dataHandle);

    //! Gets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5OhlcvDataGetMaximumReadBufferBytes(H5OhlcvDataHandle dataHandle);

    //! Sets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    //! \param value The new value.
    DLL_PUBLIC void DLL_CC h5OhlcvDataSetMaximumReadBufferBytes(H5OhlcvDataHandle dataHandle, unsigned long long value);

    //! Converts ticks to a nearest index.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks to convert.
    //! \param index The index of the ticks.
    //! \param match If the index matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataTicksIndex(H5OhlcvDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match);

    //! Converts a ticks range to an index range.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range.
    //! \param ticksTo The last ticks of the range.
    //! \param indexFrom The index of the first item in the range.
    //! \param indexTo The index of the last item in the range.
    //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
    //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataTicksIndexRange(H5OhlcvDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo);

    //! Fetches all stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataFetchAll(H5OhlcvDataHandle dataHandle, OhlcvDataAppender collect);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param from A beginning date and time.
    //! \param to An ending date and time.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataFetchTickRange(H5OhlcvDataHandle dataHandle, OhlcvDataAppender collect, long long from, long long to);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param index The index of the first item in the range.
    //! \param count The number of items in the range.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataFetchIndexRange(H5OhlcvDataHandle dataHandle, OhlcvDataAppender collect, unsigned long long index, unsigned long long count);

    //! Adds an array of ohlcv items to the data.
    //! \param dataHandle The data handle.
    //! \param input A pointer to an array of input ohlcv items.
    //! \param inputCount A number of ohlcv items in the array.
    //! \param duplicateTimeTicks The duplicate time tick policy.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataAdd(H5OhlcvDataHandle dataHandle, const Ohlcv* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose);

    //! Spreads the duplicate time ticks making one-tick increments.
    //! \param input A pointer to the input data array.
    //! \param inputCount The number of the data items in the \c input array.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating if the time ticks were spread.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataSpreadDuplicateTimeTicks(Ohlcv* input, size_t inputCount, bool verbose);

    //! Deletes a range of indices from the dataset.
    //! \param dataHandle The data handle.
    //! \param indexFrom The first index of the range (inclusive).
    //! \param indexTo The last index of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteIndexRange(H5OhlcvDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo);

    //! Deletes a range of ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range (inclusive).
    //! \param ticksTo The last ticks of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteTickRange(H5OhlcvDataHandle dataHandle, long long ticksFrom, long long ticksTo);

    //! Deletes all ticks before a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteAllBeforeTick(H5OhlcvDataHandle dataHandle, long long ticks);

    //! Deletes all ticks before a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteAllBeforeIndex(H5OhlcvDataHandle dataHandle, hsize_t index);

    //! Deletes all ticks after a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteAllAfterTick(H5OhlcvDataHandle dataHandle, long long ticks);

    //! Deletes all ticks after a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteAllAfterIndex(H5OhlcvDataHandle dataHandle, hsize_t index);

    // ----------------------------------------------------------------------
    // OhlcvPriceOnlyData
    // ----------------------------------------------------------------------

    //! Represents an ohlc (open, high, low, close) entity on a certain time interval.
    struct OhlcvPriceOnly
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The opening price.
        double open_;

        //! The highest price.
        double high_;

        //! The lowest price.
        double low_;

        //! The closing price.
        double close_;
    };

    //! Callback function to append price-only ohlcv data item.
    //! The \c ticks parameter represents the number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
    typedef void(*OhlcvPriceOnlyDataAppender)(long long ticks, double open, double high, double low, double close) noexcept;

    //! Flushes and closes an access to the price-only ohlcv data. The \c dataHandle is not valid after this call.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5OhlcvPriceOnlyDataClose(H5OhlcvPriceOnlyDataHandle dataHandle);

    //! Flushes the price-only ohlcv data.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5OhlcvPriceOnlyDataFlush(H5OhlcvPriceOnlyDataHandle dataHandle);

    //! The date and time ticks of the first price-only ohlcv data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5OhlcvPriceOnlyDataFirstTicks(H5OhlcvPriceOnlyDataHandle dataHandle);

    //! The date and time ticks of the last price-only ohlcv data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5OhlcvPriceOnlyDataLastTicks(H5OhlcvPriceOnlyDataHandle dataHandle);

    //! The number of the price-only ohlcv data items.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5OhlcvPriceOnlyDataCount(H5OhlcvPriceOnlyDataHandle dataHandle);

    //! Gets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5OhlcvPriceOnlyDataGetMaximumReadBufferBytes(H5OhlcvPriceOnlyDataHandle dataHandle);

    //! Sets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    //! \param value The new value.
    DLL_PUBLIC void DLL_CC h5OhlcvPriceOnlyDataSetMaximumReadBufferBytes(H5OhlcvPriceOnlyDataHandle dataHandle, unsigned long long value);

    //! Converts ticks to a nearest index.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks to convert.
    //! \param index The index of the ticks.
    //! \param match If the index matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataTicksIndex(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match);

    //! Converts a ticks range to an index range.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range.
    //! \param ticksTo The last ticks of the range.
    //! \param indexFrom The index of the first item in the range.
    //! \param indexTo The index of the last item in the range.
    //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
    //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataTicksIndexRange(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo);

    //! Fetches all stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataFetchAll(H5OhlcvPriceOnlyDataHandle dataHandle, OhlcvPriceOnlyDataAppender collect);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param from A beginning date and time.
    //! \param to An ending date and time.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataFetchTickRange(H5OhlcvPriceOnlyDataHandle dataHandle, OhlcvPriceOnlyDataAppender collect, long long from, long long to);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param index The index of the first item in the range.
    //! \param count The number of items in the range.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataFetchIndexRange(H5OhlcvPriceOnlyDataHandle dataHandle, OhlcvPriceOnlyDataAppender collect, unsigned long long index, unsigned long long count);

    //! Adds an array of price-only ohlcv items to the data.
    //! \param dataHandle The data handle.
    //! \param input A pointer to an array of input price-only ohlcv items.
    //! \param inputCount A number of price-only ohlcv items in the array.
    //! \param duplicateTimeTicks The duplicate time tick policy.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataAdd(H5OhlcvPriceOnlyDataHandle dataHandle, const OhlcvPriceOnly* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose);

    //! Spreads the duplicate time ticks making one-tick increments.
    //! \param input A pointer to the input data array.
    //! \param inputCount The number of the data items in the \c input array.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating if the time ticks were spread.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataSpreadDuplicateTimeTicks(OhlcvPriceOnly* input, size_t inputCount, bool verbose);

    //! Deletes a range of indices from the dataset.
    //! \param dataHandle The data handle.
    //! \param indexFrom The first index of the range (inclusive).
    //! \param indexTo The last index of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteIndexRange(H5OhlcvPriceOnlyDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo);

    //! Deletes a range of ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range (inclusive).
    //! \param ticksTo The last ticks of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteTickRange(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo);

    //! Deletes all ticks before a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteAllBeforeTick(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticks);

    //! Deletes all ticks before a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteAllBeforeIndex(H5OhlcvPriceOnlyDataHandle dataHandle, hsize_t index);

    //! Deletes all ticks after a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteAllAfterTick(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticks);

    //! Deletes all ticks after a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteAllAfterIndex(H5OhlcvPriceOnlyDataHandle dataHandle, hsize_t index);

    // ----------------------------------------------------------------------
    // QuoteData
    // ----------------------------------------------------------------------

    //! Represents a quote.
    struct Quote
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The ask price.
        double askPrice_;

        //! The bid price.
        double bidPrice_;

        //! The ask size.
        double askSize_;

        //! The bid size.
        double bidSize_;
    };

    //! Callback function to append quote data item.
    //! The \c ticks parameter represents the number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
    typedef void(*QuoteDataAppender)(long long ticks, double askPrice, double bidPrice, double askSize, double bidSize) noexcept;

    //! Flushes and closes an access to the quote data. The \c dataHandle is not valid after this call.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5QuoteDataClose(H5QuoteDataHandle dataHandle);

    //! Flushes the quote data.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5QuoteDataFlush(H5QuoteDataHandle dataHandle);

    //! The date and time ticks of the first quote data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5QuoteDataFirstTicks(H5QuoteDataHandle dataHandle);

    //! The date and time ticks of the last quote data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5QuoteDataLastTicks(H5QuoteDataHandle dataHandle);

    //! The number of the quote data items.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5QuoteDataCount(H5QuoteDataHandle dataHandle);

    //! Gets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    //! \return The maximal size in bytes of the read buffer.
    DLL_PUBLIC unsigned long long DLL_CC h5QuoteDataGetMaximumReadBufferBytes(H5QuoteDataHandle dataHandle);

    //! Sets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    //! \param value The new value.
    DLL_PUBLIC void DLL_CC h5QuoteDataSetMaximumReadBufferBytes(H5QuoteDataHandle dataHandle, unsigned long long value);

    //! Converts ticks to a nearest index.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks to convert.
    //! \param index The index of the ticks.
    //! \param match If the index matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataTicksIndex(H5QuoteDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match);

    //! Converts a ticks range to an index range.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range.
    //! \param ticksTo The last ticks of the range.
    //! \param indexFrom The index of the first item in the range.
    //! \param indexTo The index of the last item in the range.
    //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
    //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataTicksIndexRange(H5QuoteDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo);

    //! Fetches all stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataFetchAll(H5QuoteDataHandle dataHandle, QuoteDataAppender collect);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param from A beginning date and time.
    //! \param to An ending date and time.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataFetchTickRange(H5QuoteDataHandle dataHandle, QuoteDataAppender collect, long long from, long long to);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param index The index of the first item in the range.
    //! \param count The number of items in the range.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataFetchIndexRange(H5QuoteDataHandle dataHandle, QuoteDataAppender collect, unsigned long long index, unsigned long long count);

    //! Adds an array of quote items to the data.
    //! \param dataHandle The data handle.
    //! \param input A pointer to an array of input quote items.
    //! \param inputCount A number of quote items in the array.
    //! \param duplicateTimeTicks The duplicate time tick policy.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataAdd(H5QuoteDataHandle dataHandle, const Quote* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose);

    //! Spreads the duplicate time ticks making one-tick increments.
    //! \param input A pointer to the input data array.
    //! \param inputCount The number of the data items in the \c input array.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating if the time ticks were spread.
    DLL_PUBLIC bool DLL_CC h5QuoteDataSpreadDuplicateTimeTicks(Quote* input, size_t inputCount, bool verbose);

    //! Deletes a range of indices from the dataset.
    //! \param dataHandle The data handle.
    //! \param indexFrom The first index of the range (inclusive).
    //! \param indexTo The last index of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteIndexRange(H5QuoteDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo);

    //! Deletes a range of ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range (inclusive).
    //! \param ticksTo The last ticks of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteTickRange(H5QuoteDataHandle dataHandle, long long ticksFrom, long long ticksTo);

    //! Deletes all ticks before a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteAllBeforeTick(H5QuoteDataHandle dataHandle, long long ticks);

    //! Deletes all ticks before a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteAllBeforeIndex(H5QuoteDataHandle dataHandle, hsize_t index);

    //! Deletes all ticks after a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteAllAfterTick(H5QuoteDataHandle dataHandle, long long ticks);

    //! Deletes all ticks after a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteAllAfterIndex(H5QuoteDataHandle dataHandle, hsize_t index);

    // ----------------------------------------------------------------------
    // QuotePriceOnlyData
    // ----------------------------------------------------------------------

    //! Represents a quote without an ask/bid size (price only).
    struct QuotePriceOnly
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The ask price.
        double askPrice_;

        //! The bid price.
        double bidPrice_;
    };

    //! Callback function to append price-only quote data item.
    //! The \c ticks parameter represents the number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
    typedef void(*QuotePriceOnlyDataAppender)(long long ticks, double askPrice, double bidPrice) noexcept;

    //! Flushes and closes an access to the price-only quote data. The \c dataHandle is not valid after this call.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5QuotePriceOnlyDataClose(H5QuotePriceOnlyDataHandle dataHandle);

    //! Flushes the price-only quote data.
    //! \param dataHandle The data handle.
    DLL_PUBLIC void DLL_CC h5QuotePriceOnlyDataFlush(H5QuotePriceOnlyDataHandle dataHandle);

    //! The date and time ticks of the first price-only quote data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5QuotePriceOnlyDataFirstTicks(H5QuotePriceOnlyDataHandle dataHandle);

    //! The date and time ticks of the last price-only quote data item.
    //! \param dataHandle The data handle.
    DLL_PUBLIC long long DLL_CC h5QuotePriceOnlyDataLastTicks(H5QuotePriceOnlyDataHandle dataHandle);

    //! The number of the price-only quote data items.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5QuotePriceOnlyDataCount(H5QuotePriceOnlyDataHandle dataHandle);

    //! Gets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    DLL_PUBLIC unsigned long long DLL_CC h5QuotePriceOnlyDataGetMaximumReadBufferBytes(H5QuotePriceOnlyDataHandle dataHandle);

    //! Sets the maximal size in bytes of the read buffer, 0 means unlimited.
    //! \param dataHandle The data handle.
    //! \param value The new value.
    DLL_PUBLIC void DLL_CC h5QuotePriceOnlyDataSetMaximumReadBufferBytes(H5QuotePriceOnlyDataHandle dataHandle, unsigned long long value);

    //! Converts ticks to a nearest index.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks to convert.
    //! \param index The index of the ticks.
    //! \param match If the index matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataTicksIndex(H5QuotePriceOnlyDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match);

    //! Converts a ticks range to an index range.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range.
    //! \param ticksTo The last ticks of the range.
    //! \param indexFrom The index of the first item in the range.
    //! \param indexTo The index of the last item in the range.
    //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
    //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataTicksIndexRange(H5QuotePriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo);

    //! Fetches all stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataFetchAll(H5QuotePriceOnlyDataHandle dataHandle, QuotePriceOnlyDataAppender collect);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param from A beginning date and time.
    //! \param to An ending date and time.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataFetchTickRange(H5QuotePriceOnlyDataHandle dataHandle, QuotePriceOnlyDataAppender collect, long long from, long long to);

    //! Fetches a specified range of stored items.
    //! \param dataHandle The data handle.
    //! \param collect A callback to fetch data into.
    //! \param index The index of the first item in the range.
    //! \param count The number of items in the range.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataFetchIndexRange(H5QuotePriceOnlyDataHandle dataHandle, QuotePriceOnlyDataAppender collect, unsigned long long index, unsigned long long count);

    //! Adds an array of price-only quote items to the data.
    //! \param dataHandle The data handle.
    //! \param input A pointer to an array of input price-only quote items.
    //! \param inputCount A number of price-only quote items in the array.
    //! \param duplicateTimeTicks The duplicate time tick policy.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataAdd(H5QuotePriceOnlyDataHandle dataHandle, const QuotePriceOnly* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose);

    //! Spreads the duplicate time ticks making one-tick increments.
    //! \param input A pointer to the input data array.
    //! \param inputCount The number of the data items in the \c input array.
    //! \param verbose Trace information messages.
    //! \return A boolean indicating if the time ticks were spread.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataSpreadDuplicateTimeTicks(QuotePriceOnly* input, size_t inputCount, bool verbose);

    //! Deletes a range of indices from the dataset.
    //! \param dataHandle The data handle.
    //! \param indexFrom The first index of the range (inclusive).
    //! \param indexTo The last index of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteIndexRange(H5QuotePriceOnlyDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo);

    //! Deletes a range of ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticksFrom The first ticks of the range (inclusive).
    //! \param ticksTo The last ticks of the range (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteTickRange(H5QuotePriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo);

    //! Deletes all ticks before a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteAllBeforeTick(H5QuotePriceOnlyDataHandle dataHandle, long long ticks);

    //! Deletes all ticks before a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteAllBeforeIndex(H5QuotePriceOnlyDataHandle dataHandle, hsize_t index);

    //! Deletes all ticks after a specified ticks from the dataset.
    //! \param dataHandle The data handle.
    //! \param ticks The ticks (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteAllAfterTick(H5QuotePriceOnlyDataHandle dataHandle, long long ticks);

    //! Deletes all ticks after a specified index from the dataset.
    //! \param dataHandle The data handle.
    //! \param index The index (inclusive).
    //! \return A boolean indicating the success of the operation.
    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteAllAfterIndex(H5QuotePriceOnlyDataHandle dataHandle, hsize_t index);

#ifdef __cplusplus
}
#endif
