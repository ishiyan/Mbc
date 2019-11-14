namespace Mbcs.H5
{
    /// <summary>
    /// The data kind enumeration.
    /// </summary>
    public enum DataKind
    {
        /// <summary>The trade price/volume.</summary>
        TradeAperiodic = 0x0000000,
        /// <summary>The trade price (no volume).</summary>
        TradePriceOnlyAperiodic = 0x1000000,
        /// <summary>The ask/bid price/size quote.</summary>
        QuoteAperiodic = 0x2000000,
        /// <summary>The ask/bid price (no size) quote.</summary>
        QuotePriceOnlyAperiodic = 0x3000000,

        /// <summary>The open/high/low/close/volume without any specific time frame.</summary>
        OhlcvAperiodic = 0x4000000,
        /// <summary>The 1-second open/high/low/close/volume.</summary>
        OhlcvSecond1 = 0x4000101,
        /// <summary>The 2-second open/high/low/close/volume.</summary>
        OhlcvSecond2 = 0x4000102,
        /// <summary>The 3-second open/high/low/close/volume.</summary>
        OhlcvSecond3 = 0x4000103,
        /// <summary>The 4-second open/high/low/close/volume.</summary>
        OhlcvSecond4 = 0x4000104,
        /// <summary>The 5-second open/high/low/close/volume.</summary>
        OhlcvSecond5 = 0x4000105,
        /// <summary>The 6-second open/high/low/close/volume.</summary>
        OhlcvSecond6 = 0x4000106,
        /// <summary>The 10-second open/high/low/close/volume.</summary>
        OhlcvSecond10 = 0x400010a,
        /// <summary>The 12-second open/high/low/close/volume.</summary>
        OhlcvSecond12 = 0x400010c,
        /// <summary>The 15-second open/high/low/close/volume.</summary>
        OhlcvSecond15 = 0x400010f,
        /// <summary>The 20-second open/high/low/close/volume.</summary>
        OhlcvSecond20 = 0x4000114,
        /// <summary>The 30-second open/high/low/close/volume.</summary>
        OhlcvSecond30 = 0x400011e,
        /// <summary>The 1-minute open/high/low/close/volume.</summary>
        OhlcvMinute1 = 0x4000201,
        /// <summary>The 2-minute open/high/low/close/volume.</summary>
        OhlcvMinute2 = 0x4000202,
        /// <summary>The 3-minute open/high/low/close/volume.</summary>
        OhlcvMinute3 = 0x4000203,
        /// <summary>The 4-minute open/high/low/close/volume.</summary>
        OhlcvMinute4 = 0x4000204,
        /// <summary>The 5-minute open/high/low/close/volume.</summary>
        OhlcvMinute5 = 0x4000205,
        /// <summary>The 6-minute open/high/low/close/volume.</summary>
        OhlcvMinute6 = 0x4000206,
        /// <summary>The 10-minute open/high/low/close/volume.</summary>
        OhlcvMinute10 = 0x400020a,
        /// <summary>The 12-minute open/high/low/close/volume.</summary>
        OhlcvMinute12 = 0x400020c,
        /// <summary>The 15-minute open/high/low/close/volume.</summary>
        OhlcvMinute15 = 0x400020f,
        /// <summary>The 20-minute open/high/low/close/volume.</summary>
        OhlcvMinute20 = 0x4000214,
        /// <summary>The 30-minute open/high/low/close/volume.</summary>
        OhlcvMinute30 = 0x400021e,
        /// <summary>The 1-hour open/high/low/close/volume.</summary>
        OhlcvHour1 = 0x4000301,
        /// <summary>The 2-hour open/high/low/close/volume.</summary>
        OhlcvHour2 = 0x4000302,
        /// <summary>The 3-hour open/high/low/close/volume.</summary>
        OhlcvHour3 = 0x4000303,
        /// <summary>The 4-hour open/high/low/close/volume.</summary>
        OhlcvHour4 = 0x4000304,
        /// <summary>The 6-hour open/high/low/close/volume.</summary>
        OhlcvHour6 = 0x4000306,
        /// <summary>The 8-hour open/high/low/close/volume.</summary>
        OhlcvHour8 = 0x4000308,
        /// <summary>The 12-hour open/high/low/close/volume.</summary>
        OhlcvHour12 = 0x400030c,
        /// <summary>The 1-day open/high/low/close/volume.</summary>
        OhlcvDay1 = 0x4000401,
        /// <summary>The 1-week open/high/low/close/volume.</summary>
        OhlcvWeek1 = 0x4000501,
        /// <summary>The 2-week open/high/low/close/volume.</summary>
        OhlcvWeek2 = 0x4000502,
        /// <summary>The 3-week open/high/low/close/volume.</summary>
        OhlcvWeek3 = 0x4000503,
        /// <summary>The 1-month open/high/low/close/volume.</summary>
        OhlcvMonth1 = 0x4000601,
        /// <summary>The 2-month open/high/low/close/volume.</summary>
        OhlcvMonth2 = 0x4000602,
        /// <summary>The 3-month open/high/low/close/volume.</summary>
        OhlcvMonth3 = 0x4000603,
        /// <summary>The 4-month open/high/low/close/volume.</summary>
        OhlcvMonth4 = 0x4000604,
        /// <summary>The 6-month open/high/low/close/volume.</summary>
        OhlcvMonth6 = 0x4000606,
        /// <summary>The 1-year open/high/low/close/volume.</summary>
        OhlcvYear1 = 0x4000701,

        /// <summary>The open/high/low/close/volume ask price/size without any specific time frame.</summary>
        OhlcvAskAperiodic = 0x4010000,
        /// <summary>The 1-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond1 = 0x4010101,
        /// <summary>The 2-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond2 = 0x4010102,
        /// <summary>The 3-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond3 = 0x4010103,
        /// <summary>The 4-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond4 = 0x4010104,
        /// <summary>The 5-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond5 = 0x4010105,
        /// <summary>The 6-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond6 = 0x4010106,
        /// <summary>The 10-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond10 = 0x401010a,
        /// <summary>The 12-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond12 = 0x401010c,
        /// <summary>The 15-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond15 = 0x401010f,
        /// <summary>The 20-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond20 = 0x4010114,
        /// <summary>The 30-second open/high/low/close/volume ask price/size.</summary>
        OhlcvAskSecond30 = 0x401011e,
        /// <summary>The 1-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute1 = 0x4010201,
        /// <summary>The 2-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute2 = 0x4010202,
        /// <summary>The 3-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute3 = 0x4010203,
        /// <summary>The 4-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute4 = 0x4010204,
        /// <summary>The 5-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute5 = 0x4010205,
        /// <summary>The 6-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute6 = 0x4010206,
        /// <summary>The 10-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute10 = 0x401020a,
        /// <summary>The 12-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute12 = 0x401020c,
        /// <summary>The 15-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute15 = 0x401020f,
        /// <summary>The 20-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute20 = 0x4010214,
        /// <summary>The 30-minute open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMinute30 = 0x401021e,
        /// <summary>The 1-hour open/high/low/close/volume ask price/size.</summary>
        OhlcvAskHour1 = 0x4010301,
        /// <summary>The 2-hour open/high/low/close/volume ask price/size.</summary>
        OhlcvAskHour2 = 0x4010302,
        /// <summary>The 3-hour open/high/low/close/volume ask price/size.</summary>
        OhlcvAskHour3 = 0x4010303,
        /// <summary>The 4-hour open/high/low/close/volume ask price/size.</summary>
        OhlcvAskHour4 = 0x4010304,
        /// <summary>The 6-hour open/high/low/close/volume ask price/size.</summary>
        OhlcvAskHour6 = 0x4010306,
        /// <summary>The 8-hour open/high/low/close/volume ask price/size.</summary>
        OhlcvAskHour8 = 0x4010308,
        /// <summary>The 12-hour open/high/low/close/volume ask price/size.</summary>
        OhlcvAskHour12 = 0x401030c,
        /// <summary>The 1-day open/high/low/close/volume ask price/size.</summary>
        OhlcvAskDay1 = 0x4010401,
        /// <summary>The 1-week open/high/low/close/volume ask price/size.</summary>
        OhlcvAskWeek1 = 0x4010501,
        /// <summary>The 2-week open/high/low/close/volume ask price/size.</summary>
        OhlcvAskWeek2 = 0x4010502,
        /// <summary>The 3-week open/high/low/close/volume ask price/size.</summary>
        OhlcvAskWeek3 = 0x4010503,
        /// <summary>The 1-month open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMonth1 = 0x4010601,
        /// <summary>The 2-month open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMonth2 = 0x4010602,
        /// <summary>The 3-month open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMonth3 = 0x4010603,
        /// <summary>The 4-month open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMonth4 = 0x4010604,
        /// <summary>The 6-month open/high/low/close/volume ask price/size.</summary>
        OhlcvAskMonth6 = 0x4010606,
        /// <summary>The 1-year open/high/low/close/volume ask price/size.</summary>
        OhlcvAskYear1 = 0x4010701,

        /// <summary>The open/high/low/close/volume bid price/size without any specific time frame.</summary>
        OhlcvBidAperiodic = 0x4020000,
        /// <summary>The 1-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond1 = 0x4020101,
        /// <summary>The 2-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond2 = 0x4020102,
        /// <summary>The 3-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond3 = 0x4020103,
        /// <summary>The 4-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond4 = 0x4020104,
        /// <summary>The 5-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond5 = 0x4020105,
        /// <summary>The 6-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond6 = 0x4020106,
        /// <summary>The 10-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond10 = 0x402010a,
        /// <summary>The 12-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond12 = 0x402010c,
        /// <summary>The 15-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond15 = 0x402010f,
        /// <summary>The 20-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond20 = 0x4020114,
        /// <summary>The 30-second open/high/low/close/volume bid price/size.</summary>
        OhlcvBidSecond30 = 0x402011e,
        /// <summary>The 1-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute1 = 0x4020201,
        /// <summary>The 2-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute2 = 0x4020202,
        /// <summary>The 3-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute3 = 0x4020203,
        /// <summary>The 4-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute4 = 0x4020204,
        /// <summary>The 5-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute5 = 0x4020205,
        /// <summary>The 6-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute6 = 0x4020206,
        /// <summary>The 10-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute10 = 0x402020a,
        /// <summary>The 12-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute12 = 0x402020c,
        /// <summary>The 15-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute15 = 0x402020f,
        /// <summary>The 20-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute20 = 0x4020214,
        /// <summary>The 30-minute open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMinute30 = 0x402021e,
        /// <summary>The 1-hour open/high/low/close/volume bid price/size.</summary>
        OhlcvBidHour1 = 0x4020301,
        /// <summary>The 2-hour open/high/low/close/volume bid price/size.</summary>
        OhlcvBidHour2 = 0x4020302,
        /// <summary>The 3-hour open/high/low/close/volume bid price/size.</summary>
        OhlcvBidHour3 = 0x4020303,
        /// <summary>The 4-hour open/high/low/close/volume bid price/size.</summary>
        OhlcvBidHour4 = 0x4020304,
        /// <summary>The 6-hour open/high/low/close/volume bid price/size.</summary>
        OhlcvBidHour6 = 0x4020306,
        /// <summary>The 8-hour open/high/low/close/volume bid price/size.</summary>
        OhlcvBidHour8 = 0x4020308,
        /// <summary>The 12-hour open/high/low/close/volume bid price/size.</summary>
        OhlcvBidHour12 = 0x402030c,
        /// <summary>The 1-day open/high/low/close/volume bid price/size.</summary>
        OhlcvBidDay1 = 0x4020401,
        /// <summary>The 1-week open/high/low/close/volume bid price/size.</summary>
        OhlcvBidWeek1 = 0x4020501,
        /// <summary>The 2-week open/high/low/close/volume bid price/size.</summary>
        OhlcvBidWeek2 = 0x4020502,
        /// <summary>The 3-week open/high/low/close/volume bid price/size.</summary>
        OhlcvBidWeek3 = 0x4020503,
        /// <summary>The 1-month open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMonth1 = 0x4020601,
        /// <summary>The 2-month open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMonth2 = 0x4020602,
        /// <summary>The 3-month open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMonth3 = 0x4020603,
        /// <summary>The 4-month open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMonth4 = 0x4020604,
        /// <summary>The 6-month open/high/low/close/volume bid price/size.</summary>
        OhlcvBidMonth6 = 0x4020606,
        /// <summary>The 1-year open/high/low/close/volume bid price/size.</summary>
        OhlcvBidYear1 = 0x4020701,

        /// <summary>The open/high/low/close without any specific time frame.</summary>
        OhlcvPriceOnlyAperiodic = 0x5000000,
        /// <summary>The 1-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond1 = 0x5000101,
        /// <summary>The 2-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond2 = 0x5000102,
        /// <summary>The 3-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond3 = 0x5000103,
        /// <summary>The 4-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond4 = 0x5000104,
        /// <summary>The 5-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond5 = 0x5000105,
        /// <summary>The 6-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond6 = 0x5000106,
        /// <summary>The 10-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond10 = 0x500010a,
        /// <summary>The 12-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond12 = 0x500010c,
        /// <summary>The 15-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond15 = 0x500010f,
        /// <summary>The 20-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond20 = 0x5000114,
        /// <summary>The 30-second open/high/low/close.</summary>
        OhlcvPriceOnlySecond30 = 0x500011e,
        /// <summary>The 1-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute1 = 0x5000201,
        /// <summary>The 2-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute2 = 0x5000202,
        /// <summary>The 3-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute3 = 0x5000203,
        /// <summary>The 4-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute4 = 0x5000204,
        /// <summary>The 5-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute5 = 0x5000205,
        /// <summary>The 6-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute6 = 0x5000206,
        /// <summary>The 10-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute10 = 0x500020a,
        /// <summary>The 12-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute12 = 0x500020c,
        /// <summary>The 15-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute15 = 0x500020f,
        /// <summary>The 20-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute20 = 0x5000214,
        /// <summary>The 30-minute open/high/low/close.</summary>
        OhlcvPriceOnlyMinute30 = 0x500021e,
        /// <summary>The 1-hour open/high/low/close.</summary>
        OhlcvPriceOnlyHour1 = 0x5000301,
        /// <summary>The 2-hour open/high/low/close.</summary>
        OhlcvPriceOnlyHour2 = 0x5000302,
        /// <summary>The 3-hour open/high/low/close.</summary>
        OhlcvPriceOnlyHour3 = 0x5000303,
        /// <summary>The 4-hour open/high/low/close.</summary>
        OhlcvPriceOnlyHour4 = 0x5000304,
        /// <summary>The 6-hour open/high/low/close.</summary>
        OhlcvPriceOnlyHour6 = 0x5000306,
        /// <summary>The 8-hour open/high/low/close.</summary>
        OhlcvPriceOnlyHour8 = 0x5000308,
        /// <summary>The 12-hour open/high/low/close.</summary>
        OhlcvPriceOnlyHour12 = 0x500030c,
        /// <summary>The 1-day open/high/low/close.</summary>
        OhlcvPriceOnlyDay1 = 0x5000401,
        /// <summary>The 1-week open/high/low/close.</summary>
        OhlcvPriceOnlyWeek1 = 0x5000501,
        /// <summary>The 2-week open/high/low/close.</summary>
        OhlcvPriceOnlyWeek2 = 0x5000502,
        /// <summary>The 3-week open/high/low/close.</summary>
        OhlcvPriceOnlyWeek3 = 0x5000503,
        /// <summary>The 1-month open/high/low/close.</summary>
        OhlcvPriceOnlyMonth1 = 0x5000601,
        /// <summary>The 2-month open/high/low/close.</summary>
        OhlcvPriceOnlyMonth2 = 0x5000602,
        /// <summary>The 3-month open/high/low/close.</summary>
        OhlcvPriceOnlyMonth3 = 0x5000603,
        /// <summary>The 4-month open/high/low/close.</summary>
        OhlcvPriceOnlyMonth4 = 0x5000604,
        /// <summary>The 6-month open/high/low/close.</summary>
        OhlcvPriceOnlyMonth6 = 0x5000606,
        /// <summary>The 1-year open/high/low/close.</summary>
        OhlcvPriceOnlyYear1 = 0x5000701,

        /// <summary>The open/high/low/close ask price without any specific time frame.</summary>
        OhlcvPriceOnlyAskAperiodic = 0x5010000,
        /// <summary>The 1-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond1 = 0x5010101,
        /// <summary>The 2-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond2 = 0x5010102,
        /// <summary>The 3-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond3 = 0x5010103,
        /// <summary>The 4-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond4 = 0x5010104,
        /// <summary>The 5-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond5 = 0x5010105,
        /// <summary>The 6-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond6 = 0x5010106,
        /// <summary>The 10-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond10 = 0x501010a,
        /// <summary>The 12-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond12 = 0x501010c,
        /// <summary>The 15-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond15 = 0x501010f,
        /// <summary>The 20-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond20 = 0x5010114,
        /// <summary>The 30-second open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskSecond30 = 0x501011e,
        /// <summary>The 1-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute1 = 0x5010201,
        /// <summary>The 2-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute2 = 0x5010202,
        /// <summary>The 3-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute3 = 0x5010203,
        /// <summary>The 4-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute4 = 0x5010204,
        /// <summary>The 5-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute5 = 0x5010205,
        /// <summary>The 6-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute6 = 0x5010206,
        /// <summary>The 10-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute10 = 0x501020a,
        /// <summary>The 12-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute12 = 0x501020c,
        /// <summary>The 15-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute15 = 0x501020f,
        /// <summary>The 20-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute20 = 0x5010214,
        /// <summary>The 30-minute open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMinute30 = 0x501021e,
        /// <summary>The 1-hour open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskHour1 = 0x5010301,
        /// <summary>The 2-hour open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskHour2 = 0x5010302,
        /// <summary>The 3-hour open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskHour3 = 0x5010303,
        /// <summary>The 4-hour open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskHour4 = 0x5010304,
        /// <summary>The 6-hour open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskHour6 = 0x5010306,
        /// <summary>The 8-hour open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskHour8 = 0x5010308,
        /// <summary>The 12-hour open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskHour12 = 0x501030c,
        /// <summary>The 1-day open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskDay1 = 0x5010401,
        /// <summary>The 1-week open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskWeek1 = 0x5010501,
        /// <summary>The 2-week open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskWeek2 = 0x5010502,
        /// <summary>The 3-week open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskWeek3 = 0x5010503,
        /// <summary>The 1-month open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMonth1 = 0x5010601,
        /// <summary>The 2-month open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMonth2 = 0x5010602,
        /// <summary>The 3-month open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMonth3 = 0x5010603,
        /// <summary>The 4-month open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMonth4 = 0x5010604,
        /// <summary>The 6-month open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskMonth6 = 0x5010606,
        /// <summary>The 1-year open/high/low/close ask price.</summary>
        OhlcvPriceOnlyAskYear1 = 0x5010701,

        /// <summary>The open/high/low/close bid price without any specific time frame.</summary>
        OhlcvPriceOnlyBidAperiodic = 0x5020000,
        /// <summary>The 1-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond1 = 0x5020101,
        /// <summary>The 2-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond2 = 0x5020102,
        /// <summary>The 3-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond3 = 0x5020103,
        /// <summary>The 4-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond4 = 0x5020104,
        /// <summary>The 5-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond5 = 0x5020105,
        /// <summary>The 6-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond6 = 0x5020106,
        /// <summary>The 10-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond10 = 0x502010a,
        /// <summary>The 12-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond12 = 0x502010c,
        /// <summary>The 15-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond15 = 0x502010f,
        /// <summary>The 20-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond20 = 0x5020114,
        /// <summary>The 30-second open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidSecond30 = 0x502011e,
        /// <summary>The 1-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute1 = 0x5020201,
        /// <summary>The 2-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute2 = 0x5020202,
        /// <summary>The 3-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute3 = 0x5020203,
        /// <summary>The 4-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute4 = 0x5020204,
        /// <summary>The 5-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute5 = 0x5020205,
        /// <summary>The 6-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute6 = 0x5020206,
        /// <summary>The 10-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute10 = 0x502020a,
        /// <summary>The 12-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute12 = 0x502020c,
        /// <summary>The 15-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute15 = 0x502020f,
        /// <summary>The 20-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute20 = 0x5020214,
        /// <summary>The 30-minute open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMinute30 = 0x502021e,
        /// <summary>The 1-hour open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidHour1 = 0x5020301,
        /// <summary>The 2-hour open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidHour2 = 0x5020302,
        /// <summary>The 3-hour open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidHour3 = 0x5020303,
        /// <summary>The 4-hour open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidHour4 = 0x5020304,
        /// <summary>The 6-hour open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidHour6 = 0x5020306,
        /// <summary>The 8-hour open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidHour8 = 0x5020308,
        /// <summary>The 12-hour open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidHour12 = 0x502030c,
        /// <summary>The 1-day open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidDay1 = 0x5020401,
        /// <summary>The 1-week open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidWeek1 = 0x5020501,
        /// <summary>The 2-week open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidWeek2 = 0x5020502,
        /// <summary>The 3-week open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidWeek3 = 0x5020503,
        /// <summary>The 1-month open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMonth1 = 0x5020601,
        /// <summary>The 2-month open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMonth2 = 0x5020602,
        /// <summary>The 3-month open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMonth3 = 0x5020603,
        /// <summary>The 4-month open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMonth4 = 0x5020604,
        /// <summary>The 6-month open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidMonth6 = 0x5020606,
        /// <summary>The 1-year open/high/low/close bid price.</summary>
        OhlcvPriceOnlyBidYear1 = 0x5020701,

        /// <summary>The scalar without any specific time frame.</summary>
        ScalarAperiodic = 0x6000000,
        /// <summary>The 1-second scalar.</summary>
        ScalarSecond1 = 0x6000101,
        /// <summary>The 2-second scalar.</summary>
        ScalarSecond2 = 0x6000102,
        /// <summary>The 3-second scalar.</summary>
        ScalarSecond3 = 0x6000103,
        /// <summary>The 4-second scalar.</summary>
        ScalarSecond4 = 0x6000104,
        /// <summary>The 5-second scalar.</summary>
        ScalarSecond5 = 0x6000105,
        /// <summary>The 6-second scalar.</summary>
        ScalarSecond6 = 0x6000106,
        /// <summary>The 10-second scalar.</summary>
        ScalarSecond10 = 0x600010a,
        /// <summary>The 12-second scalar.</summary>
        ScalarSecond12 = 0x600010c,
        /// <summary>The 15-second scalar.</summary>
        ScalarSecond15 = 0x600010f,
        /// <summary>The 20-second scalar.</summary>
        ScalarSecond20 = 0x6000114,
        /// <summary>The 30-second scalar.</summary>
        ScalarSecond30 = 0x600011e,
        /// <summary>The 1-minute scalar.</summary>
        ScalarMinute1 = 0x6000201,
        /// <summary>The 2-minute scalar.</summary>
        ScalarMinute2 = 0x6000202,
        /// <summary>The 3-minute scalar.</summary>
        ScalarMinute3 = 0x6000203,
        /// <summary>The 4-minute scalar.</summary>
        ScalarMinute4 = 0x6000204,
        /// <summary>The 5-minute scalar.</summary>
        ScalarMinute5 = 0x6000205,
        /// <summary>The 6-minute scalar.</summary>
        ScalarMinute6 = 0x6000206,
        /// <summary>The 10-minute scalar.</summary>
        ScalarMinute10 = 0x600020a,
        /// <summary>The 12-minute scalar.</summary>
        ScalarMinute12 = 0x600020c,
        /// <summary>The 15-minute scalar.</summary>
        ScalarMinute15 = 0x600020f,
        /// <summary>The 20-minute scalar.</summary>
        ScalarMinute20 = 0x6000214,
        /// <summary>The 30-minute scalar.</summary>
        ScalarMinute30 = 0x600021e,
        /// <summary>The 1-hour scalar.</summary>
        ScalarHour1 = 0x6000301,
        /// <summary>The 2-hour scalar.</summary>
        ScalarHour2 = 0x6000302,
        /// <summary>The 3-hour scalar.</summary>
        ScalarHour3 = 0x6000303,
        /// <summary>The 4-hour scalar.</summary>
        ScalarHour4 = 0x6000304,
        /// <summary>The 6-hour scalar.</summary>
        ScalarHour6 = 0x6000306,
        /// <summary>The 8-hour scalar.</summary>
        ScalarHour8 = 0x6000308,
        /// <summary>The 12-hour scalar.</summary>
        ScalarHour12 = 0x600030c,
        /// <summary>The 1-day scalar.</summary>
        ScalarDay1 = 0x6000401,
        /// <summary>The 1-week scalar.</summary>
        ScalarWeek1 = 0x6000501,
        /// <summary>The 2-week scalar.</summary>
        ScalarWeek2 = 0x6000502,
        /// <summary>The 3-week scalar.</summary>
        ScalarWeek3 = 0x6000503,
        /// <summary>The 1-month scalar.</summary>
        ScalarMonth1 = 0x6000601,
        /// <summary>The 2-month scalar.</summary>
        ScalarMonth2 = 0x6000602,
        /// <summary>The 3-month scalar.</summary>
        ScalarMonth3 = 0x6000603,
        /// <summary>The 4-month scalar.</summary>
        ScalarMonth4 = 0x6000604,
        /// <summary>The 6-month scalar.</summary>
        ScalarMonth6 = 0x6000606,
        /// <summary>The 1-year scalar.</summary>
        ScalarYear1 = 0x6000701,

        /// <summary>The adjusted open/high/low/close/volume without any specific time frame.</summary>
        OhlcvAdjustedAperiodic = 0x7000000,
        /// <summary>The 1-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond1 = 0x7000101,
        /// <summary>The 2-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond2 = 0x7000102,
        /// <summary>The 3-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond3 = 0x7000103,
        /// <summary>The 4-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond4 = 0x7000104,
        /// <summary>The 5-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond5 = 0x7000105,
        /// <summary>The 6-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond6 = 0x7000106,
        /// <summary>The 10-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond10 = 0x700010a,
        /// <summary>The 12-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond12 = 0x700010c,
        /// <summary>The 15-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond15 = 0x700010f,
        /// <summary>The 20-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond20 = 0x7000114,
        /// <summary>The 30-second adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedSecond30 = 0x700011e,
        /// <summary>The 1-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute1 = 0x7000201,
        /// <summary>The 2-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute2 = 0x7000202,
        /// <summary>The 3-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute3 = 0x7000203,
        /// <summary>The 4-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute4 = 0x7000204,
        /// <summary>The 5-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute5 = 0x7000205,
        /// <summary>The 6-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute6 = 0x7000206,
        /// <summary>The 10-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute10 = 0x700020a,
        /// <summary>The 12-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute12 = 0x700020c,
        /// <summary>The 15-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute15 = 0x700020f,
        /// <summary>The 20-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute20 = 0x7000214,
        /// <summary>The 30-minute adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMinute30 = 0x700021e,
        /// <summary>The 1-hour adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedHour1 = 0x7000301,
        /// <summary>The 2-hour adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedHour2 = 0x7000302,
        /// <summary>The 3-hour adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedHour3 = 0x7000303,
        /// <summary>The 4-hour adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedHour4 = 0x7000304,
        /// <summary>The 6-hour adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedHour6 = 0x7000306,
        /// <summary>The 8-hour adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedHour8 = 0x7000308,
        /// <summary>The 12-hour adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedHour12 = 0x700030c,
        /// <summary>The 1-day adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedDay1 = 0x7000401,
        /// <summary>The 1-week adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedWeek1 = 0x7000501,
        /// <summary>The 2-week adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedWeek2 = 0x7000502,
        /// <summary>The 3-week adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedWeek3 = 0x7000503,
        /// <summary>The 1-month adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMonth1 = 0x7000601,
        /// <summary>The 2-month adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMonth2 = 0x7000602,
        /// <summary>The 3-month adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMonth3 = 0x7000603,
        /// <summary>The 4-month adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMonth4 = 0x7000604,
        /// <summary>The 6-month adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedMonth6 = 0x7000606,
        /// <summary>The 1-year adjusted open/high/low/close/volume.</summary>
        OhlcvAdjustedYear1 = 0x7000701,

        /// <summary>The adjusted open/high/low/close without any specific time frame.</summary>
        OhlcvAdjustedPriceOnlyAperiodic = 0x8000000,
        /// <summary>The 1-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond1 = 0x8000101,
        /// <summary>The 2-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond2 = 0x8000102,
        /// <summary>The 3-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond3 = 0x8000103,
        /// <summary>The 4-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond4 = 0x8000104,
        /// <summary>The 5-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond5 = 0x8000105,
        /// <summary>The 6-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond6 = 0x8000106,
        /// <summary>The 10-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond10 = 0x800010a,
        /// <summary>The 12-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond12 = 0x800010c,
        /// <summary>The 15-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond15 = 0x800010f,
        /// <summary>The 20-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond20 = 0x8000114,
        /// <summary>The 30-second adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlySecond30 = 0x800011e,
        /// <summary>The 1-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute1 = 0x8000201,
        /// <summary>The 2-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute2 = 0x8000202,
        /// <summary>The 3-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute3 = 0x8000203,
        /// <summary>The 4-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute4 = 0x8000204,
        /// <summary>The 5-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute5 = 0x8000205,
        /// <summary>The 6-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute6 = 0x8000206,
        /// <summary>The 10-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute10 = 0x800020a,
        /// <summary>The 12-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute12 = 0x800020c,
        /// <summary>The 15-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute15 = 0x800020f,
        /// <summary>The 20-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute20 = 0x8000214,
        /// <summary>The 30-minute adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMinute30 = 0x800021e,
        /// <summary>The 1-hour adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyHour1 = 0x8000301,
        /// <summary>The 2-hour adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyHour2 = 0x8000302,
        /// <summary>The 3-hour adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyHour3 = 0x8000303,
        /// <summary>The 4-hour adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyHour4 = 0x8000304,
        /// <summary>The 6-hour adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyHour6 = 0x8000306,
        /// <summary>The 8-hour adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyHour8 = 0x8000308,
        /// <summary>The 12-hour adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyHour12 = 0x800030c,
        /// <summary>The 1-day adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyDay1 = 0x8000401,
        /// <summary>The 1-week adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyWeek1 = 0x8000501,
        /// <summary>The 2-week adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyWeek2 = 0x8000502,
        /// <summary>The 3-week adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyWeek3 = 0x8000503,
        /// <summary>The 1-month adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMonth1 = 0x8000601,
        /// <summary>The 2-month adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMonth2 = 0x8000602,
        /// <summary>The 3-month adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMonth3 = 0x8000603,
        /// <summary>The 4-month adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMonth4 = 0x8000604,
        /// <summary>The 6-month adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyMonth6 = 0x8000606,
        /// <summary>The 1-year adjusted open/high/low/close.</summary>
        OhlcvAdjustedPriceOnlyYear1 = 0x8000701
    }
}
