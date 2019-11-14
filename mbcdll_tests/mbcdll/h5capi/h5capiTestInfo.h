#pragma once

#include <memory>
#include <vector>
#include <string>
#include "h5capi.h"

namespace h5capi {

    class TestGroupInfo;

    class TestObjectInfo
    {
    public:
        std::string name_;
        std::string path_;
        std::shared_ptr<TestGroupInfo> parent_;

        TestObjectInfo(const char* name, std::shared_ptr<TestGroupInfo>& parent);
        TestObjectInfo();
        TestObjectInfo(const TestObjectInfo&) = delete;
        TestObjectInfo(const TestObjectInfo&&) = delete;
        void operator=(const TestObjectInfo&) = delete;
        void operator=(const TestObjectInfo&&) = delete;
        virtual ~TestObjectInfo() = default;
    };

    class TestDataInfo : public TestObjectInfo
    {
    public:
        DataType parsedDataType_{};
        DataKind parsedDataKind_{};
        OhlcvKind parsedOhlcvKind_{};
        ScalarKind parsedScalarKind_{};
        TradeKind parsedTradeKind_{};
        QuoteKind parsedQuoteKind_{};
        DataTimeFrame parsedTimeFrame_{};
        bool isValidName_{};

        TestDataInfo(const char* name, std::shared_ptr<TestGroupInfo>& parent, DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind,
            ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName);
        TestDataInfo(const TestDataInfo& other) = delete;
        TestDataInfo(const TestDataInfo&& other) = delete;
        void operator=(const TestDataInfo& other) = delete;
        void operator=(const TestDataInfo&&) = delete;
        ~TestDataInfo() = default;
    };

    class TestGroupInfo : public TestObjectInfo
    {
    public:
        std::vector<std::shared_ptr<TestDataInfo>> datasets_;
        std::vector<std::shared_ptr<TestGroupInfo>> groups_;

        TestGroupInfo(const char* name, std::shared_ptr<TestGroupInfo>& parent);
        TestGroupInfo();
        TestGroupInfo(const TestGroupInfo& other) = delete;
        TestGroupInfo(const TestGroupInfo&& other) = delete;
        void operator=(const TestGroupInfo& other) = delete;
        void operator=(const TestGroupInfo&&) = delete;
        ~TestGroupInfo() = default;
    };

}
