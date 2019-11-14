#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>

#include "mbc/numerics/random/mersenneTwisterSfmt.h"
#include "mbc/testharness.h"

template <class T>
class MersenneTwisterSfmtTests
{
public:
    explicit MersenneTwisterSfmtTests(const char* groupName)
    {
        // ReSharper disable CppDeclaratorNeverUsed
        th::registrar reg01("fillArray32MinimalLength: conformance to reference implementation",
            groupName, th::auto_timeout, LOC, &fillArray32MinimalLength);
        th::registrar reg02("fillArray64MinimalLength: conformance to reference implementation",
            groupName, th::auto_timeout, LOC, &fillArray64MinimalLength);
        th::registrar reg03("nextUint: conformance to reference implementation, single seed",
            groupName, th::auto_timeout, LOC, &nextUint_singleSeed);
        th::registrar reg04("nextUint: conformance to reference implementation, array seed",
            groupName, th::auto_timeout, LOC, &nextUint_arraySeed);
        th::registrar reg05("nextUint64: conformance to reference implementation, single seed",
            groupName, th::auto_timeout, LOC, &nextUint64_singleSeed);
        th::registrar reg06("nextUint64: conformance to reference implementation, array seed",
            groupName, th::auto_timeout, LOC, &nextUint64_arraySeed);
        th::registrar reg07("fillArray32: conformance to reference implementation, single seed",
            groupName, th::auto_timeout, LOC, &fillArray32_singleSeed);
        th::registrar reg08("fillArray32: conformance to reference implementation, array seed",
            groupName, th::auto_timeout, LOC, &fillArray32_arraySeed);
        th::registrar reg09("fillArray64: conformance to reference implementation, single seed",
            groupName, th::auto_timeout, LOC, &fillArray64_singleSeed);
        th::registrar reg10("fillArray64: conformance to reference implementation, array seed",
            groupName, th::auto_timeout, LOC, &fillArray64_arraySeed);
        th::registrar reg11("nextDouble: conformance to reference implementation, single seed",
            groupName, th::auto_timeout, LOC, &nextDouble_singleSeed);
        th::registrar reg12("nextDouble: conformance to reference implementation, array seed",
            groupName, th::auto_timeout, LOC, &nextDouble_arraySeed);
        th::registrar reg13("nextDouble64: conformance to reference implementation, single seed",
            groupName, th::auto_timeout, LOC, &nextDouble64_singleSeed);
        th::registrar reg14("nextDouble64: conformance to reference implementation, array seed",
            groupName, th::auto_timeout, LOC, &nextDouble64_arraySeed);
        // ReSharper restore CppDeclaratorNeverUsed
    }
private:
    static std::string indexToString(const int index)
    {
        return "[" + std::to_string(index) + "]";
    }

    static unsigned int maxUlp(const unsigned int value, const bool ignoreRoundoffErrors)
    {
        return ignoreRoundoffErrors ? value : 0;
    }

    static void fillArray32MinimalLength(const th::test_context&)
    {
        ASSERT_EQUAL(referenceImplementation_minArrayLength32_, T::fillArray32MinimalLength);
    }

    static void fillArray64MinimalLength(const th::test_context&)
    {
        ASSERT_EQUAL(referenceImplementation_minArrayLength64_, T::fillArray64MinimalLength);
    }

    static void nextUint_singleSeed(const th::test_context&)
    {
        T t(referenceImplementation_singleSeed_uint32_);
        for (int i = 0; i < referenceImplementation_singleSeed_uint32_dataLength_; ++i)
        {
            const unsigned int expected{ referenceImplementation_singleSeed_uint32_data_[i] };
            const unsigned int actual{ t.nextUint() };
            ASSERT_EQUAL(expected, actual) << indexToString(i);
        }
    }

    static void nextUint_arraySeed(const th::test_context&)
    {
        T t(referenceImplementation_arraySeed_uint32(), referenceImplementation_arraySeed_uint32_length_);
        for (int i = 0; i < referenceImplementation_arraySeed_uint32_dataLength_; ++i)
        {
            const unsigned int expected{ referenceImplementation_arraySeed_uint32_data_[i] };
            const unsigned int actual{ t.nextUint() };
            ASSERT_EQUAL(expected, actual) << indexToString(i);
        }
    }

