#include "mbc/testharness.h"
#include "h5capi.h"
#include "h5capiTestInfo.h"

namespace h5capi { namespace th = mbc::testharness;

void createEnumerationDatasets(const char* fileName);
void createEnumerationDatasetsInReverseOrder(const char* fileName);

TESTGROUP("h5 capi instrument")
{
    // ---------------------------------------------------------------------------
    // Close
    // ---------------------------------------------------------------------------

    TESTCASE("after close is called, instrument handle is unusable")
    {
        const char* fileName{ "capi_file_after_close_is_called_instrument_handle_is_unusable.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "foobar", true);
        h5InstrumentFlush(instrument);
        h5InstrumentClose(instrument);
        h5InterceptErrorStack(nullptr);
        const bool status = h5InstrumentEnumerateDatasets(instrument, nullptr, false);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(status) << ": enumerate datasets should return false";
    }

    // ---------------------------------------------------------------------------
    // Dataset enumeration
    // ---------------------------------------------------------------------------

    TESTCASE("enumerates datasets unsorted")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_instrument_enumerates_datasets_unsorted.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/d/e/f", false);

        const bool status = h5InstrumentEnumerateDatasets(instrument, [](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
            DataTimeFrame timeFrame, bool isValidName, const char* name, const char* path) noexcept
        {
            try
            {
                std::shared_ptr<TestDataInfo> di{ new TestDataInfo(name, gi, dataType, dataKind, ohlcvKind, scalarKind, tradeKind, quoteKind, timeFrame, isValidName) };
                di->path_ = path;
                dataEnumeratorVector.push_back(di);
            }
            catch (...)
            {
            }
        }, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                             << ": datasets[0] name should be s_m03";
        ASSERT_EQUAL("/s_m03", ds->path_.c_str())                            << ": datasets[0] path should be /s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                  << ": datasets[0] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)           << ": datasets[0] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)             << ": datasets[0] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)           << ": datasets[0] time frame should be minute 3";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                             << ": datasets[1] name should be T____";
        ASSERT_EQUAL("/T____", ds->path_.c_str())                            << ": datasets[1] path should be /T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)          << ": datasets[1] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_) << ": datasets[1] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates datasets sorted")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_instrument_enumerates_datasets_sorted.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/d/e/f", false);

        const bool status = h5InstrumentEnumerateDatasets(instrument, [](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
            DataTimeFrame timeFrame, bool isValidName, const char* name, const char* path) noexcept
        {
            try
            {
                std::shared_ptr<TestDataInfo> di{ new TestDataInfo(name, gi, dataType, dataKind, ohlcvKind, scalarKind, tradeKind, quoteKind, timeFrame, isValidName) };
                di->path_ = path;
                dataEnumeratorVector.push_back(di);
            }
            catch (...)
            {
            }
        }, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                             << ": datasets[1] name should be T____";
        ASSERT_EQUAL("/T____", ds->path_.c_str())                            << ": datasets[1] path should be /T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)          << ": datasets[1] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_) << ": datasets[1] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[1] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                             << ": datasets[0] name should be s_m03";
        ASSERT_EQUAL("/s_m03", ds->path_.c_str())                            << ": datasets[0] path should be /s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                  << ": datasets[0] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)           << ": datasets[0] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)             << ": datasets[0] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)           << ": datasets[0] time frame should be minute 3";
        dataEnumeratorVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Scalar
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing scalar data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_scalar_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5ScalarDataHandle data = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Hour3, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing scalar data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_scalar_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5ScalarDataHandle data = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing scalar data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_scalar_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5ScalarDataHandle data = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing scalar data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_can_open_non-existing_scalar_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        const H5ScalarDataHandle data = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        h5ScalarDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing scalar data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_scalar_data_when_opened_file_read-write.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5ScalarDataHandle data = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Hour3, true);
        h5ScalarDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadWrite(fileName, false);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5ScalarDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing scalar data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_scalar_data_when_opened_file_read-only.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5ScalarDataHandle data = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Hour3, true);
        h5ScalarDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5ScalarDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Trade
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing trade data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_trade_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5TradeDataHandle data = h5InstrumentOpenTradeData(instrument, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing trade data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_trade_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5TradeDataHandle data = h5InstrumentOpenTradeData(instrument, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing trade data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_trade_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5TradeDataHandle data = h5InstrumentOpenTradeData(instrument, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing trade data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_can_open_non-existing_trade_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        const H5TradeDataHandle data = h5InstrumentOpenTradeData(instrument, true);
        const bool isNullptr = data == nullptr;
        h5TradeDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing trade data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_trade_data_when_opened_file_read-write.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5TradeDataHandle data = h5InstrumentOpenTradeData(instrument, true);
        h5TradeDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadWrite(fileName, false);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenTradeData(instrument, false);
        const bool isNullptr = data == nullptr;
        h5TradeDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing trade data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_trade_data_when_opened_file_read-only.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5TradeDataHandle data = h5InstrumentOpenTradeData(instrument, true);
        h5TradeDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenTradeData(instrument, false);
        const bool isNullptr = data == nullptr;
        h5TradeDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Trade price-only
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing trade price-only data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_trade_price-only_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5TradePriceOnlyDataHandle data = h5InstrumentOpenTradePriceOnlyData(instrument, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing trade price-only data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_trade_price-only_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5TradePriceOnlyDataHandle data = h5InstrumentOpenTradePriceOnlyData(instrument, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing trade price-only data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_trade_price-only_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5TradePriceOnlyDataHandle data = h5InstrumentOpenTradePriceOnlyData(instrument, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing trade price-only data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_can_open_non-existing_trade_price-only_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        const H5TradePriceOnlyDataHandle data = h5InstrumentOpenTradePriceOnlyData(instrument, true);
        const bool isNullptr = data == nullptr;
        h5TradePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing trade price-only data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_trade_price-only_data_when_opened_file_read-write.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5TradePriceOnlyDataHandle data = h5InstrumentOpenTradePriceOnlyData(instrument, true);
        h5TradePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadWrite(fileName, false);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenTradePriceOnlyData(instrument, false);
        const bool isNullptr = data == nullptr;
        h5TradePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing trade price-only data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_trade_price-only_data_when_opened_file_read-only.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5TradePriceOnlyDataHandle data = h5InstrumentOpenTradePriceOnlyData(instrument, true);
        h5TradePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenTradePriceOnlyData(instrument, false);
        const bool isNullptr = data == nullptr;
        h5TradePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Ohlcv
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing ohlcv data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Ask, DataTimeFrame::Hour3, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Ask, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Ask, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing ohlcv data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_can_open_non-existing_ohlcv_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Ask, DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_ohlcv_data_when_opened_file_read-write.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Ask, DataTimeFrame::Hour3, true);
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadWrite(fileName, false);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Ask, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_ohlcv_data_when_opened_file_read-only.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5OhlcvDataHandle data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Ask, DataTimeFrame::Hour3, true);
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Ask, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Ohlcv price-only
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing ohlcv price-only data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_price-only_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvPriceOnlyData(instrument, OhlcvKind::Bid, DataTimeFrame::Hour3, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv price-only data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_price-only_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvPriceOnlyData(instrument, OhlcvKind::Bid, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv price-only data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_price-only_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvPriceOnlyData(instrument, OhlcvKind::Bid, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing ohlcv price-only data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_can_open_non-existing_ohlcv_price-only_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        const H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvPriceOnlyData(instrument, OhlcvKind::Bid, DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv price-only data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_ohlcv_price-only_data_when_opened_file_read-write.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvPriceOnlyData(instrument, OhlcvKind::Bid, DataTimeFrame::Hour3, true);
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadWrite(fileName, false);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenOhlcvPriceOnlyData(instrument, OhlcvKind::Bid, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv price-only data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_ohlcv_price-only_data_when_opened_file_read-only.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvPriceOnlyData(instrument, OhlcvKind::Bid, DataTimeFrame::Hour3, true);
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenOhlcvPriceOnlyData(instrument, OhlcvKind::Bid, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Ohlcv adjusted
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing ohlcv adjusted data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_adjusted_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv adjusted data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_adjusted_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv adjusted data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_adjusted_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing ohlcv adjusted data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_can_open_non-existing_ohlcv_adjusted_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        const H5OhlcvDataHandle data = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv adjusted data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_ohlcv_adjusted_data_when_opened_file_read-write.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5OhlcvDataHandle data = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, true);
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadWrite(fileName, false);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv adjusted data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_ohlcv_adjusted_data_when_opened_file_read-only.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5OhlcvDataHandle data = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, true);
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5OhlcvDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Ohlcv adjusted price-only
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing ohlcv adjusted price-only data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_adjusted_price-only_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvAdjustedPriceOnlyData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv adjusted price-only data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_adjusted_price-only_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvAdjustedPriceOnlyData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv adjusted price-only data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_ohlcv_adjusted_price-only_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvAdjustedPriceOnlyData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing ohlcv adjusted price-only data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_can_open_non-existing_ohlcv_adjusted_price-only_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        const H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvAdjustedPriceOnlyData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv adjusted price-only data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_ohlcv_adjusted_price-only_data_when_opened_file_read-write.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvAdjustedPriceOnlyData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, true);
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadWrite(fileName, false);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenOhlcvAdjustedPriceOnlyData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv adjusted price-only data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_ohlcv_adjusted_price-only_data_when_opened_file_read-only.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5OhlcvPriceOnlyDataHandle data = h5InstrumentOpenOhlcvAdjustedPriceOnlyData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, true);
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenOhlcvAdjustedPriceOnlyData(instrument, OhlcvKind::Default, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        h5OhlcvPriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Quote
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing quote data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_quote_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5QuoteDataHandle data = h5InstrumentOpenQuoteData(instrument, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing quote data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_quote_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5QuoteDataHandle data = h5InstrumentOpenQuoteData(instrument, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing quote data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_quote_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5QuoteDataHandle data = h5InstrumentOpenQuoteData(instrument, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing quote data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_can_open_non-existing_quote_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        const H5QuoteDataHandle data = h5InstrumentOpenQuoteData(instrument, true);
        const bool isNullptr = data == nullptr;
        h5QuoteDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing quote data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_quote_data_when_opened_file_read-write.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5QuoteDataHandle data = h5InstrumentOpenQuoteData(instrument, true);
        h5QuoteDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadWrite(fileName, false);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenQuoteData(instrument, false);
        const bool isNullptr = data == nullptr;
        h5QuoteDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing quote data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_quote_data_when_opened_file_read-only.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5QuoteDataHandle data = h5InstrumentOpenQuoteData(instrument, true);
        h5QuoteDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenQuoteData(instrument, false);
        const bool isNullptr = data == nullptr;
        h5QuoteDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Quote price-only
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing quote price-only data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_quote_price-only_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5QuotePriceOnlyDataHandle data = h5InstrumentOpenQuotePriceOnlyData(instrument, true);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing quote price-only data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_quote_price-only_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5QuotePriceOnlyDataHandle data = h5InstrumentOpenQuotePriceOnlyData(instrument, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing quote price-only data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_instrument_cannot_open_non-existing_quote_price-only_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        h5InterceptErrorStack(nullptr);
        const H5QuotePriceOnlyDataHandle data = h5InstrumentOpenQuotePriceOnlyData(instrument, false);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing quote price-only data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_instrument_can_open_non-existing_quote_price-only_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, false);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "/a", false);
        const H5QuotePriceOnlyDataHandle data = h5InstrumentOpenQuotePriceOnlyData(instrument, true);
        const bool isNullptr = data == nullptr;
        h5QuotePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing quote price-only data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_quote_price-only_data_when_opened_file_read-write.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5QuotePriceOnlyDataHandle data = h5InstrumentOpenQuotePriceOnlyData(instrument, true);
        h5QuotePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadWrite(fileName, false);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenQuotePriceOnlyData(instrument, false);
        const bool isNullptr = data == nullptr;
        h5QuotePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing quote price-only data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "capi_instrument_can_open_existing_quote_price-only_data_when_opened_file_read-only.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        H5InstrumentHandle instrument = h5FileOpenInstrument(file, instrumentName, true);
        H5QuotePriceOnlyDataHandle data = h5InstrumentOpenQuotePriceOnlyData(instrument, true);
        h5QuotePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        instrument = h5FileOpenInstrument(file, instrumentName, false);
        data = h5InstrumentOpenQuotePriceOnlyData(instrument, false);
        const bool isNullptr = data == nullptr;
        h5QuotePriceOnlyDataClose(data);
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

}
}
