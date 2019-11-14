#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <memory>
#include <mutex>
#include <sstream>
#include <utility>
#include <vector>

#ifdef assert
#undef assert
#endif

#ifndef _Comma
// ReSharper disable once CppInconsistentNaming
#define _Comma ,
#endif

#define LOC th::location(__FILE__, __LINE__)

#define ASSERT_EQUAL_FP(expected, actual, max_ulp)     th::assert.equal(expected _Comma actual _Comma max_ulp _Comma LOC)
#define EXPECT_EQUAL_FP(expected, actual, max_ulp) context.expect.equal(expected _Comma actual _Comma max_ulp _Comma LOC)
#define WARN_EQUAL_FP(expected, actual, max_ulp)     context.warn.equal(expected _Comma actual _Comma max_ulp _Comma LOC)

#define ASSERT_EQUAL_FP_ARRAY(array_length, array_expected, array_actual, max_ulp)     th::assert.equal_array(array_length _Comma array_expected _Comma array_actual _Comma max_ulp _Comma LOC)
#define EXPECT_EQUAL_FP_ARRAY(array_length, array_expected, array_actual, max_ulp) context.expect.equal_array(array_length _Comma array_expected _Comma array_actual _Comma max_ulp _Comma LOC)
#define WARN_EQUAL_FP_ARRAY(array_length, array_expected, array_actual, max_ulp)     context.warn.equal_array(array_length _Comma array_expected _Comma array_actual _Comma max_ulp _Comma LOC)

#define ASSERT_EQUAL(expected, actual)     th::assert.equal(expected _Comma actual _Comma LOC)
#define EXPECT_EQUAL(expected, actual) context.expect.equal(expected _Comma actual _Comma LOC)
#define WARN_EQUAL(expected, actual)     context.warn.equal(expected _Comma actual _Comma LOC)

#define ASSERT_EQUAL_ARRAY(array_length, array_expected, array_actual)     th::assert.equal_array(array_length _Comma array_expected _Comma array_actual _Comma LOC)
#define EXPECT_EQUAL_ARRAY(array_length, array_expected, array_actual) context.expect.equal_array(array_length _Comma array_expected _Comma array_actual _Comma LOC)
#define WARN_EQUAL_ARRAY(array_length, array_expected, array_actual)     context.warn.equal_array(array_length _Comma array_expected _Comma array_actual _Comma LOC)

#define ASSERT_EQUAL_CMP(expected, actual, comparer)     th::assert.equal(expected _Comma actual _Comma comparer _Comma LOC)
#define EXPECT_EQUAL_CMP(expected, actual, comparer) context.expect.equal(expected _Comma actual _Comma comparer _Comma LOC)
#define WARN_EQUAL_CMP(expected, actual, comparer)     context.warn.equal(expected _Comma actual _Comma comparer _Comma LOC)

#define ASSERT_EQUAL_ARRAY_CMP(array_length, array_expected, array_actual, comparer)     th::assert.equal_array(array_length _Comma array_expected _Comma array_actual _Comma comparer _Comma LOC)
#define EXPECT_EQUAL_ARRAY_CMP(array_length, array_expected, array_actual, comparer) context.expect.equal_array(array_length _Comma array_expected _Comma array_actual _Comma comparer _Comma LOC)
#define WARN_EQUAL_ARRAY_CMP(array_length, array_expected, array_actual, comparer)     context.warn.equal_array(array_length _Comma array_expected _Comma array_actual _Comma comparer _Comma LOC)

#define ASSERT_EQUAL_SEQ(expectedBegin, expectedEnd, actualBegin, actualEnd)     th::assert.equal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma LOC)
#define EXPECT_EQUAL_SEQ(expectedBegin, expectedEnd, actualBegin, actualEnd) context.expect.equal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma LOC)
#define WARN_EQUAL_SEQ(expectedBegin, expectedEnd, actualBegin, actualEnd)     context.warn.equal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma LOC)

#define ASSERT_EQUAL_SEQ_CMP(expectedBegin, expectedEnd, actualBegin, actualEnd, comparer)     th::assert.equal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma comparer _Comma LOC)
#define EXPECT_EQUAL_SEQ_CMP(expectedBegin, expectedEnd, actualBegin, actualEnd, comparer) context.expect.equal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma comparer _Comma LOC)
#define WARN_EQUAL_SEQ_CMP(expectedBegin, expectedEnd, actualBegin, actualEnd, comparer)     context.warn.equal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma comparer _Comma LOC)

#define ASSERT_UNEQUAL_FP(expected, actual, max_ulp)     th::assert.unequal(expected _Comma actual _Comma max_ulp _Comma LOC)
#define EXPECT_UNEQUAL_FP(expected, actual, max_ulp) context.expect.unequal(expected _Comma actual _Comma max_ulp _Comma LOC)
#define WARN_UNEQUAL_FP(expected, actual, max_ulp)     context.warn.unequal(expected _Comma actual _Comma max_ulp _Comma LOC)

#define ASSERT_UNEQUAL_FP_ARRAY(array_length, array_expected, array_actual, max_ulp)     th::assert.unequal_array(array_length _Comma array_expected _Comma array_actual _Comma max_ulp _Comma LOC)
#define EXPECT_UNEQUAL_FP_ARRAY(array_length, array_expected, array_actual, max_ulp) context.expect.unequal_array(array_length _Comma array_expected _Comma array_actual _Comma max_ulp _Comma LOC)
#define WARN_UNEQUAL_FP_ARRAY(array_length, array_expected, array_actual, max_ulp)     context.warn.unequal_array(array_length _Comma array_expected _Comma array_actual _Comma max_ulp _Comma LOC)

#define ASSERT_UNEQUAL(expected, actual)     th::assert.unequal(expected _Comma actual _Comma LOC)
#define EXPECT_UNEQUAL(expected, actual) context.expect.unequal(expected _Comma actual _Comma LOC)
#define WARN_UNEQUAL(expected, actual)     context.warn.unequal(expected _Comma actual _Comma LOC)

#define ASSERT_UNEQUAL_ARRAY(array_length, array_expected, array_actual)     th::assert.unequal_array(array_length _Comma array_expected _Comma array_actual _Comma LOC)
#define EXPECT_UNEQUAL_ARRAY(array_length, array_expected, array_actual) context.expect.unequal_array(array_length _Comma array_expected _Comma array_actual _Comma LOC)
#define WARN_UNEQUAL_ARRAY(array_length, array_expected, array_actual)     context.warn.unequal_array(array_length _Comma array_expected _Comma array_actual _Comma LOC)

#define ASSERT_UNEQUAL_CMP(expected, actual, comparer)     th::assert.unequal(expected _Comma actual _Comma comparer _Comma LOC)
#define EXPECT_UNEQUAL_CMP(expected, actual, comparer) context.expect.unequal(expected _Comma actual _Comma comparer _Comma LOC)
#define WARN_UNEQUAL_CMP(expected, actual, comparer)     context.warn.unequal(expected _Comma actual _Comma comparer _Comma LOC)

#define ASSERT_UNEQUAL_ARRAY_CMP(array_length, array_expected, array_actual, comparer)     th::assert.unequal_array(array_length _Comma array_expected _Comma array_actual _Comma comparer _Comma LOC)
#define EXPECT_UNEQUAL_ARRAY_CMP(array_length, array_expected, array_actual, comparer) context.expect.unequal_array(array_length _Comma array_expected _Comma array_actual _Comma comparer _Comma LOC)
#define WARN_UNEQUAL_ARRAY_CMP(array_length, array_expected, array_actual, comparer)     context.warn.unequal_array(array_length _Comma array_expected _Comma array_actual _Comma comparer _Comma LOC)

#define ASSERT_UNEQUAL_SEQ(expectedBegin, expectedEnd, actualBegin, actualEnd)     th::assert.unequal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma LOC)
#define EXPECT_UNEQUAL_SEQ(expectedBegin, expectedEnd, actualBegin, actualEnd) context.expect.unequal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma LOC)
#define WARN_UNEQUAL_SEQ(expectedBegin, expectedEnd, actualBegin, actualEnd)     context.warn.unequal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma LOC)

#define ASSERT_UNEQUAL_SEQ_CMP(expectedBegin, expectedEnd, actualBegin, actualEnd, comparer)     th::assert.unequal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma comparer _Comma LOC)
#define EXPECT_UNEQUAL_SEQ_CMP(expectedBegin, expectedEnd, actualBegin, actualEnd, comparer) context.expect.unequal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma comparer _Comma LOC)
#define WARN_UNEQUAL_SEQ_CMP(expectedBegin, expectedEnd, actualBegin, actualEnd, comparer)     context.warn.unequal_seq(expectedBegin _Comma expectedEnd _Comma actualBegin _Comma actualEnd _Comma comparer _Comma LOC)

#define ASSERT_IS_TRUE(x)     th::assert.is_true(x _Comma LOC)
#define EXPECT_IS_TRUE(x) context.expect.is_true(x _Comma LOC)
#define WARN_IS_TRUE(x)     context.warn.is_true(x _Comma LOC)

#define ASSERT_IS_FALSE(x)     th::assert.is_false(x _Comma LOC)
#define EXPECT_IS_FALSE(x) context.expect.is_false(x _Comma LOC)
#define WARN_IS_FALSE(x)     context.warn.is_false(x _Comma LOC)

