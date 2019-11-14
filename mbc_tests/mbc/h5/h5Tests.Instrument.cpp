#include "mbc/testharness.h"
#include "mbc/h5.h"

namespace h5 { namespace m5 = mbc::h5;

using File = m5::File;
using Instrument = m5::Instrument;
using DataType = m5::DataType;
using DataKind = m5::DataKind;
using OhlcvKind = m5::OhlcvKind;
using ScalarKind = m5::ScalarKind;
using TradeKind = m5::TradeKind;
using DataTimeFrame = m5::DataTimeFrame;
using DataInfo = m5::DataInfo;
using ScalarData = m5::ScalarData;
using TradeData = m5::TradeData;
using TradePriceOnlyData = m5::TradePriceOnlyData;
using QuoteData = m5::QuoteData;
using QuotePriceOnlyData = m5::QuotePriceOnlyData;
using OhlcvData = m5::OhlcvData;
using OhlcvPriceOnlyData = m5::OhlcvPriceOnlyData;

void createEnumerationDatasets(const char* fileName);
void createEnumerationDatasetsInReverseOrder(const char* fileName);

TESTGROUP("h5 instrument")
{
    // ---------------------------------------------------------------------------
    // Close
    // ---------------------------------------------------------------------------

    TESTCASE("after close is called, instrument handle is unusable")
    {
        const char* fileName{ "file_after_close_is_called_instrument_handle_is_unusable.h5" };
        remove(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("foobar", true);
        instrument->flush();
        instrument->close();

        File::interceptErrorStack(nullptr);
        const bool status = instrument->enumerateDatasets(nullptr, false);
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(status) << ": enumerate datasets should return false";
    }

    // ---------------------------------------------------------------------------
    // Dataset enumeration
    // ---------------------------------------------------------------------------

    TESTCASE("enumerates datasets unsorted")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "instrument_enumerates_datasets_unsorted.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/d/e/f", false);

        const bool status = instrument->enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": datasets[0] name should be s_m03";
        ASSERT_EQUAL("/s_m03", ds->path())                                    << ": datasets[0] path should be /s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": datasets[0] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": datasets[0] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": datasets[0] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": datasets[0] time frame should be minute 3";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": datasets[1] name should be T____";
        ASSERT_EQUAL("/T____", ds->path())                                    << ": datasets[1] path should be /T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": datasets[1] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": datasets[1] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates datasets sorted")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "instrument_enumerates_datasets_sorted.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/d/e/f", false);

        const bool status = instrument->enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": datasets[1] name should be T____";
        ASSERT_EQUAL("/T____", ds->path())                                    << ": datasets[1] path should be /T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": datasets[1] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": datasets[1] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[1] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": datasets[0] name should be s_m03";
        ASSERT_EQUAL("/s_m03", ds->path())                                    << ": datasets[0] path should be /s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": datasets[0] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": datasets[0] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": datasets[0] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": datasets[0] time frame should be minute 3";
        dataEnumeratorVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Scalar
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing scalar data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_scalar_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<ScalarData> data = instrument->openScalarData(DataTimeFrame::Hour3, true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing scalar data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_scalar_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<ScalarData> data = instrument->openScalarData(DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing scalar data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_scalar_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<ScalarData> data = instrument->openScalarData(DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing scalar data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "instrument_can_open_non-existing_scalar_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        const std::shared_ptr<ScalarData> data = instrument->openScalarData(DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing scalar data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_scalar_data_when_opened_file_read-write.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<ScalarData> data = instrument->openScalarData(DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadWrite(fileName, false);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openScalarData(DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing scalar data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_scalar_data_when_opened_file_read-only.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<ScalarData> data = instrument->openScalarData(DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadOnly(fileName);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openScalarData(DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Trade
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing trade data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_trade_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<TradeData> data = instrument->openTradeData(true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing trade data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_trade_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<TradeData> data = instrument->openTradeData(false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing trade data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_trade_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<TradeData> data = instrument->openTradeData(false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing trade data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "instrument_can_open_non-existing_trade_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        const std::shared_ptr<TradeData> data = instrument->openTradeData(true);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing trade data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_trade_data_when_opened_file_read-write.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<TradeData> data = instrument->openTradeData(true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadWrite(fileName, false);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openTradeData(false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing trade data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_trade_data_when_opened_file_read-only.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<TradeData> data = instrument->openTradeData(true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadOnly(fileName);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openTradeData(false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Trade price-only
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing trade price-only data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_trade_price-only_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<TradePriceOnlyData> data = instrument->openTradePriceOnlyData(true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing trade price-only data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_trade_price-only_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<TradePriceOnlyData> data = instrument->openTradePriceOnlyData(false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing trade price-only data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_trade_price-only_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<TradePriceOnlyData> data = instrument->openTradePriceOnlyData(false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing trade price-only data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "instrument_can_open_non-existing_trade_price-only_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        const std::shared_ptr<TradePriceOnlyData> data = instrument->openTradePriceOnlyData(true);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing trade price-only data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_trade_price-only_data_when_opened_file_read-write.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<TradePriceOnlyData> data = instrument->openTradePriceOnlyData(true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadWrite(fileName, false);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openTradePriceOnlyData(false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing trade price-only data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_trade_price-only_data_when_opened_file_read-only.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<TradePriceOnlyData> data = instrument->openTradePriceOnlyData(true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadOnly(fileName);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openTradePriceOnlyData(false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Ohlcv
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing ohlcv data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvData> data = instrument->openOhlcvData(OhlcvKind::Ask, DataTimeFrame::Hour3, true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvData> data = instrument->openOhlcvData(OhlcvKind::Ask, DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvData> data = instrument->openOhlcvData(OhlcvKind::Ask, DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing ohlcv data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "instrument_can_open_non-existing_ohlcv_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        const std::shared_ptr<OhlcvData> data = instrument->openOhlcvData(OhlcvKind::Ask, DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_ohlcv_data_when_opened_file_read-write.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<OhlcvData> data = instrument->openOhlcvData(OhlcvKind::Ask, DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadWrite(fileName, false);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openOhlcvData(OhlcvKind::Ask, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_ohlcv_data_when_opened_file_read-only.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<OhlcvData> data = instrument->openOhlcvData(OhlcvKind::Ask, DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadOnly(fileName);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openOhlcvData(OhlcvKind::Ask, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Ohlcv price-only
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing ohlcv price-only data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_price-only_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvPriceOnlyData(OhlcvKind::Bid, DataTimeFrame::Hour3, true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv price-only data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_price-only_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvPriceOnlyData(OhlcvKind::Bid, DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv price-only data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_price-only_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvPriceOnlyData(OhlcvKind::Bid, DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing ohlcv price-only data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "instrument_can_open_non-existing_ohlcv_price-only_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        const std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvPriceOnlyData(OhlcvKind::Bid, DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv price-only data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_ohlcv_price-only_data_when_opened_file_read-write.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvPriceOnlyData(OhlcvKind::Bid, DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadWrite(fileName, false);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openOhlcvPriceOnlyData(OhlcvKind::Bid, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv price-only data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_ohlcv_price-only_data_when_opened_file_read-only.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvPriceOnlyData(OhlcvKind::Bid, DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadOnly(fileName);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openOhlcvPriceOnlyData(OhlcvKind::Bid, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Ohlcv adjusted
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing ohlcv adjusted data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_adjusted_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvData> data = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Hour3, true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv adjusted data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_adjusted_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvData> data = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv adjusted data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_adjusted_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvData> data = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing ohlcv adjusted data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "instrument_can_open_non-existing_ohlcv_adjusted_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        const std::shared_ptr<OhlcvData> data = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv adjusted data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_ohlcv_adjusted_data_when_opened_file_read-write.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<OhlcvData> data = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadWrite(fileName, false);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv adjusted data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_ohlcv_adjusted_data_when_opened_file_read-only.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<OhlcvData> data = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadOnly(fileName);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Ohlcv adjusted price-only
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing ohlcv adjusted price-only data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_adjusted_price-only_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvAdjustedPriceOnlyData(OhlcvKind::Default, DataTimeFrame::Hour3, true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv adjusted price-only data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_adjusted_price-only_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvAdjustedPriceOnlyData(OhlcvKind::Default, DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing ohlcv adjusted price-only data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_ohlcv_adjusted_price-only_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvAdjustedPriceOnlyData(OhlcvKind::Default, DataTimeFrame::Hour3, false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing ohlcv adjusted price-only data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "instrument_can_open_non-existing_ohlcv_adjusted_price-only_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        const std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvAdjustedPriceOnlyData(OhlcvKind::Default, DataTimeFrame::Hour3, true);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv adjusted price-only data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_ohlcv_adjusted_price-only_data_when_opened_file_read-write.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvAdjustedPriceOnlyData(OhlcvKind::Default, DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadWrite(fileName, false);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openOhlcvAdjustedPriceOnlyData(OhlcvKind::Default, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing ohlcv adjusted price-only data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_ohlcv_adjusted_price-only_data_when_opened_file_read-only.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<OhlcvPriceOnlyData> data = instrument->openOhlcvAdjustedPriceOnlyData(OhlcvKind::Default, DataTimeFrame::Hour3, true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadOnly(fileName);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openOhlcvAdjustedPriceOnlyData(OhlcvKind::Default, DataTimeFrame::Hour3, false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Quote
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing quote data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_quote_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<QuoteData> data = instrument->openQuoteData(true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing quote data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_quote_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<QuoteData> data = instrument->openQuoteData(false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing quote data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_quote_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<QuoteData> data = instrument->openQuoteData(false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing quote data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "instrument_can_open_non-existing_quote_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        const std::shared_ptr<QuoteData> data = instrument->openQuoteData(true);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing quote data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_quote_data_when_opened_file_read-write.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<QuoteData> data = instrument->openQuoteData(true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadWrite(fileName, false);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openQuoteData(false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing quote data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_quote_data_when_opened_file_read-only.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<QuoteData> data = instrument->openQuoteData(true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadOnly(fileName);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openQuoteData(false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Quote price-only
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing quote price-only data when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_quote_price-only_data_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<QuotePriceOnlyData> data = instrument->openQuotePriceOnlyData(true);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing quote price-only data when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_quote_price-only_data_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<QuotePriceOnlyData> data = instrument->openQuotePriceOnlyData(false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("cannot open non-existing quote price-only data when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "instrument_cannot_open_non-existing_quote_price-only_data_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<QuotePriceOnlyData> data = instrument->openQuotePriceOnlyData(false);
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_NULL(data) << ": data handle should be nullptr";
    }

    TESTCASE("can open non-existing quote price-only data when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "instrument_can_open_non-existing_quote_price-only_data_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, false);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("/a", false);
        const std::shared_ptr<QuotePriceOnlyData> data = instrument->openQuotePriceOnlyData(true);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing quote price-only data when opened file read-write")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_quote_price-only_data_when_opened_file_read-write.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<QuotePriceOnlyData> data = instrument->openQuotePriceOnlyData(true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadWrite(fileName, false);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openQuotePriceOnlyData(false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

    TESTCASE("can open existing quote price-only data when opened file read-only")
    {
        const char* instrumentName = "/foo/bar";
        const char* fileName{ "instrument_can_open_existing_quote_price-only_data_when_opened_file_read-only.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        std::shared_ptr<Instrument> instrument = file->openInstrument(instrumentName, true);
        std::shared_ptr<QuotePriceOnlyData> data = instrument->openQuotePriceOnlyData(true);
        data->close();
        instrument->close();
        file->close();

        file = File::openReadOnly(fileName);
        instrument = file->openInstrument(instrumentName, false);
        data = instrument->openQuotePriceOnlyData(false);
        const bool isNullptr = data == nullptr;
        data->close();
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isNullptr) << ": data handle should not be nullptr";
    }

}
}
