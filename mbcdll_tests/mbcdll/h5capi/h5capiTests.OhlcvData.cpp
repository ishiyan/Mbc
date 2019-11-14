#include "mbc/testharness.h"
#include "h5capi.h"

namespace h5capi { namespace th = mbc::testharness;

TESTGROUP("h5 capi ohlcv data")
{
    static const char* dataInstrument{ "/foo/bar" };

    // ---------------------------------------------------------------------------
    // Maximum read buffer bytes
    // ---------------------------------------------------------------------------

    TESTCASE("maximum read buffer bytes has correct value initially")
    {
        const char* fileName{ "capi_ohlcv_data_maximum_read_buffer_bytes_has_correct_value_initially.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);
        const auto value = h5OhlcvDataGetMaximumReadBufferBytes(data);
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_EQUAL(1024UL * 1024UL, value) << ": maximum read buffer bytes should have correct value";
    }

    TESTCASE("maximum read buffer bytes has correct value when set")
    {
        const char* fileName{ "capi_ohlcv_data_maximum_read_buffer_bytes_has_correct_value_when_set.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);
        const auto valueSet = 12345UL;
        h5OhlcvDataSetMaximumReadBufferBytes(data, valueSet);
        const auto value = h5OhlcvDataGetMaximumReadBufferBytes(data);
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_EQUAL(valueSet, value) << ": maximum read buffer bytes should have correct value";
    }

    // ---------------------------------------------------------------------------
    // Close
    // ---------------------------------------------------------------------------

    TESTCASE("after close is called, data handle is unusable")
    {
        const char* fileName{ "capi_ohlcv_data_after_close_is_called_data_handle_is_unusable.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);
        h5OhlcvDataClose(data);
        h5InterceptErrorStack(nullptr);
        const bool status = h5OhlcvDataFetchAll(data, nullptr);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(status) << ": fetchAll should return false";
    }

    // ---------------------------------------------------------------------------
    // First ticks
    // ---------------------------------------------------------------------------

    TESTCASE("first ticks is zero when data is empty")
    {
        const char* fileName{ "capi_ohlcv_data_first_ticks_is_zero_when_data_is_empty.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);
        const auto ticks = h5OhlcvDataFirstTicks(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_EQUAL(0L, ticks) << ": first ticks should be zero";
    }

    TESTCASE("first ticks has correct value when data not empty")
    {
        const long ticksExpected = 42L;
        const char* fileName{ "capi_ohlcv_data_first_ticks_has_correct_value_when_data_not_empty.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);
        std::vector<Ohlcv> vec{ {ticksExpected, 1.1, 1.1, 1.1, 1.1, 11}, {ticksExpected + 1L, 1.2, 1.2, 1.2, 1.2, 12} };
        const bool status = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Fail, false);
        const auto ticks = h5OhlcvDataFirstTicks(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status)             << ": status should be true";
        ASSERT_EQUAL(ticksExpected, ticks) << ": first ticks should have expected value";
    }

    // ---------------------------------------------------------------------------
    // Last ticks
    // ---------------------------------------------------------------------------

    TESTCASE("last ticks is zero when data is empty")
    {
        const char* fileName{ "capi_ohlcv_data_last_ticks_is_zero_when_data_is_empty.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);
        const auto ticks = h5OhlcvDataLastTicks(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_EQUAL(0L, ticks) << ": last ticks should be zero";
    }

    TESTCASE("last ticks has correct value when data not empty")
    {
        const long ticksExpected = 42L;
        const char* fileName{ "capi_ohlcv_data_last_ticks_has_correct_value_when_data_not_empty.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);
        std::vector<Ohlcv> vec{ {ticksExpected - 1L, 1.1, 1.1, 1.1, 1.1, 11}, {ticksExpected, 1.2, 1.2, 1.2, 1.2, 12} };
        const bool status = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Fail, false);
        const auto ticks = h5OhlcvDataLastTicks(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status)             << ": status should be true";
        ASSERT_EQUAL(ticksExpected, ticks) << ": last ticks should have expected value";
    }

    // ---------------------------------------------------------------------------
    // Count
    // ---------------------------------------------------------------------------

    TESTCASE("count is zero when data is empty")
    {
        const char* fileName{ "capi_ohlcv_data_count_is_zero_when_data_is_empty.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);
        const auto count = h5OhlcvDataCount(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_EQUAL(0UL, count) << ": count should be zero";
    }

    TESTCASE("count has correct value when data not empty")
    {
        const char* fileName{ "capi_ohlcv_data_count_ticks_has_correct_value_when_data_not_empty.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);
        std::vector<Ohlcv> vec{ {42L, 1.1, 1.1, 1.1, 1.1, 11}, {43L, 1.2, 1.2, 1.2, 1.2, 12} };
        const bool status = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Fail, false);
        const auto count = h5OhlcvDataCount(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status)   << ": status should be true";
        ASSERT_EQUAL(2UL, count) << ": count should have expected value";
    }

    // ---------------------------------------------------------------------------
    // Spread duplicate time ticks
    // ---------------------------------------------------------------------------

    TESTCASE("spread duplicate time ticks does nothing when ticks are different")
    {
        const auto ticksFirst = 1001L, ticksLast = 1010L;
        std::vector<Ohlcv> vec{ {ticksFirst, 1.1, 1.1, 1.1, 1.1, 11}, {ticksLast, 1.2, 1.2, 1.2, 1.2, 12} };
        const bool status = h5OhlcvDataSpreadDuplicateTimeTicks(vec.data(), vec.size(), false);

        ASSERT_IS_FALSE(status)                 << ": status should be false";
        ASSERT_EQUAL(ticksFirst, vec[0].ticks_) << ": ticks[0] should have expected value";
        ASSERT_EQUAL(ticksLast, vec[1].ticks_)  << ": ticks[1] should have expected value";
    }

    TESTCASE("spread duplicate time ticks spreads equal ticks")
    {
        const auto ticks = 1000L;
        std::vector<Ohlcv> vec{ {ticks, 1.1, 1.1, 1.1, 1.1, 11}, {ticks, 1.2, 1.2, 1.2, 1.2, 12} };
        const bool status = h5OhlcvDataSpreadDuplicateTimeTicks(vec.data(), vec.size(), false);

        ASSERT_IS_TRUE(status)                  << ": status should be true";
        ASSERT_EQUAL(ticks, vec[0].ticks_)      << ": ticks[0] should have expected value";
        ASSERT_EQUAL(ticks + 1L, vec[1].ticks_) << ": ticks[1] should have expected value";
    }

    // ---------------------------------------------------------------------------
    // Add
    // ---------------------------------------------------------------------------

    TESTCASE("adds different ticks when duplicate policy is fail")
    {
        const char* fileName{ "capi_ohlcv_data_adds_different_ticks_when_duplicate_policy_is_fail.h5" };
        remove(fileName);
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vecFirst{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusFirst = h5OhlcvDataAdd(data, vecFirst.data(), vecFirst.size(), DuplicateTimeTicks::Fail, false);
        const auto countFirst = h5OhlcvDataCount(data);

        std::vector<Ohlcv> vecSecond{
            {1L, 2.1, 2.1, 2.1, 2.1, 21},
            {3L, 2.3, 2.3, 2.3, 2.3, 23},
            {5L, 2.5, 2.5, 2.5, 2.5, 25},
            {7L, 2.7, 2.7, 2.7, 2.7, 27}
        };
        const bool statusSecond = h5OhlcvDataAdd(data, vecSecond.data(), vecSecond.size(), DuplicateTimeTicks::Fail, false);
        const auto countSecond = h5OhlcvDataCount(data);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusFirst)    << ": status first should be true";
        ASSERT_IS_TRUE(statusSecond)   << ": status second should be true";
        ASSERT_EQUAL(3UL, countFirst)  << ": count first should have expected value";
        ASSERT_EQUAL(7UL, countSecond) << ": count second should have expected value";
    }

    TESTCASE("adds different ticks when duplicate policy is skip")
    {
        const char* fileName{ "capi_ohlcv_data_adds_different_ticks_when_duplicate_policy_is_skip.h5" };
        remove(fileName);
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vecFirst{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusFirst = h5OhlcvDataAdd(data, vecFirst.data(), vecFirst.size(), DuplicateTimeTicks::Skip, false);
        const auto countFirst = h5OhlcvDataCount(data);

        std::vector<Ohlcv> vecSecond{
            {1L, 2.1, 2.1, 2.1, 2.1, 21},
            {3L, 2.3, 2.3, 2.3, 2.3, 23},
            {5L, 2.5, 2.5, 2.5, 2.5, 25},
            {7L, 2.7, 2.7, 2.7, 2.7, 27}
        };
        const bool statusSecond = h5OhlcvDataAdd(data, vecSecond.data(), vecSecond.size(), DuplicateTimeTicks::Skip, false);
        const auto countSecond = h5OhlcvDataCount(data);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusFirst)    << ": status first should be true";
        ASSERT_IS_TRUE(statusSecond)   << ": status second should be true";
        ASSERT_EQUAL(3UL, countFirst)  << ": count first should have expected value";
        ASSERT_EQUAL(7UL, countSecond) << ": count second should have expected value";
    }

    TESTCASE("adds different ticks when duplicate policy is update")
    {
        const char* fileName{ "capi_ohlcv_data_adds_different_ticks_when_duplicate_policy_is_update.h5" };
        remove(fileName);
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vecFirst{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusFirst = h5OhlcvDataAdd(data, vecFirst.data(), vecFirst.size(), DuplicateTimeTicks::Update, false);
        const auto countFirst = h5OhlcvDataCount(data);

        std::vector<Ohlcv> vecSecond{
            {1L, 2.1, 2.1, 2.1, 2.1, 21},
            {3L, 2.3, 2.3, 2.3, 2.3, 23},
            {5L, 2.5, 2.5, 2.5, 2.5, 25},
            {7L, 2.7, 2.7, 2.7, 2.7, 27}
        };
        const bool statusSecond = h5OhlcvDataAdd(data, vecSecond.data(), vecSecond.size(), DuplicateTimeTicks::Update, false);
        const auto countSecond = h5OhlcvDataCount(data);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusFirst)    << ": status first should be true";
        ASSERT_IS_TRUE(statusSecond)   << ": status second should be true";
        ASSERT_EQUAL(3UL, countFirst)  << ": count first should have expected value";
        ASSERT_EQUAL(7UL, countSecond) << ": count second should have expected value";
    }

    TESTCASE("does not add all equal ticks when duplicate policy is fail")
    {
        const char* fileName{ "capi_ohlcv_data_does_not_add_all_equal_ticks_when_duplicate_policy_is_fail.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vecFirst{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusFirst = h5OhlcvDataAdd(data, vecFirst.data(), vecFirst.size(), DuplicateTimeTicks::Fail, false);
        const auto countFirst = h5OhlcvDataCount(data);

        std::vector<Ohlcv> vecSecond{
            {2L, 2.2, 2.2, 2.2, 2.2, 22},
            {4L, 2.4, 2.4, 2.4, 2.4, 24},
            {6L, 2.6, 2.6, 2.6, 2.6, 26}
        };
        const bool statusSecond = h5OhlcvDataAdd(data, vecSecond.data(), vecSecond.size(), DuplicateTimeTicks::Fail, false);
        const auto countSecond = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusFirst)                << ": status first should be true";
        ASSERT_IS_FALSE(statusSecond)              << ": status second should be false";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(3UL, countFirst)              << ": count first should have expected value";
        ASSERT_EQUAL(3UL, countSecond)             << ": count second should have expected value";
        ASSERT_EQUAL(3UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[2].open_) << ": data[2] should have expected value";
        dataVector.clear();
    }

    TESTCASE("does not add some equal ticks when duplicate policy is fail")
    {
        const char* fileName{ "capi_ohlcv_data_does_not_add_some_equal_ticks_when_duplicate_policy_is_fail.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vecFirst{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusFirst = h5OhlcvDataAdd(data, vecFirst.data(), vecFirst.size(), DuplicateTimeTicks::Fail, false);
        const auto countFirst = h5OhlcvDataCount(data);

        std::vector<Ohlcv> vecSecond{
            {1L, 2.1, 2.1, 2.1, 2.1, 21},
            {2L, 2.2, 2.2, 2.2, 2.2, 22},
            {4L, 2.4, 2.4, 2.4, 2.4, 24},
            {7L, 2.7, 2.7, 2.7, 2.7, 27}
        };
        const bool statusSecond = h5OhlcvDataAdd(data, vecSecond.data(), vecSecond.size(), DuplicateTimeTicks::Fail, false);
        const auto countSecond = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusFirst)                << ": status first should be true";
        ASSERT_IS_FALSE(statusSecond)              << ": status second should be false";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(3UL, countFirst)              << ": count first should have expected value";
        ASSERT_EQUAL(3UL, countSecond)             << ": count second should have expected value";
        ASSERT_EQUAL(3UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[2].open_) << ": data[2] should have expected value";
        dataVector.clear();
    }

    TESTCASE("add skips equal ticks when duplicate policy is skip")
    {
        const char* fileName{ "capi_ohlcv_data_add_skips_equal_ticks_when_duplicate_policy_is_skip.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vecFirst{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusFirst = h5OhlcvDataAdd(data, vecFirst.data(), vecFirst.size(), DuplicateTimeTicks::Skip, false);
        const auto countFirst = h5OhlcvDataCount(data);

        std::vector<Ohlcv> vecSecond{
            {1L, 2.1, 2.1, 2.1, 2.1, 21},
            {2L, 2.2, 2.2, 2.2, 2.2, 22},
            {4L, 2.4, 2.4, 2.4, 2.4, 24},
            {7L, 2.7, 2.7, 2.7, 2.7, 27}
        };
        const bool statusSecond = h5OhlcvDataAdd(data, vecSecond.data(), vecSecond.size(), DuplicateTimeTicks::Skip, false);
        const auto countSecond = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusFirst)                << ": status first should be true";
        ASSERT_IS_TRUE(statusSecond)               << ": status second should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(3UL, countFirst)              << ": count first should have expected value";
        ASSERT_EQUAL(5UL, countSecond)             << ": count second should have expected value";
        ASSERT_EQUAL(5UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(2.1 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[3].open_) << ": data[3] should have expected value";
        ASSERT_IS_TRUE(2.7 == dataVector[4].open_) << ": data[4] should have expected value";
        dataVector.clear();
    }

    TESTCASE("add updatess equal ticks when duplicate policy is update")
    {
        const char* fileName{ "capi_ohlcv_data_add_updates_equal_ticks_when_duplicate_policy_is_update.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vecFirst{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusFirst = h5OhlcvDataAdd(data, vecFirst.data(), vecFirst.size(), DuplicateTimeTicks::Update, false);
        const auto countFirst = h5OhlcvDataCount(data);

        std::vector<Ohlcv> vecSecond{
            {1L, 2.1, 2.1, 2.1, 2.1, 21},
            {2L, 2.2, 2.2, 2.2, 2.2, 22},
            {4L, 2.4, 2.4, 2.4, 2.4, 24},
            {7L, 2.7, 2.7, 2.7, 2.7, 27}
        };
        const bool statusSecond = h5OhlcvDataAdd(data, vecSecond.data(), vecSecond.size(), DuplicateTimeTicks::Update, false);
        const auto countSecond = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusFirst)                << ": status first should be true";
        ASSERT_IS_TRUE(statusSecond)               << ": status second should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(3UL, countFirst)              << ": count first should have expected value";
        ASSERT_EQUAL(5UL, countSecond)             << ": count second should have expected value";
        ASSERT_EQUAL(5UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(2.1 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(2.2 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(2.4 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[3].open_) << ": data[3] should have expected value";
        ASSERT_IS_TRUE(2.7 == dataVector[4].open_) << ": data[4] should have expected value";
        dataVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Fetch all
    // ---------------------------------------------------------------------------

    TESTCASE("fetch all fetches data correctly")
    {
        const char* fileName{ "capi_ohlcv_data_fetch_all_fetches_data_correctly.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto count = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(3UL, count)                   << ": count first should have expected value";
        ASSERT_EQUAL(3UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[2].open_) << ": data[2] should have expected value";
        dataVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Fetch tick range
    // ---------------------------------------------------------------------------

    TESTCASE("fetches tick range correctly when both ticks inside range")
    {
        const char* fileName{ "capi_ohlcv_data_fetches_tick_range_correctly_when_both_ticks_inside_range.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto countAdd = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchTickRange(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        }, 3L, 5L);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, countAdd)                << ": count add should have expected value";
        ASSERT_EQUAL(3UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[2].open_) << ": data[2] should have expected value";
        dataVector.clear();
    }

    TESTCASE("fetches tick range correctly when first tick outside range")
    {
        const char* fileName{ "capi_ohlcv_data_fetches_tick_range_correctly_when_first_tick_outside_range.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto countAdd = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchTickRange(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        }, 1L, 5L);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, countAdd)                << ": count add should have expected value";
        ASSERT_EQUAL(4UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[3].open_) << ": data[3] should have expected value";
        dataVector.clear();
    }

    TESTCASE("fetches tick range correctly when last tick outside range")
    {
        const char* fileName{ "capi_ohlcv_data_fetches_tick_range_correctly_when_last_tick_outside_range.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto countAdd = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchTickRange(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        }, 3L, 7L);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, countAdd)                << ": count add should have expected value";
        ASSERT_EQUAL(4UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[3].open_) << ": data[3] should have expected value";
        dataVector.clear();
    }

    TESTCASE("fetches tick range correctly when both ticks outside range")
    {
        const char* fileName{ "capi_ohlcv_data_fetches_tick_range_correctly_when_both_ticks_outside_range.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto countAdd = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchTickRange(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        }, 1L, 7L);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, countAdd)                << ": count add should have expected value";
        ASSERT_EQUAL(5UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[3].open_) << ": data[3] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[4].open_) << ": data[4] should have expected value";
        dataVector.clear();
    }

    TESTCASE("fetches tick range correctly when both ticks after last")
    {
        const char* fileName{ "capi_ohlcv_data_fetches_tick_range_correctly_when_both_ticks_after_last.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto countAdd = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchTickRange(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        }, 7L, 9L);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)            << ": status add should be true";
        ASSERT_IS_TRUE(statusFetch)          << ": status fetch should be true";
        ASSERT_EQUAL(3UL, countAdd)          << ": count add should have expected value";
        ASSERT_EQUAL(0UL, dataVector.size()) << ": data count should have expected value";
        dataVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Fetch index range
    // ---------------------------------------------------------------------------

    TESTCASE("fetches index range correctly when index inside and count inside range")
    {
        const char* fileName{ "capi_ohlcv_data_fetches_index_range_correctly_when_index_inside_and_count_inside_range.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto countAdd = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchIndexRange(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        }, 1UL, 3UL);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, countAdd)                << ": count add should have expected value";
        ASSERT_EQUAL(3UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[2].open_) << ": data[2] should have expected value";
        dataVector.clear();
    }

    TESTCASE("fetches index range correctly when index zero and count inside range")
    {
        const char* fileName{ "capi_ohlcv_data_fetches_index_range_correctly_when_index_zero_and_count_inside_range.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto countAdd = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchIndexRange(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        }, 0UL, 4UL);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, countAdd)                << ": count add should have expected value";
        ASSERT_EQUAL(4UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[3].open_) << ": data[3] should have expected value";
        dataVector.clear();
    }

    TESTCASE("fetches index range correctly when index inside and count outside range")
    {
        const char* fileName{ "capi_ohlcv_data_fetches_index_range_correctly_when_index_inside_and_count_outside_range.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto countAdd = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchIndexRange(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        }, 1UL, 5UL);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, countAdd)                << ": count add should have expected value";
        ASSERT_EQUAL(4UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[3].open_) << ": data[3] should have expected value";
        dataVector.clear();
    }

    TESTCASE("fetches index range correctly when index zero and count outside range")
    {
        const char* fileName{ "capi_ohlcv_data_fetches_index_range_correctly_when_index_zero_and_count_outside_range.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);
        const auto countAdd = h5OhlcvDataCount(data);

        const bool statusFetch = h5OhlcvDataFetchIndexRange(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        }, 0UL, 6UL);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, countAdd)                << ": count add should have expected value";
        ASSERT_EQUAL(5UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[3].open_) << ": data[3] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[4].open_) << ": data[4] should have expected value";
        dataVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Ticks index
    // ---------------------------------------------------------------------------

    TESTCASE("ticks index is correct when tick inside and matches")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_is_correct_when_tick_inside_and_matches.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long index;
        bool match;
        const bool statusTicksIndex = h5OhlcvDataTicksIndex(data, 3L, &index, &match);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)        << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndex) << ": status ticks index should be true";
        ASSERT_EQUAL(1UL, index)         << ": index should have expected value";
        ASSERT_IS_TRUE(match)            << ": match should have expected value";
    }

    TESTCASE("ticks index is correct when tick inside and not matches")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_is_correct_when_tick_inside_and_not_matches.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long index;
        bool match;
        const bool statusTicksIndex = h5OhlcvDataTicksIndex(data, 4L, &index, &match);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)        << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndex) << ": status ticks index should be true";
        ASSERT_EQUAL(1UL, index)         << ": index should have expected value";
        ASSERT_IS_FALSE(match)           << ": match should have expected value";
    }

    TESTCASE("ticks index is correct when tick before first")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_is_correct_when_tick_before_first.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long index;
        bool match;
        const bool statusTicksIndex = h5OhlcvDataTicksIndex(data, 1L, &index, &match);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)        << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndex) << ": status ticks index should be true";
        ASSERT_EQUAL(0UL, index)         << ": index should have expected value";
        ASSERT_IS_TRUE(match)            << ": match should have expected value";
    }

    TESTCASE("ticks index is correct when tick after last")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_is_correct_when_tick_after_last.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long index;
        bool match;
        const bool statusTicksIndex = h5OhlcvDataTicksIndex(data, 6L, &index, &match);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)        << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndex) << ": status ticks index should be true";
        ASSERT_EQUAL(2UL, index)         << ": index should have expected value";
        ASSERT_IS_TRUE(match)            << ": match should have expected value";
    }

    // ---------------------------------------------------------------------------
    // Ticks index range
    // ---------------------------------------------------------------------------

    TESTCASE("ticks index range is correct when both ticks inside and match")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_range_is_correct_when_both_ticks_inside_and_match.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long indexFrom, indexTo;
        bool matchFrom, matchTo;
        const bool statusTicksIndexRange = h5OhlcvDataTicksIndexRange(data, 3L, 5L, &indexFrom, &indexTo, &matchFrom, &matchTo);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)             << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndexRange) << ": status ticks index should be true";
        ASSERT_EQUAL(1UL, indexFrom)          << ": indexFrom should have expected value";
        ASSERT_EQUAL(2UL, indexTo)            << ": indexTo should have expected value";
        ASSERT_IS_TRUE(matchFrom)             << ": matchFrom should have expected value";
        ASSERT_IS_TRUE(matchTo)               << ": matchTo should have expected value";
    }

    TESTCASE("ticks index range is correct when both ticks inside and not match")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_range_is_correct_when_both_ticks_inside_and_not_match.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {1L, 1.1, 1.1, 1.1, 1.1, 11},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long indexFrom, indexTo;
        bool matchFrom, matchTo;
        const bool statusTicksIndexRange = h5OhlcvDataTicksIndexRange(data, 2L, 4L, &indexFrom, &indexTo, &matchFrom, &matchTo);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)             << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndexRange) << ": status ticks index should be true";
        ASSERT_EQUAL(0UL, indexFrom)          << ": indexFrom should have expected value";
        ASSERT_EQUAL(1UL, indexTo)            << ": indexTo should have expected value";
        ASSERT_IS_FALSE(matchFrom)            << ": matchFrom should have expected value";
        ASSERT_IS_FALSE(matchTo)              << ": matchTo should have expected value";
    }

    TESTCASE("ticks index range is correct when first tick outside")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_range_is_correct_when_first_tick_outside.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long indexFrom, indexTo;
        bool matchFrom, matchTo;
        const bool statusTicksIndexRange = h5OhlcvDataTicksIndexRange(data, 1L, 4L, &indexFrom, &indexTo, &matchFrom, &matchTo);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)             << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndexRange) << ": status ticks index should be true";
        ASSERT_EQUAL(0UL, indexFrom)          << ": indexFrom should have expected value";
        ASSERT_EQUAL(1UL, indexTo)            << ": indexTo should have expected value";
        ASSERT_IS_TRUE(matchFrom)             << ": matchFrom should have expected value";
        ASSERT_IS_FALSE(matchTo)              << ": matchTo should have expected value";
    }

    TESTCASE("ticks index range is correct when last tick outside")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_range_is_correct_when_last_tick_outside.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long indexFrom, indexTo;
        bool matchFrom, matchTo;
        const bool statusTicksIndexRange = h5OhlcvDataTicksIndexRange(data, 4L, 6L, &indexFrom, &indexTo, &matchFrom, &matchTo);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)             << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndexRange) << ": status ticks index should be true";
        ASSERT_EQUAL(1UL, indexFrom)          << ": indexFrom should have expected value";
        ASSERT_EQUAL(2UL, indexTo)            << ": indexTo should have expected value";
        ASSERT_IS_FALSE(matchFrom)            << ": matchFrom should have expected value";
        ASSERT_IS_TRUE(matchTo)               << ": matchTo should have expected value";
    }

    TESTCASE("ticks index range is correct when both between two ticks")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_range_is_correct_when_both_between_two_ticks.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long indexFrom, indexTo;
        bool matchFrom, matchTo;
        const bool statusTicksIndexRange = h5OhlcvDataTicksIndexRange(data, 3L, 4L, &indexFrom, &indexTo, &matchFrom, &matchTo);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)             << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndexRange) << ": status ticks index should be true";
        ASSERT_EQUAL(0UL, indexFrom)          << ": indexFrom should have expected value";
        ASSERT_EQUAL(0UL, indexTo)            << ": indexTo should have expected value";
        ASSERT_IS_FALSE(matchFrom)            << ": matchFrom should have expected value";
        ASSERT_IS_FALSE(matchTo)              << ": matchTo should have expected value";
    }

    TESTCASE("ticks index range is correct when both ticks outside")
    {
        const char* fileName{ "capi_ohlcv_data_ticks_index_range_is_correct_when_both_ticks_outside.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {5L, 1.5, 1.5, 1.5, 1.5, 15}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        unsigned long long indexFrom, indexTo;
        bool matchFrom, matchTo;
        const bool statusTicksIndexRange = h5OhlcvDataTicksIndexRange(data, 1L, 6L, &indexFrom, &indexTo, &matchFrom, &matchTo);

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)             << ": status add should be true";
        ASSERT_IS_TRUE(statusTicksIndexRange) << ": status ticks index should be true";
        ASSERT_EQUAL(0UL, indexFrom)          << ": indexFrom should have expected value";
        ASSERT_EQUAL(1UL, indexTo)            << ": indexTo should have expected value";
        ASSERT_IS_TRUE(matchFrom)             << ": matchFrom should have expected value";
        ASSERT_IS_TRUE(matchTo)               << ": matchTo should have expected value";
    }

    // ---------------------------------------------------------------------------
    // Delete index range
    // ---------------------------------------------------------------------------

    TESTCASE("delete index range is correct when both indices inside")
    {
        const char* fileName{ "capi_ohlcv_data_delete_index_range_is_correct_when_both_indices_inside.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteIndexRange = h5OhlcvDataDeleteIndexRange(data, 1UL, 3UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteIndexRange)     << ": status delete index range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[1].open_) << ": data[1] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete index range is correct when first zero last inside")
    {
        const char* fileName{ "capi_ohlcv_data_delete_index_range_is_correct_when_first_zero_last_inside.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteIndexRange = h5OhlcvDataDeleteIndexRange(data, 0UL, 3UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteIndexRange)     << ": status delete index range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(1UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[0].open_) << ": data[0] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete index range is correct when first inside last outside")
    {
        const char* fileName{ "capi_ohlcv_data_delete_index_range_is_correct_when_first_inside_last_outside.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteIndexRange = h5OhlcvDataDeleteIndexRange(data, 1UL, 5UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_FALSE(statusDeleteIndexRange)    << ": status delete index range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[3].open_) << ": data[3] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[4].open_) << ": data[4] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete index range is correct when first zero last outside")
    {
        const char* fileName{ "capi_ohlcv_data_delete_index_range_is_correct_when_first_zero_last_outside.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteIndexRange = h5OhlcvDataDeleteIndexRange(data, 0UL, 5UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_FALSE(statusDeleteIndexRange)    << ": status delete index range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(5UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[3].open_) << ": data[3] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[4].open_) << ": data[4] should have expected value";
        dataVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Delete tick range
    // ---------------------------------------------------------------------------

    TESTCASE("delete tick range is correct when both inside and match")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_both_inside_and_match.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 3L, 5L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange)      << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[1].open_) << ": data[1] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete tick range is correct when both inside and no match")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_both_inside_and_no_match.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 3L, 5L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange)      << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[1].open_) << ": data[1] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete tick range is correct when both the same inside and match")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_both_the_same_inside_and_match.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 3L, 3L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange)      << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(4UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[3].open_) << ": data[3] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete tick range is correct when both the same inside and no match")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_both_the_same_inside_and_no_match.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 3L, 3L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange)      << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(4UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.4 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[2].open_) << ": data[2] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[3].open_) << ": data[3] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete tick range is correct when first outside last inside and match")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_first_outside_last_inside_and_match.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 1L, 4L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange)      << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[1].open_) << ": data[1] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete tick range is correct when first outside last inside and no match")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_first_outside_last_inside_and_no_match.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 1L, 4L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange)      << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[1].open_) << ": data[1] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete tick range is correct when first inside last outside and match")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_first_inside_last_outside_and_match.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {4L, 1.4, 1.4, 1.4, 1.4, 14},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 4L, 7L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange)      << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[1].open_) << ": data[1] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete tick range is correct when first inside last outside and no match")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_first_inside_last_outside_and_no_match.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 4L, 7L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange)      << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.2 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[1].open_) << ": data[1] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete tick range is correct when first first last last")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_first_first_last_last.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 2L, 6L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)             << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange) << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)           << ": status fetch should be true";
        ASSERT_EQUAL(0UL, dataVector.size())  << ": data count should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete tick range is correct when both outside")
    {
        const char* fileName{ "capi_ohlcv_data_delete_tick_range_is_correct_when_both_outside.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {2L, 1.2, 1.2, 1.2, 1.2, 12},
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDeleteTickRange = h5OhlcvDataDeleteTickRange(data, 1L, 7L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)             << ": status add should be true";
        ASSERT_IS_TRUE(statusDeleteTickRange) << ": status delete tick range should be true";
        ASSERT_IS_TRUE(statusFetch)           << ": status fetch should be true";
        ASSERT_EQUAL(0UL, dataVector.size())  << ": data count should have expected value";
        dataVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Delete all before tick
    // ---------------------------------------------------------------------------

    TESTCASE("delete all before tick is correct when tick inside and matches")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_before_tick_is_correct_when_tick_inside_and_matches.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllBeforeTick(data, 5L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(1UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[0].open_) << ": data[0] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all before tick is correct when tick inside and no match")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_before_tick_is_correct_when_tick_inside_and_no_match.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllBeforeTick(data, 4L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[1].open_) << ": data[0] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all before tick is correct when tick before first")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_before_tick_is_correct_when_tick_before_first.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllBeforeTick(data, 1L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[1].open_) << ": data[0] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all before tick is correct when tick after last")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_before_tick_is_correct_when_tick_after_last.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllBeforeTick(data, 7L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)            << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)         << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)          << ": status fetch should be true";
        ASSERT_EQUAL(0UL, dataVector.size()) << ": data count should have expected value";
        dataVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Delete all before index
    // ---------------------------------------------------------------------------

    TESTCASE("delete all before index is correct when index inside")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_before_index_is_correct_when_index_inside.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllBeforeIndex(data, 1UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(1UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[0].open_) << ": data[0] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all before index is correct when index zero")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_before_index_is_correct_when_index_zero.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllBeforeIndex(data, 0UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(2UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[1].open_) << ": data[1] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all before index is correct when index outside")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_before_index_is_correct_when_index_outside.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllBeforeIndex(data, 3UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_FALSE(statusDelete)              << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(3UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[2].open_) << ": data[2] should have expected value";
        dataVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Delete all after tick
    // ---------------------------------------------------------------------------

    TESTCASE("delete all after tick is correct when tick inside and matches")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_after_tick_is_correct_when_tick_inside_and_matches.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllAfterTick(data, 5L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(1UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all after tick is correct when tick inside and no matches")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_after_tick_is_correct_when_tick_inside_and_no_matches.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllAfterTick(data, 4L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(1UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all after tick is correct when tick before first")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_after_tick_is_correct_when_tick_before_first.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllAfterTick(data, 1L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)            << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)         << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)          << ": status fetch should be true";
        ASSERT_EQUAL(0UL, dataVector.size()) << ": data count should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all after tick is correct when tick after last")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_after_tick_is_correct_when_tick_after_last.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllAfterTick(data, 7L);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(3UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[1].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[2].open_) << ": data[0] should have expected value";
        dataVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Delete all after index
    // ---------------------------------------------------------------------------

    TESTCASE("delete all after index is correct when index inside")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_after_index_is_correct_when_index_inside.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllAfterIndex(data, 1UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(1UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all after index is correct when index zero")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_after_index_is_correct_when_index_zero.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllAfterIndex(data, 0UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)            << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)         << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)          << ": status fetch should be true";
        ASSERT_EQUAL(0UL, dataVector.size()) << ": data count should have expected value";
        dataVector.clear();
    }

    TESTCASE("delete all after index is correct when index outside")
    {
        const char* fileName{ "capi_ohlcv_data_delete_all_after_index_is_correct_when_index_outside.h5" };
        remove(fileName);

        static std::vector<Ohlcv> dataVector;
        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, dataInstrument, true);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Minute3, true);

        std::vector<Ohlcv> vec{
            {3L, 1.3, 1.3, 1.3, 1.3, 13},
            {5L, 1.5, 1.5, 1.5, 1.5, 15},
            {6L, 1.6, 1.6, 1.6, 1.6, 16}
        };
        const bool statusAdd = h5OhlcvDataAdd(data, vec.data(), vec.size(), DuplicateTimeTicks::Update, false);

        const bool statusDelete = h5OhlcvDataDeleteAllAfterIndex(data, 3UL);

        const bool statusFetch = h5OhlcvDataFetchAll(data, [](long long ticks, double open, double high, double low, double close, double volume) noexcept
        {
            try
            {
                const Ohlcv item{ ticks, open, high, low, close, volume };
                dataVector.push_back(item);
            }
            catch (...)
            {
            }
        });

        h5OhlcvDataFlush(data);
        h5OhlcvDataClose(data);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusAdd)                  << ": status add should be true";
        ASSERT_IS_TRUE(statusDelete)               << ": status delete should be true";
        ASSERT_IS_TRUE(statusFetch)                << ": status fetch should be true";
        ASSERT_EQUAL(3UL, dataVector.size())       << ": data count should have expected value";
        ASSERT_IS_TRUE(1.3 == dataVector[0].open_) << ": data[0] should have expected value";
        ASSERT_IS_TRUE(1.5 == dataVector[1].open_) << ": data[1] should have expected value";
        ASSERT_IS_TRUE(1.6 == dataVector[2].open_) << ": data[2] should have expected value";
        dataVector.clear();
    }

}
}