    static void nextUint64_singleSeed(const th::test_context&)
    {
        T t(referenceImplementation_singleSeed_uint64_);
        for (int i = 0; i < referenceImplementation_singleSeed_uint64_dataLength_; ++i)
        {
            const unsigned long long expected{ referenceImplementation_singleSeed_uint64_data_[i] };
            const unsigned long long actual{ t.nextUint64() };
            ASSERT_EQUAL(expected, actual) << indexToString(i);
        }

    }

    static void nextUint64_arraySeed(const th::test_context&)
    {
        T t(referenceImplementation_arraySeed_uint64(), referenceImplementation_arraySeed_uint64_length_);
        for (int i = 0; i < referenceImplementation_arraySeed_uint64_dataLength_; ++i)
        {
            const unsigned long long expected{ referenceImplementation_arraySeed_uint64_data_[i] };
            const unsigned long long actual{ t.nextUint64() };
            ASSERT_EQUAL(expected, actual) << indexToString(i);
        }
    }

    static void fillArray32_singleSeed(const th::test_context&)
    {
        T t(referenceImplementation_singleSeed_uint32_);
        int arrayLength{ arrayLength32_ };
        if (arrayLength > referenceImplementation_singleSeed_uint32_dataLength_)
            arrayLength = referenceImplementation_singleSeed_uint32_dataLength_;
        unsigned int array32[arrayLength32_];
        t.fillArray32(array32, arrayLength32_);
        ASSERT_EQUAL_ARRAY(arrayLength, referenceImplementation_singleSeed_uint32_data_, array32);
    }

    static void fillArray32_arraySeed(const th::test_context&)
    {
        T t(referenceImplementation_arraySeed_uint32(), referenceImplementation_arraySeed_uint32_length_);
        int arrayLength{ arrayLength32_ };
        if (arrayLength > referenceImplementation_arraySeed_uint32_dataLength_)
            arrayLength = referenceImplementation_arraySeed_uint32_dataLength_;
        unsigned int array32[arrayLength32_];
        t.fillArray32(array32, arrayLength32_);
        ASSERT_EQUAL_ARRAY(arrayLength, referenceImplementation_arraySeed_uint32_data_, array32);
    }

    static void fillArray64_singleSeed(const th::test_context&)
    {
        T t(referenceImplementation_singleSeed_uint64_);
        int arrayLength{ arrayLength64_ };
        if (arrayLength > referenceImplementation_singleSeed_uint64_dataLength_)
            arrayLength = referenceImplementation_singleSeed_uint64_dataLength_;
        unsigned long long array64[arrayLength64_];
        t.fillArray64(array64, arrayLength64_);
        ASSERT_EQUAL_ARRAY(arrayLength, referenceImplementation_singleSeed_uint64_data_, array64);
    }

    static void fillArray64_arraySeed(const th::test_context&)
    {
        T t(referenceImplementation_arraySeed_uint64(), referenceImplementation_arraySeed_uint64_length_);
        int arrayLength{ arrayLength64_ };
        if (arrayLength > referenceImplementation_arraySeed_uint64_dataLength_)
            arrayLength = referenceImplementation_arraySeed_uint64_dataLength_;
        unsigned long long array64[arrayLength64_];
        t.fillArray64(array64, arrayLength64_);
        ASSERT_EQUAL_ARRAY(arrayLength, referenceImplementation_arraySeed_uint64_data_, array64);
    }

    static void nextDouble_singleSeed(const th::test_context& context)
    {
        const unsigned int ulp{ maxUlp(referenceImplementation_singleSeed_double3232_maxUlp_,
            context.ignore_round_off_errors) };
        T t(referenceImplementation_singleSeed_uint32_);
        for (int i = 0; i < referenceImplementation_singleSeed_double3232_dataLength_; ++i)
        {
            const double expected{ referenceImplementation_singleSeed_double3232_data_[i] };
            const double actual{ t.nextDouble() };
            ASSERT_EQUAL_FP(expected, actual, static_cast<size_t>(ulp)) << indexToString(i);
        }
    }

