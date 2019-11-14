#include "mbc/testharness.h"
#include "mbc/h5.h"
#include "h5Tests.Common.h"

namespace h5 { namespace m5 = mbc::h5; namespace th = mbc::testharness;

using File = m5::File;
using Instrument = m5::Instrument;
using DuplicateTimeTicks = m5::DuplicateTimeTicks;
using ScalarData = m5::ScalarData;
using Scalar = m5::Scalar;

std::string to_string(const Scalar& scalar)
{
    std::ostringstream oss;
    oss << "t:" << scalar.ticks_ << ", p:" << scalar.value_;
    return oss.str();
}

TESTGROUP("h5 exploratory scalar data")
{
    TESTCASE_WITH_CONTEXT("data count, first ticks, last ticks")
    {
        const char* fileName{ "exploratory_scalar_dataCount_firstTicks_lastTicks.h5" };
        remove(fileName);

        const char* step{ "create file" };
        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        ASSERT_NOT_NULL(file.get()) << step << ": File::openReadWrite should return non-null file";

        step = "create instrument";
        std::shared_ptr<Instrument> instrument = file->openInstrument("/a/b/c/bar", true);
        ASSERT_NOT_NULL(instrument.get()) << step << ": file->openInstrument should return non-null instrument";

        step = "create trade price-only data";
        std::shared_ptr<ScalarData> data = instrument->openScalarData(DataTimeFrame::Aperiodic, true);
        ASSERT_NOT_NULL(data) << step << ": instrument->openScalarData should return non-null data";

        step = "empty data";
        verifyProperties(data, 0, 0, 0, step, context);

        step = "add initial data";
        std::vector<Scalar> scalars
        {
            { 87, 0.87 },
            { 88, 0.88 },
            { 89, 0.89 },
            { 90, 0.90 },
            { 91, 0.91 }
        };
        add(scalars, data, DuplicateTimeTicks::Fail, false, true, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "filled data";
        verifyProperties(data, scalars.size(), scalars.front().ticks_, scalars.back().ticks_, step, context);

        data.reset();
        instrument.reset();
        file.reset();
        remove(fileName);
    }

    TESTCASE_WITH_CONTEXT("add, update, prepend, append, insert with different DuplicateTimeTicks strategies")
    {
        const char* fileName{ "exploratory_scalar_add_update_prepend_append_insert.h5" };
        remove(fileName);

        const char* step{ "create file" };
        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        ASSERT_NOT_NULL(file.get()) << step << ": File::openReadWrite should return non-null file";

        step = "create instrument";
        std::shared_ptr<Instrument> instrument = file->openInstrument("/a/b/c/bar", true);
        ASSERT_NOT_NULL(instrument.get()) << step << ": file->openInstrument should return non-null instrument";

        step = "create trade price-only data";
        std::shared_ptr<ScalarData> data = instrument->openScalarData(DataTimeFrame::Aperiodic, true);
        ASSERT_NOT_NULL(data) << step << ": instrument->openScalarData should return non-null data";

        step = "add initial";
        std::vector<Scalar> scalars
        {
            {100, 1.0},
            {110, 1.1},
            {130, 1.3},
            {150, 1.5}
        };
        add(scalars, data, DuplicateTimeTicks::Update, false, true, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update first, DuplicateTimeTicks::Update";
        std::vector<Scalar> update1
        {
            {100, 1.01}
        };
        add(update1, data, DuplicateTimeTicks::Update, false, true, step, context);
        scalars[0] = update1[0];
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update first and middle, DuplicateTimeTicks::Update";
        std::vector<Scalar> update2
        {
            { 100, 1.0 },
            { 130, 1.31 }
        };
        add(update2, data, DuplicateTimeTicks::Update, false, true, step, context);
        scalars[0] = update2[0];
        scalars[2] = update2[1];
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update middle and last, DuplicateTimeTicks::Update";
        std::vector<Scalar> update3
        {
            { 130, 1.3 },
            { 150, 1.51 }
        };
        add(update3, data, DuplicateTimeTicks::Update, false, true, step, context);
        scalars[2] = update3[0];
        scalars[3] = update3[1];
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update last, DuplicateTimeTicks::Update";
        std::vector<Scalar> update4
        {
            { 150, 1.5 }
        };
        add(update4, data, DuplicateTimeTicks::Update, false, true, step, context);
        scalars[3] = update4[0];
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update first, DuplicateTimeTicks::Skip";
        add(update1, data, DuplicateTimeTicks::Skip, false, true, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update first and middle, DuplicateTimeTicks::Skip";
        add(update2, data, DuplicateTimeTicks::Skip, false, true, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update middle and last, DuplicateTimeTicks::Skip";
        add(update3, data, DuplicateTimeTicks::Skip, false, true, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update last, DuplicateTimeTicks::Skip";
        add(update4, data, DuplicateTimeTicks::Skip, false, true, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update first, DuplicateTimeTicks::Fail";
        add(update1, data, DuplicateTimeTicks::Fail, false, false, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update first and middle, DuplicateTimeTicks::Fail";
        add(update2, data, DuplicateTimeTicks::Fail, false, false, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update middle and last, DuplicateTimeTicks::Fail";
        add(update3, data, DuplicateTimeTicks::Fail, false, false, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "update last, DuplicateTimeTicks::Fail"; // This will add equal values, hence the return value will be true.
        add(update4, data, DuplicateTimeTicks::Fail, false, true, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "prepend unequal ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update5
        {
            { 97, 0.97 },
            { 98, 0.98 },
            { 99, 0.99 }
        };
        add(update5, data, DuplicateTimeTicks::Fail, false, true, step, context);
        scalars.insert(scalars.begin(), update5.begin(), update5.end());
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "prepend equal ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update6
        {
            { 94, 0.94 },
            { 94, 0.95 },
            { 94, 0.96 }
        };
        add(update6, data, DuplicateTimeTicks::Fail, true, true, step, context);
        scalars.insert(scalars.begin(), update6.begin(), update6.end());
        scalars[1].ticks_ = 95;
        scalars[2].ticks_ = 96;
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "prepend equal first ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update7
        {
            { 90, 0.90 },
            { 90, 0.91 },
            { 90, 0.92 },
            { 93, 0.93 }
        };
        add(update7, data, DuplicateTimeTicks::Fail, true, true, step, context);
        scalars.insert(scalars.begin(), update7.begin(), update7.end());
        scalars[1].ticks_ = 91;
        scalars[2].ticks_ = 92;
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "prepend equal last ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update8
        {
            { 87, 0.87 },
            { 88, 0.88 },
            { 88, 0.89 }
        };
        add(update8, data, DuplicateTimeTicks::Fail, true, true, step, context);
        scalars.insert(scalars.begin(), update8.begin(), update8.end());
        scalars[2].ticks_ = 89;
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "append equal ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update9
        {
            { 151, 1.51 },
            { 151, 1.52 },
            { 151, 1.53 }
        };
        add(update9, data, DuplicateTimeTicks::Fail, true, true, step, context);
        scalars.insert(scalars.end(), update9.begin(), update9.end());
        scalars[18].ticks_ = 152;
        scalars[19].ticks_ = 153;
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "append unequal ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update10
        {
            { 154, 1.54 },
            { 155, 1.55 },
            { 156, 1.56 }
        };
        add(update10, data, DuplicateTimeTicks::Fail, false, true, step, context);
        scalars.insert(scalars.end(), update10.begin(), update10.end());
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "append equal first ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update11
        {
            { 157, 1.57 },
            { 157, 1.58 },
            { 159, 1.59 }
        };
        add(update11, data, DuplicateTimeTicks::Fail, true, true, step, context);
        scalars.insert(scalars.end(), update11.begin(), update11.end());
        scalars[24].ticks_ = 158;
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "append equal last ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update12
        {
            { 160, 1.60 },
            { 161, 1.61 },
            { 161, 1.62 }
        };
        add(update12, data, DuplicateTimeTicks::Fail, true, true, step, context);
        scalars.insert(scalars.end(), update12.begin(), update12.end());
        scalars[28].ticks_ = 162;
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "insert single block of unique ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update13
        {
            { 131, 1.31 },
            { 132, 1.32 },
            { 133, 1.33 }
        };
        add(update13, data, DuplicateTimeTicks::Fail, false, true, step, context);
        scalars.insert(scalars.begin() + 16, update13.begin(), update13.end());
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "insert two blocks of unique ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update14
        {
            { 127, 1.27 },
            { 128, 1.28 },
            { 129, 1.29 },
            { 134, 1.34 },
            { 135, 1.35 },
            { 136, 1.36 }
        };
        add(update14, data, DuplicateTimeTicks::Fail, false, true, step, context);
        scalars.insert(scalars.begin() + 15, update14.begin(), update14.begin() + 3);
        scalars.insert(scalars.begin() + 22, update14.begin() + 3, update14.end());
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        step = "insert four blocks of both duplicate and unique ticks, DuplicateTimeTicks::Fail";
        std::vector<Scalar> update15
        {
            { 146, 1.46 },
            { 147, 1.47 },
            { 148, 1.48 },
            { 149, 1.49 },
            { 150, 1.50 },
            { 153, 1.53 },
            { 154, 1.54 },
            { 155, 1.55 },
            { 156, 1.56 }
        };
        add(update15, data, DuplicateTimeTicks::Fail, false, true, step, context);
        scalars.insert(scalars.begin() + 25, update15.begin(), update15.begin() + 4);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        data.reset();
        instrument.reset();
        file.reset();
        remove(fileName);
    }

    TESTCASE_WITH_CONTEXT("fetch tick range with custom MaximumReadBufferBytes")
    {
        const char* fileName{ "exploratory_scalar_fetchTickRange_with_custom_maximumReadBufferBytes.h5" };
        remove(fileName);

        const char* step{ "create file" };
        std::shared_ptr<File> file = File::openReadWrite(fileName, true);
        ASSERT_NOT_NULL(file.get()) << step << ": File::openReadWrite should return non-null file";

        step = "create instrument";
        std::shared_ptr<Instrument> instrument = file->openInstrument("/a/b/c/bar", true);
        ASSERT_NOT_NULL(instrument.get()) << step << ": file->openInstrument should return non-null instrument";

        step = "create trade price-only data";
        std::shared_ptr<ScalarData> data = instrument->openScalarData(DataTimeFrame::Aperiodic, true);
        ASSERT_NOT_NULL(data.get()) << step << ": instrument->openScalarData should return non-null data";

        step = "add initial";
        std::vector<Scalar> scalars
        {
            { 87, 0.87 },
            { 88, 0.88 },
            { 89, 0.89 },
            { 90, 0.90 },
            { 91, 0.91 },
            { 92, 0.92 },
            { 93, 0.93 },
            { 94, 0.94 },
            { 95, 0.95 },
            { 96, 0.96 },
            { 97, 0.97 },
            { 98, 0.98 },
            { 99, 0.99 },
            { 100, 1.00 },
            { 110, 1.10 },
            { 127, 1.27 },
            { 128, 1.28 },
            { 129, 1.29 },
            { 130, 1.30 },
            { 131, 1.31 },
            { 132, 1.32 },
            { 133, 1.33 },
            { 134, 1.34 },
            { 135, 1.35 },
            { 136, 1.36 },
            { 146, 1.46 },
            { 147, 1.47 },
            { 148, 1.48 },
            { 149, 1.49 },
            { 150, 1.50 },
            { 151, 1.51 },
            { 152, 1.52 },
            { 153, 1.53 },
            { 154, 1.54 },
            { 155, 1.55 }
        };
        add(scalars, data, DuplicateTimeTicks::Update, false, true, step, context);
        verifyFetchAll(scalars.data(), scalars.size(), data, step, context);

        data->setMaximumReadBufferBytes(3);

        step = "fetch range outside left";
        std::vector<Scalar> range1
        {
        };
        verifyFetchTickRange(range1.data(), range1.size(), data, 10, 20, step, context);

        step = "fetch inner range";
        std::vector<Scalar> range2
        {
            { 130, 1.30 },
            { 131, 1.31 },
            { 132, 1.32 },
            { 133, 1.33 },
            { 134, 1.34 },
            { 135, 1.35 },
            { 136, 1.36 },
            { 146, 1.46 },
            { 147, 1.47 },
            { 148, 1.48 },
            { 149, 1.49 },
            { 150, 1.50 }
        };
        verifyFetchTickRange(range2.data(), range2.size(), data, 130, 150, step, context);

        step = "fetch overlapping left range";
        std::vector<Scalar> range3
        {
            { 87, 0.87 },
            { 88, 0.88 },
            { 89, 0.89 },
            { 90, 0.90 },
            { 91, 0.91 },
            { 92, 0.92 },
            { 93, 0.93 },
            { 94, 0.94 },
            { 95, 0.95 },
            { 96, 0.96 },
            { 97, 0.97 },
            { 98, 0.98 },
            { 99, 0.99 },
            { 100, 1.00 },
            { 110, 1.10 },
            { 127, 1.27 },
            { 128, 1.28 },
            { 129, 1.29 },
            { 130, 1.30 }
        };
        verifyFetchTickRange(range3.data(), range3.size(), data, 0, 130, step, context);

        step = "fetch overlapping right range";
        std::vector<Scalar> range4
        {
            { 146, 1.46 },
            { 147, 1.47 },
            { 148, 1.48 },
            { 149, 1.49 },
            { 150, 1.50 },
            { 151, 1.51 },
            { 152, 1.52 },
            { 153, 1.53 },
            { 154, 1.54 },
            { 155, 1.55 }
        };
        verifyFetchTickRange(range4.data(), range4.size(), data, 145, 999, step, context);

        step = "fetch encompassing range";
        verifyFetchTickRange(scalars.data(), scalars.size(), data, 50, 190, step, context);

        step = "fetch range outside right";
        verifyFetchTickRange(range1.data(), range1.size(), data, 160, 190, step, context);

        step = "fetch empty inner range";
        verifyFetchTickRange(range1.data(), range1.size(), data, 137, 145, step, context);

        step = "fetch empty inner range where first ticks equals last ticks";
        std::vector<Scalar> range5
        {
            { 136, 1.36 }
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
