namespace Mbcs.H5
{
    /// <summary>
    /// The data type enumeration.
    /// </summary>
    public enum DataType
    {
        /// <summary>The trade (time, price, volume).</summary>
        Trade = 0,
        /// <summary>The trade without volume (time, price).</summary>
        TradePriceOnly = 1,
        /// <summary>The quote (time, ask price, ask size, bid price, bid size).</summary>
        Quote = 2,
        /// <summary>The quote without size (time, ask price, bid price).</summary>
        QuotePriceOnly = 3,
        /// <summary>The ohlcv (time, opening price, highest price, lowest price, closing price, volume).</summary>
        Ohlcv = 4,
        /// <summary>The ohlcv without volume (time, opening price, highest price, lowest price, closing price).</summary>
        OhlcvPriceOnly = 5,
        /// <summary>The scalar (time, value).</summary>
        Scalar = 6,
        /// <summary>The adjusted ohlcv (time, opening price, highest price, lowest price, closing price, volume).</summary>
        OhlcvAdjusted = 7,
        /// <summary>The adjusted ohlcv without volume (time, opening price, highest price, lowest price, closing price).</summary>
        OhlcvAdjustedPriceOnly = 8
    }
}