#define ASSERT_EMPTY(x)     th::assert.empty(x _Comma LOC)
#define EXPECT_EMPTY(x) context.expect.empty(x _Comma LOC)
#define WARN_EMPTY(x)     context.warn.empty(x _Comma LOC)

#define ASSERT_NOT_EMPTY(x)     th::assert.not_empty(x _Comma LOC)
#define EXPECT_NOT_EMPTY(x) context.expect.not_empty(x _Comma LOC)
#define WARN_NOT_EMPTY(x)     context.warn.not_empty(x _Comma LOC)

#define ASSERT_NULL(x)     th::assert.null(x _Comma LOC)
#define EXPECT_NULL(x) context.expect.null(x _Comma LOC)
#define WARN_NULL(x)     context.warn.null(x _Comma LOC)

#define ASSERT_NOT_NULL(x)     th::assert.not_null(x _Comma LOC)
#define EXPECT_NOT_NULL(x) context.expect.not_null(x _Comma LOC)
#define WARN_NOT_NULL(x)     context.warn.not_null(x _Comma LOC)

#define ASSERT_SAME(x, y)     th::assert.same(x _Comma y _Comma LOC)
#define EXPECT_SAME(x, y) context.expect.same(x _Comma y _Comma LOC)
#define WARN_SAME(x, y)     context.warn.same(x _Comma y _Comma LOC)

#define ASSERT_NOT_SAME(x, y)     th::assert.not_same(x _Comma y _Comma LOC)
#define EXPECT_NOT_SAME(x, y) context.expect.not_same(x _Comma y _Comma LOC)
#define WARN_NOT_SAME(x, y)     context.warn.not_same(x _Comma y _Comma LOC)

#define ASSERT_IN_RANGE(actual, min, max)     th::assert.in_range(actual _Comma min _Comma max _Comma LOC)
#define EXPECT_IN_RANGE(actual, min, max) context.expect.in_range(actual _Comma min _Comma max _Comma LOC)
#define WARN_IN_RANGE(actual, min, max)     context.warn.in_range(actual _Comma min _Comma max _Comma LOC)

#define ASSERT_NOT_IN_RANGE(actual, min, max)     th::assert.not_in_range(actual _Comma min _Comma max _Comma LOC)
#define EXPECT_NOT_IN_RANGE(actual, min, max) context.expect.not_in_range(actual _Comma min _Comma max _Comma LOC)
#define WARN_NOT_IN_RANGE(actual, min, max)     context.warn.not_in_range(actual _Comma min _Comma max _Comma LOC)

#define ASSERT_CONTAINS(sequence, pattern)     th::assert.contains(sequence _Comma pattern _Comma LOC)
#define EXPECT_CONTAINS(sequence, pattern) context.expect.contains(sequence _Comma pattern _Comma LOC)
#define WARN_CONTAINS(sequence, pattern)     context.warn.contains(sequence _Comma pattern _Comma LOC)

#define ASSERT_DOES_NOT_CONTAIN(sequence, pattern)     th::assert.does_not_contain(sequence _Comma pattern _Comma LOC)
#define EXPECT_DOES_NOT_CONTAIN(sequence, pattern) context.expect.does_not_contain(sequence _Comma pattern _Comma LOC)
#define WARN_DOES_NOT_CONTAIN(sequence, pattern)     context.warn.does_not_contain(sequence _Comma pattern _Comma LOC)

#define ASSERT_CONTAINS_PREDICATE(sequence, predicate)     th::assert.contains_predicate(sequence _Comma predicate _Comma LOC)
#define EXPECT_CONTAINS_PREDICATE(sequence, predicate) context.expect.contains_predicate(sequence _Comma predicate _Comma LOC)
#define WARN_CONTAINS_PREDICATE(sequence, predicate)     context.warn.contains_predicate(sequence _Comma predicate _Comma LOC)

#define ASSERT_DOES_NOT_CONTAIN_PREDICATE(sequence, predicate)     th::assert.does_not_contain_predicate(sequence _Comma predicate _Comma LOC)
#define EXPECT_DOES_NOT_CONTAIN_PREDICATE(sequence, predicate) context.expect.does_not_contain_predicate(sequence _Comma predicate _Comma LOC)
#define WARN_DOES_NOT_CONTAIN_PREDICATE(sequence, predicate)     context.warn.does_not_contain_predicate(sequence _Comma predicate _Comma LOC)

#define ASSERT_DOES_NOT_THROW(func)     th::assert.does_not_throw(func _Comma LOC)
#define EXPECT_DOES_NOT_THROW(func) context.expect.does_not_throw(func _Comma LOC)
#define WARN_DOES_NOT_THROW(func)     context.warn.does_not_throw(func _Comma LOC)

#define ASSERT_FAIL()     th::assert.fail(LOC)
#define EXPECT_FAIL() context.expect.fail(LOC)
#define WARN_FAIL()     context.warn.fail(LOC)

#define ASSERT_THROWS(e, func) th::assert.throws<e>([&](){func} _Comma LOC)

#define LOG_DEBUG() context.log.debug(LOC)
#define LOG_INFO()  context.log.info(LOC)
#define LOG_WARN()  context.log.warn(LOC)

#define CAT2(a,b) a ## b
#define CAT1(a,b) CAT2(a,b)
#define UNIQUE_TESTGROUP CAT1(tgr_, __LINE__)
#define UNIQUE_TESTCASE  CAT1(tst_, __LINE__)
#define UNIQUE_REGISTRAR CAT1(reg_, __LINE__)

#define TESTGROUP(name)\
namespace UNIQUE_TESTGROUP\
{\
    static const char* group_name{ name };\
}\
namespace UNIQUE_TESTGROUP

#define TESTCASE(name)\
static void UNIQUE_TESTCASE(const th::test_context &);\
static th::registrar UNIQUE_REGISTRAR(name, group_name, th::auto_timeout, LOC, &UNIQUE_TESTCASE);\
static void UNIQUE_TESTCASE(const th::test_context &)

#define TESTCASE_WITH_CONTEXT(name)\
static void UNIQUE_TESTCASE(const th::test_context &);\
static th::registrar UNIQUE_REGISTRAR(name, group_name, th::auto_timeout, LOC, &UNIQUE_TESTCASE);\
static void UNIQUE_TESTCASE(const th::test_context & context)

namespace mbc::testharness {
    namespace th = testharness;

    template< typename T>
    inline std::string to_string(T value) { return std::to_string(value); }

    // Char would normally be treated like an int, which is probably not what we want.
    template<>
    inline std::string to_string(const char value) { return std::string() + value; }

    // You can't normally call std::to_string with a std::string.
    template<>
    inline std::string to_string(std::string value) { return value; }

