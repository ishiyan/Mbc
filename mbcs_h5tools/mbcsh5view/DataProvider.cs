using System.Collections.Generic;
using Microsoft.Extensions.Logging;
using DataVirtualization;
using Mbcs.H5;

namespace Mbcsh5view
{
    public class TradeDataProvider : IItemsProvider<Trade>
    {
        private readonly TradeData data;
        private readonly int count;
        private readonly ILogger<TradeDataProvider> logger = App.LoggerFactory.CreateLogger<TradeDataProvider>();

        public int FetchCount() => count;

        public IList<Trade> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            logger.LogDebug($"FetchRange: start index = {startIndex}, page count = {pageCount}");
            var list = new List<Trade>(pageCount);
            data.FetchIndexRange(list, (ulong)startIndex, (ulong)pageCount);
            // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            overallCount = count;
            return list;
        }

        internal TradeDataProvider(TradeData data, long count)
        {
            this.data = data;
            this.count = (int)count;
            logger.LogDebug($"constructed: count = {count}");
        }
    }

    public class TradePriceOnlyDataProvider : IItemsProvider<TradePriceOnly>
    {
        private readonly TradePriceOnlyData data;
        private readonly int count;
        private readonly ILogger<TradePriceOnlyDataProvider> logger = App.LoggerFactory.CreateLogger<TradePriceOnlyDataProvider>();

        public int FetchCount() => count;

        public IList<TradePriceOnly> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            logger.LogDebug($"FetchRange: start index = {startIndex}, page count = {pageCount}");
            var list = new List<TradePriceOnly>(pageCount);
            data.FetchIndexRange(list, (ulong)startIndex, (ulong)pageCount);
            // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            overallCount = count;
            return list;
        }

        internal TradePriceOnlyDataProvider(TradePriceOnlyData data, long count)
        {
            this.data = data;
            this.count = (int)count;
            logger.LogDebug($"constructed: count = {count}");
        }
    }

    public class QuoteDataProvider : IItemsProvider<Quote>
    {
        private readonly QuoteData data;
        private readonly int count;
        private readonly ILogger<QuoteDataProvider> logger = App.LoggerFactory.CreateLogger<QuoteDataProvider>();

        public int FetchCount() => count;

        public IList<Quote> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            logger.LogDebug($"FetchRange: start index = {startIndex}, page count = {pageCount}");
            var list = new List<Quote>(pageCount);
            data.FetchIndexRange(list, (ulong)startIndex, (ulong)pageCount);
            // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            overallCount = count;
            return list;
        }

        internal QuoteDataProvider(QuoteData data, long count)
        {
            this.data = data;
            this.count = (int)count;
            logger.LogDebug($"constructed: count = {count}");
        }
    }

    public class QuotePriceOnlyDataProvider : IItemsProvider<QuotePriceOnly>
    {
        private readonly QuotePriceOnlyData data;
        private readonly int count;
        private readonly ILogger<QuotePriceOnlyDataProvider> logger = App.LoggerFactory.CreateLogger<QuotePriceOnlyDataProvider>();

        public int FetchCount() => count;

        public IList<QuotePriceOnly> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            logger.LogDebug($"FetchRange: start index = {startIndex}, page count = {pageCount}");
            var list = new List<QuotePriceOnly>(pageCount);
            data.FetchIndexRange(list, (ulong)startIndex, (ulong)pageCount);
            // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            overallCount = count;
            return list;
        }

        internal QuotePriceOnlyDataProvider(QuotePriceOnlyData data, long count)
        {
            this.data = data;
            this.count = (int)count;
            logger.LogDebug($"constructed: count = {count}");
        }
    }

    public class OhlcvDataProvider : IItemsProvider<Ohlcv>
    {
        private readonly OhlcvData data;
        private readonly int count;
        private readonly ILogger<OhlcvDataProvider> logger = App.LoggerFactory.CreateLogger<OhlcvDataProvider>();

        public int FetchCount() => count;

        public IList<Ohlcv> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            logger.LogDebug($"FetchRange: start index = {startIndex}, page count = {pageCount}");
            var list = new List<Ohlcv>(pageCount);
            data.FetchIndexRange(list, (ulong)startIndex, (ulong)pageCount);
            // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            overallCount = count;
            return list;
        }

        internal OhlcvDataProvider(OhlcvData data, long count)
        {
            this.data = data;
            this.count = (int)count;
            logger.LogDebug($"constructed: count = {count}");
        }
    }

    public class OhlcvPriceOnlyDataProvider : IItemsProvider<OhlcvPriceOnly>
    {
        private readonly OhlcvPriceOnlyData data;
        private readonly int count;
        private readonly ILogger<OhlcvPriceOnlyDataProvider> logger = App.LoggerFactory.CreateLogger<OhlcvPriceOnlyDataProvider>();

        public int FetchCount() => count;

        public IList<OhlcvPriceOnly> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            logger.LogDebug($"FetchRange: start index = {startIndex}, page count = {pageCount}");
            var list = new List<OhlcvPriceOnly>(pageCount);
            data.FetchIndexRange(list, (ulong)startIndex, (ulong)pageCount);
            // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            overallCount = count;
            return list;
        }

        internal OhlcvPriceOnlyDataProvider(OhlcvPriceOnlyData data, long count)
        {
            this.data = data;
            this.count = (int)count;
            logger.LogDebug($"constructed: count = {count}");
        }
    }

    public class ScalarDataProvider : IItemsProvider<Scalar>
    {
        private readonly ScalarData data;
        private readonly int count;
        private readonly ILogger<ScalarDataProvider> logger = App.LoggerFactory.CreateLogger<ScalarDataProvider>();

        public int FetchCount() => count;

        public IList<Scalar> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            logger.LogDebug($"FetchRange: start index = {startIndex}, page count = {pageCount}");
            var list = new List<Scalar>(pageCount);
            data.FetchIndexRange(list, (ulong)startIndex, (ulong)pageCount);
            // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            overallCount = count;
            return list;
        }

        internal ScalarDataProvider(ScalarData data, long count)
        {
            this.data = data;
            this.count = (int)count;
            logger.LogDebug($"constructed: count = {count}");
        }
    }
}
