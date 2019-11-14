#include "mbc/testharness.h"
#include "mbc/h5.h"

namespace h5 { namespace m5 = mbc::h5;

using FileDriver = m5::FileDriver;
using File = m5::File;
using Instrument = m5::Instrument;
using DataType = m5::DataType;
using DataKind = m5::DataKind;
using OhlcvKind = m5::OhlcvKind;
using QuoteKind = m5::QuoteKind;
using ScalarKind = m5::ScalarKind;
using TradeKind = m5::TradeKind;
using DataTimeFrame = m5::DataTimeFrame;
using DataInfo = m5::DataInfo;
using GroupInfo = m5::GroupInfo;
using ScalarData = m5::ScalarData;
using TradeData = m5::TradeData;
using TradePriceOnlyData = m5::TradePriceOnlyData;
using QuoteData = m5::QuoteData;
using QuotePriceOnlyData = m5::QuotePriceOnlyData;
using OhlcvData = m5::OhlcvData;
using OhlcvPriceOnlyData = m5::OhlcvPriceOnlyData;
using Scalar = m5::Scalar;

void createEnumerationDatasets(const char* fileName);
void createEnumerationDatasetsInReverseOrder(const char* fileName);

TESTGROUP("h5 file")
{
    // ---------------------------------------------------------------------------
    // Intercept error stack
    // ---------------------------------------------------------------------------

    TESTCASE("interceptErrorStack logs Hdf5 errors")
    {
        const char* fileName{ "file_interceptErrorStack_logs_hdf5_errors.h5" };
        remove(fileName);
        static std::vector<std::string> logVector;

        File::interceptErrorStack([](const char* message, bool isError) noexcept
        {
            try
            {
                std::string msg{ isError ? "isError=true: " : "isError=false: " };
                msg += message;
                logVector.push_back(msg);
            }
            catch (...)
            {
            }
        });
        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        File::interceptErrorStack(nullptr);

        ASSERT_IS_TRUE(file == nullptr)       << ": file handle should be nullptr";
        ASSERT_IS_TRUE(logVector.size() == 1) << ": single error message should be logged";

        ASSERT_EQUAL("isError=true: h5: Error: Failed to open the file \"file_interceptErrorStack_logs_hdf5_errors.h5\" for the read-only access", logVector[0].c_str())
            << ": logged text should be expected";
        logVector.clear();
    }

    TESTCASE("interceptErrorStack logs Hdf5 infos")
    {
        Scalar array[2] { {1001L, 2.2}, {1001L, 3.3} };	
        static std::vector<std::string> logVector{};
        
        File::interceptErrorStack([](const char* message, bool isError) noexcept
        {
            try
            {
                std::string msg{ isError ? "isError=true: " : "isError=false: " };
                msg += message;
                logVector.push_back(msg);
            }
            catch (...)
            {
            }
        });
        const bool status = ScalarData::spreadDuplicateTimeTicks(array, 2, true);
        File::interceptErrorStack(nullptr);

        ASSERT_IS_TRUE(status)            << ": status should be true";
        ASSERT_EQUAL(2, logVector.size()) << ": single error message should be logged";

        ASSERT_EQUAL("isError=false: h5: Info: Spreading duplicate input time tick: start input[0] = ()", logVector[0].c_str())         << ": logged text [0] should be expected";
        ASSERT_EQUAL("isError=false: h5: Info: Spreading duplicate input time tick: start input[1] = () -> 1002", logVector[1].c_str()) << ": logged text [1] should be expected";
        logVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Close
    // ---------------------------------------------------------------------------

    TESTCASE("after close is called, file handle is unusable")
    {
        const char* fileName{ "file_after_close_is_called_file_handle_is_unusable.h5" };
        remove(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        file->close();
        File::interceptErrorStack(nullptr);
        const bool status = file->enumerateDatasets(nullptr, false);

        remove(fileName);

        ASSERT_IS_FALSE(status) << ": enumerate datasets should return false";
    }

    // ---------------------------------------------------------------------------
    // Open instrument
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing instrument when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "file_cannot_open_non-existing_instrument_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("foo/bar", false);
        const bool isInstrumentNullptr{ instrument == nullptr };
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(isInstrumentNullptr) << ": instrument should be nullptr";
    }

    TESTCASE("can open non-existing instrument when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "file_can_open_non-existing_instrument_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("foo/bar", true);
        const bool isInstrumentNullptr{ instrument == nullptr };
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(isInstrumentNullptr) << ": instrument should be a valid pointer";
    }

    TESTCASE("cannot open non-existing instrument when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "file_cannot_open_non-existing_instrument_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        file->close();

        file = File::openReadOnly(fileName);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("foo/bar", true);
        const bool isInstrumentNullptr{ instrument == nullptr };
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(isInstrumentNullptr) << ": instrument should be nullptr";
    }

    TESTCASE("cannot open non-existing instrument when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "file_cannot_open_non-existing_instrument_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);

        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        file->close();

        file = File::openReadOnly(fileName);
        File::interceptErrorStack(nullptr);
        const std::shared_ptr<Instrument> instrument = file->openInstrument("foo/bar", false);
        const bool isInstrumentNullptr{ instrument == nullptr };
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(isInstrumentNullptr) << ": instrument should be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Enumerate content
    // ---------------------------------------------------------------------------

    TESTCASE("enumerates content tree unsorted")
    {
        const char* fileName{ "file_enumerates_content_tree_unsorted.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<GroupInfo> root = file->contentTree(false);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(root != nullptr) << ": root should not be nullptr";
        
        ASSERT_EQUAL("/", root->name())                                       << ": root name should be /";
        ASSERT_EQUAL("", root->path())                                        << ": root path should be empty";
        ASSERT_NULL(root->parent())                                           << ": root parent should be nullptr";
        ASSERT_EQUAL(0, root->datasetsCount())                                << ": root dataset count should be 0";
        ASSERT_EQUAL(3, root->groupsCount())                                  << ": root group count should be 3";

        auto gr = root->groups()[0];
        ASSERT_EQUAL("a", gr->name())                                         << ": root[0] name should be a";
        ASSERT_EQUAL("/a", gr->path())                                        << ": root[0] path should be /a";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[0] parent should be root";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[0] group count should be 0";

        auto ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[0] datasets[0] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[0] datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": root[0] datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": root[0] datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": root[0] datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": root[0] datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": root[0] datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[0] datasets[1] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[0] datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": root[0] datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": root[0] datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": root[0] datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": root[0] datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": root[0] datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": root[0] datasets[1] time frame should be day 1";

        gr = root->groups()[1];
        ASSERT_EQUAL("b", gr->name())                                         << ": root[1] name should be b";
        ASSERT_EQUAL("/b", gr->path())                                        << ": root[1] path should be /b";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[1] parent should be root";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[1] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[1] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("c", gr->name())                                         << ": root[1,0] name should be c";
        ASSERT_EQUAL("/b/c", gr->path())                                      << ": root[1,0] path should be /b/c";
        ASSERT_IS_TRUE(root->groups()[1] == gr->parent())                     << ": root[1,0] parent should be root[1]";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[1,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[1,0] group count should be 0";

        ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[1,0] datasets[0] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[1,0] datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": root[1,0] datasets[0] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path())                                << ": root[1,0] datasets[0] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": root[1,0] datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": root[1,0] datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": root[1,0] datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[1,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[1,0] datasets[1] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[1,0] datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": root[1,0] datasets[1] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path())                                << ": root[1,0] datasets[1] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": root[1,0] datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": root[1,0] datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": root[1,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[1,0] datasets[1] time frame should be aperiodic";

        gr = root->groups()[2];
        ASSERT_EQUAL("d", gr->name())                                         << ": root[2] name should be d";
        ASSERT_EQUAL("/d", gr->path())                                        << ": root[2] path should be /d";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[2] parent should be root";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[2] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[2] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("e", gr->name())                                         << ": root[2,0] name should be e";
        ASSERT_EQUAL("/d/e", gr->path())                                      << ": root[2,0] path should be /d/e";
        ASSERT_IS_TRUE(root->groups()[2] == gr->parent())                     << ": root[2,0] parent should be root[2]";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[2,0] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[2,0] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("f", gr->name())                                         << ": root[2,0,0] name should be f";
        ASSERT_EQUAL("/d/e/f", gr->path())                                    << ": root[2,0,0] path should be /d/e/f";
        ASSERT_IS_TRUE(root->groups()[2]->groups()[0] == gr->parent())        << ": root[2,0,0] parent should be root[2,0]";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[2,0,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[2,0,0] group count should be 0";

        ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[2,0,0] datasets[0] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[2,0,0] datasets[0] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": root[2,0,0] datasets[0] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path())                              << ": root[2,0,0] datasets[0] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": root[2,0,0] datasets[0] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": root[2,0,0] datasets[0] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": root[2,0,0] datasets[0] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": root[2,0,0] datasets[0] time frame should be minute 3";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[2,0,0] datasets[1] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[2,0,0] datasets[1] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": root[2,0,0] datasets[1] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path())                              << ": root[2,0,0] datasets[1] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": root[2,0,0] datasets[1] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": root[2,0,0] datasets[1] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": root[2,0,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[2,0,0] datasets[1] time frame should be aperiodic";
    }

    TESTCASE("enumerates content tree sorted")
    {
        const char* fileName{ "file_enumerates_content_tree_sorted.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const std::shared_ptr<GroupInfo> root = file->contentTree(true);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(root != nullptr) << ": root should not be nullptr";
        
        ASSERT_EQUAL("/", root->name())                                       << ": root name should be /";
        ASSERT_EQUAL("", root->path())                                        << ": root path should be empty";
        ASSERT_NULL(root->parent())                                           << ": root parent should be nullptr";
        ASSERT_EQUAL(0, root->datasetsCount())                                << ": root dataset count should be 0";
        ASSERT_EQUAL(3, root->groupsCount())                                  << ": root group count should be 3";

        auto gr = root->groups()[0];
        ASSERT_EQUAL("a", gr->name())                                         << ": root[0] name should be a";
        ASSERT_EQUAL("/a", gr->path())                                        << ": root[0] path should be /a";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[0] parent should be root";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[0] group count should be 0";

        auto ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[0] datasets[0] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[0] datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": root[0] datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": root[0] datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": root[0] datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": root[0] datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": root[0] datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[0] datasets[1] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[0] datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": root[0] datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": root[0] datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": root[0] datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": root[0] datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": root[0] datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": root[0] datasets[1] time frame should be day 1";

        gr = root->groups()[1];
        ASSERT_EQUAL("b", gr->name())                                         << ": root[1] name should be b";
        ASSERT_EQUAL("/b", gr->path())                                        << ": root[1] path should be /b";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[1] parent should be root";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[1] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[1] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("c", gr->name())                                         << ": root[1,0] name should be c";
        ASSERT_EQUAL("/b/c", gr->path())                                      << ": root[1,0] path should be /b/c";
        ASSERT_IS_TRUE(root->groups()[1] == gr->parent())                     << ": root[1,0] parent should be root[1]";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[1,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[1,0] group count should be 0";

        ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[1,0] datasets[0] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[1,0] datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": root[1,0] datasets[0] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path())                                << ": root[1,0] datasets[0] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": root[1,0] datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": root[1,0] datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": root[1,0] datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[1,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[1,0] datasets[1] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[1,0] datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": root[1,0] datasets[1] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path())                                << ": root[1,0] datasets[1] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": root[1,0] datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": root[1,0] datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": root[1,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[1,0] datasets[1] time frame should be aperiodic";

        gr = root->groups()[2];
        ASSERT_EQUAL("d", gr->name())                                         << ": root[2] name should be d";
        ASSERT_EQUAL("/d", gr->path())                                        << ": root[2] path should be /d";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[2] parent should be root";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[2] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[2] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("e", gr->name())                                         << ": root[2,0] name should be e";
        ASSERT_EQUAL("/d/e", gr->path())                                      << ": root[2,0] path should be /d/e";
        ASSERT_IS_TRUE(root->groups()[2] == gr->parent())                     << ": root[2,0] parent should be root[2]";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[2,0] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[2,0] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("f", gr->name())                                         << ": root[2,0,0] name should be f";
        ASSERT_EQUAL("/d/e/f", gr->path())                                    << ": root[2,0,0] path should be /d/e/f";
        ASSERT_IS_TRUE(root->groups()[2]->groups()[0] == gr->parent())        << ": root[2,0,0] parent should be root[2,0]";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[2,0,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[2,0,0] group count should be 0";

        ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[2,0,0] datasets[0] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[2,0,0] datasets[0] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": root[2,0,0] datasets[0] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path())                              << ": root[2,0,0] datasets[0] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": root[2,0,0] datasets[0] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": root[2,0,0] datasets[0] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": root[2,0,0] datasets[0] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[2,0,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[2,0,0] datasets[1] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[2,0,0] datasets[1] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": root[2,0,0] datasets[1] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path())                              << ": root[2,0,0] datasets[1] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": root[2,0,0] datasets[1] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": root[2,0,0] datasets[1] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": root[2,0,0] datasets[1] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": root[2,0,0] datasets[1] time frame should be minute 3";
    }

    TESTCASE("enumerates content tree unsorted statically")
    {
        const char* fileName{ "file_enumerates_content_tree_unsorted_statically.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<GroupInfo> root = File::contentTree(fileName, FileDriver::Sec2, false);

        remove(fileName);

        ASSERT_IS_TRUE(root != nullptr) << ": root should not be nullptr";
        
        ASSERT_EQUAL("/", root->name())                                       << ": root name should be /";
        ASSERT_EQUAL("", root->path())                                        << ": root path should be empty";
        ASSERT_NULL(root->parent())                                           << ": root parent should be nullptr";
        ASSERT_EQUAL(0, root->datasetsCount())                                << ": root dataset count should be 0";
        ASSERT_EQUAL(3, root->groupsCount())                                  << ": root group count should be 3";

        auto gr = root->groups()[0];
        ASSERT_EQUAL("a", gr->name())                                         << ": root[0] name should be a";
        ASSERT_EQUAL("/a", gr->path())                                        << ": root[0] path should be /a";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[0] parent should be root";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[0] group count should be 0";

        auto ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[0] datasets[0] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[0] datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": root[0] datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": root[0] datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": root[0] datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": root[0] datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": root[0] datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[0] datasets[1] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[0] datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": root[0] datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": root[0] datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": root[0] datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": root[0] datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": root[0] datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": root[0] datasets[1] time frame should be day 1";

        gr = root->groups()[1];
        ASSERT_EQUAL("b", gr->name())                                         << ": root[1] name should be b";
        ASSERT_EQUAL("/b", gr->path())                                        << ": root[1] path should be /b";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[1] parent should be root";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[1] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[1] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("c", gr->name())                                         << ": root[1,0] name should be c";
        ASSERT_EQUAL("/b/c", gr->path())                                      << ": root[1,0] path should be /b/c";
        ASSERT_IS_TRUE(root->groups()[1] == gr->parent())                     << ": root[1,0] parent should be root[1]";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[1,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[1,0] group count should be 0";

        ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[1,0] datasets[0] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[1,0] datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": root[1,0] datasets[0] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path())                                << ": root[1,0] datasets[0] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": root[1,0] datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": root[1,0] datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": root[1,0] datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[1,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[1,0] datasets[1] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[1,0] datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": root[1,0] datasets[1] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path())                                << ": root[1,0] datasets[1] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": root[1,0] datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": root[1,0] datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": root[1,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[1,0] datasets[1] time frame should be aperiodic";

        gr = root->groups()[2];
        ASSERT_EQUAL("d", gr->name())                                         << ": root[2] name should be d";
        ASSERT_EQUAL("/d", gr->path())                                        << ": root[2] path should be /d";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[2] parent should be root";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[2] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[2] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("e", gr->name())                                         << ": root[2,0] name should be e";
        ASSERT_EQUAL("/d/e", gr->path())                                      << ": root[2,0] path should be /d/e";
        ASSERT_IS_TRUE(root->groups()[2] == gr->parent())                     << ": root[2,0] parent should be root[2]";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[2,0] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[2,0] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("f", gr->name())                                         << ": root[2,0,0] name should be f";
        ASSERT_EQUAL("/d/e/f", gr->path())                                    << ": root[2,0,0] path should be /d/e/f";
        ASSERT_IS_TRUE(root->groups()[2]->groups()[0] == gr->parent())        << ": root[2,0,0] parent should be root[2,0]";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[2,0,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[2,0,0] group count should be 0";

        ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[2,0,0] datasets[0] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[2,0,0] datasets[0] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": root[2,0,0] datasets[0] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path())                              << ": root[2,0,0] datasets[0] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": root[2,0,0] datasets[0] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": root[2,0,0] datasets[0] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": root[2,0,0] datasets[0] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": root[2,0,0] datasets[0] time frame should be minute 3";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[2,0,0] datasets[1] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[2,0,0] datasets[1] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": root[2,0,0] datasets[1] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path())                              << ": root[2,0,0] datasets[1] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": root[2,0,0] datasets[1] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": root[2,0,0] datasets[1] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": root[2,0,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[2,0,0] datasets[1] time frame should be aperiodic";
    }

    TESTCASE("enumerates content tree sorted statically")
    {
        const char* fileName{ "file_enumerates_content_tree_sorted_statically.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const std::shared_ptr<GroupInfo> root = File::contentTree(fileName, FileDriver::Sec2, true);

        remove(fileName);

        ASSERT_IS_TRUE(root != nullptr) << ": root should not be nullptr";
        
        ASSERT_EQUAL("/", root->name())                                       << ": root name should be /";
        ASSERT_EQUAL("", root->path())                                        << ": root path should be empty";
        ASSERT_NULL(root->parent())                                           << ": root parent should be nullptr";
        ASSERT_EQUAL(0, root->datasetsCount())                                << ": root dataset count should be 0";
        ASSERT_EQUAL(3, root->groupsCount())                                  << ": root group count should be 3";

        auto gr = root->groups()[0];
        ASSERT_EQUAL("a", gr->name())                                         << ": root[0] name should be a";
        ASSERT_EQUAL("/a", gr->path())                                        << ": root[0] path should be /a";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[0] parent should be root";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[0] group count should be 0";

        auto ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[0] datasets[0] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[0] datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": root[0] datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": root[0] datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": root[0] datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": root[0] datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": root[0] datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[0] datasets[1] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[0] datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": root[0] datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": root[0] datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": root[0] datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": root[0] datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": root[0] datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": root[0] datasets[1] time frame should be day 1";

        gr = root->groups()[1];
        ASSERT_EQUAL("b", gr->name())                                         << ": root[1] name should be b";
        ASSERT_EQUAL("/b", gr->path())                                        << ": root[1] path should be /b";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[1] parent should be root";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[1] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[1] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("c", gr->name())                                         << ": root[1,0] name should be c";
        ASSERT_EQUAL("/b/c", gr->path())                                      << ": root[1,0] path should be /b/c";
        ASSERT_IS_TRUE(root->groups()[1] == gr->parent())                     << ": root[1,0] parent should be root[1]";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[1,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[1,0] group count should be 0";

        ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[1,0] datasets[0] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[1,0] datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": root[1,0] datasets[0] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path())                                << ": root[1,0] datasets[0] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": root[1,0] datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": root[1,0] datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": root[1,0] datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[1,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[1,0] datasets[1] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[1,0] datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": root[1,0] datasets[1] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path())                                << ": root[1,0] datasets[1] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": root[1,0] datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": root[1,0] datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": root[1,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[1,0] datasets[1] time frame should be aperiodic";

        gr = root->groups()[2];
        ASSERT_EQUAL("d", gr->name())                                         << ": root[2] name should be d";
        ASSERT_EQUAL("/d", gr->path())                                        << ": root[2] path should be /d";
        ASSERT_IS_TRUE(root.get() == gr->parent())                            << ": root[2] parent should be root";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[2] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[2] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("e", gr->name())                                         << ": root[2,0] name should be e";
        ASSERT_EQUAL("/d/e", gr->path())                                      << ": root[2,0] path should be /d/e";
        ASSERT_IS_TRUE(root->groups()[2] == gr->parent())                     << ": root[2,0] parent should be root[2]";
        ASSERT_EQUAL(0, gr->datasetsCount())                                  << ": root[2,0] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groupsCount())                                    << ": root[2,0] group count should be 1";

        gr = gr->groups()[0];
        ASSERT_EQUAL("f", gr->name())                                         << ": root[2,0,0] name should be f";
        ASSERT_EQUAL("/d/e/f", gr->path())                                    << ": root[2,0,0] path should be /d/e/f";
        ASSERT_IS_TRUE(root->groups()[2]->groups()[0] == gr->parent())        << ": root[2,0,0] parent should be root[2,0]";
        ASSERT_EQUAL(2, gr->datasetsCount())                                  << ": root[2,0,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groupsCount())                                    << ": root[2,0,0] group count should be 0";

        ds = gr->datasets()[0];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[2,0,0] datasets[0] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[2,0,0] datasets[0] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": root[2,0,0] datasets[0] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path())                              << ": root[2,0,0] datasets[0] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": root[2,0,0] datasets[0] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": root[2,0,0] datasets[0] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": root[2,0,0] datasets[0] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": root[2,0,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets()[1];
        ASSERT_IS_TRUE(gr == ds->parent())                                    << ": root[2,0,0] datasets[1] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": root[2,0,0] datasets[1] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": root[2,0,0] datasets[1] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path())                              << ": root[2,0,0] datasets[1] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": root[2,0,0] datasets[1] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": root[2,0,0] datasets[1] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": root[2,0,0] datasets[1] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": root[2,0,0] datasets[1] time frame should be minute 3";
    }

    // ---------------------------------------------------------------------------
    // Enumerate datasets
    // ---------------------------------------------------------------------------

    TESTCASE("enumerates datasets unsorted")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_enumerates_datasets_unsorted.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const bool status = file->enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, false);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": datasets[1] time frame should be day 1";

        ds = &dataEnumeratorVector[2];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[2] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path())                                << ": datasets[2] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[2] time frame should be aperiodic";

        ds = &dataEnumeratorVector[3];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[3] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path())                                << ": datasets[3] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[3] time frame should be aperiodic";

        ds = &dataEnumeratorVector[4];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[4] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": datasets[4] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path())                              << ": datasets[4] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": datasets[4] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": datasets[4] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": datasets[4] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": datasets[4] time frame should be minute 3";

        ds = &dataEnumeratorVector[5];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[5] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": datasets[5] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path())                              << ": datasets[5] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": datasets[5] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": datasets[5] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[5] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[5] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates datasets sorted")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_enumerates_datasets_sorted.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const bool status = file->enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, true);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": datasets[1] time frame should be day 1";

        ds = &dataEnumeratorVector[2];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[2] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path())                                << ": datasets[2] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[2] time frame should be aperiodic";

        ds = &dataEnumeratorVector[3];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[3] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path())                                << ": datasets[3] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[3] time frame should be aperiodic";

        ds = &dataEnumeratorVector[4];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[4] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": datasets[4] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path())                              << ": datasets[4] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": datasets[4] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": datasets[4] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[4] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[4] time frame should be aperiodic";

        ds = &dataEnumeratorVector[5];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[5] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": datasets[5] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path())                              << ": datasets[5] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": datasets[5] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": datasets[5] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": datasets[5] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": datasets[5] time frame should be minute 3";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates datasets unsorted statically")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_enumerates_datasets_unsorted_statically.h5" };
        createEnumerationDatasets(fileName);

        const bool status = File::enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, fileName, FileDriver::Sec2, false);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": datasets[1] time frame should be day 1";

        ds = &dataEnumeratorVector[2];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[2] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path())                                << ": datasets[2] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[2] time frame should be aperiodic";

        ds = &dataEnumeratorVector[3];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[3] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path())                                << ": datasets[3] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[3] time frame should be aperiodic";

        ds = &dataEnumeratorVector[4];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[4] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": datasets[4] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path())                              << ": datasets[4] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": datasets[4] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": datasets[4] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": datasets[4] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": datasets[4] time frame should be minute 3";

        ds = &dataEnumeratorVector[5];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[5] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": datasets[5] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path())                              << ": datasets[5] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": datasets[5] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": datasets[5] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[5] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[5] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates datasets sorted statically")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_enumerates_datasets_sorted_statically.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const bool status = File::enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, fileName, FileDriver::Sec2, true);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": datasets[1] time frame should be day 1";

        ds = &dataEnumeratorVector[2];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[2] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path())                                << ": datasets[2] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[2] time frame should be aperiodic";

        ds = &dataEnumeratorVector[3];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[3] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path())                                << ": datasets[3] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[3] time frame should be aperiodic";

        ds = &dataEnumeratorVector[4];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[4] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": datasets[4] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path())                              << ": datasets[4] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": datasets[4] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": datasets[4] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[4] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[4] time frame should be aperiodic";

        ds = &dataEnumeratorVector[5];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[5] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": datasets[5] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path())                              << ": datasets[5] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": datasets[5] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": datasets[5] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": datasets[5] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": datasets[5] time frame should be minute 3";
        dataEnumeratorVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Enumerate instrument datasets
    // ---------------------------------------------------------------------------

    TESTCASE("enumerates instrument datasets unsorted")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_enumerates_instrument_datasets_unsorted.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const bool status = file->enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, "/b/c", false);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                       << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                             << ": datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                             << ": datasets[0] name should be q____";
        ASSERT_EQUAL("/q____", ds->path())                            << ": datasets[0] path should be /q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())           << ": datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())  << ": datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())       << ": datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame()) << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                       << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                             << ": datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                             << ": datasets[1] name should be t____";
        ASSERT_EQUAL("/t____", ds->path())                            << ": datasets[1] path should be /t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())           << ": datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())  << ": datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())       << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame()) << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates instrument datasets sorted")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_enumerates_instrument_datasets_sorted.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const bool status = file->enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, "/b/c", true);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                       << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                             << ": datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                             << ": datasets[0] name should be q____";
        ASSERT_EQUAL("/q____", ds->path())                            << ": datasets[0] path should be /q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())           << ": datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())  << ": datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())       << ": datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame()) << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                       << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                             << ": datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                             << ": datasets[1] name should be t____";
        ASSERT_EQUAL("/t____", ds->path())                            << ": datasets[1] path should be /t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())           << ": datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())  << ": datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())       << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame()) << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates instrument datasets unsorted statically")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_enumerates_instrument_datasets_unsorted_statically.h5" };
        createEnumerationDatasets(fileName);

        const bool status = File::enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, fileName, FileDriver::Sec2, "/b/c", false);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                       << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                             << ": datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                             << ": datasets[0] name should be q____";
        ASSERT_EQUAL("/q____", ds->path())                            << ": datasets[0] path should be /q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())           << ": datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())  << ": datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())       << ": datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame()) << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                       << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                             << ": datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                             << ": datasets[1] name should be t____";
        ASSERT_EQUAL("/t____", ds->path())                            << ": datasets[1] path should be /t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())           << ": datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())  << ": datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())       << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame()) << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates instrument datasets sorted statically")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_enumerates_instrument_datasets_sorted_statically.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const bool status = File::enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, fileName, FileDriver::Sec2, "/b/c", true);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                       << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                             << ": datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                             << ": datasets[0] name should be q____";
        ASSERT_EQUAL("/q____", ds->path())                            << ": datasets[0] path should be /q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())           << ": datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())  << ": datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())       << ": datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame()) << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                       << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                             << ": datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                             << ": datasets[1] name should be t____";
        ASSERT_EQUAL("/t____", ds->path())                            << ": datasets[1] path should be /t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())           << ": datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())  << ": datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())       << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame()) << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Delete object
    // ---------------------------------------------------------------------------

    TESTCASE("when file opened read-write, after deleting existing object datasets enumerated correctly")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_when_file_opened_read-write_after_deleting_existing_object_datasets_enumerated_correctly.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusDeleteBc = file->deleteObject("/b/c", true);
        const bool statusDeleteD = file->deleteObject("/d", true);
        const bool statusDeleteA = file->deleteObject("/a/j_d01", true);
        const bool statusEnumeration = file->enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, false);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(statusDeleteBc)                                 << ": statusDeleteBc should be true";
        ASSERT_IS_TRUE(statusDeleteD)                                  << ": statusDeleteD should be true";
        ASSERT_IS_TRUE(statusDeleteA)                                  << ": statusDeleteA should be true";
        ASSERT_IS_TRUE(statusEnumeration)                              << ": statusEnumeration should be true";
        ASSERT_EQUAL(1, static_cast<int>(dataEnumeratorVector.size())) << ": there should be a single dataset discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                        << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                              << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                              << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                           << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())            << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())   << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())        << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())  << ": datasets[0] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("when file opened read-write, after deleting existing object it cannot be opened again")
    {
        const char* fileName{ "file_when_file_opened_read-write_after_deleting_existing_object_it_cannot_be_opened_again.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusDeleteA = file->deleteObject("/a", true);
        File::interceptErrorStack(nullptr);
        const auto instrument = file->openInstrument("/a", false);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(statusDeleteA) << ": statusDeleteA should be true";
        ASSERT_NULL(instrument)       << ": instrument should be nullptr";
    }

    TESTCASE("when file opened read-write, non-existing object cannot be deleted")
    {
        const char* fileName{ "file_when_file_opened_read-write_non-existing_object_cannot_be_deleted.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        File::interceptErrorStack([](const char* message, bool isError) noexcept
        {
            try
            {
                std::string msg{ isError ? "isError=true: " : "isError=false: " };
                msg += message;
                logVector.push_back(msg);
            }
            catch (...)
            {
            }
        });

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusDelete = file->deleteObject("/foobar", true);
        file->close();
        File::interceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_FALSE(statusDelete)      << ": statusDeleteA should be false";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-only, existing object cannot be deleted")
    {
        const char* fileName{ "file_when_file_opened_read-only_existing_object_cannot_be_deleted.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        File::interceptErrorStack([](const char* message, bool isError) noexcept
        {
            try
            {
                std::string msg{ isError ? "isError=true: " : "isError=false: " };
                msg += message;
                logVector.push_back(msg);
            }
            catch (...)
            {
            }
        });

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const bool statusDelete = file->deleteObject("/a", true);
        file->close();
        File::interceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_FALSE(statusDelete)      << ": statusDeleteA should be false";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Move instrument
    // ---------------------------------------------------------------------------

    TESTCASE("when file opened read-write, after moving existing instrument datasets enumerated correctly")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_when_file_opened_read-write_after_moving_existing_instrument_datasets_enumerated_correctly.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusMoveBc = file->moveInstrument("/b/c", "/foo");
        const bool statusMoveDef = file->moveInstrument("/d/e/f", "/bar");
        const bool statusEnumeration = file->enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, false);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(statusMoveBc)      << ": statusMoveBc should be true";
        ASSERT_IS_TRUE(statusMoveDef)     << ": statusMoveDef should be true";
        ASSERT_IS_TRUE(statusEnumeration) << ": statusEnumeration should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": datasets[1] time frame should be day 1";

        ds = &dataEnumeratorVector[2];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[2] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/foo/q____", ds->path())                                << ": datasets[2] path should be /foo/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[2] time frame should be aperiodic";

        ds = &dataEnumeratorVector[3];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[3] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/foo/t____", ds->path())                                << ": datasets[3] path should be /foo/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[3] time frame should be aperiodic";

        ds = &dataEnumeratorVector[4];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[4] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": datasets[4] name should be s_m03";
        ASSERT_EQUAL("/bar/s_m03", ds->path())                                << ": datasets[4] path should be /bar/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": datasets[4] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": datasets[4] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": datasets[4] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": datasets[4] time frame should be minute 3";

        ds = &dataEnumeratorVector[5];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[5] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": datasets[5] name should be T____";
        ASSERT_EQUAL("/bar/T____", ds->path())                                << ": datasets[5] path should be /bar/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": datasets[5] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": datasets[5] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[5] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[5] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("when file opened read-write, after moving existing instrument old path cannot be opened")
    {
        const char* fileName{ "file_when_file_opened_read-write_after_moving_existing_instrument_old_path_cannot_be_opened.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        File::interceptErrorStack([](const char* message, bool isError) noexcept
        {
            try
            {
                std::string msg{ isError ? "isError=true: " : "isError=false: " };
                msg += message;
                logVector.push_back(msg);
            }
            catch (...)
            {
            }
        });

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusMoveA = file->moveInstrument("/a", "/foo/bar");
        const auto instrument = file->openInstrument("/a", false);
        file->close();
        File::interceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_TRUE(statusMoveA)        << ": statusMoveA should be true";
        ASSERT_NULL(instrument)            << ": instrument should be nullptr";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-write, after moving existing instrument new path can be opened")
    {
        const char* fileName{ "file_when_file_opened_read-write_after_moving_existing_instrument_new_path_can_be_opened.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusMoveA = file->moveInstrument("/a", "/foo/bar");
        const auto instrument = file->openInstrument("/foo/bar", false);
        const bool isNull = instrument == nullptr;
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(statusMoveA) << ": statusMoveA should be true";
        ASSERT_IS_FALSE(isNull)     << ": isNull should be false";
    }

    TESTCASE("when file opened read-write, moving non-existing instrument is impossible")
    {
        const char* fileName{ "file_when_file_opened_read-write_moving_non-existing_instrument_is_impossible.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        File::interceptErrorStack([](const char* message, bool isError) noexcept
        {
            try
            {
                std::string msg{ isError ? "isError=true: " : "isError=false: " };
                msg += message;
                logVector.push_back(msg);
            }
            catch (...)
            {
            }
        });

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusMove = file->moveInstrument("/foobar", "/foo/bar");
        file->close();
        File::interceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_FALSE(statusMove)        << ": statusMove should be false";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-only, moving existing instrument is impossible")
    {
        const char* fileName{ "file_when_file_opened_read-only_moving_existing_instrument_is_impossible.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        const bool statusMove = file->moveInstrument("/a", "/foo/bar");
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(statusMove) << ": statusMove should be false";
    }

    // ---------------------------------------------------------------------------
    // Rename instrument
    // ---------------------------------------------------------------------------

    TESTCASE("when file opened read-write, after renaming existing instrument datasets enumerated correctly")
    {
        std::vector<DataInfo> dataEnumeratorVector;

        const char* fileName{ "file_when_file_opened_read-write_after_renaming_existing_instrument_datasets_enumerated_correctly.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusRenameBc = file->renameInstrument("/b/c", "foo");
        const bool statusRenameDef = file->renameInstrument("/d/e/f", "bar");
        const bool statusEnumeration = file->enumerateDatasets([&dataEnumeratorVector](const DataInfo& dataInfo) noexcept
        {
            try
            {
                dataEnumeratorVector.push_back(dataInfo);
            }
            catch (...)
            {
            }
        }, false);
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(statusRenameBc)      << ": statusRenameBc should be true";
        ASSERT_IS_TRUE(statusRenameDef)     << ": statusRenameDef should be true";
        ASSERT_IS_TRUE(statusEnumeration) << ": statusEnumeration should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        DataInfo* ds = &dataEnumeratorVector[0];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[0] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name())                                     << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path())                                  << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType())                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind())          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[0] time frame should be aperiodic";

        ds = &dataEnumeratorVector[1];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[1] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name())                                     << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path())                                  << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType())           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind())       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind())               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame())              << ": datasets[1] time frame should be day 1";

        ds = &dataEnumeratorVector[2];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[2] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name())                                     << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/foo/q____", ds->path())                              << ": datasets[2] path should be /b/foo/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType())                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind())          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind())               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[2] time frame should be aperiodic";

        ds = &dataEnumeratorVector[3];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[3] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name())                                     << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/foo/t____", ds->path())                              << ": datasets[3] path should be /b/foo/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType())                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind())          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[3] time frame should be aperiodic";

        ds = &dataEnumeratorVector[4];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[4] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name())                                     << ": datasets[4] name should be s_m03";
        ASSERT_EQUAL("/d/e/bar/s_m03", ds->path())                            << ": datasets[4] path should be /d/e/bar/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType())                  << ": datasets[4] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind())           << ": datasets[4] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind())             << ": datasets[4] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame())           << ": datasets[4] time frame should be minute 3";

        ds = &dataEnumeratorVector[5];
        ASSERT_IS_TRUE(nullptr != ds->parent())                               << ": datasets[5] parent should not be nullptr";
        ASSERT_IS_TRUE(ds->isValidName())                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("T____", ds->name())                                     << ": datasets[5] name should be T____";
        ASSERT_EQUAL("/d/e/bar/T____", ds->path())                            << ": datasets[5] path should be /d/e/bar/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType())          << ": datasets[5] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind()) << ": datasets[5] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind())               << ": datasets[5] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame())         << ": datasets[5] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("when file opened read-write, after renaming existing instrument old path cannot be opened")
    {
        const char* fileName{ "file_when_file_opened_read-write_after_renaming_existing_instrument_old_path_cannot_be_opened.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        File::interceptErrorStack([](const char* message, bool isError) noexcept
        {
            try
            {
                std::string msg{ isError ? "isError=true: " : "isError=false: " };
                msg += message;
                logVector.push_back(msg);
            }
            catch (...)
            {
            }
        });

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusRenameA = file->renameInstrument("/a", "foobar");
        const auto instrument = file->openInstrument("/a", false);
        file->close();
        File::interceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_TRUE(statusRenameA)      << ": statusRenameA should be true";
        ASSERT_NULL(instrument)            << ": instrument should be nullptr";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-write, after renaming existing instrument new path can be opened")
    {
        const char* fileName{ "file_when_file_opened_read-write_after_renaming_existing_instrument_new_path_can_be_opened.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusRenameA = file->renameInstrument("/a", "foobar");
        const auto instrument = file->openInstrument("/foobar", false);
        const bool isNull = instrument == nullptr;
        instrument->close();
        file->close();

        remove(fileName);

        ASSERT_IS_TRUE(statusRenameA) << ": statusRenameA should be true";
        ASSERT_IS_FALSE(isNull)       << ": isNull should be false";
    }

    TESTCASE("when file opened read-write, renaming non-existing instrument is impossible")
    {
        const char* fileName{ "file_when_file_opened_read-write_renaming_non-existing_instrument_is_impossible.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        File::interceptErrorStack([](const char* message, bool isError) noexcept
        {
            try
            {
                std::string msg{ isError ? "isError=true: " : "isError=false: " };
                msg += message;
                logVector.push_back(msg);
            }
            catch (...)
            {
            }
        });

        const std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        const bool statusRename = file->renameInstrument("/foo", "bar");
        file->close();
        File::interceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_FALSE(statusRename)      << ": statusRename should be false";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-only, renaming existing instrument is impossible")
    {
        const char* fileName{ "file_when_file_opened_read-only_renaming_existing_instrument_is_impossible.h5" };
        createEnumerationDatasets(fileName);

        const std::shared_ptr<File> file = File::openReadOnly(fileName);
        File::interceptErrorStack(nullptr);
        const bool statusRename = file->renameInstrument("/a", "foobar");
        file->close();

        remove(fileName);

        ASSERT_IS_FALSE(statusRename) << ": statusRename should be false";
    }
}
}