    template<>
    inline std::string to_string(std::chrono::nanoseconds value)
    {
        return
            std::to_string(std::chrono::duration_cast<std::chrono::duration<float>>(value).count()) + " sec (" +
            std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(value).count()) + " millisecond, " +
            std::to_string(value.count()) + " nanosecond)";
    }

    struct location
    {
        std::string file_;
        int line_;

        location(std::string file, const int line) :
            file_(std::move(file)), line_{ line } {}
        location(std::string&& file, const int line) :
            file_(std::move(file)), line_{ line } {}
        location(const char* file, const int line) :
            file_(file), line_{ line } {}
        location() :
            line_{0} {}
        ~location() = default;

        location(const location&) = default;
        location(location&& other) noexcept :
            file_(std::move(other.file_)), line_{ other.line_ } {}
        location& operator =(const location&) = default;
        location& operator =(location&& other) noexcept
        {
            if (this != &other)
            {
                file_ = std::move(other.file_);
                line_ = other.line_;
            }
            return *this;
        }
    };

    template<>
    inline std::string to_string(const location& value)
    {
        if (value.file_.empty())
            return std::string();
        return value.file_ + "(" + std::to_string(value.line_) + ")";
    }

    namespace detail
    {
        // Maps a size in bytes to a primitive type with that size.
        template <size_t size> struct type_with_size
        {
            typedef void uns_int;
        };

        // The specialization for size 4.
        template <> struct type_with_size <4>
        {
            typedef int sig_int;
            typedef unsigned int uns_int;
        };

        // The specialization for size 8.
        template <> struct type_with_size <8>
        {
            typedef long long sig_int;
            typedef unsigned long long uns_int;
        };

        // Read the following for more details.
        // http://en.wikipedia.org/wiki/Unit_in_the_last_place
        // http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
        // http://randomascii.wordpress.com/category/floating-point/
        template <typename T> class floating_point
        {
        public:
            // Constructs an instance from a raw floating-point number.
            // On an Intel CPU, passing a non-normalized NAN (Not A Number) around may change its bits,
            // although the new value is guaranteed to be also a NAN.
            // Therefore, don't expect this constructor to preserve the bits in x when x is a NAN.
            explicit floating_point(const T& x) { u_.value_ = x; }

            // How many ULPs (Units in the Last Place) we want to tolerate when comparing two numbers.
            // The larger the value, the more error we allow.
            // A value of 0 means that two numbers must be exactly the same to be considered equal.
            // The maximum error of a single floating-point operation is 0.5 units in the last place.
            // On Intel CPU's, all floating-point calculations are done with 80-bit precision, while double has 64 bits.
            // Therefore, 4 should be enough for ordinary use.
            static const size_t default_max_ulp{ 4 };

            // Returns the floating-point number that represent the positive infinity.
            static T infinity() { return reinterpret_bits(exponent_bit_mask); }

            // Returns true if this is NAN (not a number).
            [[nodiscard]] bool is_nan() const
            {
                // It's a NAN if the exponent bits are all ones and the fraction bits are not entirely zeros.
                // exponent_bits() == exponent_bit_mask && fraction_bits() != 0;
                bits b{ u_.bits_ };
                return (exponent_bit_mask & b) == exponent_bit_mask &&
                    (fraction_bit_mask & u_.bits_) != 0;
            }

            // Returns true if this number is at most max_ulp (Units in the Last Place) away from rhs.
            // In particular, this function:
            // - returns false if either number is (or both are) NAN, as IEEE standard defines;
            // - treats really large numbers as almost equal to infinity;
            // - thinks +0.0 and -0.0 are 0 ULP apart.
            [[nodiscard]] bool almost_equals(const floating_point& rhs, const size_t max_ulp = default_max_ulp) const
            {
                // The IEEE standard says that any comparison operation involving a NAN must return false.
                if (is_nan() || rhs.is_nan())
                    return false;
                return distance_from(rhs) <= max_ulp;
            }

            // Returns the distance from the rhs in ULP (Units in the Last Place).
            [[nodiscard]] size_t distance_from(const floating_point& rhs) const
            {
                return static_cast<size_t>(distance_between_sign_and_magnitude_numbers(u_.bits_, rhs.u_.bits_));
            }

        private:
            // Defines the unsigned integer type that has the same size as the floating point number.
            typedef typename type_with_size<sizeof(T)>::uns_int bits;

            // The number of bits in a number.
            static const size_t bit_count{ 8 * sizeof(T) };

            // The number of fraction bits in a number.
            static const size_t fraction_bit_count{ std::numeric_limits<T>::digits - 1 };

            // The number of exponent bits in a number.
            static const size_t exponent_bit_count{ bit_count - 1 - fraction_bit_count };

            // The mask for the sign bit.
            static const bits sign_bit_mask{ static_cast<bits>(1) << (bit_count - 1) };

            // The mask for the fraction bits.
            static const bits fraction_bit_mask{ ~static_cast<bits>(0) >> (exponent_bit_count + 1) };

            // The mask for the exponent bits.
            static const bits exponent_bit_mask{ ~(sign_bit_mask | fraction_bit_mask) };

            // The data type used to store the actual floating-point number.
            union floating_point_union
            {
                T value_;   // The raw floating-point number.
                bits bits_; // The bits that represent the number.
            } u_;

            // Converts an integer from the sign-and-magnitude representation to the biased representation.
            // More precisely, let N be 2 to the power of (bit_count - 1), an integer x is represented by the unsigned number x + N.
            // For instance,
            //   -N + 1 (the most negative number representable using sign-and-magnitude) is represented by 1;
            //   0      is represented by N; and
            //   N - 1  (the biggest number representable using sign-and-magnitude) is represented by 2N - 1.
            // Read http://en.wikipedia.org/wiki/Signed_number_representations for more details on signed number representations.
            static bits sign_and_magnitude_to_biased(const bits& sam)
            {
                if (sign_bit_mask & sam)
                    return ~sam + 1; // The sam represents a negative number.
                else
                    return sign_bit_mask | sam; // The sam represents a positive number.
            }

            // Given two numbers in the sign-and-magnitude representation, returns the distance between them as an unsigned number.
            static bits distance_between_sign_and_magnitude_numbers(const bits& sam1, const bits& sam2)
            {
                const bits biased1{ sign_and_magnitude_to_biased(sam1) };
                const bits biased2{ sign_and_magnitude_to_biased(sam2) };
                return biased1 >= biased2 ? (biased1 - biased2) : (biased2 - biased1);
            }

            // Reinterprets a bit pattern as a floating-point number.
            static T reinterpret_bits(const bits bits)
            {
                floating_point fp{ 0 };
                fp.u_.bits_ = bits;
                return fp.u_.value_;
            }

            // Returns the exponent bits of this number.
            [[nodiscard]] bits exponent_bits() const { return exponent_bit_mask & u_.bits_; }

            // Returns the fraction bits of this number.
            [[nodiscard]] bits fraction_bits() const { return fraction_bit_mask & u_.bits_; }

            // Returns the sign bit of this number.
            [[nodiscard]] bits sign_bit() const { return sign_bit_mask & u_.bits_; }
        };

        template<typename T> struct has_empty
        {
        private:
            template<typename U, U> class check {};

            template<typename C> static char f(check<bool (C::*)() const, &C::empty> *);

            template<typename C> static long f(...);

        public:
            static const bool value = (sizeof(f<T>(nullptr)) == sizeof(char));
        };

        template<typename T>
        static std::string range_to_string(T begin, T end)
        {
            typedef decltype(*begin) val_type;
            std::string result("[ ");
            std::for_each(std::forward<T>(begin), std::forward<T>(end),
                [&result](val_type val) { result += th::to_string(std::forward<val_type>(val)) + ", "; });
            result[result.size() - 2] = ' ';
            result[result.size() - 1] = ']';
            return result;
        }

        class assert_context
        {
        public:
            assert_context(std::string&& call, location&& loc) noexcept :
                call_(std::move(call)), user_message_{ std::make_shared<std::stringstream>() }, location_{ std::move(loc) } {}

            assert_context(const assert_context&) = default;
            assert_context(assert_context&&) = default;

            assert_context& operator =(const assert_context&) = delete;
            assert_context& operator =(assert_context&&) = delete;
            ~assert_context() = default;

            assert_context& custom_message(std::string&& message) { custom_message_ = std::move(message); return *this; }
            assert_context& expected(std::string&& expected) { expected_ = std::move(expected); return *this; }
            assert_context& actual(std::string&& actual) { actual_ = std::move(actual); return *this; }
            assert_context& append_user_message(const std::string&& user_message) { *user_message_ << user_message; return *this; }
            assert_context& append_user_message(const char* user_message) { *user_message_ << user_message; return *this; }

            const std::string& call()           const { return call_; }
            const std::string& custom_message() const { return custom_message_; }
            const std::string& expected()       const { return expected_; }
            const std::string& actual()         const { return actual_; }
            const location&    located()        const { return location_; }

            static assert_context none() { return assert_context(std::string(), th::location()); }

            const std::string& user_message() const { user_message_string = user_message_->str(); return user_message_string; }

            std::string to_string() const
            {
                std::string message = call_ + " failure";
                const std::string user_message = user_message_->str();
                if (!user_message.empty())
                {
                    message += ": " + user_message;
                    if (!custom_message_.empty())
                        message += "\n" + custom_message_;
                }
                else if (!custom_message_.empty())
                    message += ": " + custom_message_;
                if (!expected_.empty() || !actual_.empty())
                {
                    message += "\nexpected: " + expected_;
                    message += "\nactual  : " + actual_;
                }
                return message;
            }

        private:
            std::string call_;
            std::string custom_message_;
            std::string expected_;
            std::string actual_;
            mutable std::string user_message_string;
            std::shared_ptr<std::stringstream> user_message_;
            location location_;
        };

        class failure
        {
        public:
            failure(assert_context&& assert_context,
                    std::function<void(const detail::assert_context&)> on_failure_complete) :
                on_failure_complete_{std::move(on_failure_complete)}, assert_context_{ std::move(assert_context) } {}

            failure(failure&& other) noexcept :
                on_failure_complete_{ nullptr },
                assert_context_{ std::move(other.assert_context_) } {}

            failure(const failure& other) :
                on_failure_complete_{ nullptr },
                assert_context_{ other.assert_context_ } {}

            failure& operator =(const failure&) = delete;
            failure& operator =(failure&&) = delete;

            ~failure() noexcept(false)
            {
                if (on_failure_complete_ != nullptr)
                    on_failure_complete_(assert_context_);
            }

            static failure none(){ return failure(); }

            template<typename T> failure& operator <<(T&& value)
            {
                assert_context_.append_user_message(std::forward<T>(value));
                return *this;
            }
            template<typename T> failure& operator <<(const T& value)
            {
                assert_context_.append_user_message(value);
                return *this;
            }

        private:
            failure() :
                on_failure_complete_{ nullptr },
                assert_context_{ assert_context::none() } {}

            std::function<void(const assert_context&)> on_failure_complete_;
            assert_context assert_context_;
        };
        template<typename T>
        template<typename C>
        inline char has_empty<T>::f(check<bool(C::*)()const, &C::empty>*)
        {
            return 0;
        }
        template<typename T>
        template<typename C>
        inline long has_empty<T>::f(...)
        {
            return 0;
        }
    }

    class assert
    {
    public:
        explicit assert(std::string&& call_prefix = "assert.",
            std::function<void(const detail::assert_context&)>&& handle_failure =
                [](const detail::assert_context& a) { throw a; }) noexcept :  // NOLINT(cert-err60-cpp, hicpp-exception-baseclass)
            call_prefix_(std::move(call_prefix)), handle_failure_{ std::move(handle_failure) } {}

        assert(const assert&) noexcept = default;
        assert(assert&&) noexcept = default;
        assert& operator =(const assert&) = delete;
        assert& operator =(assert&&) = delete;
        ~assert() = default;

        // equal --------------------------------------------------------------
        template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, detail::failure>::type
            equal(const T expected, const T actual, const size_t max_ulp, location&& loc = location()) const
        {
            detail::floating_point<T>  fp_expected{ expected }, fp_actual{ actual };
            const size_t ulp{ fp_expected.distance_from(fp_actual) };
            if (ulp > max_ulp)
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "equal<" + typeid(T).name() + ", ULP = " + std::to_string(max_ulp) + ">",
                    std::move(loc))
                    .expected(to_string_with_full_precision(expected))
                    .actual(to_string_with_full_precision(actual) + ", ULP distance = " + std::to_string(ulp))));
            }
            return on_success();
        }

        detail::failure equal(const double expected, const float actual, const size_t max_ulp, location&& loc = location()) const
        {
            return equal<float>(static_cast<float>(expected), actual, max_ulp, std::move(loc));
        }

        detail::failure equal(const float expected, const double actual, const size_t max_ulp, location&& loc = location()) const
        {
            return equal<float>(expected, static_cast<float>(actual), max_ulp, std::move(loc));
        }

        template<typename Texp, typename Tact, typename C>
        typename std::enable_if<!(std::is_floating_point<Texp>::value && std::is_floating_point<Tact>::value),
            detail::failure>::type
            equal(Texp&& expected, Tact&& actual, C&& comparer, location&& loc) const
        {
            if (!comparer(std::forward<Texp>(expected), std::forward<Tact>(actual)))
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "equal<" + typeid(Texp).name() + ", " + typeid(Tact).name() + ">",
                    std::move(loc))
                    .expected(th::to_string(std::forward<Texp>(expected)))
                    .actual(th::to_string(std::forward<Tact>(actual)))));
            }
            return on_success();
        }

        template<typename T>
        typename std::enable_if<std::is_enum<T>::value, detail::failure>::type
            equal(const T expected, const T actual, location&& loc = location()) const
        {
            if (expected != actual)
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "equal<" + typeid(T).name() + ">",
                    std::move(loc))
                    .expected(th::to_string(static_cast<int>(expected)))
                    .actual(th::to_string(static_cast<int>(actual)))));
            }
            return on_success();
        }

        template<typename Texp, typename Tact>
        typename std::enable_if<
            std::is_integral<Texp>::value && std::is_integral<Tact>::value,
            detail::failure>::type
            equal(const Texp expected, const Tact actual, location&& loc = location()) const
        {
            if (static_cast<unsigned long long>(expected) != static_cast<unsigned long long>(actual))
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "equal<" + typeid(Texp).name() + ", " + typeid(Tact).name() + ">",
                    std::move(loc))
                    .expected(th::to_string(expected))
                    .actual(th::to_string(actual))));
            }
            return on_success();
        }

        template<typename Texp, typename Tact>
        typename std::enable_if<
            !(std::is_floating_point<Texp>::value && std::is_floating_point<Tact>::value) &&
            !(std::is_integral<Texp>::value && std::is_integral<Tact>::value) &&
            !(std::is_enum<Texp>::value && std::is_enum<Tact>::value) &&
            !(std::is_constructible<std::string, Texp>::value && std::is_constructible<std::string, Tact>::value),
            detail::failure>::type
            equal(Texp expected, Tact actual, location&& loc = location()) const
        {
            return equal(std::forward<Texp>(expected), std::forward<Tact>(actual),
                [](Texp&& exp, Tact&& act) { return exp == act; },
                std::move(loc));
        }

        template<typename Texp, typename Tact>
        typename std::enable_if<
            !(std::is_floating_point<Texp>::value && std::is_floating_point<Tact>::value) &&
            (std::is_constructible<std::string, Texp>::value && std::is_constructible<std::string, Tact>::value),
            detail::failure>::type
            equal(Texp expected, Tact actual, location&& loc = location()) const
        {
            return equal(std::string(std::forward<Texp>(expected)), std::string(std::forward<Tact>(actual)),
                [](std::string&& exp, std::string&& act) { return exp == act; },
                std::move(loc));
        }

        template<typename Texp, typename Tact, typename C>
        detail::failure equal_seq(Texp&& expectedBegin, Texp&& expectedEnd, Tact&& actualBegin, Tact&& actualEnd,
            C&& comparer, location&& loc = location()) const
        {
            auto expected = expectedBegin;
            auto actual = actualBegin;
            size_t index{ 0 };
            while (expected != expectedEnd && actual != actualEnd)
            {
                if (!comparer(*expected, *actual))
                    break;
                ++expected;
                ++actual;
                ++index;
            }
            if (expected != expectedEnd || actual != actualEnd)
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "equal<" + typeid(Texp).name() + ", " + typeid(Tact).name() + ">",
                    std::move(loc))
                    .custom_message("sequence unequal at location " + std::to_string(index))
                    .expected(detail::range_to_string(
                        std::forward<Texp>(expectedBegin), std::forward<Texp>(expectedEnd)))
                    .actual(detail::range_to_string(
                        std::forward<Tact>(actualBegin), std::forward<Tact>(actualEnd)))));
            }
            return on_success();
        }

        template<typename Texp, typename Tact>
        detail::failure equal_seq(Texp&& expectedBegin, Texp&& expectedEnd, Tact&& actualBegin, Tact&& actualEnd,
            location&& loc = location()) const
        {
            return equal_seq(std::forward<Texp>(expectedBegin), std::forward<Texp>(expectedEnd),
                std::forward<Tact>(actualBegin), std::forward<Tact>(actualEnd),
                [](decltype(*expectedBegin)&& a, decltype(*actualBegin)&& b) { return a == b; },
                std::move(loc));
        }

        template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, detail::failure>::type
            equal_array(const int array_length, const T* array_expected, const T* array_actual, const size_t max_ulp,
            location&& loc = location()) const
        {
            std::ostringstream out;
            int failed{ 0 };
            size_t ulp_max{ 0 };
            for (int i = 0; i < array_length; ++i)
            {
                T expected{ array_expected[i] }, actual{ array_actual[i] };
                detail::floating_point<T>  fp_expected{ expected }, fp_actual{ actual };
                const size_t ulp{ fp_expected.distance_from(fp_actual) };
                if (ulp > max_ulp)
                {
                    ++failed;
                    if (ulp_max < ulp)
                        ulp_max = ulp;
                    out << "\nexpected [" << std::to_string(i) << "] " << to_string_with_full_precision(expected);
                    out << "\n  actual [" << std::to_string(i) << "] " << to_string_with_full_precision(actual) << ", ULP distance = " << std::to_string(ulp);
                }
            }
            if (failed > 0)
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "equal<array of " + typeid(T).name() + ", ULP = " + std::to_string(max_ulp) + ">",
                    std::forward<location>(loc))
                    .custom_message(std::forward<std::string>(out.str()) + "\n\nfailed " +
                        std::to_string(failed) + " out of " + std::to_string(array_length) +
                        " (max ULP distance = " + std::to_string(ulp_max) + ")")));
            }
            return on_success();
        }

        template<typename T, typename C>
        typename std::enable_if<!std::is_floating_point<T>::value, detail::failure>::type
            equal_array(const int array_length, const T* array_expected, const T* array_actual, C&& comparer,
            location&& loc = location()) const
        {
            std::ostringstream out;
            int failed{ 0 };
            for (int i = 0; i < array_length; ++i)
            {
                T expected{ array_expected[i] }, actual{ array_actual[i] };
                if (!comparer(std::forward<T>(expected), std::forward<T>(actual)))
                {
                    ++failed;
                    out << "\nexpected [" << std::to_string(i) << "] " << th::to_string(expected);
                    out << "\nactual   [" << std::to_string(i) << "] " << th::to_string(actual);
                }
            }
            if (failed > 0)
            {
                return on_failure(std::move(detail::assert_context(call_prefix_ + "equal<array of " + typeid(T).name() + ">",
                    std::forward<location>(loc))
                    .custom_message(std::forward<std::string>(out.str()) + "\n\nfailed " +
                    std::to_string(failed) + " out of " + std::to_string(array_length))));
            }
            return on_success();
        }

        template<typename T>
        typename std::enable_if<!std::is_floating_point<T>::value, detail::failure>::type
            equal_array(int array_length, const T* array_expected, const T* array_actual,
            location&& loc = location()) const
        {
            return equal_array(array_length, array_expected, array_actual,
                [](T&& expected, T&& actual) { return expected == actual; },
                std::forward<location>(loc));
        }

        // unequal ------------------------------------------------------------
        template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, detail::failure>::type
            unequal(T expected, T actual, const size_t max_ulp, location &&loc = location()) const
        {
            detail::floating_point<T>  fp_expected{ expected }, fp_actual{ actual };
            const size_t ulp{ fp_expected.distance_from(fp_actual) };
            if (ulp <= max_ulp)
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "unequal<" + typeid(T).name() + ", ULP = " + std::to_string(max_ulp) + ">",
                    std::move(loc))
                    .expected(to_string_with_full_precision(std::forward<T>(expected)))
                    .actual(to_string_with_full_precision(std::forward<T>(actual)) + ", ULP distance = " + std::to_string(ulp))));
            }
            return on_success();
        }

        detail::failure unequal(const double expected, const float actual, const size_t max_ulp, location&& loc = location()) const
        {
            return unequal<float>(static_cast<float>(expected), actual, max_ulp, std::move(loc));
        }

        detail::failure unequal(const float expected, const double actual, const size_t max_ulp, location&& loc = location()) const
        {
            return unequal<float>(expected, static_cast<float>(actual), max_ulp, std::move(loc));
        }

        template<typename Texp, typename Tact, typename C>
        typename std::enable_if<!(std::is_floating_point<Texp>::value && std::is_floating_point<Tact>::value),
            detail::failure>::type
            unequal(Texp&& expected, Tact&& actual, C&& comparer, location&& loc = location()) const
        {
            if (comparer(std::forward<Texp>(expected), std::forward<Tact>(actual)))
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "unequal<" + typeid(Texp).name() + ", " + typeid(Tact).name() + ">",
                    std::move(loc))
                    .expected(th::to_string(std::forward<Texp>(expected)))
                    .actual(th::to_string(std::forward<Tact>(actual)))));
            }
            return on_success();
        }

        template<typename Texp, typename Tact>
        typename std::enable_if<
            !(std::is_floating_point<Texp>::value && std::is_floating_point<Tact>::value) &&
            !(std::is_constructible<std::string, Texp>::value && std::is_constructible<std::string, Tact>::value),
            detail::failure>::type
            unequal(Texp expected, Tact actual, location&& loc = location()) const
        {
            return unequal(std::forward<Texp>(expected), std::forward<Tact>(actual),
                [](Texp&& exp, Tact&& act) { return exp == act; },
                std::move(loc));
        }

        template<typename Texp, typename Tact>
        typename std::enable_if<
            !(std::is_floating_point<Texp>::value && std::is_floating_point<Tact>::value) &&
            (std::is_constructible<std::string, Texp>::value && std::is_constructible<std::string, Tact>::value),
            detail::failure>::type
            unequal(Texp expected, Tact actual, location&& loc = location()) const
        {
            return unequal(std::string(std::forward<Texp>(expected)), std::string(std::forward<Tact>(actual)),
                [](std::string&& exp, std::string&& act) { return exp == act; },
                std::move(loc));
        }

        template<typename Texp, typename Tact, typename C>
        detail::failure unequal_seq(Texp&& expectedBegin, Texp&& expectedEnd, Tact&& actualBegin, Tact&& actualEnd,
            C&& comparer, location&& loc = location()) const
        {
            auto expected = expectedBegin;
            auto actual = actualBegin;
            size_t index{ 0 };
            while (expected != expectedEnd && actual != actualEnd)
            {
                if (comparer(*expected, *actual))
                    break;
                ++expected;
                ++actual;
                ++index;
            }
            if (expected != expectedEnd || actual != actualEnd)
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "unequal<" + typeid(Texp).name() + ", " + typeid(Tact).name() + ">",
                    std::move(loc))
                    .custom_message("sequence equal at location " + std::to_string(index))
                    .expected(detail::range_to_string(
                    std::forward<Texp>(expectedBegin), std::forward<Texp>(expectedEnd)))
                    .actual(detail::range_to_string(
                    std::forward<Tact>(actualBegin), std::forward<Tact>(actualEnd)))));
            }
            return on_success();
        }

        template<typename Texp, typename Tact>
        detail::failure unequal_seq(Texp&& expectedBegin, Texp&& expectedEnd, Tact&& actualBegin, Tact&& actualEnd,
            location&& loc = location()) const
        {
            return unequal_seq(std::forward<Texp>(expectedBegin), std::forward<Texp>(expectedEnd),
                std::forward<Tact>(actualBegin), std::forward<Tact>(actualEnd),
                [](decltype(*expectedBegin) && a, decltype(*actualBegin) && b) { return a == b; },
                std::move(loc));
        }

        template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, detail::failure>::type
            unequal_array(const int array_length, const T* array_expected, const T* array_actual, const size_t max_ulp,
            location&& loc = location()) const
        {
            std::ostringstream out;
            int failed{ 0 };
            size_t ulp_max{ 0 };
            for (int i = 0; i < array_length; ++i)
            {
                T expected{ array_expected[i] }, actual{ array_actual[i] };
                detail::floating_point<T>  fp_expected{ expected }, fp_actual{ actual };
                const size_t ulp{ fp_expected.distance_from(fp_actual) };
                if (ulp <= max_ulp)
                {
                    ++failed;
                    if (ulp_max < ulp)
                        ulp_max = ulp;
                    out << "\nexpected [" << std::to_string(i) << "] " << to_string_with_full_precision(expected);
                    out << "\nactual   [" << std::to_string(i) << "] " << to_string_with_full_precision(actual) << ", ULP distance = " << std::to_string(ulp);
                }
            }
            if (failed > 0)
            {
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "equal<array of " + typeid(T).name() + ", ULP = " + std::to_string(max_ulp) + ">",
                    std::forward<location>(loc))
                    .custom_message(std::forward<std::string>(out.str()) + "\n\nfailed " +
                    std::to_string(failed) + " out of " + std::to_string(array_length) +
                    " (max ULP distance =" + std::to_string(ulp_max) + ")")));
            }
            return on_success();
        }

        template<typename T, typename C>
        typename std::enable_if<!std::is_floating_point<T>::value, detail::failure>::type
            unequal_array(const int array_length, const T* array_expected, const T* array_actual, C&& comparer,
            location&& loc = location()) const
        {
            std::ostringstream out;
            int failed{ 0 };
            for (int i = 0; i < array_length; ++i)
            {
                T expected{ array_expected[i] }, actual{ array_actual[i] };
                if (comparer(std::forward<T>(expected), std::forward<T>(actual)))
                {
                    ++failed;
                    out << "\nexpected [" << std::to_string(i) << "] " << th::to_string(expected);
                    out << "\nactual   [" << std::to_string(i) << "] " << th::to_string(actual);
                }
            }
            if (failed > 0)
            {
                return on_failure(std::move(detail::assert_context(call_prefix_ + "unequal<array of " + typeid(T).name() + ">",
                    std::forward<location>(loc))
                    .custom_message(std::forward<std::string>(out.str()) + "\n\nfailed " +
                    std::to_string(failed) + " out of " + std::to_string(array_length))));
            }
            return on_success();
        }

        template<typename T>
        typename std::enable_if<!std::is_floating_point<T>::value, detail::failure>::type
            unequal_array(int array_length, const T* array_expected, const T* array_actual,
            location&& loc = location()) const
        {
            return unequal_array(array_length, array_expected, array_actual,
                [](T&& expected, T&& actual) { return expected == actual; },
                std::forward<location>(loc));
        }

        // is_true ------------------------------------------------------------
        detail::failure is_true(const bool b, location&& loc = location()) const
        {
            if (b)
                return on_success();
            return on_failure(std::move(detail::assert_context(call_prefix_ + "is_true",
                std::move(loc))
                .expected(std::string("true")).actual(std::string("false"))));
        }

        // is_false -----------------------------------------------------------
        detail::failure is_false(const bool b, location&& loc = location()) const
        {
            if (b)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "is_false",
                    std::move(loc))
                    .expected(std::string("false")).actual(std::string("true"))));
            return on_success();
        }

        // empty --------------------------------------------------------------
        template<typename T>
        typename std::enable_if<detail::has_empty<typename std::remove_reference<T>::type>::value, detail::failure>::type
            empty(T&& sequence, location&& loc = location()) const
        {
            if (!sequence.empty())
                return on_failure(std::move(detail::assert_context(call_prefix_ + "empty<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        template<typename T>
        typename std::enable_if<!detail::has_empty<typename std::remove_reference<T>::type>::value, detail::failure>::type
            empty(T&& sequence, location&& loc = location()) const
        {
            if (std::begin(std::forward<T>(sequence)) != std::end(std::forward<T>(sequence)))
                return on_failure(std::move(detail::assert_context(call_prefix_ + "empty<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        // not_empty ----------------------------------------------------------
        template<typename T>
        typename std::enable_if<detail::has_empty<typename std::remove_reference<T>::type>::value, detail::failure>::type
            not_empty(T&& sequence, location&& loc = location()) const
        {
            if (sequence.empty())
                return on_failure(std::move(detail::assert_context(call_prefix_ + "not_empty<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        template<typename T>
        typename std::enable_if<!detail::has_empty<typename std::remove_reference<T>::type>::value, detail::failure>::type
            not_empty(T&& sequence, location&& loc = location()) const
        {
            if (std::begin(std::forward<T>(sequence)) == std::end(std::forward<T>(sequence)))
                return on_failure(std::move(detail::assert_context(call_prefix_ + "not_empty<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        // null ---------------------------------------------------------------
        template<typename T>
        detail::failure null(T&& value, location&& loc = location()) const
        {
            if (value != nullptr)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "null<" + typeid(T).name() + ">",
                    std::move(loc))
                    .expected(std::string("nullptr")).actual(std::string("not nullptr"))));
            return on_success();
        }

        // not_null -----------------------------------------------------------
        template<typename T>
        detail::failure not_null(T&& value, location&& loc = location()) const
        {
            if (value == nullptr)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "not_null<" + typeid(T).name() + ">",
                    std::move(loc))
                    .expected(std::string("not nullptr")).actual(std::string("nullptr"))));
            return on_success();
        }

        // same ---------------------------------------------------------------
        template<typename T>
        detail::failure same(const T& expected, const T& actual, location&& loc = location()) const
        {
            if (&expected != &actual)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "same<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        template<typename T>
        detail::failure same(T* expected, T* actual, location&& loc = location()) const
        {
            if (expected != actual)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "same<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        template<typename T>
        detail::failure same(const T* expected, const T* actual, location&& loc = location()) const
        {
            if (expected != actual)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "same<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        // not_same -----------------------------------------------------------
        template<typename T>
        detail::failure not_same(const T& expected, const T& actual, location&& loc = location()) const
        {
            if (&expected == &actual)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "not_same<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        template<typename T>
        detail::failure not_same(T* expected, T* actual, location&& loc = location()) const
        {
            if (expected == actual)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "not_same<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        template<typename T>
        detail::failure not_same(const T* expected, const T* actual, location&& loc = location()) const
        {
            if (expected == actual)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "not_same<" + typeid(T).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        // in_range -----------------------------------------------------------
        template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, detail::failure>::type
            in_range(T&& actual, T&& min, T&& max, location&& loc = location()) const
        {
            if (min >= max)
                throw std::invalid_argument(call_prefix_ + "in_range argument error: min (" +
                    to_string_with_full_precision(std::forward<T>(min)) + ") must be strictly less than max (" +
                    to_string_with_full_precision(std::forward<T>(max)) + ")");
            if (actual < min || actual >= max)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "in_range<" + typeid(T).name() + ">",
                    std::move(loc))
                    .expected("[" + to_string_with_full_precision(std::forward<T>(min)) + " - " +
                        to_string_with_full_precision(std::forward<T>(max)) + ")")
                    .actual(to_string_with_full_precision(std::forward<T>(actual)))));
            return on_success();
        }

        template<typename T>
        typename std::enable_if<!std::is_floating_point<T>::value, detail::failure>::type
            in_range(T&& actual, T&& min, T&& max, location&& loc = location()) const
        {
            if (min >= max)
                throw std::invalid_argument(call_prefix_ + "in_range argument error: min (" +
                    th::to_string(std::forward<T>(min)) + ") must be strictly less than max (" +
                    th::to_string(std::forward<T>(max)) + ")");
            if (actual < min || actual >= max)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "in_range<" + typeid(T).name() + ">",
                    std::move(loc))
                    .expected("[" + th::to_string(std::forward<T>(min)) + " - " + th::to_string(std::forward<T>(max)) + ")")
                    .actual(th::to_string(std::forward<T>(actual)))));
            return on_success();
        }

        // not_in_range -------------------------------------------------------
        template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, detail::failure>::type
            not_in_range(T&& actual, T&& min, T&& max, location&& loc = location()) const
        {
            if (min >= max)
                throw std::invalid_argument(call_prefix_ + "not_in_range argument error: min (" +
                    to_string_with_full_precision(std::forward<T>(min)) + ") must be strictly less than max (" +
                    to_string_with_full_precision(std::forward<T>(max)) + ")");
            if (actual >= min && actual < max)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "not in_range<" + typeid(T).name() + ">",
                    std::move(loc))
                    .expected("[" + to_string_with_full_precision(std::forward<T>(min)) + " - " +
                        to_string_with_full_precision(std::forward<T>(max)) + ")")
                    .actual(to_string_with_full_precision(std::forward<T>(actual)))));
            return on_success();
        }

        template<typename T>
        typename std::enable_if<!std::is_floating_point<T>::value, detail::failure>::type
            not_in_range(T&& actual, T&& min, T&& max, location&& loc = location()) const
        {
            if (min >= max)
                throw std::invalid_argument(call_prefix_ + "not_in_range argument error: min (" +
                    th::to_string(std::forward<T>(min)) + ") must be strictly less than max (" +
                    th::to_string(std::forward<T>(max)) + ")");
            if (actual >= min && actual < max)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "not_in_range<" + typeid(T).name() + ">",
                    std::move(loc))
                    .expected("[" + th::to_string(std::forward<T>(min)) + " - " + th::to_string(std::forward<T>(max)) + ")")
                    .actual(th::to_string(std::forward<T>(actual)))));
            return on_success();
        }

        // contains -----------------------------------------------------------
        template<typename T, typename V>
        typename std::enable_if<!std::is_constructible<std::string, T>::value || !std::is_constructible<std::string, V>::value, detail::failure>::type
            contains(const T& sequence, V&& value, location&& loc = location()) const
        {
            if (std::find(std::begin(sequence), std::end(sequence), std::forward<V>(value)) == std::end(sequence))
                return on_failure(std::move(detail::assert_context(
                    call_prefix_ + "contains<" + typeid(T).name() + ", " + typeid(V).name() + ">",
                    std::move(loc))));
            return on_success();
        }

        template<typename T, typename V>
        typename std::enable_if<std::is_constructible<std::string, T>::value && std::is_constructible<std::string, V>::value, detail::failure>::type
            contains(T actual, V value, location&& loc = location()) const
        {
            return contains(std::string(std::forward<T>(actual)), std::string(std::forward<V>(value)), std::move(loc));
        }

        detail::failure contains(const std::string& actual, const std::string& value, location&& loc = location()) const
        {
            if (actual.find(value) == std::string::npos)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "contains<string, string>",
                    std::move(loc))
                    .expected(std::string(value)) // Can't assume actual or value can be moved.
                    .actual(std::string(actual))));
            return on_success();
        }

        // does_not_contain ---------------------------------------------------
        template<typename T, typename V>
        typename std::enable_if<!std::is_constructible<std::string, T>::value || !std::is_constructible<std::string, V>::value, detail::failure>::type
            does_not_contain(const T& sequence, V&& value, location&& loc = location()) const
        {
            if (std::find(std::begin(sequence), std::end(sequence), std::forward<V>(value)) != std::end(sequence))
                return on_failure(std::move(detail::assert_context(
                call_prefix_ + "does_not_contain<" + typeid(T).name() + ", " + typeid(V).name() + ">",
                std::move(loc))));
            return on_success();
        }

        template<typename T, typename V>
        typename std::enable_if<std::is_constructible<std::string, T>::value && std::is_constructible<std::string, V>::value, detail::failure>::type
            does_not_contain(T actual, V value, location&& loc = location()) const
        {
            return does_not_contain(std::string(std::forward<T>(actual)), std::string(std::forward<V>(value)), std::move(loc));
        }

        detail::failure does_not_contain(const std::string& actual, const std::string& value, location&& loc = location()) const
        {
            const auto found = actual.find(value);
            if (found != std::string::npos)
                return on_failure(std::move(detail::assert_context(call_prefix_ + "does_not_contain<string, string>", std::move(loc))
                .custom_message("found \"" + value + "\" at position " + std::to_string(found))));
            return on_success();
        }

        // contains_predicate -------------------------------------------------
        template<typename T, typename P>
        detail::failure contains_predicate(const T& sequence, P&& predicate, location&& loc = location()) const
        {
            if (std::find(std::begin(sequence), std::end(sequence), std::forward<P>(predicate)) == std::end(sequence))
                return on_failure(std::move(detail::assert_context(
                call_prefix_ + "contains_predicate<" + typeid(T).name() + ", " + typeid(P).name() + ">",
                std::move(loc))));
            return on_success();
        }

        // does_not_contain_predicate -----------------------------------------
        template<typename T, typename P>
        detail::failure does_not_contain_predicate(const T& sequence, P&& predicate, location&& loc = location()) const
        {
            auto found = std::find(begin(sequence), end(sequence), std::forward<P>(predicate));
            if (found != std::end(sequence))
                return on_failure(std::move(detail::assert_context(
                call_prefix_ + "does_not_contain_predicate<" + typeid(T).name() + ", " + typeid(P).name() + ">",
                std::move(loc))
                .custom_message("found matching value at position " + th::to_string(std::distance(std::begin(sequence), found)))));
            return on_success();
        }

        // does_not_throw -----------------------------------------------------
        template<typename T> detail::failure does_not_throw(T&& function, location&& loc = location()) const
        {
            try
            {
                function();
            }
            catch (const std::exception& e)
            {
                return on_failure(detail::assert_context(call_prefix_ + "does_not_throw<" + typeid(T).name() + ">",
                    std::move(loc))
                    .expected("no exception").actual(std::string(typeid(e).name()) + ": " + e.what()));
            }
            catch (...)
            {
                return on_failure(detail::assert_context(call_prefix_ + "does_not_throw<" + typeid(T).name() + ">",
                    std::move(loc))
                    .expected("no exception").actual("unknown exception"));
            }
            return on_success();
        }

        // fail ---------------------------------------------------------------
        detail::failure fail(location&& loc = location()) const
        {
            return on_failure(detail::assert_context(call_prefix_ + "fail", std::move(loc)));
        }

    protected:
        std::string call_prefix_;

    private:
        std::function<void(const detail::assert_context&)> handle_failure_;

        [[nodiscard]] detail::failure on_failure(detail::assert_context assert_context) const { return detail::failure(std::move(assert_context), handle_failure_); }

        static detail::failure on_success() { return detail::failure::none(); }

        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, std::string>::type
            to_string_with_precision(const T value, const int precision) const
        {
            std::ostringstream out;
            out << std::setprecision(precision) << value;
            return out.str();
        }

        template <typename T>
        [[nodiscard]] typename std::enable_if<std::is_floating_point<T>::value, std::string>::type
            to_string_with_full_precision(const T value) const
        {
            const int digits{ std::numeric_limits<T>::digits };
            const int digits10{ std::numeric_limits<T>::digits10 };
            std::ostringstream out;
            out << std::setprecision(digits) << value << " (" << std::setprecision(digits10) << value << ")";
            return out.str();
        }
    };

    const class : public assert
    {
    private:
        template<typename E, typename F>
        static typename std::enable_if<!std::is_base_of<std::exception, E>::value, E>::type
            impl(const std::string& call_prefix, F&& func, const char *user_message, location&& loc = location())
        {
            try
            {
                func();
            }
            catch (const E& e)
            {
                return e;
            }
            catch (const std::exception& e)
            {
                throw detail::assert_context(call_prefix + "throws", std::forward<location>(loc))
                    .expected(typeid(E).name())
                    .actual(std::string(typeid(e).name()) + ": " + e.what())
                    .append_user_message(std::string(user_message));
            }
            catch (...)
            {
                throw detail::assert_context(call_prefix + "throws", std::forward<location>(loc))
                    .expected(typeid(E).name())
                    .actual("unknown exception")
                    .append_user_message(std::string(user_message));
            }
            throw detail::assert_context(call_prefix + "throws", std::forward<location>(loc))
                .expected(typeid(E).name())
                .actual("no exception")
                .append_user_message(std::string(user_message));
        }

        // Partial specialization for catching std::exception.
        template<typename E, typename F>
        static typename std::enable_if<std::is_base_of<std::exception, E>::value, E>::type
            impl(const std::string& call_prefix, F&& func, const char* user_message, location&& loc = location())
         {
             try
             {
                 func();
             }
             catch (const E& e)
             {
                 return e;
             }
             catch (...)
             {
                 throw detail::assert_context(call_prefix + "throws", std::forward<location>(loc))
                     .expected(typeid(std::exception).name())
                     .actual("unknown exception")
                     .append_user_message(std::string(user_message));
             }
             throw detail::assert_context(call_prefix + "throws", std::forward<location>(loc))
                 .expected(typeid(std::exception).name())
                 .actual("no exception")
                 .append_user_message(std::string(user_message));
         }

    public:
        template<typename E, typename F> E throws(F&& func, const char *user_message,
            location&& loc = location()) const
        {
            return impl<E, F>(call_prefix_,
                std::forward<decltype(func)>(func), user_message, std::forward<location>(loc));
        }

        template<typename E, typename F> E throws(F&& func,
            location&& loc = location()) const
        {
            return impl<E, F>(call_prefix_,
                std::forward<decltype(func)>(func), "", std::forward<location>(loc));
        }
    } assert;

    namespace detail
    {
        typedef enum class severity { debug, info, warning, expect, assert, fatal } severity;

        // ReSharper disable once CppInconsistentNaming
        class event_interface
        {
        protected:
            event_interface() = default;
            virtual ~event_interface() = default;

        public:
            event_interface(const event_interface&) = delete;
            event_interface(const event_interface&&) = delete;
            event_interface& operator =(const event_interface&) = delete;
            event_interface& operator =(event_interface&&) = delete;

            [[nodiscard]] virtual bool            is_failure()            const = 0;
            [[nodiscard]] virtual bool            is_assert()             const = 0;
            [[nodiscard]] virtual severity        severity_level()        const = 0;
            [[nodiscard]] virtual const char*     message()               const = 0;
            [[nodiscard]] virtual const char*     to_string()             const = 0;
            [[nodiscard]] virtual const location& located()               const = 0;
            [[nodiscard]] virtual const char*     assert_call()           const = 0;
            [[nodiscard]] virtual const char*     assert_user_message()   const = 0;
            [[nodiscard]] virtual const char*     assert_custom_message() const = 0;
            [[nodiscard]] virtual const char*     assert_expected()       const = 0;
            [[nodiscard]] virtual const char*     assert_actual()         const = 0;
        };

        class event final : public event_interface
        {
        public:
            event(const detail::severity severity, std::string message, th::location&& loc = th::location()) :
                severity_{ severity }, assert_context_{ assert_context::none() }, message_(std::move(message)),
                location_{ std::move(loc) } {}

            event(const detail::severity severity, const assert_context& assert_context) :
                severity_{ severity }, assert_context_{ assert_context }, location_{ assert_context.located() } {}

            event(const std::exception& e) :
                severity_{ severity::fatal }, assert_context_{ assert_context::none() },
                message_(std::string("uncaught std::exception: ") + e.what()) {}

            event(event&& other) noexcept :
                severity_{ other.severity_ }, assert_context_{ std::move(other.assert_context_) },
                message_(std::move(other.message_)), location_{ std::move(other.location_) } {}

            event() = delete;
            event(const event&) = delete;
            event& operator =(const event&) = delete;
            event& operator =(event&&) = delete;
            ~event() = default;

            bool            is_assert()      const override { return !assert_context_.call().empty(); }
            bool            is_failure()     const override { return severity_ > severity::warning; }
            severity        severity_level() const override { return severity_; }
            const char*     message()        const override { return message_.c_str(); }
            const location& located()        const override { return location_; }
            const char*     to_string()      const override
            {
                if (buffer_.empty())
                    buffer_ = is_assert() ? assert_context_.to_string() : message_;
                return buffer_.c_str();
            }
            const char* assert_call()           const override { return assert_context_.call().c_str(); }
            const char* assert_user_message()   const override { return assert_context_.user_message().c_str(); }
            const char* assert_custom_message() const override { return assert_context_.custom_message().c_str(); }
            const char* assert_expected()       const override { return assert_context_.expected().c_str(); }
            const char* assert_actual()         const override { return assert_context_.actual().c_str(); }

        private:
            severity severity_;
            assert_context assert_context_;
            std::string message_;
            location location_;
            mutable std::string buffer_;
        };
    }

    template<>
    inline std::string to_string(const detail::severity value)
    {
        switch (value)
        {
            case detail::severity::fatal:   return "fatal";
            case detail::severity::assert:  return "assert";
            case detail::severity::expect:  return "expect";
            case detail::severity::warning: return "warning";
            case detail::severity::info:    return "info";
            case detail::severity::debug:   return "debug";
        }
        return "???";
    }

    namespace detail
    {
        class testcase;
    }

    class log
    {
    private:
        class logger
        {
        public:
            class message
            {
            public:
                message(std::function<void(const std::string&, location&&)> record_message,
                    location&& loc = location()) :
                    record_message_{std::move(record_message)}, location_{ std::move(loc) } {}

                message(message&& other) noexcept :  // NOLINT(bugprone-exception-escape)
                    record_message_{ std::move(other.record_message_) }, location_{ std::move(other.location_) }
                {
                    message_ << other.message_.str();
                    other.record_message_ = nullptr;
                }

                message(const message& other) :
                    record_message_{ other.record_message_ }, location_{ other.location_ }
                {
                    message_ << other.message_.str();
                    const_cast<message&>(other).record_message_ = nullptr;
                }

                ~message()
                {
                    if (record_message_ != nullptr)
                        record_message_(message_.str(), std::move(location_));
                }

                message() = delete;
                message& operator =(const message&) = delete;
                message& operator =(message&&) = delete;

                template<typename T> message& operator <<(const T& value) { message_ << value; return *this; }

            private:
                std::function<void(const std::string&, location&&)> record_message_;
                std::stringstream message_;
                location location_;
            };

            logger(std::function<void(const std::string&, location&&)> record_message) :
                record_message_{std::move(record_message)} {}

            logger() = delete;
            logger(const logger&) = delete;
            logger(logger&&) = delete;
            logger& operator =(const logger&) = delete;
            logger& operator =(logger&&) = delete;
            ~logger() = default;

            template<typename T> message operator <<(const T& value) const { return (message{ record_message_ } << value); }

            message operator()(location&& loc) const { return message{ record_message_, std::move(loc) }; }

        private:
            std::function<void(const std::string&, location&&)> record_message_;
        };

    public:
        log(detail::testcase& testcase);

        const logger debug;
        const logger info;
        const logger warn;

        log() = delete;
        log(const log&) = delete;
        log(log&&) = delete;
        log& operator =(const log&) = delete;
        log& operator =(log&&) = delete;
        ~log() = default;
    };

    class expect : public assert
    {
    public:
        expect(detail::testcase& testcase);

        expect() = delete;
        expect(const expect &) = delete;
        expect(expect &&) = delete;
        expect & operator =(const expect &) = delete;
        expect & operator =(expect &&) = delete;
        ~expect() = default;
    };

    class warn : public assert
    {
    public:
        warn(detail::testcase& testcase);

        warn() = delete;
        warn(const warn&) = delete;
        warn(warn&&) = delete;
        warn& operator =(const warn&) = delete;
        warn& operator =(warn&&) = delete;
        ~warn() = default;
    };

    // ReSharper disable once CppInconsistentNaming
    struct test_context
    {
        bool ignore_round_off_errors;
        // ReSharper disable CppRedundantQualifier
        const th::expect expect;
        const th::warn warn;
        const th::log log;
        // ReSharper restore CppRedundantQualifier

        test_context(detail::testcase& testcase, const bool ignore_round_off_errors = false ) :
            ignore_round_off_errors{ ignore_round_off_errors }, expect{ testcase }, warn{ testcase }, log{ testcase } {}

        test_context() = delete;
        test_context(const test_context&) = delete;
        test_context(test_context&&) = delete;
        test_context& operator=(const test_context&) = delete;
        test_context& operator=(test_context&&) = delete;
        ~test_context() = default;
    };

    const int no_timeout{ -1 };
    const int auto_timeout{ 0 };

    namespace detail
    {
        struct options
        {
            bool ignore_round_off_errors;
            bool verbose;
            bool list;
            std::vector<std::string> groups;
            std::vector<std::string> testcases;
            std::string xml_output;
            int time_limit_milliseconds;
            int thread_limit;
            bool sort;
            bool aggregate;
            bool color_output;

            options() :
                ignore_round_off_errors{ true }, verbose{ false }, list{ false }, time_limit_milliseconds{ 10000 },
                thread_limit{ 1 }, sort{ false }, aggregate{ false }, color_output{ true } {}

            options(const options&) = default;
            options(options&&) = delete;
            options& operator =(const options&) = delete;
            options& operator =(options&&) = delete;
            ~options() = default;
        };

        // ReSharper disable once CppInconsistentNaming
        class testcase_interface
        {
        protected:
            virtual ~testcase_interface() = default;

        public:
            testcase_interface() = default;
            testcase_interface(const testcase_interface&) = delete;
            testcase_interface(testcase_interface&&) = delete;
            testcase_interface& operator =(const testcase_interface&) = delete;
            testcase_interface& operator =(testcase_interface&&) = delete;

            [[nodiscard]] virtual const char*     name()    const = 0;
            [[nodiscard]] virtual const char*     group()   const = 0;
            [[nodiscard]] virtual const location& located() const = 0;
            [[nodiscard]] virtual int             id()      const = 0;
        };

        class testcase final : public testcase_interface
        {
        public:
            testcase() = delete;
            testcase(const testcase&) = delete;
            testcase(testcase&&) = delete;
            testcase& operator =(const testcase&) = delete;
            testcase& operator =(testcase&&) = delete;
            ~testcase() = default;

            testcase(std::string&& name, std::string group, std::function<void(const test_context&)>&& func,
                const long long timeout_milliseconds, th::location&& loc = th::location()) :
                name_(std::move(name)), group_(std::move(group)), location_{ loc }, func_{ std::move(func) },
                timeout_milliseconds_{ timeout_milliseconds }, failure_event_logged_{ false }, id_{ next_id() } {}

            [[nodiscard]] const char*     name()    const override { return name_.c_str(); }
            [[nodiscard]] const char*     group()   const override { return group_.c_str(); }
            [[nodiscard]] const location& located() const override { return location_; }
            [[nodiscard]] int             id()      const override { return id_; }

            bool run(const options& opt)
            {
                const test_context context{ *this, opt.ignore_round_off_errors };
                testStart_ = std::chrono::high_resolution_clock::now();
                try
                {
                    func_(context);
                }
                catch (const assert_context& a)
                {
                    add_event(event(severity::assert, a));
                }
                catch (const std::exception& e)
                {
                    add_event(event(e));
                }
                catch (...)
                {
                    add_event(event(severity::fatal, "Unknown exception caught: test has crashed."));
                }
                testStop_ = std::chrono::high_resolution_clock::now();
                return !failure_event_logged_;
            }

            void add_event(event&& evt)
            {
                std::lock_guard<std::mutex> lock(eventLock_);
                events_.push_back(std::move(evt));
                if (events_.back().is_failure())
                    failure_event_logged_ = true;
            }

            [[nodiscard]] std::chrono::nanoseconds duration() const
            {
                return std::chrono::duration_cast<std::chrono::nanoseconds>(testStop_ - testStart_);
            }

            [[nodiscard]] long long timeout_milliseconds() const { return timeout_milliseconds_; }
            [[nodiscard]] const std::vector<event>& events() const { return events_; }

        private:
            static int next_id()
            {
                static int id = -1;
                return ++id;
            }

            const std::string name_;
            const std::string group_;
            location location_;
            std::function<void(const test_context&)> func_;
            long long timeout_milliseconds_;
            typedef decltype(std::chrono::high_resolution_clock::now()) time_stamp;
            time_stamp testStart_;
            time_stamp testStop_;
            std::mutex eventLock_;
            std::vector<event> events_;
            bool failure_event_logged_;
            int id_;
        };
    }

    inline log::log(detail::testcase& testcase) :
        debug{ [&](const std::string& message, location&& loc)
            {
                testcase.add_event(detail::event(detail::severity::debug, message, std::move(loc)));
            }},
        info{ [&](const std::string& message, location&& loc)
            {
                testcase.add_event(detail::event(detail::severity::info, message, std::move(loc)));
            }},
        warn{ [&](const std::string& message, location&& loc)
            {
                testcase.add_event(detail::event(detail::severity::warning, message, std::move(loc)));
            }} {}

    inline expect::expect(detail::testcase& testcase) :
        assert{ std::string("expect."), [&](const detail::assert_context& assert)
            {
                testcase.add_event(detail::event(detail::severity::expect, assert));
            }} {}

    inline warn::warn(detail::testcase& testcase) :
        assert{ std::string("warn."), [&](const detail::assert_context& assert)
            {
                testcase.add_event(detail::event(detail::severity::warning, assert));
            }} {}

    namespace detail
    {
        // ReSharper disable once CppInconsistentNaming
        struct testcollection
        {
            testcollection() = default;
            testcollection(const testcollection&) = delete;
            testcollection(testcollection&&) = delete;
            testcollection& operator =(const testcollection&) = delete;
            testcollection& operator =(testcollection&&) = delete;
            ~testcollection() = default;

            static testcollection& instance() { static testcollection collection; return collection; }
            std::vector<std::shared_ptr<testcase>> tests;
        };
    }

    struct registrar
    {
        registrar() = delete;
        registrar(const registrar&) = delete;
        registrar(registrar&&) = delete;
        registrar& operator =(const registrar&) = delete;
        registrar& operator =(registrar&&) = delete;
        ~registrar() = default;

        registrar(const char* name, const char* group, int timeout_milliseconds, location&& loc,
            std::function<void(const test_context&)>&& func)
        {
            detail::testcollection::instance().tests.push_back(std::make_shared<detail::testcase>(
                std::string(name), std::string(group), std::move(func), timeout_milliseconds, std::move(loc)));
        }

        registrar(std::string&& name, std::string&& group, int timeout_milliseconds, location&& loc,
            std::function<void(const test_context&)>&& func)
        {
            detail::testcollection::instance().tests.push_back(std::make_shared<detail::testcase>(
                std::move(name), std::move(group), std::move(func), timeout_milliseconds, std::move(loc)));
        }

        registrar(const char*name, std::string&& group, int timeout_milliseconds, location&& loc,
            std::function<void(const test_context&)>&& func)
        {
            detail::testcollection::instance().tests.push_back(std::make_shared<detail::testcase>(
                std::string(name), std::move(group), std::move(func), timeout_milliseconds, std::move(loc)));
        }

        registrar(std::string&& name, const char*group, int timeout_milliseconds, location&& loc,
            std::function<void(const test_context&)>&& func)
        {
            detail::testcollection::instance().tests.push_back(std::make_shared<detail::testcase>(
                std::move(name), std::string(group), std::move(func), timeout_milliseconds, std::move(loc)));
        }
    };

    namespace shell
    {
        namespace detail
        {
            class output_interface
            {
            protected:
                virtual ~output_interface() = default;

            public:
                output_interface() = default;
                output_interface(const output_interface&) = delete;
                output_interface(output_interface&&) = delete;
                output_interface& operator =(const output_interface&) = delete;
                output_interface& operator =(output_interface&&) = delete;

                virtual void report_start(const th::detail::testcase_interface& testcase) = 0;
                virtual void report_event(const th::detail::testcase_interface& testcase, const th::detail::event_interface& evt) = 0;
                virtual void report_skip(const th::detail::testcase_interface& testcase, const char* reason) = 0;
                virtual void report_finish(const th::detail::testcase_interface& testcase, std::chrono::nanoseconds nanoseconds) = 0;
                virtual void report_complete(int total, int skipped, int failed, std::chrono::nanoseconds nanoseconds_total) = 0;
            };

            typedef std::function<void(const th::detail::testcase_interface&)> enumerate_testcase_callback;

            typedef std::function<bool(const th::detail::testcase_interface&)> filter_callback;
        }

        namespace util
        {
            typedef void(*enumerate_tests_func)(detail::enumerate_testcase_callback callback);
            typedef int(*run_tests_func)(detail::output_interface&, detail::filter_callback,
                const th::detail::options&, std::chrono::milliseconds, int);
        }
    }
}

namespace th = mbc::testharness;
namespace thd = th::detail;
