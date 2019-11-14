#include "mbc/testharness.h"
#include "h5capi.h"

namespace h5capi { namespace th = mbc::testharness;

TESTGROUP("h5 capi data")
{
    // ---------------------------------------------------------------------------
    // Default maximum read buffer bytes
    // ---------------------------------------------------------------------------

    TESTCASE("default maximum read buffer bytes has correct value")
    {
        ASSERT_EQUAL(1024UL * 1024UL, h5DataGetDefaultMaximumReadBufferBytes()) << ": default";
    }

    TESTCASE("when set, default maximum read buffer bytes has correct value")
    {
        const auto valueNew = 123UL;
        const auto valueOld{ h5DataGetDefaultMaximumReadBufferBytes() };

        h5DataSetDefaultMaximumReadBufferBytes(valueNew);

        ASSERT_EQUAL(valueNew, h5DataGetDefaultMaximumReadBufferBytes()) << ": default";
        h5DataSetDefaultMaximumReadBufferBytes(valueOld);
    }

}
}
