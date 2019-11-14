#pragma once
#include "mbc/testharness.h"
#include "mbc/h5.h"

namespace h5 { namespace m5 = mbc::h5;

using File = m5::File;
using Instrument = m5::Instrument;
using DataTimeFrame = m5::DataTimeFrame;
using OhlcvKind = m5::OhlcvKind;
using OhlcvData = m5::OhlcvData;
using QuoteData = m5::QuoteData;
using ScalarData = m5::ScalarData;
using TradeData = m5::TradeData;
using TradePriceOnlyData = m5::TradePriceOnlyData;
using DuplicateTimeTicks = m5::DuplicateTimeTicks;

void createEnumerationDatasets(const char* fileName);
void createEnumerationDatasetsInReverseOrder(const char* fileName);
const char* to_string(DuplicateTimeTicks duplicateTimeTicks);

    std::string to_string(const mbc::h5::Ohlcv& ohlcv);
std::string to_string(const mbc::h5::OhlcvPriceOnly& ohlcv);
std::string to_string(const mbc::h5::Quote& ohlcv);
std::string to_string(const mbc::h5::QuotePriceOnly& ohlcv);
std::string to_string(const mbc::h5::Trade& ohlcv);
std::string to_string(const mbc::h5::TradePriceOnly& ohlcv);
std::string to_string(const mbc::h5::Scalar& ohlcv);

template<typename T>
bool verifyProperties(std::shared_ptr<T>& data, hsize_t count, long long firstTicks, long long lastTicks, const char* prefix, const th::test_context& context)
{
    const hsize_t expectedCount{ data->count() };
    EXPECT_EQUAL(expectedCount, count) << prefix;

    const long long expectedFirstTicks{ data->firstTicks() };
    EXPECT_EQUAL(expectedFirstTicks, firstTicks) << prefix;

    const long long expectedLastTicks{ data->lastTicks() };
    EXPECT_EQUAL(expectedLastTicks, lastTicks) << prefix;

    return expectedCount == count && expectedFirstTicks == firstTicks && expectedLastTicks == lastTicks;
}

template<typename T>
bool verify(std::vector<T>& sample, const T* pattern, const size_t patternLength, const char* prefix, const th::test_context& context)
{
    const char* verificationError{ ": verification error: " };
    const size_t sampleLength{ sample.size() };
    if (sampleLength == patternLength)
    {
        for (size_t i = 0; i < sampleLength; ++i)
        {
            if (sample[i].ticks_ != pattern[i].ticks_)
            {
                std::ostringstream msg;
                msg << std::endl << prefix << verificationError << "index " << i << std::endl;
                for (size_t j = 0; j < sampleLength; ++j)
                    msg << prefix << verificationError << j << (j == i ? " * (" : "   (") << to_string(sample[j]) << ")<-(" << to_string(pattern[j]) << ")" << std::endl;
                EXPECT_FAIL() << msg.str();
                return false;
            }
        }
        return true;
    }
    std::ostringstream msg;
    msg << std::endl << prefix << verificationError << "stored data length:" << sampleLength << ", pattern length:" << patternLength << std::endl;
    const size_t k{ patternLength > sampleLength ? patternLength : sampleLength };
    for (size_t j = 0; j < k; ++j)
    {
        if (sampleLength > j&& patternLength > j)
            msg << prefix << verificationError << j << "(" << to_string(sample[j]) << ")<-(" << to_string(pattern[j]) << ")" << std::endl;
        else if (sampleLength <= j)
            msg << prefix << verificationError << j << "()<-(" << to_string(pattern[j]) << ")" << std::endl;
        else //if (patternLength <= j)
            msg << prefix << verificationError << j << "(" << to_string(sample[j]) << ")<-()" << std::endl;
    }
    EXPECT_FAIL() << msg.str();
    return false;
}

template<typename T, typename Y>
bool verifyFetchAll(const T* pattern, size_t patternLength, std::shared_ptr<Y>& data, const char* prefix, const th::test_context& context)
{
    std::vector<T> sample{};
    if (!data->fetchAll([&sample](const T* ptr, long long count) { while (count-- > 0) sample.push_back(*ptr++); }))
        return false;
    return verify(sample, pattern, patternLength, prefix, context);
}

template<typename T, typename Y>
bool verifyFetchTickRange(const T* pattern, size_t patternLength, std::shared_ptr<Y>& data, long long fromTicks, long long toTicks, const char* prefix, const th::test_context& context)
{
    std::vector<T> sample{};
    if (!data->fetchTickRange([&sample](const T* ptr, long long count) { while (count-- > 0) sample.push_back(*ptr++); }, fromTicks, toTicks))
        return false;
    return verify(sample, pattern, patternLength, prefix, context);
}

template<typename T, typename Y>
bool add(std::vector<T>& vector, std::shared_ptr<Y>& data, DuplicateTimeTicks duplicateTimeTicks, const bool resultOfSpread, const bool resultOfAddition, const char* prefix, const th::test_context& context)
{
    const char* spreadShouldReturn{ ": data->spreadDuplicateTimeTicks should return " };

    const bool result1{ data->spreadDuplicateTimeTicks(vector, true) };
    if (resultOfSpread)
        EXPECT_IS_TRUE(result1) << prefix << spreadShouldReturn << "true";
    else
        EXPECT_IS_FALSE(result1) << prefix << spreadShouldReturn << "false";

    const char* dataAdd{ ": data->add(" };
    const char* shouldReturn{ ") should return " };

    const bool result2{ data->add(vector.data(), vector.size(), duplicateTimeTicks, true) };
    if (resultOfAddition)
        EXPECT_IS_TRUE(result2) << prefix << dataAdd << to_string(duplicateTimeTicks) << shouldReturn << "true";
    else
        EXPECT_IS_FALSE(result2) << prefix << dataAdd << to_string(duplicateTimeTicks) << shouldReturn << "false";
    return !result1 && result2;
}

}
