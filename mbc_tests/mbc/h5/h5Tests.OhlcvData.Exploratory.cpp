#include "mbc/testharness.h"
#include "mbc/h5.h"
#include "h5Tests.Common.h"

namespace h5 { namespace m5 = mbc::h5; namespace th = mbc::testharness;

using File = m5::File;
using Instrument = m5::Instrument;
using DuplicateTimeTicks = m5::DuplicateTimeTicks;
using OhlcvData = m5::OhlcvData;
using Ohlcv = m5::Ohlcv;

std::string to_string(const Ohlcv& ohlcv)
{
    std::ostringstream oss;
    oss << "t:" << ohlcv.ticks_ << ", o:" << ohlcv.open_ << ", h:" << ohlcv.high_ << ", l:" << ohlcv.low_ << ", c:" << ohlcv.close_ << ", v:" << ohlcv.volume_;
    return oss.str();
}

TESTGROUP("h5 exploratory ohlcv data")
{
    TESTCASE_WITH_CONTEXT("data count, first ticks, last ticks")
    {
        const char* fileName{ "exploratory_ohlcv_dataCount_firstTicks_lastTicks.h5" };
        remove(fileName);

        const char* step{ "create file" };
        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        ASSERT_NOT_NULL(file.get()) << step << ": File::openReadWrite should return non-null file";

        step = "create instrument";
        std::shared_ptr<Instrument> instrument = file->openInstrument("/a/b/c/bar", true);
        ASSERT_NOT_NULL(instrument.get()) << step << ": file->openInstrument should return non-null instrument";

        step = "create ohlcv data";
        std::shared_ptr<OhlcvData> data = instrument->openOhlcvData(OhlcvKind::Default, DataTimeFrame::Aperiodic, true);
        ASSERT_NOT_NULL(data) << step << ": instrument->openOhlcvData should return non-null data";

        step = "empty data";
        verifyProperties(data, 0, 0, 0, step, context);

        step = "add initial data";
        std::vector<Ohlcv> ohlcvs
        {
            { 87, 0.87, 0.87, 0.87, 0.87, 87 },
            { 88, 0.88, 0.88, 0.88, 0.88, 88 },
            { 89, 0.89, 0.89, 0.89, 0.89, 89 },
            { 90, 0.90, 0.90, 0.90, 0.90, 90 },
            { 91, 0.91, 0.91, 0.91, 0.91, 91 }
        };
        add(ohlcvs, data, DuplicateTimeTicks::Fail, false, true, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "filled data";
        verifyProperties(data, ohlcvs.size(), ohlcvs.front().ticks_, ohlcvs.back().ticks_, step, context);

        data.reset();
        instrument.reset();
        file.reset();
        remove(fileName);
    }

    TESTCASE_WITH_CONTEXT("add, update, prepend, append, insert with different DuplicateTimeTicks strategies")
    {
        const char* fileName{ "exploratory_ohlcv_add_update_prepend_append_insert.h5" };
        remove(fileName);

        const char* step{ "create file" };
        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        ASSERT_NOT_NULL(file.get()) << step << ": File::openReadWrite should return non-null file";

        step = "create instrument";
        std::shared_ptr<Instrument> instrument = file->openInstrument("/a/b/c/bar", true);
        ASSERT_NOT_NULL(instrument.get()) << step << ": file->openInstrument should return non-null instrument";

        step = "create ohlcv data";
        std::shared_ptr<OhlcvData> data = instrument->openOhlcvData(OhlcvKind::Default, DataTimeFrame::Aperiodic, true);
        ASSERT_NOT_NULL(data) << step << ": instrument->openOhlcvData should return non-null data";

        step = "add initial";
        std::vector<Ohlcv> ohlcvs
        {
            {100, 1.0, 1.0, 1.0, 1.0, 100.0},
            {110, 1.1, 1.1, 1.1, 1.1, 110.0},
            {130, 1.3, 1.3, 1.3, 1.3, 130.0},
            {150, 1.5, 1.5, 1.5, 1.5, 150.0}
        };
        add(ohlcvs, data, DuplicateTimeTicks::Update, false, true, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update first, DuplicateTimeTicks::Update";
        std::vector<Ohlcv> update1
        {
            {100, 1.01, 1.01, 1.01, 1.01, 101}
        };
        add(update1, data, DuplicateTimeTicks::Update, false, true, step, context);
        ohlcvs[0] = update1[0];
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update first and middle, DuplicateTimeTicks::Update";
        std::vector<Ohlcv> update2
        {
            { 100, 1.0, 1.0, 1.0, 1.0, 100 },
            { 130, 1.31, 1.31, 1.31, 1.31, 131 }
        };
        add(update2, data, DuplicateTimeTicks::Update, false, true, step, context);
        ohlcvs[0] = update2[0];
        ohlcvs[2] = update2[1];
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update middle and last, DuplicateTimeTicks::Update";
        std::vector<Ohlcv> update3
        {
            { 130, 1.3, 1.3, 1.3, 1.3, 130 },
            { 150, 1.51, 1.51, 1.51, 1.51, 151 }
        };
        add(update3, data, DuplicateTimeTicks::Update, false, true, step, context);
        ohlcvs[2] = update3[0];
        ohlcvs[3] = update3[1];
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update last, DuplicateTimeTicks::Update";
        std::vector<Ohlcv> update4
        {
            { 150, 1.5, 1.5, 1.5, 1.5, 150 }
        };
        add(update4, data, DuplicateTimeTicks::Update, false, true, step, context);
        ohlcvs[3] = update4[0];
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update first, DuplicateTimeTicks::Skip";
        add(update1, data, DuplicateTimeTicks::Skip, false, true, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update first and middle, DuplicateTimeTicks::Skip";
        add(update2, data, DuplicateTimeTicks::Skip, false, true, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update middle and last, DuplicateTimeTicks::Skip";
        add(update3, data, DuplicateTimeTicks::Skip, false, true, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update last, DuplicateTimeTicks::Skip";
        add(update4, data, DuplicateTimeTicks::Skip, false, true, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update first, DuplicateTimeTicks::Fail";
        add(update1, data, DuplicateTimeTicks::Fail, false, false, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update first and middle, DuplicateTimeTicks::Fail";
        add(update2, data, DuplicateTimeTicks::Fail, false, false, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update middle and last, DuplicateTimeTicks::Fail";
        add(update3, data, DuplicateTimeTicks::Fail, false, false, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "update last, DuplicateTimeTicks::Fail"; // This will add equal values, hence the return value will be true.
        add(update4, data, DuplicateTimeTicks::Fail, false, true, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "prepend unequal ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update5
        {
            { 97, 0.97, 0.97, 0.97, 0.97, 97 },
            { 98, 0.98, 0.98, 0.98, 0.98, 98 },
            { 99, 0.99, 0.99, 0.99, 0.99, 99 }
        };
        add(update5, data, DuplicateTimeTicks::Fail, false, true, step, context);
        ohlcvs.insert(ohlcvs.begin(), update5.begin(), update5.end());
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "prepend equal ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update6
        {
            { 94, 0.94, 0.94, 0.94, 0.94, 94 },
            { 94, 0.95, 0.95, 0.95, 0.95, 95 },
            { 94, 0.96, 0.96, 0.96, 0.96, 96 }
        };
        add(update6, data, DuplicateTimeTicks::Fail, true, true, step, context);
        ohlcvs.insert(ohlcvs.begin(), update6.begin(), update6.end());
        ohlcvs[1].ticks_ = 95;
        ohlcvs[2].ticks_ = 96;
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "prepend equal first ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update7
        {
            { 90, 0.90, 0.90, 0.90, 0.90, 90 },
            { 90, 0.91, 0.91, 0.91, 0.91, 91 },
            { 90, 0.92, 0.92, 0.92, 0.92, 92 },
            { 93, 0.93, 0.93, 0.93, 0.93, 93 }
        };
        add(update7, data, DuplicateTimeTicks::Fail, true, true, step, context);
        ohlcvs.insert(ohlcvs.begin(), update7.begin(), update7.end());
        ohlcvs[1].ticks_ = 91;
        ohlcvs[2].ticks_ = 92;
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "prepend equal last ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update8
        {
            { 87, 0.87, 0.87, 0.87, 0.87, 87 },
            { 88, 0.88, 0.88, 0.88, 0.88, 88 },
            { 88, 0.89, 0.89, 0.89, 0.89, 89 }
        };
        add(update8, data, DuplicateTimeTicks::Fail, true, true, step, context);
        ohlcvs.insert(ohlcvs.begin(), update8.begin(), update8.end());
        ohlcvs[2].ticks_ = 89;
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "append equal ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update9
        {
            { 151, 1.51, 1.51, 1.51, 1.51, 151 },
            { 151, 1.52, 1.52, 1.52, 1.52, 152 },
            { 151, 1.53, 1.53, 1.53, 1.53, 153 }
        };
        add(update9, data, DuplicateTimeTicks::Fail, true, true, step, context);
        ohlcvs.insert(ohlcvs.end(), update9.begin(), update9.end());
        ohlcvs[18].ticks_ = 152;
        ohlcvs[19].ticks_ = 153;
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "append unequal ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update10
        {
            { 154, 1.54, 1.54, 1.54, 1.54, 154 },
            { 155, 1.55, 1.55, 1.55, 1.55, 155 },
            { 156, 1.56, 1.56, 1.56, 1.56, 156 }
        };
        add(update10, data, DuplicateTimeTicks::Fail, false, true, step, context);
        ohlcvs.insert(ohlcvs.end(), update10.begin(), update10.end());
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "append equal first ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update11
        {
            { 157, 1.57, 1.57, 1.57, 1.57, 157 },
            { 157, 1.58, 1.58, 1.58, 1.58, 158 },
            { 159, 1.59, 1.59, 1.59, 1.59, 159 }
        };
        add(update11, data, DuplicateTimeTicks::Fail, true, true, step, context);
        ohlcvs.insert(ohlcvs.end(), update11.begin(), update11.end());
        ohlcvs[24].ticks_ = 158;
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "append equal last ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update12
        {
            { 160, 1.60, 1.60, 1.60, 1.60, 160 },
            { 161, 1.61, 1.61, 1.61, 1.61, 161 },
            { 161, 1.62, 1.62, 1.62, 1.62, 162 }
        };
        add(update12, data, DuplicateTimeTicks::Fail, true, true, step, context);
        ohlcvs.insert(ohlcvs.end(), update12.begin(), update12.end());
        ohlcvs[28].ticks_ = 162;
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "insert single block of unique ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update13
        {
            { 131, 1.31, 1.31, 1.31, 1.31, 131 },
            { 132, 1.32, 1.32, 1.32, 1.32, 132 },
            { 133, 1.33, 1.33, 1.33, 1.33, 133 }
        };
        add(update13, data, DuplicateTimeTicks::Fail, false, true, step, context);
        ohlcvs.insert(ohlcvs.begin() + 16, update13.begin(), update13.end());
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "insert two blocks of unique ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update14
        {
            { 127, 1.27, 1.27, 1.27, 1.27, 127 },
            { 128, 1.28, 1.28, 1.28, 1.28, 128 },
            { 129, 1.29, 1.29, 1.29, 1.29, 129 },
            { 134, 1.34, 1.34, 1.34, 1.34, 134 },
            { 135, 1.35, 1.35, 1.35, 1.35, 135 },
            { 136, 1.36, 1.36, 1.36, 1.36, 136 }
        };
        add(update14, data, DuplicateTimeTicks::Fail, false, true, step, context);
        ohlcvs.insert(ohlcvs.begin() + 15, update14.begin(), update14.begin() + 3);
        ohlcvs.insert(ohlcvs.begin() + 22, update14.begin() + 3, update14.end());
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        step = "insert four blocks of both duplicate and unique ticks, DuplicateTimeTicks::Fail";
        std::vector<Ohlcv> update15
        {
            { 146, 1.46, 1.46, 1.46, 1.46, 146 },
            { 147, 1.47, 1.47, 1.47, 1.47, 147 },
            { 148, 1.48, 1.48, 1.48, 1.48, 148 },
            { 149, 1.49, 1.49, 1.49, 1.49, 149 },
            { 150, 1.50, 1.50, 1.50, 1.50, 150 },
            { 153, 1.53, 1.53, 1.53, 1.53, 153 },
            { 154, 1.54, 1.54, 1.54, 1.54, 154 },
            { 155, 1.55, 1.55, 1.55, 1.55, 155 },
            { 156, 1.56, 1.56, 1.56, 1.56, 156 }
        };
        add(update15, data, DuplicateTimeTicks::Fail, false, true, step, context);
        ohlcvs.insert(ohlcvs.begin() + 25, update15.begin(), update15.begin() + 4);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        data.reset();
        instrument.reset();
        file.reset();
        remove(fileName);
    }

    TESTCASE_WITH_CONTEXT("fetch tick range with custom MaximumReadBufferBytes")
    {
        const char* fileName{ "exploratory_ohlcv_fetchTickRange_with_custom_maximumReadBufferBytes.h5" };
        remove(fileName);

        const char* step{ "create file" };
        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        ASSERT_NOT_NULL(file.get()) << step << ": File::openReadWrite should return non-null file";

        step = "create instrument";
        std::shared_ptr<Instrument> instrument = file->openInstrument("/a/b/c/bar", true);
        ASSERT_NOT_NULL(instrument.get()) << step << ": file->openInstrument should return non-null instrument";

        step = "create ohlcv data";
        std::shared_ptr<OhlcvData> data = instrument->openOhlcvData(OhlcvKind::Default, DataTimeFrame::Aperiodic, true);
        ASSERT_NOT_NULL(data.get()) << step << ": instrument->openOhlcvData should return non-null data";

        step = "add initial";
        std::vector<Ohlcv> ohlcvs
        {
            { 87, 0.87, 0.87, 0.87, 0.87, 87 },
            { 88, 0.88, 0.88, 0.88, 0.88, 88 },
            { 89, 0.89, 0.89, 0.89, 0.89, 89 },
            { 90, 0.90, 0.90, 0.90, 0.90, 90 },
            { 91, 0.91, 0.91, 0.91, 0.91, 91 },
            { 92, 0.92, 0.92, 0.92, 0.92, 92 },
            { 93, 0.93, 0.93, 0.93, 0.93, 93 },
            { 94, 0.94, 0.94, 0.94, 0.94, 94 },
            { 95, 0.95, 0.95, 0.95, 0.95, 95 },
            { 96, 0.96, 0.96, 0.96, 0.96, 96 },
            { 97, 0.97, 0.97, 0.97, 0.97, 97 },
            { 98, 0.98, 0.98, 0.98, 0.98, 98 },
            { 99, 0.99, 0.99, 0.99, 0.99, 99 },
            { 100, 1.00, 1.00, 1.00, 1.00, 100 },
            { 110, 1.10, 1.10, 1.10, 1.10, 110 },
            { 127, 1.27, 1.27, 1.27, 1.27, 127 },
            { 128, 1.28, 1.28, 1.28, 1.28, 128 },
            { 129, 1.29, 1.29, 1.29, 1.29, 129 },
            { 130, 1.30, 1.30, 1.30, 1.30, 130 },
            { 131, 1.31, 1.31, 1.31, 1.31, 131 },
            { 132, 1.32, 1.32, 1.32, 1.32, 132 },
            { 133, 1.33, 1.33, 1.33, 1.33, 133 },
            { 134, 1.34, 1.34, 1.34, 1.34, 134 },
            { 135, 1.35, 1.35, 1.35, 1.35, 135 },
            { 136, 1.36, 1.36, 1.36, 1.36, 136 },
            { 146, 1.46, 1.46, 1.46, 1.46, 146 },
            { 147, 1.47, 1.47, 1.47, 1.47, 147 },
            { 148, 1.48, 1.48, 1.48, 1.48, 148 },
            { 149, 1.49, 1.49, 1.49, 1.49, 149 },
            { 150, 1.50, 1.50, 1.50, 1.50, 150 },
            { 151, 1.51, 1.51, 1.51, 1.51, 151 },
            { 152, 1.52, 1.52, 1.52, 1.52, 152 },
            { 153, 1.53, 1.53, 1.53, 1.53, 153 },
            { 154, 1.54, 1.54, 1.54, 1.54, 154 },
            { 155, 1.55, 1.55, 1.55, 1.55, 155 }
        };
        add(ohlcvs, data, DuplicateTimeTicks::Update, false, true, step, context);
        verifyFetchAll(ohlcvs.data(), ohlcvs.size(), data, step, context);

        data->setMaximumReadBufferBytes(3);

        step = "fetch range outside left";
        std::vector<Ohlcv> range1
        {
        };
        verifyFetchTickRange(range1.data(), range1.size(), data, 10, 20, step, context);

        step = "fetch inner range";
        std::vector<Ohlcv> range2
        {
            { 130, 1.30, 1.30, 1.30, 1.30, 130 },
            { 131, 1.31, 1.31, 1.31, 1.31, 131 },
            { 132, 1.32, 1.32, 1.32, 1.32, 132 },
            { 133, 1.33, 1.33, 1.33, 1.33, 133 },
            { 134, 1.34, 1.34, 1.34, 1.34, 134 },
            { 135, 1.35, 1.35, 1.35, 1.35, 135 },
            { 136, 1.36, 1.36, 1.36, 1.36, 136 },
            { 146, 1.46, 1.46, 1.46, 1.46, 146 },
            { 147, 1.47, 1.47, 1.47, 1.47, 147 },
            { 148, 1.48, 1.48, 1.48, 1.48, 148 },
            { 149, 1.49, 1.49, 1.49, 1.49, 149 },
            { 150, 1.50, 1.50, 1.50, 1.50, 150 }
        };
        verifyFetchTickRange(range2.data(), range2.size(), data, 130, 150, step, context);

        step = "fetch overlapping left range";
        std::vector<Ohlcv> range3
        {
            { 87, 0.87, 0.87, 0.87, 0.87, 87 },
            { 88, 0.88, 0.88, 0.88, 0.88, 88 },
            { 89, 0.89, 0.89, 0.89, 0.89, 89 },
            { 90, 0.90, 0.90, 0.90, 0.90, 90 },
            { 91, 0.91, 0.91, 0.91, 0.91, 91 },
            { 92, 0.92, 0.92, 0.92, 0.92, 92 },
            { 93, 0.93, 0.93, 0.93, 0.93, 93 },
            { 94, 0.94, 0.94, 0.94, 0.94, 94 },
            { 95, 0.95, 0.95, 0.95, 0.95, 95 },
            { 96, 0.96, 0.96, 0.96, 0.96, 96 },
            { 97, 0.97, 0.97, 0.97, 0.97, 97 },
            { 98, 0.98, 0.98, 0.98, 0.98, 98 },
            { 99, 0.99, 0.99, 0.99, 0.99, 99 },
            { 100, 1.00, 1.00, 1.00, 1.00, 100 },
            { 110, 1.10, 1.10, 1.10, 1.10, 110 },
            { 127, 1.27, 1.27, 1.27, 1.27, 127 },
            { 128, 1.28, 1.28, 1.28, 1.28, 128 },
            { 129, 1.29, 1.29, 1.29, 1.29, 129 },
            { 130, 1.30, 1.30, 1.30, 1.30, 130 }
        };
        verifyFetchTickRange(range3.data(), range3.size(), data, 0, 130, step, context);

        step = "fetch overlapping right range";
        std::vector<Ohlcv> range4
        {
            { 146, 1.46, 1.46, 1.46, 1.46, 146 },
            { 147, 1.47, 1.47, 1.47, 1.47, 147 },
            { 148, 1.48, 1.48, 1.48, 1.48, 148 },
            { 149, 1.49, 1.49, 1.49, 1.49, 149 },
            { 150, 1.50, 1.50, 1.50, 1.50, 150 },
            { 151, 1.51, 1.51, 1.51, 1.51, 151 },
            { 152, 1.52, 1.52, 1.52, 1.52, 152 },
            { 153, 1.53, 1.53, 1.53, 1.53, 153 },
            { 154, 1.54, 1.54, 1.54, 1.54, 154 },
            { 155, 1.55, 1.55, 1.55, 1.55, 155 }
        };
        verifyFetchTickRange(range4.data(), range4.size(), data, 145, 999, step, context);

        step = "fetch encompassing range";
        verifyFetchTickRange(ohlcvs.data(), ohlcvs.size(), data, 50, 190, step, context);

        step = "fetch range outside right";
        verifyFetchTickRange(range1.data(), range1.size(), data, 160, 190, step, context);

        step = "fetch empty inner range";
        verifyFetchTickRange(range1.data(), range1.size(), data, 137, 145, step, context);

        step = "fetch empty inner range where first ticks equals last ticks";
        std::vector<Ohlcv> range5
        {
            { 136, 1.36, 1.36, 1.36, 1.36, 136 }
        };
        verifyFetchTickRange(range5.data(), range5.size(), data, 136, 136, step, context);

        step = "fetch empty inner range where first ticks less than last ticks";
        verifyFetchTickRange(range1.data(), range1.size(), data, 145, 137, step, context);

        data.reset();
        instrument.reset();
        file.reset();
        remove(fileName);
    }
}

}
