#include "mbc/testharness.h"
#include "h5capi.h"
#include "h5capiTestInfo.h"

namespace h5capi { namespace th = mbc::testharness;

void createEnumerationDatasets(const char* fileName);
void createEnumerationDatasetsInReverseOrder(const char* fileName);

TESTGROUP("h5 capi file")
{
    // ---------------------------------------------------------------------------
    // Intercept error stack
    // ---------------------------------------------------------------------------

    TESTCASE("interceptErrorStack logs Hdf5 errors")
    {
        const char* fileName{ "capi_file_interceptErrorStack_logs_hdf5_errors.h5" };
        remove(fileName);
        static std::vector<std::string> logVector{};

        h5InterceptErrorStack([](const char* message, bool isError) noexcept
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
        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        h5InterceptErrorStack(nullptr);

        ASSERT_IS_TRUE(file == nullptr)       << ": file handle should be nullptr";
        ASSERT_IS_TRUE(logVector.size() == 1) << ": single error message should be logged";

        ASSERT_EQUAL("isError=true: h5: Error: Failed to open the file \"capi_file_interceptErrorStack_logs_hdf5_errors.h5\" for the read-only access", logVector[0].c_str())
            << ": logged text should be expected";
        logVector.clear();
    }

    TESTCASE("interceptErrorStack logs Hdf5 infos")
    {
        Scalar array[2] { {1001L, 2.2}, {1001L, 3.3} };	
        static std::vector<std::string> logVector{};
        
        h5InterceptErrorStack([](const char* message, bool isError) noexcept
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
        const bool status = h5ScalarDataSpreadDuplicateTimeTicks(array, 2, true);
        h5InterceptErrorStack(nullptr);

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
        const char* fileName{ "capi_file_after_close_is_called_file_handle_is_unusable.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        h5FileClose(file);
        h5InterceptErrorStack(nullptr);
        const bool status = h5FileEnumerateDatasets(file, nullptr, false);

        remove(fileName);

        ASSERT_IS_FALSE(status) << ": enumerate datasets should return false";
    }

    // ---------------------------------------------------------------------------
    // Open instrument
    // ---------------------------------------------------------------------------

    TESTCASE("cannot open non-existing instrument when opened file read-write and createNonExistent is false")
    {
        const char* fileName{ "capi_file_cannot_open_non-existing_instrument_when_opened_file_read-write_and_createNonExistent_is_false.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        h5InterceptErrorStack(nullptr);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "foo/bar", false);
        const bool isInstrumentNullptr{ instrument == nullptr };
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(isInstrumentNullptr) << ": instrument should be nullptr";
    }

    TESTCASE("can open non-existing instrument when opened file read-write and createNonExistent is true")
    {
        const char* fileName{ "capi_file_can_open_non-existing_instrument_when_opened_file_read-write_and_createNonExistent_is_true.h5" };
        remove(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "foo/bar", true);
        const bool isInstrumentNullptr{ instrument == nullptr };
        h5InstrumentClose(instrument);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(isInstrumentNullptr) << ": instrument should be a valid pointer";
    }

    TESTCASE("cannot open non-existing instrument when opened file read-only and createNonExistent is true")
    {
        const char* fileName{ "capi_file_cannot_open_non-existing_instrument_when_opened_file_read-only_and_createNonExistent_is_true.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        h5InterceptErrorStack(nullptr);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "foo/bar", true);
        const bool isInstrumentNullptr{ instrument == nullptr };
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(isInstrumentNullptr) << ": instrument should be nullptr";
    }

    TESTCASE("cannot open non-existing instrument when opened file read-only and createNonExistent is false")
    {
        const char* fileName{ "capi_file_cannot_open_non-existing_instrument_when_opened_file_read-only_and_createNonExistent_is_false.h5" };
        remove(fileName);

        H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        h5FileClose(file);

        file = h5FileOpenReadOnly(fileName);
        h5InterceptErrorStack(nullptr);
        const H5InstrumentHandle instrument = h5FileOpenInstrument(file, "foo/bar", false);
        const bool isInstrumentNullptr{ instrument == nullptr };
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(isInstrumentNullptr) << ": instrument should be nullptr";
    }

    // ---------------------------------------------------------------------------
    // Enumerate content
    // ---------------------------------------------------------------------------

    static void groupInfoAddDataInfo(const void* groupInfoHandle, DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind,
        TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, const char* name, const char* path) noexcept
    {
        try
        {
            const auto cgi = static_cast<const std::shared_ptr<TestGroupInfo>*>(groupInfoHandle);
            const auto gi = const_cast<std::shared_ptr<TestGroupInfo>*>(cgi);

            std::shared_ptr<TestDataInfo> di{ new TestDataInfo(name, *gi, dataType, dataKind, ohlcvKind, scalarKind, tradeKind, quoteKind, timeFrame, isValidName) };
            di->path_ = path;
            (*gi)->datasets_.push_back(di);
        }
        catch (...)
        {
        }
    }

    static const void* groupInfoAddGroupInfo(const void* groupInfoHandle, const char* name, const char* path) noexcept
    {
        std::shared_ptr<TestGroupInfo>* gi{};
        try
        {
            const auto cgi = static_cast<const std::shared_ptr<TestGroupInfo>*>(groupInfoHandle);
            gi = const_cast<std::shared_ptr<TestGroupInfo>*>(cgi);

            std::shared_ptr<TestGroupInfo> child{ new TestGroupInfo(name, *gi) };
            child->path_ = path;

            (*gi)->groups_.push_back(child);
        }
        catch (...)
        {
        }

        return static_cast<const void*>(&(*gi)->groups_.back());
    }

    TESTCASE("enumerates content tree unsorted")
    {
        const char* fileName{ "capi_file_enumerates_content_tree_unsorted.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        std::shared_ptr<TestGroupInfo> root{ new TestGroupInfo() };
        const bool status = h5FileEnumerateContentTree(file, static_cast<const void*>(&root), groupInfoAddDataInfo, groupInfoAddGroupInfo, false);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        
        ASSERT_EQUAL("/", root->name_.c_str())                                   << ": root name should be /";
        ASSERT_EQUAL("", root->path_.c_str())                                    << ": root path should be empty";
        ASSERT_NULL(root->parent_.get())                                         << ": root parent should be nullptr";
        ASSERT_EQUAL(0, root->datasets_.size())                                  << ": root dataset count should be 0";
        ASSERT_EQUAL(3, root->groups_.size())                                    << ": root group count should be 3";

        auto gr = root->groups_[0];
        ASSERT_EQUAL("a", gr->name_.c_str())                                     << ": root[0] name should be a";
        ASSERT_EQUAL("/a", gr->path_.c_str())                                    << ": root[0] path should be /a";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[0] parent should be root";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[0] group count should be 0";

        auto ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[0] datasets[0] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[0] datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                                 << ": root[0] datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                              << ": root[0] datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                       << ": root[0] datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)              << ": root[0] datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)                   << ": root[0] datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[0] datasets[1] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[0] datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                                 << ": root[0] datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                              << ": root[0] datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)               << ": root[0] datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)           << ": root[0] datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)                   << ": root[0] datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)                  << ": root[0] datasets[1] time frame should be day 1";

        gr = root->groups_[1];
        ASSERT_EQUAL("b", gr->name_.c_str())                                     << ": root[1] name should be b";
        ASSERT_EQUAL("/b", gr->path_.c_str())                                    << ": root[1] path should be /b";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[1] parent should be root";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[1] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[1] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("c", gr->name_.c_str())                                     << ": root[1,0] name should be c";
        ASSERT_EQUAL("/b/c", gr->path_.c_str())                                  << ": root[1,0] path should be /b/c";
        ASSERT_IS_TRUE(root->groups_[1].get() == gr->parent_.get())              << ": root[1,0] parent should be root[1]";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[1,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[1,0] group count should be 0";

        ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[1,0] datasets[0] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[1,0] datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                                 << ": root[1,0] datasets[0] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path_.c_str())                            << ": root[1,0] datasets[0] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                       << ": root[1,0] datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)              << ": root[1,0] datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)                   << ": root[1,0] datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[1,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[1,0] datasets[1] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[1,0] datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                                 << ": root[1,0] datasets[1] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path_.c_str())                            << ": root[1,0] datasets[1] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                       << ": root[1,0] datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)              << ": root[1,0] datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)                   << ": root[1,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[1,0] datasets[1] time frame should be aperiodic";

        gr = root->groups_[2];
        ASSERT_EQUAL("d", gr->name_.c_str())                                     << ": root[2] name should be d";
        ASSERT_EQUAL("/d", gr->path_.c_str())                                    << ": root[2] path should be /d";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[2] parent should be root";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[2] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[2] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("e", gr->name_.c_str())                                     << ": root[2,0] name should be e";
        ASSERT_EQUAL("/d/e", gr->path_.c_str())                                  << ": root[2,0] path should be /d/e";
        ASSERT_IS_TRUE(root->groups_[2].get() == gr->parent_.get())              << ": root[2,0] parent should be root[2]";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[2,0] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[2,0] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("f", gr->name_.c_str())                                     << ": root[2,0,0] name should be f";
        ASSERT_EQUAL("/d/e/f", gr->path_.c_str())                                << ": root[2,0,0] path should be /d/e/f";
        ASSERT_IS_TRUE(root->groups_[2]->groups_[0].get() == gr->parent_.get())  << ": root[2,0,0] parent should be root[2,0]";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[2,0,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[2,0,0] group count should be 0";

        ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[2,0,0] datasets[0] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[2,0,0] datasets[0] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                                 << ": root[2,0,0] datasets[0] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path_.c_str())                          << ": root[2,0,0] datasets[0] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                      << ": root[2,0,0] datasets[0] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)               << ": root[2,0,0] datasets[0] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)                 << ": root[2,0,0] datasets[0] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)               << ": root[2,0,0] datasets[0] time frame should be minute 3";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[2,0,0] datasets[1] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[2,0,0] datasets[1] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                                 << ": root[2,0,0] datasets[1] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path_.c_str())                          << ": root[2,0,0] datasets[1] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)              << ": root[2,0,0] datasets[1] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_)     << ": root[2,0,0] datasets[1] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)                   << ": root[2,0,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[2,0,0] datasets[1] time frame should be aperiodic";
    }

    TESTCASE("enumerates content tree sorted")
    {
        const char* fileName{ "capi_file_enumerates_content_tree_sorted.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        std::shared_ptr<TestGroupInfo> root{ new TestGroupInfo() };
        const bool status = h5FileEnumerateContentTree(file, static_cast<const void*>(&root), groupInfoAddDataInfo, groupInfoAddGroupInfo, true);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        
        ASSERT_EQUAL("/", root->name_.c_str())                                   << ": root name should be /";
        ASSERT_EQUAL("", root->path_.c_str())                                    << ": root path should be empty";
        ASSERT_NULL(root->parent_.get())                                         << ": root parent should be nullptr";
        ASSERT_EQUAL(0, root->datasets_.size())                                  << ": root dataset count should be 0";
        ASSERT_EQUAL(3, root->groups_.size())                                    << ": root group count should be 3";

        auto gr = root->groups_[0];
        ASSERT_EQUAL("a", gr->name_.c_str())                                     << ": root[0] name should be a";
        ASSERT_EQUAL("/a", gr->path_.c_str())                                    << ": root[0] path should be /a";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[0] parent should be root";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[0] group count should be 0";

        auto ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[0] datasets[0] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[0] datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                                 << ": root[0] datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                              << ": root[0] datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                       << ": root[0] datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)              << ": root[0] datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)                   << ": root[0] datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[0] datasets[1] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[0] datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                                 << ": root[0] datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                              << ": root[0] datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)               << ": root[0] datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)           << ": root[0] datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)                   << ": root[0] datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)                  << ": root[0] datasets[1] time frame should be day 1";

        gr = root->groups_[1];
        ASSERT_EQUAL("b", gr->name_.c_str())                                     << ": root[1] name should be b";
        ASSERT_EQUAL("/b", gr->path_.c_str())                                    << ": root[1] path should be /b";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[1] parent should be root";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[1] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[1] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("c", gr->name_.c_str())                                     << ": root[1,0] name should be c";
        ASSERT_EQUAL("/b/c", gr->path_.c_str())                                  << ": root[1,0] path should be /b/c";
        ASSERT_IS_TRUE(root->groups_[1].get() == gr->parent_.get())              << ": root[1,0] parent should be root[1]";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[1,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[1,0] group count should be 0";

        ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[1,0] datasets[0] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[1,0] datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                                 << ": root[1,0] datasets[0] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path_.c_str())                            << ": root[1,0] datasets[0] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                       << ": root[1,0] datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)              << ": root[1,0] datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)                   << ": root[1,0] datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[1,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[1,0] datasets[1] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[1,0] datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                                 << ": root[1,0] datasets[1] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path_.c_str())                            << ": root[1,0] datasets[1] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                       << ": root[1,0] datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)              << ": root[1,0] datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)                   << ": root[1,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[1,0] datasets[1] time frame should be aperiodic";

        gr = root->groups_[2];
        ASSERT_EQUAL("d", gr->name_.c_str())                                     << ": root[2] name should be d";
        ASSERT_EQUAL("/d", gr->path_.c_str())                                    << ": root[2] path should be /d";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[2] parent should be root";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[2] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[2] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("e", gr->name_.c_str())                                     << ": root[2,0] name should be e";
        ASSERT_EQUAL("/d/e", gr->path_.c_str())                                  << ": root[2,0] path should be /d/e";
        ASSERT_IS_TRUE(root->groups_[2].get() == gr->parent_.get())              << ": root[2,0] parent should be root[2]";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[2,0] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[2,0] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("f", gr->name_.c_str())                                     << ": root[2,0,0] name should be f";
        ASSERT_EQUAL("/d/e/f", gr->path_.c_str())                                << ": root[2,0,0] path should be /d/e/f";
        ASSERT_IS_TRUE(root->groups_[2]->groups_[0].get() == gr->parent_.get())  << ": root[2,0,0] parent should be root[2,0]";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[2,0,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[2,0,0] group count should be 0";

        ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[2,0,0] datasets[0] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[2,0,0] datasets[0] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                                 << ": root[2,0,0] datasets[0] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path_.c_str())                          << ": root[2,0,0] datasets[0] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)              << ": root[2,0,0] datasets[0] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_)     << ": root[2,0,0] datasets[0] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)                   << ": root[2,0,0] datasets[0] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[2,0,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[2,0,0] datasets[1] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[2,0,0] datasets[1] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                                 << ": root[2,0,0] datasets[1] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path_.c_str())                          << ": root[2,0,0] datasets[1] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                      << ": root[2,0,0] datasets[1] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)               << ": root[2,0,0] datasets[1] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)                 << ": root[2,0,0] datasets[1] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)               << ": root[2,0,0] datasets[1] time frame should be minute 3";
    }

    TESTCASE("enumerates content tree unsorted statically")
    {
        const char* fileName{ "capi_file_enumerates_content_tree_unsorted_statically.h5" };
        createEnumerationDatasets(fileName);

        std::shared_ptr<TestGroupInfo> root{ new TestGroupInfo() };
        const bool status = h5FileEnumerateContentTreeStatic(fileName, FileDriver::Sec2, static_cast<const void*>(&root), groupInfoAddDataInfo, groupInfoAddGroupInfo, false);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        
        ASSERT_EQUAL("/", root->name_.c_str())                                   << ": root name should be /";
        ASSERT_EQUAL("", root->path_.c_str())                                    << ": root path should be empty";
        ASSERT_NULL(root->parent_.get())                                         << ": root parent should be nullptr";
        ASSERT_EQUAL(0, root->datasets_.size())                                  << ": root dataset count should be 0";
        ASSERT_EQUAL(3, root->groups_.size())                                    << ": root group count should be 3";

        auto gr = root->groups_[0];
        ASSERT_EQUAL("a", gr->name_.c_str())                                     << ": root[0] name should be a";
        ASSERT_EQUAL("/a", gr->path_.c_str())                                    << ": root[0] path should be /a";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[0] parent should be root";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[0] group count should be 0";

        auto ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[0] datasets[0] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[0] datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                                 << ": root[0] datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                              << ": root[0] datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                       << ": root[0] datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)              << ": root[0] datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)                   << ": root[0] datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[0] datasets[1] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[0] datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                                 << ": root[0] datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                              << ": root[0] datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)               << ": root[0] datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)           << ": root[0] datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)                   << ": root[0] datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)                  << ": root[0] datasets[1] time frame should be day 1";

        gr = root->groups_[1];
        ASSERT_EQUAL("b", gr->name_.c_str())                                     << ": root[1] name should be b";
        ASSERT_EQUAL("/b", gr->path_.c_str())                                    << ": root[1] path should be /b";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[1] parent should be root";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[1] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[1] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("c", gr->name_.c_str())                                     << ": root[1,0] name should be c";
        ASSERT_EQUAL("/b/c", gr->path_.c_str())                                  << ": root[1,0] path should be /b/c";
        ASSERT_IS_TRUE(root->groups_[1].get() == gr->parent_.get())              << ": root[1,0] parent should be root[1]";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[1,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[1,0] group count should be 0";

        ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[1,0] datasets[0] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[1,0] datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                                 << ": root[1,0] datasets[0] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path_.c_str())                            << ": root[1,0] datasets[0] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                       << ": root[1,0] datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)              << ": root[1,0] datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)                   << ": root[1,0] datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[1,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[1,0] datasets[1] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[1,0] datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                                 << ": root[1,0] datasets[1] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path_.c_str())                            << ": root[1,0] datasets[1] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                       << ": root[1,0] datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)              << ": root[1,0] datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)                   << ": root[1,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[1,0] datasets[1] time frame should be aperiodic";

        gr = root->groups_[2];
        ASSERT_EQUAL("d", gr->name_.c_str())                                     << ": root[2] name should be d";
        ASSERT_EQUAL("/d", gr->path_.c_str())                                    << ": root[2] path should be /d";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[2] parent should be root";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[2] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[2] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("e", gr->name_.c_str())                                     << ": root[2,0] name should be e";
        ASSERT_EQUAL("/d/e", gr->path_.c_str())                                  << ": root[2,0] path should be /d/e";
        ASSERT_IS_TRUE(root->groups_[2].get() == gr->parent_.get())              << ": root[2,0] parent should be root[2]";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[2,0] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[2,0] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("f", gr->name_.c_str())                                     << ": root[2,0,0] name should be f";
        ASSERT_EQUAL("/d/e/f", gr->path_.c_str())                                << ": root[2,0,0] path should be /d/e/f";
        ASSERT_IS_TRUE(root->groups_[2]->groups_[0].get() == gr->parent_.get())  << ": root[2,0,0] parent should be root[2,0]";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[2,0,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[2,0,0] group count should be 0";

        ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[2,0,0] datasets[0] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[2,0,0] datasets[0] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                                 << ": root[2,0,0] datasets[0] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path_.c_str())                          << ": root[2,0,0] datasets[0] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                      << ": root[2,0,0] datasets[0] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)               << ": root[2,0,0] datasets[0] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)                 << ": root[2,0,0] datasets[0] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)               << ": root[2,0,0] datasets[0] time frame should be minute 3";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[2,0,0] datasets[1] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[2,0,0] datasets[1] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                                 << ": root[2,0,0] datasets[1] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path_.c_str())                          << ": root[2,0,0] datasets[1] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)              << ": root[2,0,0] datasets[1] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_)     << ": root[2,0,0] datasets[1] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)                   << ": root[2,0,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[2,0,0] datasets[1] time frame should be aperiodic";
    }

    TESTCASE("enumerates content tree sorted statically")
    {
        const char* fileName{ "capi_file_enumerates_content_tree_sorted_statically.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        std::shared_ptr<TestGroupInfo> root{ new TestGroupInfo() };
        const bool status = h5FileEnumerateContentTreeStatic(fileName, FileDriver::Sec2, static_cast<const void*>(&root), groupInfoAddDataInfo, groupInfoAddGroupInfo, true);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        
        ASSERT_EQUAL("/", root->name_.c_str())                                   << ": root name should be /";
        ASSERT_EQUAL("", root->path_.c_str())                                    << ": root path should be empty";
        ASSERT_NULL(root->parent_.get())                                         << ": root parent should be nullptr";
        ASSERT_EQUAL(0, root->datasets_.size())                                  << ": root dataset count should be 0";
        ASSERT_EQUAL(3, root->groups_.size())                                    << ": root group count should be 3";

        auto gr = root->groups_[0];
        ASSERT_EQUAL("a", gr->name_.c_str())                                     << ": root[0] name should be a";
        ASSERT_EQUAL("/a", gr->path_.c_str())                                    << ": root[0] path should be /a";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[0] parent should be root";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[0] group count should be 0";

        auto ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[0] datasets[0] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[0] datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                                 << ": root[0] datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                              << ": root[0] datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                       << ": root[0] datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)              << ": root[0] datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)                   << ": root[0] datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[0] datasets[1] parent should be root[0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[0] datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                                 << ": root[0] datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                              << ": root[0] datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)               << ": root[0] datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)           << ": root[0] datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)                   << ": root[0] datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)                  << ": root[0] datasets[1] time frame should be day 1";

        gr = root->groups_[1];
        ASSERT_EQUAL("b", gr->name_.c_str())                                     << ": root[1] name should be b";
        ASSERT_EQUAL("/b", gr->path_.c_str())                                    << ": root[1] path should be /b";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[1] parent should be root";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[1] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[1] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("c", gr->name_.c_str())                                     << ": root[1,0] name should be c";
        ASSERT_EQUAL("/b/c", gr->path_.c_str())                                  << ": root[1,0] path should be /b/c";
        ASSERT_IS_TRUE(root->groups_[1].get() == gr->parent_.get())              << ": root[1,0] parent should be root[1]";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[1,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[1,0] group count should be 0";

        ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[1,0] datasets[0] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[1,0] datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                                 << ": root[1,0] datasets[0] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path_.c_str())                            << ": root[1,0] datasets[0] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                       << ": root[1,0] datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)              << ": root[1,0] datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)                   << ": root[1,0] datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[1,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[1,0] datasets[1] parent should be root[1,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[1,0] datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                                 << ": root[1,0] datasets[1] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path_.c_str())                            << ": root[1,0] datasets[1] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                       << ": root[1,0] datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)              << ": root[1,0] datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)                   << ": root[1,0] datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[1,0] datasets[1] time frame should be aperiodic";

        gr = root->groups_[2];
        ASSERT_EQUAL("d", gr->name_.c_str())                                     << ": root[2] name should be d";
        ASSERT_EQUAL("/d", gr->path_.c_str())                                    << ": root[2] path should be /d";
        ASSERT_IS_TRUE(root.get() == gr->parent_.get())                          << ": root[2] parent should be root";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[2] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[2] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("e", gr->name_.c_str())                                     << ": root[2,0] name should be e";
        ASSERT_EQUAL("/d/e", gr->path_.c_str())                                  << ": root[2,0] path should be /d/e";
        ASSERT_IS_TRUE(root->groups_[2].get() == gr->parent_.get())              << ": root[2,0] parent should be root[2]";
        ASSERT_EQUAL(0, gr->datasets_.size())                                    << ": root[2,0] dataset count should be 0";
        ASSERT_EQUAL(1, gr->groups_.size())                                      << ": root[2,0] group count should be 1";

        gr = gr->groups_[0];
        ASSERT_EQUAL("f", gr->name_.c_str())                                     << ": root[2,0,0] name should be f";
        ASSERT_EQUAL("/d/e/f", gr->path_.c_str())                                << ": root[2,0,0] path should be /d/e/f";
        ASSERT_IS_TRUE(root->groups_[2]->groups_[0].get() == gr->parent_.get())  << ": root[2,0,0] parent should be root[2,0]";
        ASSERT_EQUAL(2, gr->datasets_.size())                                    << ": root[2,0,0] dataset count should be 2";
        ASSERT_EQUAL(0, gr->groups_.size())                                      << ": root[2,0,0] group count should be 0";

        ds = gr->datasets_[0];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[2,0,0] datasets[0] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[2,0,0] datasets[0] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                                 << ": root[2,0,0] datasets[0] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path_.c_str())                          << ": root[2,0,0] datasets[0] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)              << ": root[2,0,0] datasets[0] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_)     << ": root[2,0,0] datasets[0] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)                   << ": root[2,0,0] datasets[0] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)             << ": root[2,0,0] datasets[0] time frame should be aperiodic";

        ds = gr->datasets_[1];
        ASSERT_IS_TRUE(gr.get() == ds->parent_.get())                            << ": root[2,0,0] datasets[1] parent should be root[2,0,0]";
        ASSERT_IS_TRUE(ds->isValidName_)                                         << ": root[2,0,0] datasets[1] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                                 << ": root[2,0,0] datasets[1] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path_.c_str())                          << ": root[2,0,0] datasets[1] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                      << ": root[2,0,0] datasets[1] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)               << ": root[2,0,0] datasets[1] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)                 << ": root[2,0,0] datasets[1] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)               << ": root[2,0,0] datasets[1] time frame should be minute 3";
    }

    // ---------------------------------------------------------------------------
    // Enumerate datasets
    // ---------------------------------------------------------------------------

    TESTCASE("enumerates datasets unsorted")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_enumerates_datasets_unsorted.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const bool status = h5FileEnumerateDatasets(file, [](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                             << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                          << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                             << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                          << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)              << ": datasets[1] time frame should be day 1";

        ds = dataEnumeratorVector[2].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[2] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                             << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path_.c_str())                        << ": datasets[2] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[2] time frame should be aperiodic";

        ds = dataEnumeratorVector[3].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[3] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                             << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path_.c_str())                        << ": datasets[3] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[3] time frame should be aperiodic";

        ds = dataEnumeratorVector[4].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[4] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                             << ": datasets[4] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path_.c_str())                      << ": datasets[4] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                  << ": datasets[4] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)           << ": datasets[4] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)             << ": datasets[4] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)           << ": datasets[4] time frame should be minute 3";

        ds = dataEnumeratorVector[5].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[5] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                             << ": datasets[5] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path_.c_str())                      << ": datasets[5] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)          << ": datasets[5] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_) << ": datasets[5] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[5] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[5] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates datasets sorted")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_enumerates_datasets_sorted.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const bool status = h5FileEnumerateDatasets(file, [](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                             << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                          << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                             << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                          << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)              << ": datasets[1] time frame should be day 1";

        ds = dataEnumeratorVector[2].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[2] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                             << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path_.c_str())                        << ": datasets[2] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[2] time frame should be aperiodic";

        ds = dataEnumeratorVector[3].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[3] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                             << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path_.c_str())                        << ": datasets[3] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[3] time frame should be aperiodic";

        ds = dataEnumeratorVector[4].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[4] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                             << ": datasets[4] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path_.c_str())                      << ": datasets[4] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)          << ": datasets[4] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_) << ": datasets[4] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[4] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[4] time frame should be aperiodic";

        ds = dataEnumeratorVector[5].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[5] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                             << ": datasets[5] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path_.c_str())                      << ": datasets[5] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                  << ": datasets[5] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)           << ": datasets[5] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)             << ": datasets[5] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)           << ": datasets[5] time frame should be minute 3";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates datasets unsorted statically")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_enumerates_datasets_unsorted_statically.h5" };
        createEnumerationDatasets(fileName);

        const bool status = h5FileEnumerateDatasetsStatic([](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind,
            TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, const char* name, const char* path) noexcept
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
        }, fileName, FileDriver::Sec2, false);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                             << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                          << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                             << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                          << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)              << ": datasets[1] time frame should be day 1";

        ds = dataEnumeratorVector[2].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[2] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                             << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path_.c_str())                        << ": datasets[2] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[2] time frame should be aperiodic";

        ds = dataEnumeratorVector[3].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[3] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                             << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path_.c_str())                        << ": datasets[3] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[3] time frame should be aperiodic";

        ds = dataEnumeratorVector[4].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[4] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                             << ": datasets[4] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path_.c_str())                      << ": datasets[4] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                  << ": datasets[4] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)           << ": datasets[4] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)             << ": datasets[4] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)           << ": datasets[4] time frame should be minute 3";

        ds = dataEnumeratorVector[5].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[5] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                             << ": datasets[5] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path_.c_str())                      << ": datasets[5] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)          << ": datasets[5] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_) << ": datasets[5] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[5] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[5] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates datasets sorted statically")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_enumerates_datasets_sorted_statically.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const bool status = h5FileEnumerateDatasetsStatic([](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        }, fileName, FileDriver::Sec2, true);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                             << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                          << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                             << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                          << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)              << ": datasets[1] time frame should be day 1";

        ds = dataEnumeratorVector[2].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[2] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                             << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/c/q____", ds->path_.c_str())                        << ": datasets[2] path should be /b/c/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[2] time frame should be aperiodic";

        ds = dataEnumeratorVector[3].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[3] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                             << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/c/t____", ds->path_.c_str())                        << ": datasets[3] path should be /b/c/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[3] time frame should be aperiodic";

        ds = dataEnumeratorVector[4].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[4] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                             << ": datasets[4] name should be T____";
        ASSERT_EQUAL("/d/e/f/T____", ds->path_.c_str())                      << ": datasets[4] path should be /d/e/f/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)          << ": datasets[4] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_) << ": datasets[4] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[4] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[4] time frame should be aperiodic";

        ds = dataEnumeratorVector[5].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[5] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                             << ": datasets[5] name should be s_m03";
        ASSERT_EQUAL("/d/e/f/s_m03", ds->path_.c_str())                      << ": datasets[5] path should be /d/e/f/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                  << ": datasets[5] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)           << ": datasets[5] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)             << ": datasets[5] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)           << ": datasets[5] time frame should be minute 3";
        dataEnumeratorVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Enumerate instrument datasets
    // ---------------------------------------------------------------------------

    TESTCASE("enumerates instrument datasets unsorted")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_enumerates_instrument_datasets_unsorted.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const bool status = h5FileEnumerateInstrumentDatasets(file, [](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        }, "/b/c", false);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                 << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                             << ": datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                     << ": datasets[0] name should be q____";
        ASSERT_EQUAL("/q____", ds->path_.c_str())                    << ": datasets[0] path should be /q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)           << ": datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)  << ": datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)       << ": datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_) << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                 << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                             << ": datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                     << ": datasets[1] name should be t____";
        ASSERT_EQUAL("/t____", ds->path_.c_str())                    << ": datasets[1] path should be /t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)           << ": datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)  << ": datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)       << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_) << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates instrument datasets sorted")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_enumerates_instrument_datasets_sorted.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const bool status = h5FileEnumerateInstrumentDatasets(file, [](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        }, "/b/c", true);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                 << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                             << ": datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                     << ": datasets[0] name should be q____";
        ASSERT_EQUAL("/q____", ds->path_.c_str())                    << ": datasets[0] path should be /q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)           << ": datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)  << ": datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)       << ": datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_) << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                 << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                             << ": datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                     << ": datasets[1] name should be t____";
        ASSERT_EQUAL("/t____", ds->path_.c_str())                    << ": datasets[1] path should be /t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)           << ": datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)  << ": datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)       << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_) << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates instrument datasets unsorted statically")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_enumerates_instrument_datasets_unsorted_statically.h5" };
        createEnumerationDatasets(fileName);

        const bool status = h5FileEnumerateInstrumentDatasetsStatic([](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        }, fileName, FileDriver::Sec2, "/b/c", false);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                 << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                             << ": datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                     << ": datasets[0] name should be q____";
        ASSERT_EQUAL("/q____", ds->path_.c_str())                    << ": datasets[0] path should be /q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)           << ": datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)  << ": datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)       << ": datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_) << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                 << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                             << ": datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                     << ": datasets[1] name should be t____";
        ASSERT_EQUAL("/t____", ds->path_.c_str())                    << ": datasets[1] path should be /t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)           << ": datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)  << ": datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)       << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_) << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("enumerates instrument datasets sorted statically")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_enumerates_instrument_datasets_sorted_statically.h5" };
        createEnumerationDatasetsInReverseOrder(fileName);

        const bool status = h5FileEnumerateInstrumentDatasetsStatic([](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        }, fileName, FileDriver::Sec2, "/b/c", true);

        remove(fileName);

        ASSERT_IS_TRUE(status) << ": status should be true";
        ASSERT_EQUAL(2, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 2 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                 << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                             << ": datasets[0] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                     << ": datasets[0] name should be q____";
        ASSERT_EQUAL("/q____", ds->path_.c_str())                    << ": datasets[0] path should be /q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)           << ": datasets[0] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)  << ": datasets[0] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)       << ": datasets[0] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_) << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                 << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                             << ": datasets[1] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                     << ": datasets[1] name should be t____";
        ASSERT_EQUAL("/t____", ds->path_.c_str())                    << ": datasets[1] path should be /t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)           << ": datasets[1] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)  << ": datasets[1] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)       << ": datasets[1] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_) << ": datasets[1] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    // ---------------------------------------------------------------------------
    // Delete object
    // ---------------------------------------------------------------------------

    TESTCASE("when file opened read-write, after deleting existing object datasets enumerated correctly")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_when_file_opened_read-write_after_deleting_existing_object_datasets_enumerated_correctly.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusDeleteBc = h5FileDeleteObject(file, "/b/c", true);
        const bool statusDeleteD = h5FileDeleteObject(file, "/d", true);
        const bool statusDeleteA = h5FileDeleteObject(file, "/a/j_d01", true);
        const bool statusEnumeration = h5FileEnumerateDatasets(file, [](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusDeleteBc)                                 << ": statusDeleteBc should be true";
        ASSERT_IS_TRUE(statusDeleteD)                                  << ": statusDeleteD should be true";
        ASSERT_IS_TRUE(statusDeleteA)                                  << ": statusDeleteA should be true";
        ASSERT_IS_TRUE(statusEnumeration)                              << ": statusEnumeration should be true";
        ASSERT_EQUAL(1, static_cast<int>(dataEnumeratorVector.size())) << ": there should be a single dataset discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                             << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                          << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[0] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("when file opened read-write, after deleting existing object it cannot be opened again")
    {
        const char* fileName{ "capi_file_when_file_opened_read-write_after_deleting_existing_object_it_cannot_be_opened_again.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusDeleteA = h5FileDeleteObject(file, "/a", true);
        h5InterceptErrorStack(nullptr);
        const auto instrumentHandle = h5FileOpenInstrument(file, "/a", false);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusDeleteA) << ": statusDeleteA should be true";
        ASSERT_NULL(instrumentHandle) << ": instrumentHandle should be nullptr";
    }

    TESTCASE("when file opened read-write, non-existing object cannot be deleted")
    {
        const char* fileName{ "capi_file_when_file_opened_read-write_non-existing_object_cannot_be_deleted.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        h5InterceptErrorStack([](const char* message, bool isError) noexcept
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

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusDelete = h5FileDeleteObject(file, "/foobar", true);
        h5FileClose(file);
        h5InterceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_FALSE(statusDelete)      << ": statusDeleteA should be false";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-only, existing object cannot be deleted")
    {
        const char* fileName{ "capi_file_when_file_opened_read-only_existing_object_cannot_be_deleted.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        h5InterceptErrorStack([](const char* message, bool isError) noexcept
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

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        const bool statusDelete = h5FileDeleteObject(file, "/a", true);
        h5FileClose(file);
        h5InterceptErrorStack(nullptr);

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
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_when_file_opened_read-write_after_moving_existing_instrument_datasets_enumerated_correctly.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusMoveBc = h5FileMoveInstrument(file, "/b/c", "/foo");
        const bool statusMoveDef = h5FileMoveInstrument(file, "/d/e/f", "/bar");
        const bool statusEnumeration = h5FileEnumerateDatasets(file, [](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusMoveBc)      << ": statusMoveBc should be true";
        ASSERT_IS_TRUE(statusMoveDef)     << ": statusMoveDef should be true";
        ASSERT_IS_TRUE(statusEnumeration) << ": statusEnumeration should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                             << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                          << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                             << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                          << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)              << ": datasets[1] time frame should be day 1";

        ds = dataEnumeratorVector[2].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[2] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                             << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/foo/q____", ds->path_.c_str())                        << ": datasets[2] path should be /foo/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[2] time frame should be aperiodic";

        ds = dataEnumeratorVector[3].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[3] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                             << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/foo/t____", ds->path_.c_str())                        << ": datasets[3] path should be /foo/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[3] time frame should be aperiodic";

        ds = dataEnumeratorVector[4].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[4] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                             << ": datasets[4] name should be s_m03";
        ASSERT_EQUAL("/bar/s_m03", ds->path_.c_str())                        << ": datasets[4] path should be /bar/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                  << ": datasets[4] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)           << ": datasets[4] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)             << ": datasets[4] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)           << ": datasets[4] time frame should be minute 3";

        ds = dataEnumeratorVector[5].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[5] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                             << ": datasets[5] name should be T____";
        ASSERT_EQUAL("/bar/T____", ds->path_.c_str())                        << ": datasets[5] path should be /bar/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)          << ": datasets[5] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_) << ": datasets[5] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[5] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[5] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("when file opened read-write, after moving existing instrument old path cannot be opened")
    {
        const char* fileName{ "capi_file_when_file_opened_read-write_after_moving_existing_instrument_old_path_cannot_be_opened.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        h5InterceptErrorStack([](const char* message, bool isError) noexcept
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

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusMoveA = h5FileMoveInstrument(file, "/a", "/foo/bar");
        const auto instrumentHandle = h5FileOpenInstrument(file, "/a", false);
        h5FileClose(file);
        h5InterceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_TRUE(statusMoveA)        << ": statusMoveA should be true";
        ASSERT_NULL(instrumentHandle)      << ": instrumentHandle should be nullptr";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-write, after moving existing instrument new path can be opened")
    {
        const char* fileName{ "capi_file_when_file_opened_read-write_after_moving_existing_instrument_new_path_can_be_opened.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusMoveA = h5FileMoveInstrument(file, "/a", "/foo/bar");
        const auto instrumentHandle = h5FileOpenInstrument(file, "/foo/bar", false);
        const bool isNull = instrumentHandle == nullptr;
        h5InstrumentClose(instrumentHandle);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusMoveA) << ": statusMoveA should be true";
        ASSERT_IS_FALSE(isNull)     << ": isNull should be false";
    }

    TESTCASE("when file opened read-write, moving non-existing instrument is impossible")
    {
        const char* fileName{ "capi_file_when_file_opened_read-write_moving_non-existing_instrument_is_impossible.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        h5InterceptErrorStack([](const char* message, bool isError) noexcept
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

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusMove = h5FileMoveInstrument(file, "/foobar", "/foo/bar");
        h5FileClose(file);
        h5InterceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_FALSE(statusMove)        << ": statusMove should be false";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-only, moving existing instrument is impossible")
    {
        const char* fileName{ "capi_file_when_file_opened_read-only_moving_existing_instrument_is_impossible.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        h5InterceptErrorStack(nullptr);
        const bool statusMove = h5FileMoveInstrument(file, "/a", "/foo/bar");
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(statusMove) << ": statusMove should be false";
    }

    // ---------------------------------------------------------------------------
    // Rename instrument
    // ---------------------------------------------------------------------------

    TESTCASE("when file opened read-write, after renaming existing instrument datasets enumerated correctly")
    {
        static std::vector<std::shared_ptr<TestDataInfo>> dataEnumeratorVector;
        static std::shared_ptr<TestGroupInfo> gi{};

        const char* fileName{ "capi_file_when_file_opened_read-write_after_renaming_existing_instrument_datasets_enumerated_correctly.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusRenameBc = h5FileRenameInstrument(file, "/b/c", "foo");
        const bool statusRenameDef = h5FileRenameInstrument(file, "/d/e/f", "bar");
        const bool statusEnumeration = h5FileEnumerateDatasets(file, [](DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind,
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
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusRenameBc)      << ": statusRenameBc should be true";
        ASSERT_IS_TRUE(statusRenameDef)     << ": statusRenameDef should be true";
        ASSERT_IS_TRUE(statusEnumeration)   << ": statusEnumeration should be true";
        ASSERT_EQUAL(6, static_cast<int>(dataEnumeratorVector.size())) << ": there should be 6 datasets discovered";

        TestDataInfo* ds = dataEnumeratorVector[0].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[0] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[0] should have valid name";
        ASSERT_EQUAL("c____", ds->name_.c_str())                             << ": datasets[0] name should be c____";
        ASSERT_EQUAL("/a/c____", ds->path_.c_str())                          << ": datasets[0] path should be /a/c____";
        ASSERT_EQUAL(DataType::Ohlcv, ds->parsedDataType_)                   << ": datasets[0] data type should be ohlcv";
        ASSERT_EQUAL(DataKind::OhlcvAperiodic, ds->parsedDataKind_)          << ": datasets[0] data kind should be ohlcv aperiodic";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[0] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[0] time frame should be aperiodic";

        ds = dataEnumeratorVector[1].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[1] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[1] should have valid name";
        ASSERT_EQUAL("j_d01", ds->name_.c_str())                             << ": datasets[1] name should be j_d01";
        ASSERT_EQUAL("/a/j_d01", ds->path_.c_str())                          << ": datasets[1] path should be /a/j_d01";
        ASSERT_EQUAL(DataType::OhlcvAdjusted, ds->parsedDataType_)           << ": datasets[1] data type should be ohlcv adjusted";
        ASSERT_EQUAL(DataKind::OhlcvAdjustedDay1, ds->parsedDataKind_)       << ": datasets[1] data kind should be ohlcv adjusted day 1";
        ASSERT_EQUAL(OhlcvKind::Default, ds->parsedOhlcvKind_)               << ": datasets[1] ohlcv kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Day1, ds->parsedTimeFrame_)              << ": datasets[1] time frame should be day 1";

        ds = dataEnumeratorVector[2].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[2] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[2] should have valid name";
        ASSERT_EQUAL("q____", ds->name_.c_str())                             << ": datasets[2] name should be q____";
        ASSERT_EQUAL("/b/foo/q____", ds->path_.c_str())                      << ": datasets[2] path should be /b/foo/q____";
        ASSERT_EQUAL(DataType::Quote, ds->parsedDataType_)                   << ": datasets[2] data type should be quote";
        ASSERT_EQUAL(DataKind::QuoteAperiodic, ds->parsedDataKind_)          << ": datasets[2] data kind should be quote aperiodic";
        ASSERT_EQUAL(QuoteKind::Default, ds->parsedQuoteKind_)               << ": datasets[2] quote kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[2] time frame should be aperiodic";

        ds = dataEnumeratorVector[3].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[3] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[3] should have valid name";
        ASSERT_EQUAL("t____", ds->name_.c_str())                             << ": datasets[3] name should be t____";
        ASSERT_EQUAL("/b/foo/t____", ds->path_.c_str())                      << ": datasets[3] path should be /b/foo/t____";
        ASSERT_EQUAL(DataType::Trade, ds->parsedDataType_)                   << ": datasets[3] data type should be trade";
        ASSERT_EQUAL(DataKind::TradeAperiodic, ds->parsedDataKind_)          << ": datasets[3] data kind should be trade aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[3] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[3] time frame should be aperiodic";

        ds = dataEnumeratorVector[4].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[4] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[4] should have valid name";
        ASSERT_EQUAL("s_m03", ds->name_.c_str())                             << ": datasets[4] name should be s_m03";
        ASSERT_EQUAL("/d/e/bar/s_m03", ds->path_.c_str())                    << ": datasets[4] path should be /d/e/bar/s_m03";
        ASSERT_EQUAL(DataType::Scalar, ds->parsedDataType_)                  << ": datasets[4] data type should be scalar";
        ASSERT_EQUAL(DataKind::ScalarMinute3, ds->parsedDataKind_)           << ": datasets[4] data kind should be scalar minute 3";
        ASSERT_EQUAL(ScalarKind::Default, ds->parsedScalarKind_)             << ": datasets[4] scalar kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Minute3, ds->parsedTimeFrame_)           << ": datasets[4] time frame should be minute 3";

        ds = dataEnumeratorVector[5].get();
        ASSERT_IS_TRUE(nullptr == ds->parent_.get())                         << ": datasets[5] parent should be nullptr";
        ASSERT_IS_TRUE(ds->isValidName_)                                     << ": datasets[5] should have valid name";
        ASSERT_EQUAL("T____", ds->name_.c_str())                             << ": datasets[5] name should be T____";
        ASSERT_EQUAL("/d/e/bar/T____", ds->path_.c_str())                    << ": datasets[5] path should be /d/e/bar/T____";
        ASSERT_EQUAL(DataType::TradePriceOnly, ds->parsedDataType_)          << ": datasets[5] data type should be trade price-only";
        ASSERT_EQUAL(DataKind::TradePriceOnlyAperiodic, ds->parsedDataKind_) << ": datasets[5] data kind should be trade price-only aperiodic";
        ASSERT_EQUAL(TradeKind::Default, ds->parsedTradeKind_)               << ": datasets[5] trade kind should be default";
        ASSERT_EQUAL(DataTimeFrame::Aperiodic, ds->parsedTimeFrame_)         << ": datasets[5] time frame should be aperiodic";
        dataEnumeratorVector.clear();
    }

    TESTCASE("when file opened read-write, after renaming existing instrument old path cannot be opened")
    {
        const char* fileName{ "capi_file_when_file_opened_read-write_after_renaming_existing_instrument_old_path_cannot_be_opened.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        h5InterceptErrorStack([](const char* message, bool isError) noexcept
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

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusRenameA = h5FileRenameInstrument(file, "/a", "foobar");
        const auto instrumentHandle = h5FileOpenInstrument(file, "/a", false);
        h5FileClose(file);
        h5InterceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_TRUE(statusRenameA)      << ": statusRenameA should be true";
        ASSERT_NULL(instrumentHandle)      << ": instrumentHandle should be nullptr";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-write, after renaming existing instrument new path can be opened")
    {
        const char* fileName{ "capi_file_when_file_opened_read-write_after_renaming_existing_instrument_new_path_can_be_opened.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusRenameA = h5FileRenameInstrument(file, "/a", "foobar");
        const auto instrumentHandle = h5FileOpenInstrument(file, "/foobar", false);
        const bool isNull = instrumentHandle == nullptr;
        h5InstrumentClose(instrumentHandle);
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_TRUE(statusRenameA) << ": statusRenameA should be true";
        ASSERT_IS_FALSE(isNull)       << ": isNull should be false";
    }

    TESTCASE("when file opened read-write, renaming non-existing instrument is impossible")
    {
        const char* fileName{ "capi_file_when_file_opened_read-write_renaming_non-existing_instrument_is_impossible.h5" };
        createEnumerationDatasets(fileName);

        static std::vector<std::string> logVector{};
        h5InterceptErrorStack([](const char* message, bool isError) noexcept
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

        const H5FileHandle file = h5FileOpenReadWrite(fileName, true);
        const bool statusRename = h5FileRenameInstrument(file, "/foo", "bar");
        h5FileClose(file);
        h5InterceptErrorStack(nullptr);

        remove(fileName);

        ASSERT_IS_FALSE(statusRename)      << ": statusRename should be false";
        ASSERT_IS_TRUE(!logVector.empty()) << ": there should be error messages logged";
        logVector.clear();
    }

    TESTCASE("when file opened read-only, renaming existing instrument is impossible")
    {
        const char* fileName{ "capi_file_when_file_opened_read-only_renaming_existing_instrument_is_impossible.h5" };
        createEnumerationDatasets(fileName);

        const H5FileHandle file = h5FileOpenReadOnly(fileName);
        h5InterceptErrorStack(nullptr);
        const bool statusRename = h5FileRenameInstrument(file, "/a", "foobar");
        h5FileClose(file);

        remove(fileName);

        ASSERT_IS_FALSE(statusRename) << ": statusRename should be false";
    }
}
}
