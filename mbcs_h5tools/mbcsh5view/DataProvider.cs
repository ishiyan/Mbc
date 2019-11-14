using System.Collections.Generic;
using System.Globalization;
using DataVirtualization;
using Mbh5;

namespace Mbhv
{
    public class TradeDataProvider : IItemsProvider<Trade>
    {
        private readonly TradeData data;
        private readonly int count;

        public int FetchCount()
        {
            return count; // (int)data.Count;
        }

        public IList<Trade> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            var list = new List<Trade>(pageCount);
            data.Fetch(list, startIndex, pageCount);
            overallCount = count; // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            return list;
        }

        internal TradeDataProvider(TradeData data, long count)
        {
            this.data = data;
            this.count = (int)/*data.C*/count;
        }
    }

    public class TradePriceOnlyDataProvider : IItemsProvider<TradePriceOnly>
    {
        private readonly TradePriceOnlyData data;
        private readonly int count;

        public int FetchCount()
        {
            return count; // (int)data.Count;
        }

        public IList<TradePriceOnly> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            var list = new List<TradePriceOnly>(pageCount);
            data.Fetch(list, startIndex, pageCount);
            overallCount = count; // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            return list;
        }

        internal TradePriceOnlyDataProvider(TradePriceOnlyData data, long count)
        {
            this.data = data;
            this.count = (int)/*data.C*/count;
        }
    }

    public class QuoteDataProvider : IItemsProvider<Quote>
    {
        private readonly QuoteData data;
        private readonly int count;

        public int FetchCount()
        {
            return count; // (int)data.Count;
        }

        public IList<Quote> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            var list = new List<Quote>(pageCount);
            data.Fetch(list, startIndex, pageCount);
            overallCount = count; // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            return list;
        }

        internal QuoteDataProvider(QuoteData data, long count)
        {
            this.data = data;
            this.count = (int)/*data.C*/count;
        }
    }

    public class QuotePriceOnlyDataProvider : IItemsProvider<QuotePriceOnly>
    {
        private readonly QuotePriceOnlyData data;
        private readonly int count;

        public int FetchCount()
        {
            return count; // (int)data.Count;
        }

        private readonly long ticksPerSec = System.Diagnostics.Stopwatch.Frequency;
        private readonly System.Diagnostics.Stopwatch sw = System.Diagnostics.Stopwatch.StartNew();
        private long totalCount;

        public IList<QuotePriceOnly> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            var list = new List<QuotePriceOnly>(pageCount);
            sw.Start();
            data.Fetch(list, startIndex, pageCount);
            sw.Stop();
            totalCount += list.Count;
            System.Diagnostics.Trace.WriteLine(string.Format(CultureInfo.InvariantCulture, "Throughput: {0} Mbytes/sec, {1} ticks, {2} count", (double)(totalCount * QuotePriceOnly.SizeOf) * ticksPerSec / (sw.ElapsedTicks * 1024 * 1024), sw.ElapsedTicks, totalCount));
            overallCount = count; // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            return list;
        }

        internal QuotePriceOnlyDataProvider(QuotePriceOnlyData data, long count)
        {
            this.data = data;
            this.count = (int)/*data.C*/count;
            sw.Reset();
        }
    }

    [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1704:IdentifiersShouldBeSpelledCorrectly", MessageId = "Ohlcv")]
    public class OhlcvDataProvider : IItemsProvider<Ohlcv>
    {
        private readonly OhlcvData data;
        private readonly int count;

        public int FetchCount()
        {
            return count; // (int)data.Count;
        }

        public IList<Ohlcv> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            var list = new List<Ohlcv>(pageCount);
            data.Fetch(list, startIndex, pageCount);
            overallCount = count; // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            return list;
        }

        internal OhlcvDataProvider(OhlcvData data, long count)
        {
            this.data = data;
            this.count = (int)/*data.C*/count;
        }
    }

    [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1704:IdentifiersShouldBeSpelledCorrectly", MessageId = "Ohlcv")]
    public class OhlcvPriceOnlyDataProvider : IItemsProvider<OhlcvPriceOnly>
    {
        private readonly OhlcvPriceOnlyData data;
        private readonly int count;

        public int FetchCount()
        {
            return count; // (int)data.Count;
        }

        public IList<OhlcvPriceOnly> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            var list = new List<OhlcvPriceOnly>(pageCount);
            data.Fetch(list, startIndex, pageCount);
            overallCount = count; // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            return list;
        }

        internal OhlcvPriceOnlyDataProvider(OhlcvPriceOnlyData data, long count)
        {
            this.data = data;
            this.count = (int)/*data.C*/count;
        }
    }

    public class ScalarDataProvider : IItemsProvider<Scalar>
    {
        private readonly ScalarData data;
        private readonly int count;

        public int FetchCount()
        {
            return count; // (int)data.Count;
        }

        public IList<Scalar> FetchRange(int startIndex, int pageCount, out int overallCount)
        {
            var list = new List<Scalar>(pageCount);
            data.Fetch(list, startIndex, pageCount);
            overallCount = count; // In this case it's ok not to get the count again because we're assuming the data in the database is not changing.
            return list;
        }

        internal ScalarDataProvider(ScalarData data, long count)
        {
            this.data = data;
            this.count = (int)/*data.C*/count;
        }
    }
}
