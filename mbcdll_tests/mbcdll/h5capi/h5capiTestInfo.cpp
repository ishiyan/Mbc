#include "h5capiTestInfo.h"

namespace h5capi {

    // ----------------------------------------------------------------------
    // TestObjectInfo
    // ----------------------------------------------------------------------

    TestObjectInfo::TestObjectInfo(const char* name, std::shared_ptr<TestGroupInfo>& parent) :
        name_{ name },
        path_{parent == nullptr ? name : parent->path_.c_str() },
        parent_{ parent }
    {
        if (parent != nullptr)
        {
            path_ += "/";
            path_ += name;
        }
    }

    TestObjectInfo::TestObjectInfo() :
        name_{ "/" },
        path_{ "" },
        parent_{ nullptr }
    {
    }

    // ----------------------------------------------------------------------
    // TestDataInfo
    // ----------------------------------------------------------------------

    TestDataInfo::TestDataInfo(const char* name, std::shared_ptr<TestGroupInfo>& parent, DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind,
        ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName) :
        TestObjectInfo(name, parent),
        parsedDataType_{ dataType },
        parsedDataKind_{ dataKind },
        parsedOhlcvKind_{ ohlcvKind },
        parsedScalarKind_{ scalarKind },
        parsedTradeKind_{ tradeKind },
        parsedQuoteKind_{ quoteKind },
        parsedTimeFrame_{ timeFrame },
        isValidName_{ isValidName }
    {
    }

    // ----------------------------------------------------------------------
    // TestGroupInfo
    // ----------------------------------------------------------------------

    TestGroupInfo::TestGroupInfo(const char* name, std::shared_ptr<TestGroupInfo>& parent) :
        TestObjectInfo(name, parent)
    {
    }

    TestGroupInfo::TestGroupInfo() :
        TestObjectInfo()
    {
    }

}