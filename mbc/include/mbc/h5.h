#pragma once

#include <memory>
#include <cstring>
#include <functional>
#include <vector>

#ifndef _HDF5_H
typedef unsigned long long hsize_t;
typedef long long hid_t;
#endif

namespace mbc::h5 {

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

        //! Are both quote ask/bid price and size identical to an \p other quote item.
        //! \param other A quote item to compare to.
        //! \return A boolean indicating the identity.
        [[nodiscard]] bool isIdentical(const Quote& other) const
        {
            return (askPrice_ == other.askPrice_) && (bidPrice_ == other.bidPrice_) && (askSize_ == other.askSize_) && (bidSize_ == other.bidSize_);
        }

        //! Are the quote ask/bid price or size different from an \p other quote item.
        //! \param other A quote item to compare to.
        //! \return A boolean indicating the difference.
        [[nodiscard]] bool isDifferent(const Quote& other) const
        {
            return (askPrice_ != other.askPrice_) || (bidPrice_ != other.bidPrice_) || (askSize_ != other.askSize_) || (bidSize_ != other.bidSize_);
        }
    };

    //! Represents a quote without an ask/bid size (price only).
    struct QuotePriceOnly
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The ask price.
        double askPrice_;

        //! The bid price.
        double bidPrice_;

        //! Are both quote ask/bid price identical to an \p other quote item.
        //! \param other A quote item to compare to.
        //! \return A boolean indicating the identity.
        [[nodiscard]] bool isIdentical(const QuotePriceOnly& other) const
        {
            return (askPrice_ == other.askPrice_) && (bidPrice_ == other.bidPrice_);
        }

        //! Are the quote ask/bid price or size different from an \p other quote item.
        //! \param other A quote item to compare to.
        //! \return A boolean indicating the difference.
        [[nodiscard]] bool isDifferent(const QuotePriceOnly& other) const
        {
            return (askPrice_ != other.askPrice_) || (bidPrice_ != other.bidPrice_);
        }
    };

    //! Represents a trade (price and volume).
    struct Trade
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The trade price.
        double price_;

        //! The trade volume.
        double volume_;

        //! Are both trade price and volume identical to an \p other trade item.
        //! \param other A trade item to compare to.
        //! \return A boolean indicating the identity.
        [[nodiscard]] bool isIdentical(const Trade& other) const
        {
            return (price_ == other.price_) && (volume_ == other.volume_);
        }

        //! Are trade price or volume different from an \p other trade item.
        //! \param other A trade item to compare to.
        //! \return A boolean indicating the difference.
        [[nodiscard]] bool isDifferent(const Trade& other) const
        {
            return (price_ != other.price_) || (volume_ != other.volume_);
        }
    };

    //! Represents a trade without volume (price only).
    struct TradePriceOnly
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The trade price.
        double price_;

        //! Is trade price identical to an \p other trade item.
        //! \param other A trade item to compare to.
        //! \return A boolean indicating the identity.
        [[nodiscard]] bool isIdentical(const TradePriceOnly& other) const
        {
            return price_ == other.price_;
        }

        //! Is trade price different from an \p other trade item.
        //! \param other A trade item to compare to.
        //! \return A boolean indicating the difference.
        [[nodiscard]] bool isDifferent(const TradePriceOnly& other) const
        {
            return price_ != other.price_;
        }
    };

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

        //! Are both ohlcv prices and volume identical to an \p other ohlcv item.
        //! \param other An ohlcv item to compare to.
        //! \return A boolean indicating the identity.
        [[nodiscard]] bool isIdentical(const Ohlcv& other) const
        {
            return (open_ == other.open_) && (high_ == other.high_) && (low_ == other.low_) && (close_ == other.close_) && (volume_ == other.volume_);
        }

        //! Are ohlcv price or volume different from an \p other ohlcv item.
        //! \param other An ohlcv item to compare to.
        //! \return A boolean indicating the difference.
        [[nodiscard]] bool isDifferent(const Ohlcv& other) const
        {
            return (open_ != other.open_) || (high_ != other.high_) || (low_ != other.low_) || (close_ != other.close_) || (volume_ != other.volume_);
        }
    };

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

        //! Are ohlc prices identical to an \p other ohlc item.
        //! \param other An ohlc item to compare to.
        //! \return A boolean indicating the identity.
        [[nodiscard]] bool isIdentical(const OhlcvPriceOnly& other) const
        {
            return (open_ == other.open_) && (high_ == other.high_) && (low_ == other.low_) && (close_ == other.close_);
        }

        //! Are ohlc prices different from an \p other ohlc item.
        //! \param other An ohlcv item to compare to.
        //! \return A boolean indicating the difference.
        [[nodiscard]] bool isDifferent(const OhlcvPriceOnly& other) const
        {
            return (open_ != other.open_) || (high_ != other.high_) || (low_ != other.low_) || (close_ != other.close_);
        }
    };

    //! Represents a scalar (time and value pair).
    struct Scalar
    {
        //! The number of 100-nanosecond ticks since 12:00 A.M., January, 1, 0001.
        long long ticks_;

        //! The value.
        double value_;

        //! Is scalar value identical to an \p other scalar item.
        //! \param other A scalar item to compare to.
        //! \return A boolean indicating the identity.
        [[nodiscard]] bool isIdentical(const Scalar& other) const
        {
            return value_ == other.value_;
        }

        //! Is scalar value different from an \p other scalar item.
        //! \param other A scalar item to compare to.
        //! \return A boolean indicating the difference.
        [[nodiscard]] bool isDifferent(const Scalar& other) const
        {
            return value_ != other.value_;
        }
    };

    // Forward declarations.
    class Instrument;
    class Data;
    class QuoteData;
    class QuotePriceOnlyData;
    class TradeData;
    class TradePriceOnlyData;
    class OhlcvData;
    class OhlcvPriceOnlyData;
    class ScalarData;
    class ObjectInfo;
    class DataInfo;
    class GroupInfo;

    template <size_t SIZE> class CharBuf
    {
    public:
        CharBuf(const CharBuf&) = delete;
        CharBuf(const CharBuf&&) = delete;
        void operator=(const CharBuf&) = delete;
        void operator=(const CharBuf&&) = delete;

    private:
        friend class ObjectInfo;
        friend class DataInfo;
        friend class GroupInfo;
        friend class File;
        friend class Instrument;

        explicit CharBuf(const char* str) :
            heapBuf_(nullptr),
            realBuf_(stackBuf_),
            capacity_(SIZE),
            size_(0)
        {
            stackBuf_[0] = '\0';
            append(str);
        }

        explicit CharBuf() :
            heapBuf_(nullptr),
            realBuf_(stackBuf_),
            capacity_(SIZE),
            size_(0)
        {
            stackBuf_[0] = '\0';
        }

        ~CharBuf()
        {
            delete[] heapBuf_;
        }

        [[nodiscard]] const char* c_str() const noexcept { return realBuf_; }

        void append(const char* str)
        {
            size_t newSize = strlen(str);
            if (0 == newSize)
                return;
            newSize += size_;
            if (capacity_ < newSize)
            {
                char* const newBuf = new char[1 + newSize];
                if (0 == size_)
                    newBuf[0] = '\0';
                else
                    strcpy(newBuf, realBuf_);
                delete[] heapBuf_;
                heapBuf_ = newBuf;
                realBuf_ = newBuf;
                capacity_ = newSize;
            }
            if (0 == size_)
                strcpy(realBuf_, str);
            else
                strcat(realBuf_, str);
            size_ = newSize;
        }

        void assign(const char* str)
        {
            size_ = 0;
            realBuf_[0] = '\0';
            append(str);
        }

        char stackBuf_[SIZE + 1]{}, * heapBuf_, * realBuf_;
        size_t  capacity_, size_;
    };

    //! Encapsulates a HDF5 object info.
    class ObjectInfo
    {
    public:
        //! The name of the object.
        [[nodiscard]] const char* name() const noexcept { return name_.c_str(); }

        //! The full path to the object.
        [[nodiscard]] const char* path() const noexcept { return path_.c_str(); }

        //! The parent group.
        [[nodiscard]] const GroupInfo* parent() const noexcept { return parent_; }

        ObjectInfo(const ObjectInfo&) = delete;
        ObjectInfo(const ObjectInfo&&) = delete;
        void operator=(const ObjectInfo&) = delete;
        void operator=(const ObjectInfo&&) = delete;
        virtual ~ObjectInfo() = default;

    private:
        friend class DataInfo;
        friend class GroupInfo;

        explicit ObjectInfo(const char* name, const GroupInfo* parent);
        explicit ObjectInfo();

        CharBuf<64-1> name_;
        CharBuf<256-1> path_;
        const GroupInfo* parent_;
    };

    //! Encapsulates a HDF5 dataset info.
    class DataInfo final : public ObjectInfo
    {
    public:
        //! The data type. Valid if DataInfo::isValidName() is true.
        [[nodiscard]] DataType parsedDataType() const noexcept { return dataType_; }

        //! The data kind. Valid if DataInfo::isValidName() is true.
        [[nodiscard]] DataKind parsedDataKind() const noexcept { return dataKind_; }

        //! The ohlcv kind. Valid if DataInfo::isValidName() is true.
        [[nodiscard]] OhlcvKind parsedOhlcvKind() const noexcept { return ohlcvKind_; }

        //! The scalar kind. Valid if DataInfo::isValidName() is true.
        [[nodiscard]] ScalarKind parsedScalarKind() const noexcept { return scalarKind_; }

        //! The trade kind. Valid if DataInfo::isValidName() is true.
        [[nodiscard]] TradeKind parsedTradeKind() const noexcept { return tradeKind_; }

        //! The quote kind. Valid if DataInfo::isValidName() is true.
        [[nodiscard]] QuoteKind parsedQuoteKind() const noexcept { return quoteKind_; }

        //! The data time frame. Valid if DataInfo::isValidName() is true.
        [[nodiscard]] DataTimeFrame parsedTimeFrame() const noexcept { return timeFrame_; }

        //! If the data name is a valid one.
        [[nodiscard]] bool isValidName() const noexcept { return isValidName_; }

        DataInfo(const char* name, const GroupInfo* parent);
        DataInfo(const DataInfo&);
        DataInfo(const DataInfo&&) noexcept;

        void operator=(const DataInfo&) = delete;
        void operator=(const DataInfo&&) = delete;
        ~DataInfo() = default;

    private:
        friend class GroupInfo;
        friend class File;
        friend class Instrument;

        static int compare(const void* first, const void* second) noexcept;

        DataType dataType_;
        DataKind dataKind_;
        OhlcvKind ohlcvKind_;
        ScalarKind scalarKind_;
        TradeKind tradeKind_;
        QuoteKind quoteKind_;
        DataTimeFrame timeFrame_;
        bool isValidName_;
    };

    template <typename T, size_t SIZE, size_t DELTA> class PtrBuf
    {
    public:
        PtrBuf(const PtrBuf&) = delete;
        PtrBuf(const PtrBuf&&) = delete;
        void operator=(const PtrBuf&) = delete;
        void operator=(const PtrBuf&&) = delete;

     private:
        friend class GroupInfo;
        friend class File;

        PtrBuf() :
            stackBuf_{},
            heapBuf_{ nullptr },
            realBuf_{ stackBuf_ },
            capacity_{ SIZE },
            size_{ 0 }
        {
        }

        ~PtrBuf()
        {
            for (size_t i = 0; i != size_; ++i)
                delete realBuf_[i];
            delete[] heapBuf_;
        }

        void append(const T* item)
        {
            if (capacity_ < size_ + 1)
            {
                auto** newBuf = new T * [DELTA + capacity_];
                if (0 < size_)
                    memcpy(static_cast<void*>(newBuf), static_cast<const void*>(realBuf_), sizeof(T*) * size_);
                delete[] heapBuf_;
                heapBuf_ = newBuf;
                realBuf_ = newBuf;
                capacity_ += DELTA;
            }
            realBuf_[size_] = const_cast<T*>(item);
            ++size_;
        }

        [[nodiscard]] size_t size() const noexcept { return size_; }
        [[nodiscard]] const T** data() const noexcept { return const_cast<const T * *>(realBuf_); }

        T* stackBuf_[SIZE], ** heapBuf_, ** realBuf_;
        size_t  capacity_, size_;
    };

    typedef PtrBuf<DataInfo, 128, 128> DataInfoList;

    //! Callback function to enumerate h5::DataInfo items.
    typedef std::function<void(const DataInfo&)> DataInfoEnumerator;

    //! Callback function to enumerate h5::GroupInfo items.
    typedef std::function<void(const GroupInfo*)> GroupInfoEnumerator;

    //! Encapsulates a HDF5 group info.
    class GroupInfo final : public ObjectInfo
    {
    public:
        //! The number of datasets in this group.
        [[nodiscard]] size_t datasetsCount() const noexcept { return datasets_.size(); }

        //! The datasets in this group.
        [[nodiscard]] const DataInfo** datasets() const noexcept { return datasets_.data(); }

        //! Enumerates datasets in this group.
        //! \param enumerate The enumeration function.
        void enumerateDatasets(const DataInfoEnumerator& enumerate) const;

        //! The number of sub-groups in this group.
        [[nodiscard]] size_t groupsCount() const noexcept { return groups_.size(); }

        //! The sub-groups in this group.
        [[nodiscard]] const GroupInfo** groups() const noexcept { return groups_.data(); }

        //! Enumerates groups in this group.
        //! \param enumerate The enumeration function.
        void enumerateGroups(const GroupInfoEnumerator& enumerate) const;

    private:
        friend class ObjectInfo;
        friend class File;
        friend class Instrument;

        GroupInfo(const char* name, const GroupInfo* parent);
        GroupInfo();

        static bool iterateGroup(hid_t groupOrFileId, const char* groupName, GroupInfo* parentGroupInfo);
        void sort() const;
        static int compare(const void* first, const void* second) noexcept;
        void moveDatasetsTo(DataInfoList* datasets);
        void enumerateDatasetsRecursive(const DataInfoEnumerator& enumerate) const;

        PtrBuf<DataInfo, 16, 16> datasets_;
        PtrBuf<GroupInfo, 32, 32> groups_;

    public:
        GroupInfo(const GroupInfo&) = delete;
        GroupInfo(const GroupInfo&&) = delete;
        void operator=(const GroupInfo&) = delete;
        void operator=(const GroupInfo&&) = delete;
        ~GroupInfo() = default;
    };

    //! Callback function to redirects the HDF5 error messages.
    //! The first argument specifies the message.
    //! If the second argument is \c true, the message is an error.
    typedef void(*ErrorLogger)(const char*, bool) noexcept;

    //! Encapsulates the access to a h5 file.
    class File final
    {
    public:
        //! Constructs a new instance.
        File(const char* filePath, hid_t fileId, bool readOnly = true);

        //! Destructs the instance.
        ~File() noexcept;

        File(const File&) = delete;
        File(const File&&) = delete;
        void operator=(const File&) = delete;
        void operator=(const File&&) = delete;

        //! Scans a hierarchical tree structure of the content of the specified HDF5 file.
        //! \param filePath The path to HDF5 file to scan.
        //! \param fileDriver Specifies a file driver to be used to open the file.
        //! \param sort Specifies whether to sort the hierarchical tree structure.
        //! \return The root group info object, transferring the ownership to the caller.
        static std::shared_ptr<GroupInfo> contentTree(const char* filePath, FileDriver fileDriver, bool sort);

        //! Scans a hierarchical tree structure of the content of the opened HDF5 file.
        //! \param sort Specifies whether to sort the hierarchical tree structure.
        //! \return The root group info object, transferring the ownership to the caller.
        [[nodiscard]] std::shared_ptr<GroupInfo> contentTree(bool sort) const;

        //! Enumerates datasets of the h5 file.
        //! \param enumerate A callback to enumerate datasets.
        //! \param sort Specifies whether to sort the list of datasets.
        [[nodiscard]] bool enumerateDatasets(const DataInfoEnumerator& enumerate, bool sort) const;

        //! Enumerates datasets of the instrument's group.
        //! \param enumerate A callback to enumerate datasets.
        //! \param instrumentPath The instrument to scan.
        //! \param sort Specifies whether to sort the list of datasets.
        [[nodiscard]] bool enumerateDatasets(const DataInfoEnumerator& enumerate, const char* instrumentPath, bool sort) const;

        //! Enumerates datasets from the specified h5 file.
        //! \param enumerate A callback to enumerate datasets.
        //! \param filePath A path to the h5 file.
        //! \param fileDriver Specifies a file driver to be used to open the file.
        //! \param sort Specifies whether to sort the data content.
        static bool enumerateDatasets(const DataInfoEnumerator& enumerate, const char* filePath, FileDriver fileDriver, bool sort);

        //! Enumerates datasets from the specified instrument within the specified h5 file.
        //! \param enumerate A callback to enumerate datasets.
        //! \param filePath A path to the h5 file.
        //! \param fileDriver Specifies a file driver to be used to open the file.
        //! \param instrumentPath A path to the instrument to scan.
        //! \param sort Specifies whether to sort the data content.
        static bool enumerateDatasets(const DataInfoEnumerator& enumerate, const char* filePath, FileDriver fileDriver, const char* instrumentPath, bool sort);

        //! Redirects the HDF5 error messages to the caller-supplied callback function; otherwise, the HDF5 library writes them to the console.
        //! Should be called once at the very beginning.
        static void interceptErrorStack(ErrorLogger errorLogger);

        //! Flushes the data and closes an access to the h5 file.
        void close();

        //! Flushes the h5 file.
        void flush() const;

        //! Opens an instrument.
        //! \param instrumentPath The instrument path to open.
        //! \param createNonexistent Create the instrument if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to an instrument object.
        std::shared_ptr<Instrument> openInstrument(const char* instrumentPath, bool createNonexistent = false);

        //! Deletes an object with all descending objects from the h5 file.
        //! \param objectPath The object path to delete.
        //! \param deleteEmptyParentPath Delete the parent path if empty.
        //! \return A boolean indicating the success of the operation.
        bool deleteObject(const char* objectPath, bool deleteEmptyParentPath) const;

        //! Moves an instrument from one path to another within the same h5 file.
        //! \param instrumentPathOld The original instrument path to move.
        //! \param instrumentPathNew The new moved instrument path.
        //! \return A boolean indicating the success of the operation.
        bool moveInstrument(const char* instrumentPathOld, const char* instrumentPathNew) const;

        //! Renames an instrument. The path to the instrument remains the same, only the name will be changed.
        //! \param instrumentPath The instrument path including the name.
        //! \param instrumentNewName The new name of the instrument.
        //! \return A boolean indicating the success of the operation.
        bool renameInstrument(const char* instrumentPath, const char* instrumentNewName) const;

        //! Opens a h5 file for reading.
        //! \param filePath The h5 file path to open.
        //! \param fileDriver The file driver to use.
        //! \return A shared pointer to a read-only h5 file object.
        static std::shared_ptr<File> openReadOnly(const char* filePath, FileDriver fileDriver = FileDriver::Sec2);

        //! Opens a h5 file for both reading and writing.
        //! \param filePath The h5 file path to open.
        //! \param createNonexistent Create file if it does not exist.
        //! \param fileDriver The file driver to use.
        //! \param corkTheCache If you have memory to spare, you could "cork the cache" until you reached a suitable point to update the metadata in the file, and call flush.
        //! \return A shared pointer to a writable h5 file object.
        static std::shared_ptr<File> openReadWrite(const char* filePath, bool createNonexistent, FileDriver fileDriver = FileDriver::Sec2, bool corkTheCache = false);
 
    private:
        std::string filePath_;
        hid_t fileId_;
        bool readOnly_;
    };

    //! Encapsulates the access to an instrument.
    class Instrument final
    {
    public:
        Instrument(const Instrument&) = delete;
        Instrument(const Instrument&&) = delete;
        void operator=(const Instrument&) = delete;
        void operator=(const Instrument&&) = delete;

        //! Constructs a new instance.
        Instrument(const char* instrumentPath, hid_t groupId, bool readOnly, std::string filePath);

        //! Destructs the instance.
        ~Instrument();

        //! Flushes the data and closes an access to the instrument.
        void close();

        //! Flushes the instrument.
        void flush() const;

        //! Enumerates a flat non-recursive list of datasets of the instrument's group.
        //! \param enumerate A reference to the \c DataInfoEnumerator callback function.
        //! \param sort Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool enumerateDatasets(const DataInfoEnumerator& enumerate, bool sort) const;

        //! Opens an instrument's quote data.
        //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to a h5::QuoteData object.
        [[nodiscard]] std::shared_ptr<QuoteData> openQuoteData(bool createNonexistent = false) const;

        //! Opens an instrument's price-only quote data.
        //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to a h5::QuotePriceOnlyData object.
        [[nodiscard]] std::shared_ptr<QuotePriceOnlyData> openQuotePriceOnlyData(bool createNonexistent = false) const;

        //! Opens an instrument's trade data.
        //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to a h5::TradeData object.
        [[nodiscard]] std::shared_ptr<TradeData> openTradeData(bool createNonexistent = false) const;

        //! Opens an instrument's price-only trade data.
        //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to a h5::TradePriceOnlyData object.
        [[nodiscard]] std::shared_ptr<TradePriceOnlyData> openTradePriceOnlyData(bool createNonexistent = false) const;

        //! Opens an instrument's ohlcv data.
        //! \param kind A kind of the data to open.
        //! \param timeFrame A time frame of the data to open.
        //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to a h5::OhlcvData object.
        [[nodiscard]] std::shared_ptr<OhlcvData> openOhlcvData(OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent = false) const;

        //! Opens an instrument's price-only ohlcv data.
        //! \param kind A kind of the data to open.
        //! \param timeFrame A time frame of the data to open.
        //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to a h5::OhlcvPriceOnlyData object.
        [[nodiscard]] std::shared_ptr<OhlcvPriceOnlyData> openOhlcvPriceOnlyData(OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent = false) const;

        //! Opens an adjusted instrument's ohlcv data.
        //! \param kind A kind of the data to open.
        //! \param timeFrame A time frame of the data to open.
        //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to a h5::OhlcvData object.
        [[nodiscard]] std::shared_ptr<OhlcvData> openOhlcvAdjustedData(OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent = false) const;

        //! Opens an adjusted instrument's price-only ohlcv data.
        //! \param kind A kind of the data to open.
        //! \param timeFrame A time frame of the data to open.
        //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to a h5::OhlcvPriceOnlyData object.
        [[nodiscard]] std::shared_ptr<OhlcvPriceOnlyData> openOhlcvAdjustedPriceOnlyData(OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent = false) const;

        //! Opens an instrument's scalar data.
        //! \param timeFrame A time frame of the data to open.
        //! \param createNonexistent Create the data if it does not exist; works for writable h5 files only.
        //! \return A shared pointer to a h5::ScalarData object.
        [[nodiscard]] std::shared_ptr<ScalarData> openScalarData(DataTimeFrame timeFrame, bool createNonexistent = false) const;

    private:
        friend class Data;

        [[nodiscard]] static int compareDataset(const void* first, const void* second) noexcept;

        [[nodiscard]] inline std::shared_ptr<OhlcvData> openOhlcvData(OhlcvKind kind, DataTimeFrame timeFrame, bool adjusted, bool createNonexistent) const;
        [[nodiscard]] inline std::shared_ptr<OhlcvPriceOnlyData> openOhlcvPriceOnlyData(OhlcvKind kind, DataTimeFrame timeFrame, bool adjusted, bool createNonexistent) const;

        std::string filePath_;
        std::string instrumentPath_;
        hid_t groupId_;
        bool readOnly_;
    };

    template <typename T> using Collect = std::function<void(const T*, hsize_t)>;

    //! Encapsulates the access to the instrument's data.
    class Data
    {
    public:
        Data(const Data&) = delete;
        Data(const Data&&) = delete;
        void operator=(const Data&) = delete;
        void operator=(const Data&&) = delete;

        //! Destructs the instance.
        virtual ~Data();

        //! Flushes the dataset.
        void flush() const;

        //! Closes an access to the data.
        void close();

        //! The date and time ticks of the first data item.
        [[nodiscard]] long long firstTicks() const;

        //! The date and time ticks of the last data item.
        [[nodiscard]] long long lastTicks() const;

        //! The number of data items.
        [[nodiscard]] hsize_t count() const;

        //! The maximal size in bytes of the read buffer, 0 means unlimited.
        [[nodiscard]] hsize_t maximumReadBufferBytes() const
        {
            return maximumReadBufferBytes_;
        }

        //! Sets the maximal size in bytes of the read buffer, 0 means unlimited.
        void setMaximumReadBufferBytes(const hsize_t value)
        {
            maximumReadBufferBytes_ = value;
        }

        //! The default maximal size in bytes of the read buffer, 0 means unlimited.
        static hsize_t defaultMaximumReadBufferBytes()
        {
            return defaultMaximumReadBufferBytes_;
        }

        //! Sets the default maximal size in bytes of the read buffer, 0 means unlimited.
        static void setDefaultMaximumReadBufferBytes(const hsize_t value)
        {
            defaultMaximumReadBufferBytes_ = value;
        }

        //! Converts ticks to an index.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        virtual bool ticksIndex(long long ticks, hsize_t& index, bool& match) const = 0;

        //! Converts a ticks range to an index range.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \param indexFrom The index of the first item in the range.
        //! \param indexTo The index of the last item in the range.
        //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
        //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        virtual bool ticksIndexRange(long long ticksFrom, long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const = 0;

    private:
        friend class Instrument;
        friend class QuoteData;
        friend class QuotePriceOnlyData;
        friend class TradeData;
        friend class TradePriceOnlyData;
        friend class OhlcvData;
        friend class OhlcvPriceOnlyData;
        friend class ScalarData;

        //! Constructs a new instance.
        Data(const Instrument* instrument, const char* name, hid_t datasetId, hid_t nativeTypeId, hid_t standardTypeId, bool readOnly);

        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input An input data vector.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        template <typename T> static bool spreadList(std::vector<T>& input, bool verbose);

        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input A pointer to the input data array.
        //! \param inputCount The number of the data items in the \c input array.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        template <typename T> static bool spreadList(T* input, size_t inputCount, bool verbose);

        //! Deletes a range of indices from the dataset.
        //! \param indexFrom The first index of the range.
        //! \param indexTo The last index of the range.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool deleteIndexRange(hsize_t indexFrom, hsize_t indexTo) const;

        //! Deletes a range of ticks from the dataset.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool deleteTickRange(long long ticksFrom, long long ticksTo) const;

        //! Writes an array of input data to the dataset.
        //! \param input A pointer to an array of input items.
        //! \param inputCount A number of items in the array.
        //! \param duplicateTimeTicks The duplicate time tick policy.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool addData(const T* input, hsize_t inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const;

        //! Writes a list of input data to the dataset.
        //! \param input A vector of input items.
        //! \param duplicateTimeTicks The duplicate time tick policy.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool addData(std::vector<T>& input, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const;

        //! Fetches a range of stored data from the dataset.
        //! \param collect A callback to fetch data into.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool fetchData(Collect<T> collect, long long ticksFrom, long long ticksTo) const;

        //! Fetches a range of stored data from the dataset without locking.
        //! \param collect A callback to fetch data into.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool fetchDataNoLock(Collect<T> collect, long long ticksFrom, long long ticksTo) const;

        //! Converts ticks to an index without locking.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool ticksToIndexNoLock(long long ticks, hsize_t& index, bool& match) const;

        //! Converts ticks to an index.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool ticksToIndex(long long ticks, hsize_t& index, bool& match) const;

        //! Converts a ticks range to an index range.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \param indexFrom The index of the first item in the range.
        //! \param indexTo The index of the last item in the range.
        //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
        //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool ticksToIndexRange(long long ticksFrom, long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const;

        //! Fetches a range of stored data from the dataset.
        //! \param collect A callback to fetch data into.
        //! \param index The index of the first item in the range.
        //! \param count The number of items in the range.
        //! \return A boolean indicating the success of the operation.
        template <typename T> [[nodiscard]] bool fetchFromIndexRange(Collect<T> collect, hsize_t index, hsize_t count) const;

        static hsize_t defaultMaximumReadBufferBytes_;
        hsize_t  maximumReadBufferBytes_;
        std::string filePath_;
        std::string datasetPath_;
        std::string name_;
        hid_t datasetId_;
        hid_t nativeTypeId_;
        hid_t standardTypeId_;
        bool readOnly_;
    };

    typedef Collect<Quote> CollectQuote;

    //! Encapsulates the access to the instrument's h5::Quote data.
    class QuoteData final : public Data
    {
    public:
        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input An input data vector.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(std::vector<Quote>& input, bool verbose);

        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input A pointer to the input data array.
        //! \param inputCount The number of the data items in the \c input array.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(Quote* input, size_t inputCount, bool verbose);

        //! Adds an array of quotes to the data.
        //! \param input A pointer to an array of input quotes.
        //! \param inputCount A number of quotes in the array.
        //! \param duplicateTimeTicks The duplicate time tick policy.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool add(const Quote* input, hsize_t inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const;

        //! Fetches all stored items.
        //! \param collect A callback to fetch data into.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchAll(const CollectQuote& collect) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param from A beginning date and time.
        //! \param to An ending date and time.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchTickRange(const CollectQuote& collect, long long from, long long to) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param index The index of the first item in the range.
        //! \param count The number of items in the range.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchIndexRange(const CollectQuote& collect, hsize_t index, hsize_t count) const;

        //! Converts ticks to a nearest index.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndex(long long ticks, hsize_t& index, bool& match) const override;

        //! Converts a ticks range to an index range.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \param indexFrom The index of the first item in the range.
        //! \param indexTo The index of the last item in the range.
        //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
        //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndexRange(long long ticksFrom, long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const override;

        //! Deletes a range of indices from the dataset.
        //! \param indexFrom The first index of the range (inclusive).
        //! \param indexTo The last index of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteIndexRange(hsize_t indexFrom, hsize_t indexTo) const;

        //! Deletes a range of ticks from the dataset .
        //! \param ticksFrom The first ticks of the range (inclusive).
        //! \param ticksTo The last ticks of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteTickRange(long long ticksFrom, long long ticksTo) const;

        //! Deletes all ticks before a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeTick(long long ticks) const;

        //! Deletes all ticks before a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeIndex(hsize_t index) const;

        //! Deletes all ticks after a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterTick(long long ticks) const;

        //! Deletes all ticks after a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterIndex(hsize_t index) const;

        QuoteData() = delete;
        QuoteData(const QuoteData&) = delete;
        QuoteData(const QuoteData&&) = delete;
        void operator=(const QuoteData&) = delete;
        void operator=(const QuoteData&&) = delete;
        ~QuoteData() = default;

    private:
        friend class Instrument;

        //! Constructs a new instance.
        QuoteData(const Instrument* instrument, const char* name, hid_t datasetId, hid_t nativeTypeId, hid_t standardTypeId, bool readOnly);

    };

    typedef Collect<QuotePriceOnly> CollectQuotePriceOnly;

    //! Encapsulates the access to the instrument's h5::QuotePriceOnly data.
    class QuotePriceOnlyData final : public Data
    {
    public:
        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input An input data vector.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(std::vector<QuotePriceOnly>& input, bool verbose);

        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input A pointer to the input data array.
        //! \param inputCount The number of the data items in the \c input array.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(QuotePriceOnly* input, size_t inputCount, bool verbose);

        //! Adds an array of price-only quotes to the data.
        //! \param input A pointer to an array of input price-only quotes.
        //! \param inputCount A number of price-only quotes in the array.
        //! \param duplicateTimeTicks The duplicate time tick policy.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool add(const QuotePriceOnly* input, hsize_t inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const;

        //! Fetches all stored items.
        //! \param collect A callback to fetch data into.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchAll(const CollectQuotePriceOnly& collect) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param from A beginning date and time.
        //! \param to An ending date and time.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchTickRange(const CollectQuotePriceOnly& collect, long long from, long long to) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param index The index of the first item in the range.
        //! \param count The number of items in the range.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchIndexRange(const CollectQuotePriceOnly& collect, hsize_t index, hsize_t count) const;

        //! Converts ticks to a nearest index.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndex(long long ticks, hsize_t& index, bool& match) const override;

        //! Converts a ticks range to an index range.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \param indexFrom The index of the first item in the range.
        //! \param indexTo The index of the last item in the range.
        //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
        //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndexRange(long long ticksFrom, long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const override;

        //! Deletes a range of indices from the dataset.
        //! \param indexFrom The first index of the range (inclusive).
        //! \param indexTo The last index of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteIndexRange(hsize_t indexFrom, hsize_t indexTo) const;

        //! Deletes a range of ticks from the dataset .
        //! \param ticksFrom The first ticks of the range (inclusive).
        //! \param ticksTo The last ticks of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteTickRange(long long ticksFrom, long long ticksTo) const;

        //! Deletes all ticks before a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeTick(long long ticks) const;

        //! Deletes all ticks before a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeIndex(hsize_t index) const;

        //! Deletes all ticks after a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterTick(long long ticks) const;

        //! Deletes all ticks after a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterIndex(hsize_t index) const;

        QuotePriceOnlyData() = delete;
        QuotePriceOnlyData(const QuotePriceOnlyData&) = delete;
        QuotePriceOnlyData(const QuotePriceOnlyData&&) = delete;
        void operator=(const QuotePriceOnlyData&) = delete;
        void operator=(const QuotePriceOnlyData&&) = delete;
        ~QuotePriceOnlyData() = default;

    private:
        friend class Instrument;

        //! Constructs a new instance.
        QuotePriceOnlyData(const Instrument* instrument, const char* name, hid_t datasetId, hid_t nativeTypeId, hid_t standardTypeId, bool readOnly);
    };

    typedef Collect<Trade> CollectTrade;

    //! Encapsulates the access to the instrument's h5::Trade data.
    class TradeData final : public Data
    {
    public:
        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input An input data vector.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(std::vector<Trade>& input, bool verbose);

        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input A pointer to the input data array.
        //! \param inputCount The number of the data items in the \c input array.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(Trade* input, size_t inputCount, bool verbose);

        //! Adds a list of trades to the data.
        //! \param input A pointer to an array of input trades.
        //! \param inputCount A number of trades in the array.
        //! \param duplicateTimeTicks The duplicate time tick policy.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool add(const Trade* input, hsize_t inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const;

        //! Fetches all stored items.
        //! \param collect A callback to fetch data into.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchAll(const CollectTrade& collect) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param from A beginning date and time.
        //! \param to An ending date and time.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchTickRange(const CollectTrade& collect, long long from, long long to) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param index The index of the first item in the range.
        //! \param count The number of items in the range.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchIndexRange(const CollectTrade& collect, hsize_t index, hsize_t count) const;

        //! Converts ticks to a nearest index.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndex(long long ticks, hsize_t& index, bool& match) const override;

        //! Converts a ticks range to an index range.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \param indexFrom The index of the first item in the range.
        //! \param indexTo The index of the last item in the range.
        //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
        //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndexRange(long long ticksFrom, long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const override;

        //! Deletes a range of indices from the dataset.
        //! \param indexFrom The first index of the range (inclusive).
        //! \param indexTo The last index of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteIndexRange(hsize_t indexFrom, hsize_t indexTo) const;

        //! Deletes a range of ticks from the dataset .
        //! \param ticksFrom The first ticks of the range (inclusive).
        //! \param ticksTo The last ticks of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteTickRange(long long ticksFrom, long long ticksTo) const;

        //! Deletes all ticks before a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeTick(long long ticks) const;

        //! Deletes all ticks before a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeIndex(hsize_t index) const;

        //! Deletes all ticks after a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterTick(long long ticks) const;

        //! Deletes all ticks after a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterIndex(hsize_t index) const;

        TradeData() = delete;
        TradeData(const TradeData&) = delete;
        TradeData(const TradeData&&) = delete;
        void operator=(const TradeData&) = delete;
        void operator=(const TradeData&&) = delete;
        ~TradeData() = default;

    private:
        friend class Instrument;

        //! Constructs a new instance.
        TradeData(const Instrument* instrument, const char* name, hid_t datasetId, hid_t nativeTypeId, hid_t standardTypeId, bool readOnly);
    };

    typedef Collect<TradePriceOnly> CollectTradePriceOnly;

    //! Encapsulates the access to the instrument's h5::TradePriceOnly data.
    class TradePriceOnlyData final : public Data
    {
    public:
        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input An input data vector.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(std::vector<TradePriceOnly>& input, bool verbose);

        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input A pointer to the input data array.
        //! \param inputCount The number of the data items in the \c input array.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(TradePriceOnly* input, size_t inputCount, bool verbose);

        //! Adds an array of price-only trades to the data.
        //! \param input A pointer to an array of input price-only trades.
        //! \param inputCount A number of price-only trades in the array.
        //! \param duplicateTimeTicks The duplicate time tick policy.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool add(const TradePriceOnly* input, hsize_t inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const;

        //! Fetches all stored items.
        //! \param collect A callback to fetch data into.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchAll(const CollectTradePriceOnly& collect) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param from A beginning date and time.
        //! \param to An ending date and time.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchTickRange(const CollectTradePriceOnly& collect, long long from, long long to) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param index The index of the first item in the range.
        //! \param count The number of items in the range.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchIndexRange(const CollectTradePriceOnly& collect, hsize_t index, hsize_t count) const;

        //! Converts ticks to a nearest index.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndex(long long ticks, hsize_t& index, bool& match) const override;

        //! Converts a ticks range to an index range.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \param indexFrom The index of the first item in the range.
        //! \param indexTo The index of the last item in the range.
        //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
        //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndexRange(long long ticksFrom, long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const override;

        //! Deletes a range of indices from the dataset.
        //! \param indexFrom The first index of the range (inclusive).
        //! \param indexTo The last index of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteIndexRange(hsize_t indexFrom, hsize_t indexTo) const;

        //! Deletes a range of ticks from the dataset .
        //! \param ticksFrom The first ticks of the range (inclusive).
        //! \param ticksTo The last ticks of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteTickRange(long long ticksFrom, long long ticksTo) const;

        //! Deletes all ticks before a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeTick(long long ticks) const;

        //! Deletes all ticks before a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeIndex(hsize_t index) const;

        //! Deletes all ticks after a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterTick(long long ticks) const;

        //! Deletes all ticks after a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterIndex(hsize_t index) const;

        TradePriceOnlyData() = delete;
        TradePriceOnlyData(const TradePriceOnlyData&) = delete;
        TradePriceOnlyData(const TradePriceOnlyData&&) = delete;
        void operator=(const TradePriceOnlyData&) = delete;
        void operator=(const TradePriceOnlyData&&) = delete;
        ~TradePriceOnlyData() = default;

    private:
        friend class Instrument;

        //! Constructs a new instance.
        TradePriceOnlyData(const Instrument* instrument, const char* name, hid_t datasetId, hid_t nativeTypeId, hid_t standardTypeId, bool readOnly);
    };

    typedef Collect<Ohlcv> CollectOhlcv;

    //! Encapsulates the access to the instrument's h5::Ohlcv data.
    class OhlcvData final : public Data
    {
    public:
        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input An input data vector.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(std::vector<Ohlcv>& input, bool verbose);

        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input A pointer to the input data array.
        //! \param inputCount The number of the data items in the \c input array.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(Ohlcv* input, size_t inputCount, bool verbose);

        //! Adds an array of ohlcv items to the data.
        //! \param input A pointer to an array of input ohlcv items.
        //! \param inputCount A number of ohlcv items in the array.
        //! \param duplicateTimeTicks The duplicate time tick policy.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool add(const Ohlcv* input, hsize_t inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const;

        //! Fetches all stored items.
        //! \param collect A callback to fetch data into.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchAll(const CollectOhlcv& collect) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param from A beginning date and time.
        //! \param to An ending date and time.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchTickRange(const CollectOhlcv& collect, long long from, long long to) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param index The index of the first item in the range.
        //! \param count The number of items in the range.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchIndexRange(const CollectOhlcv& collect, hsize_t index, hsize_t count) const;

        //! Converts ticks to a nearest index.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndex(long long ticks, hsize_t& index, bool& match) const override;

        //! Converts a ticks range to an index range.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \param indexFrom The index of the first item in the range.
        //! \param indexTo The index of the last item in the range.
        //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
        //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndexRange(long long ticksFrom, long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const override;

        //! Deletes a range of indices from the dataset.
        //! \param indexFrom The first index of the range (inclusive).
        //! \param indexTo The last index of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteIndexRange(hsize_t indexFrom, hsize_t indexTo) const;

        //! Deletes a range of ticks from the dataset .
        //! \param ticksFrom The first ticks of the range (inclusive).
        //! \param ticksTo The last ticks of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteTickRange(long long ticksFrom, long long ticksTo) const;

        //! Deletes all ticks before a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeTick(long long ticks) const;

        //! Deletes all ticks before a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeIndex(hsize_t index) const;

        //! Deletes all ticks after a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterTick(long long ticks) const;

        //! Deletes all ticks after a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterIndex(hsize_t index) const;

        OhlcvData() = delete;
        OhlcvData(const OhlcvData&) = delete;
        OhlcvData(const OhlcvData&&) = delete;
        void operator=(const OhlcvData&) = delete;
        void operator=(const OhlcvData&&) = delete;
        ~OhlcvData() = default;

    private:
        friend class Instrument;

        //! Constructs a new instance.
        OhlcvData(const Instrument* instrument, const char* name, hid_t datasetId, hid_t nativeTypeId, hid_t standardType, bool readOnly);
    };

    typedef Collect<OhlcvPriceOnly> CollectOhlcvPriceOnly;

    //! Encapsulates the access to the instrument's h5::OhlcvPriceOnly data.
    class OhlcvPriceOnlyData final : public Data
    {
    public:
        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input An input data vector.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(std::vector<OhlcvPriceOnly>& input, bool verbose);

        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input A pointer to the input data array.
        //! \param inputCount The number of the data items in the \c input array.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(OhlcvPriceOnly* input, size_t inputCount, bool verbose);

        //! Adds an array of price-only ohlcv items to the data.
        //! \param input A pointer to an array of input price-only ohlcv items.
        //! \param inputCount A number of price-only ohlcv items in the array.
        //! \param duplicateTimeTicks The duplicate time tick policy.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool add(const OhlcvPriceOnly* input, hsize_t inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const;

        //! Fetches all stored items.
        //! \param collect A callback to fetch data into.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchAll(const CollectOhlcvPriceOnly& collect) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param from A beginning date and time.
        //! \param to An ending date and time.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchTickRange(const CollectOhlcvPriceOnly& collect, long long from, long long to) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param index The index of the first item in the range.
        //! \param count The number of items in the range.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchIndexRange(const CollectOhlcvPriceOnly& collect, hsize_t index, hsize_t count) const;

        //! Converts ticks to a nearest index.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndex(long long ticks, hsize_t& index, bool& match) const override;

        //! Converts a ticks range to an index range.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \param indexFrom The index of the first item in the range.
        //! \param indexTo The index of the last item in the range.
        //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
        //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndexRange(long long ticksFrom, long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const override;

        //! Deletes a range of indices from the dataset.
        //! \param indexFrom The first index of the range (inclusive).
        //! \param indexTo The last index of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteIndexRange(hsize_t indexFrom, hsize_t indexTo) const;

        //! Deletes a range of ticks from the dataset .
        //! \param ticksFrom The first ticks of the range (inclusive).
        //! \param ticksTo The last ticks of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteTickRange(long long ticksFrom, long long ticksTo) const;

        //! Deletes all ticks before a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeTick(long long ticks) const;

        //! Deletes all ticks before a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeIndex(hsize_t index) const;

        //! Deletes all ticks after a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterTick(long long ticks) const;

        //! Deletes all ticks after a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterIndex(hsize_t index) const;

        OhlcvPriceOnlyData() = delete;
        OhlcvPriceOnlyData(const OhlcvPriceOnlyData&) = delete;
        OhlcvPriceOnlyData(const OhlcvPriceOnlyData&&) = delete;
        void operator=(const OhlcvPriceOnlyData&) = delete;
        void operator=(const OhlcvPriceOnlyData&&) = delete;
        ~OhlcvPriceOnlyData() = default;

    private:
        friend class Instrument;

        //! Constructs a new instance.
        OhlcvPriceOnlyData(const Instrument* instrument, const char* name, hid_t datasetId, hid_t nativeTypeId, hid_t standardType, bool readOnly);
    };

    typedef Collect<Scalar> CollectScalar;

    //! Encapsulates the access to the instrument's h5::Scalar data.
    class ScalarData final : public Data
    {
    public:
        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input An input data vector.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(std::vector<Scalar>& input, bool verbose);

        //! Spreads the duplicate time ticks making one-tick increments.
        //! \param input A pointer to the input data array.
        //! \param inputCount The number of the data items in the \c input array.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating if the time ticks were spread.
        static bool spreadDuplicateTimeTicks(Scalar* input, size_t inputCount, bool verbose);

        //! Adds an array of scalar items to the data.
        //! \param input A pointer to an array of input scalar items.
        //! \param inputCount A number of scalar items in the array.
        //! \param duplicateTimeTicks The duplicate time tick policy.
        //! \param verbose Trace information messages.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool add(const Scalar* input, hsize_t inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const;

        //! Fetches all stored items.
        //! \param collect A callback to fetch data into.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchAll(const CollectScalar& collect) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param from A beginning date and time.
        //! \param to An ending date and time.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchTickRange(const CollectScalar& collect, long long from, long long to) const;

        //! Fetches a specified range of stored items.
        //! \param collect A callback to fetch data into.
        //! \param index The index of the first item in the range.
        //! \param count The number of items in the range.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool fetchIndexRange(const CollectScalar& collect, hsize_t index, hsize_t count) const;

        //! Converts ticks to a nearest index.
        //! \param ticks The ticks to convert.
        //! \param index The index of the ticks.
        //! \param match If the index matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndex(long long ticks, hsize_t& index, bool& match) const override;

        //! Converts a ticks range to an index range.
        //! \param ticksFrom The first ticks of the range.
        //! \param ticksTo The last ticks of the range.
        //! \param indexFrom The index of the first item in the range.
        //! \param indexTo The index of the last item in the range.
        //! \param matchFrom If the index of the first item in the range matches the ticks exactly or is a lower neighbor.
        //! \param matchTo If the index of the last item in the range matches the ticks exactly or is a lower neighbor.
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool ticksIndexRange(long long ticksFrom, long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const override;

        //! Deletes a range of indices from the dataset.
        //! \param indexFrom The first index of the range (inclusive).
        //! \param indexTo The last index of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteIndexRange(hsize_t indexFrom, hsize_t indexTo) const;

        //! Deletes a range of ticks from the dataset .
        //! \param ticksFrom The first ticks of the range (inclusive).
        //! \param ticksTo The last ticks of the range (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteTickRange(long long ticksFrom, long long ticksTo) const;

        //! Deletes all ticks before a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeTick(long long ticks) const;

        //! Deletes all ticks before a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllBeforeIndex(hsize_t index) const;

        //! Deletes all ticks after a specified ticks from the dataset.
        //! \param ticks The ticks (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterTick(long long ticks) const;

        //! Deletes all ticks after a specified index from the dataset.
        //! \param index The index (inclusive).
        //! \return A boolean indicating the success of the operation.
        [[nodiscard]] bool deleteAllAfterIndex(hsize_t index) const;

        ScalarData() = delete;
        ScalarData(const ScalarData&) = delete;
        ScalarData(const ScalarData&&) = delete;
        void operator=(const ScalarData&) = delete;
        void operator=(const ScalarData&&) = delete;
        ~ScalarData() = default;

    private:
        friend class Instrument;

        //! Constructs a new instance.
        ScalarData(const Instrument* instrument, const char* name, hid_t datasetId, hid_t nativeTypeId, hid_t standardType, bool readOnly);
    };
}
