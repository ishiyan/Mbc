using System;

namespace Mbcs.H5
{
    /// <summary>
    /// Extension methods to format the date and time of temporal entities.
    /// </summary>
    public static class TemporalEntity
    {
        /// <summary>
        /// The DateTime stamp with a fraction of second.
        /// </summary>
        /// <param name="dateTime">The date and time.</param>
        public static string DateTimeStampWithFractionOfSecond(this DateTime dateTime) => dateTime.ToString("yyyy/MM/dd HH:mm:ss.fffffff");

        /// <summary>
        /// The DateTime stamp with a fraction of second.
        /// </summary>
        /// <param name="ticks">The date and time ticks.</param>
        public static string DateTimeStampWithFractionOfSecond(this long ticks) => new DateTime(ticks).DateTimeStampWithFractionOfSecond();

        /// <summary>
        /// The DateTime stamp with a fraction of second.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string DateTimeStampWithFractionOfSecond(this Quote quote) => quote.Ticks.DateTimeStampWithFractionOfSecond();

        /// <summary>
        /// The DateTime stamp with a fraction of second.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string DateTimeStampWithFractionOfSecond(this QuotePriceOnly quote) => quote.Ticks.DateTimeStampWithFractionOfSecond();

        /// <summary>
        /// The DateTime stamp with a fraction of second.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string DateTimeStampWithFractionOfSecond(this Trade trade) => trade.Ticks.DateTimeStampWithFractionOfSecond();

        /// <summary>
        /// The DateTime stamp with a fraction of second.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string DateTimeStampWithFractionOfSecond(this TradePriceOnly trade) => trade.Ticks.DateTimeStampWithFractionOfSecond();

        /// <summary>
        /// The DateTime stamp with a fraction of second.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string DateTimeStampWithFractionOfSecond(this Ohlcv ohlcv) => ohlcv.Ticks.DateTimeStampWithFractionOfSecond();

        /// <summary>
        /// The DateTime stamp with a fraction of second.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string DateTimeStampWithFractionOfSecond(this OhlcvPriceOnly ohlcv) => ohlcv.Ticks.DateTimeStampWithFractionOfSecond();

        /// <summary>
        /// The DateTime stamp with a fraction of second.
        /// </summary>
        /// <param name="scalar">The scalar.</param>
        public static string DateTimeStampWithFractionOfSecond(this Scalar scalar) => scalar.Ticks.DateTimeStampWithFractionOfSecond();

        /// <summary>
        /// The DateTime stamp.
        /// </summary>
        /// <param name="dateTime">The date and time.</param>
        public static string DateTimeStamp(this DateTime dateTime) => dateTime.ToString("yyyy/MM/dd HH:mm:ss");

        /// <summary>
        /// The DateTime stamp.
        /// </summary>
        /// <param name="ticks">The date and time ticks.</param>
        public static string DateTimeStamp(this long ticks) => new DateTime(ticks).DateTimeStamp();

        /// <summary>
        /// The DateTime stamp.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string DateTimeStamp(this Quote quote) => quote.Ticks.DateTimeStamp();

        /// <summary>
        /// The DateTime stamp.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string DateTimeStamp(this QuotePriceOnly quote) => quote.Ticks.DateTimeStamp();

        /// <summary>
        /// The DateTime stamp.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string DateTimeStamp(this Trade trade) => trade.Ticks.DateTimeStamp();

        /// <summary>
        /// The DateTime stamp.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string DateTimeStamp(this TradePriceOnly trade) => trade.Ticks.DateTimeStamp();

        /// <summary>
        /// The DateTime stamp.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string DateTimeStamp(this Ohlcv ohlcv) => ohlcv.Ticks.DateTimeStamp();

        /// <summary>
        /// The DateTime stamp.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string DateTimeStamp(this OhlcvPriceOnly ohlcv) => ohlcv.Ticks.DateTimeStamp();

        /// <summary>
        /// The DateTime stamp.
        /// </summary>
        /// <param name="scalar">The scalar.</param>
        public static string DateTimeStamp(this Scalar scalar) => scalar.Ticks.DateTimeStamp();

        /// <summary>
        /// The date stamp.
        /// </summary>
        /// <param name="dateTime">The date and time.</param>
        public static string DateStamp(this DateTime dateTime) => dateTime.ToString("yyyy/MM/dd");

        /// <summary>
        /// The date stamp.
        /// </summary>
        /// <param name="ticks">The date and time ticks.</param>
        public static string DateStamp(this long ticks) => new DateTime(ticks).DateStamp();

        /// <summary>
        /// The date stamp.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string DateStamp(this Quote quote) => quote.Ticks.DateStamp();