    static void nextDouble_arraySeed(const th::test_context& context)
    {
        const unsigned int ulp{ maxUlp(referenceImplementation_arraySeed_double3232_maxUlp_,
            context.ignore_round_off_errors) };
        T t(referenceImplementation_arraySeed_uint32(), referenceImplementation_arraySeed_uint32_length_);
        for (int i = 0; i < referenceImplementation_arraySeed_double3232_dataLength_; ++i)
        {
            const double expected{ referenceImplementation_arraySeed_double3232_data_[i] };
            const double actual{ t.nextDouble() };
            ASSERT_EQUAL_FP(expected, actual, static_cast<size_t>(ulp)) << indexToString(i);
        }
    }

    static void nextDouble64_singleSeed(const th::test_context& context)
    {
        const unsigned int ulp{ maxUlp(referenceImplementation_singleSeed_double64_maxUlp_,
            context.ignore_round_off_errors) };
        T t(referenceImplementation_singleSeed_uint64_);
        for (int i = 0; i < referenceImplementation_singleSeed_double64_dataLength_; ++i)
        {
            const double expected{ referenceImplementation_singleSeed_double64_data_[i] };
            const double actual{ t.nextDouble64() };
            ASSERT_EQUAL_FP(expected, actual, static_cast<size_t>(ulp)) << indexToString(i);
        }
    }

    static void nextDouble64_arraySeed(const th::test_context& context)
    {
        const unsigned int ulp{ maxUlp(referenceImplementation_arraySeed_double64_maxUlp_,
            context.ignore_round_off_errors) };
        T t(referenceImplementation_arraySeed_uint64(), referenceImplementation_arraySeed_uint64_length_);
        for (int i = 0; i < referenceImplementation_arraySeed_double64_dataLength_; ++i)
        {
            const double expected{ referenceImplementation_arraySeed_double64_data_[i] };
            const double actual{ t.nextDouble64() };
            ASSERT_EQUAL_FP(expected, actual, static_cast<size_t>(ulp)) << indexToString(i);
        }

    }

    static const int  referenceImplementation_singleSeed_uint32_{ 1234 };
    static const int  referenceImplementation_singleSeed_uint64_{ 4321 };
    static const int  referenceImplementation_arraySeed_uint32_length_{ 4 };
    static const int* referenceImplementation_arraySeed_uint32()
    {
        static int data[4]{ 0x1234, 0x5678, 0x9abc, 0xdef0 };
        return data;
    }
    static const int  referenceImplementation_arraySeed_uint64_length_{ 5 };
    static const int* referenceImplementation_arraySeed_uint64()
    {
        static int data[5]{ 5, 4, 3, 2, 1 };
        return data;
    }
    static const int  referenceImplementation_minArrayLength32_;
    static const int  referenceImplementation_minArrayLength64_;
    static const int  arrayLength32_;
    static const int  arrayLength64_;

    static const unsigned int referenceImplementation_singleSeed_uint32_data_[];
    static const int          referenceImplementation_singleSeed_uint32_dataLength_;

    static const unsigned int referenceImplementation_arraySeed_uint32_data_[];
    static const int          referenceImplementation_arraySeed_uint32_dataLength_;

    static const double       referenceImplementation_singleSeed_double3232_data_[];
    static const int          referenceImplementation_singleSeed_double3232_dataLength_;
    static const unsigned int referenceImplementation_singleSeed_double3232_maxUlp_;

    static const double       referenceImplementation_arraySeed_double3232_data_[];
    static const int          referenceImplementation_arraySeed_double3232_dataLength_;
    static const unsigned int referenceImplementation_arraySeed_double3232_maxUlp_;

    static const unsigned long long referenceImplementation_singleSeed_uint64_data_[];
    static const int                referenceImplementation_singleSeed_uint64_dataLength_;

    static const unsigned long long referenceImplementation_arraySeed_uint64_data_[];
    static const int                referenceImplementation_arraySeed_uint64_dataLength_;

    static const double       referenceImplementation_singleSeed_double64_data_[];
    static const int          referenceImplementation_singleSeed_double64_dataLength_;
    static const unsigned int referenceImplementation_singleSeed_double64_maxUlp_;

    static const double       referenceImplementation_arraySeed_double64_data_[];
    static const int          referenceImplementation_arraySeed_double64_dataLength_;
    static const unsigned int referenceImplementation_arraySeed_double64_maxUlp_;
};
