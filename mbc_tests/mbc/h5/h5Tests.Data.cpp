#include "mbc/testharness.h"
#include "mbc/h5.h"

namespace h5 { namespace m5 = mbc::h5;

using Data = m5::Data;

TESTGROUP("h5 data")
{
    // ---------------------------------------------------------------------------
    // Default maximum read buffer bytes
    // ---------------------------------------------------------------------------

    TESTCASE("default maximum read buffer bytes has correct value")
    {
        ASSERT_EQUAL(1024UL * 1024UL, Data::defaultMaximumReadBufferBytes()) << ": default";
    }

    TESTCASE("when set, default maximum read buffer bytes has correct value")
    {
        const auto valueNew = 123UL;
        const auto valueOld{ Data::defaultMaximumReadBufferBytes() };

        Data::setDefaultMaximumReadBufferBytes(valueNew);

        ASSERT_EQUAL(valueNew, Data::defaultMaximumReadBufferBytes()) << ": default";
        Data::setDefaultMaximumReadBufferBytes(valueOld);
    }

}
}