        /// <summary>
        /// The date stamp.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string DateStamp(this QuotePriceOnly quote) => quote.Ticks.DateStamp();

        /// <summary>
        /// The date stamp.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string DateStamp(this Trade trade) => trade.Ticks.DateStamp();

        /// <summary>
        /// The date stamp.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string DateStamp(this TradePriceOnly trade) => trade.Ticks.DateStamp();

        /// <summary>
        /// The date stamp.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string DateStamp(this Ohlcv ohlcv) => ohlcv.Ticks.DateStamp();

        /// <summary>
        /// The date stamp.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string DateStamp(this OhlcvPriceOnly ohlcv) => ohlcv.Ticks.DateStamp();

        /// <summary>
        /// The date stamp.
        /// </summary>
        /// <param name="scalar">The scalar.</param>
        public static string DateStamp(this Scalar scalar) => scalar.Ticks.DateStamp();

        /// <summary>
        /// The time stamp.
        /// </summary>
        /// <param name="dateTime">The date and time.</param>
        public static string TimeStamp(this DateTime dateTime) => dateTime.ToString("HH:mm:ss");

        /// <summary>
        /// The time stamp.
        /// </summary>
        /// <param name="ticks">The date and time ticks.</param>
        public static string TimeStamp(this long ticks) => new DateTime(ticks).TimeStamp();

        /// <summary>
        /// The time stamp.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string TimeStamp(this Quote quote) => quote.Ticks.TimeStamp();

        /// <summary>
        /// The time stamp.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string TimeStamp(this QuotePriceOnly quote) => quote.Ticks.TimeStamp();

        /// <summary>
        /// The time stamp.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string TimeStamp(this Trade trade) => trade.Ticks.TimeStamp();

        /// <summary>
        /// The time stamp.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string TimeStamp(this TradePriceOnly trade) => trade.Ticks.TimeStamp();

        /// <summary>
        /// The time stamp.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string TimeStamp(this Ohlcv ohlcv) => ohlcv.Ticks.TimeStamp();

        /// <summary>
        /// The time stamp.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string TimeStamp(this OhlcvPriceOnly ohlcv) => ohlcv.Ticks.TimeStamp();

        /// <summary>
        /// The time stamp.
        /// </summary>
        /// <param name="scalar">The scalar.</param>
        public static string TimeStamp(this Scalar scalar) => scalar.Ticks.TimeStamp();

        /// <summary>
        /// The time stamp with a fraction of second.
        /// </summary>
        /// <param name="dateTime">The date and time.</param>
        public static string TimeStampWithFractionOfSecond(this DateTime dateTime) => dateTime.ToString("HH:mm:ss.fffffff");

        /// <summary>
        /// The time stamp with a fraction of second.
        /// </summary>
        /// <param name="ticks">The date and time ticks.</param>
        public static string TimeStampWithFractionOfSecond(this long ticks) => new DateTime(ticks).TimeStampWithFractionOfSecond();

        /// <summary>
        /// The time stamp with a fraction of second.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string TimeStampWithFractionOfSecond(this Quote quote) => quote.Ticks.TimeStampWithFractionOfSecond();

        /// <summary>
        /// The time stamp with a fraction of second.
        /// </summary>
        /// <param name="quote">The quote.</param>
        public static string TimeStampWithFractionOfSecond(this QuotePriceOnly quote) => quote.Ticks.TimeStampWithFractionOfSecond();

        /// <summary>
        /// The time stamp with a fraction of second.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string TimeStampWithFractionOfSecond(this Trade trade) => trade.Ticks.TimeStampWithFractionOfSecond();

        /// <summary>
        /// The time stamp with a fraction of second.
        /// </summary>
        /// <param name="trade">The trade.</param>
        public static string TimeStampWithFractionOfSecond(this TradePriceOnly trade) => trade.Ticks.TimeStampWithFractionOfSecond();

        /// <summary>
        /// The time stamp with a fraction of second.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string TimeStampWithFractionOfSecond(this Ohlcv ohlcv) => ohlcv.Ticks.TimeStampWithFractionOfSecond();

        /// <summary>
        /// The time stamp with a fraction of second.
        /// </summary>
        /// <param name="ohlcv">The ohlcv.</param>
        public static string TimeStampWithFractionOfSecond(this OhlcvPriceOnly ohlcv) => ohlcv.Ticks.TimeStampWithFractionOfSecond();

        /// <summary>
        /// The time stamp with a fraction of second.
        /// </summary>
        /// <param name="scalar">The scalar.</param>
        public static string TimeStampWithFractionOfSecond(this Scalar scalar) => scalar.Ticks.TimeStampWithFractionOfSecond();
    }
}
