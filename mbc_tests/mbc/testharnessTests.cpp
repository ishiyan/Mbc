#include "mbc/testharness.h"

namespace {

TESTGROUP("testharness::floating_point")
{
    TESTCASE("<double> can calculate the distance in 'units in the last place'")
    {
	    const th::detail::floating_point<double> fp1{ -26946.839140521406 };
        const th::detail::floating_point<double> fp2{ -26946.839140521435 };
        const size_t ulp{ fp1.distance_from(fp2) };
        ASSERT_EQUAL(ulp, static_cast<size_t>(8));
    }

    TESTCASE("<double> can compare given the accuracy in 'units in the last place'")
    {
	    const th::detail::floating_point<double> fp1{ -26946.839140521406 };
        const th::detail::floating_point<double> fp2{ -26946.839140521435 };
        ASSERT_IS_FALSE(fp1.almost_equals(fp2, 7));
        ASSERT_IS_TRUE(fp1.almost_equals(fp2, 8));
    }

    TESTCASE("<float> can calculate the distance in 'units in the last place'")
    {
	    const th::detail::floating_point<float> fp1{ 1.8391405f };
        const th::detail::floating_point<float> fp2{ 1.8391406f };
        const size_t ulp{ fp1.distance_from(fp2) };
        ASSERT_EQUAL(ulp, static_cast<size_t>(1));
    }

    TESTCASE("<float> can compare given the accuracy in 'units in the last place'")
    {
	    const th::detail::floating_point<float> fp1{ 1.8391405f };
        const th::detail::floating_point<float> fp2{ 1.8391406f };
        ASSERT_IS_FALSE(fp1.almost_equals(fp2, 0));
        ASSERT_IS_TRUE(fp1.almost_equals(fp2, 1));
    }
}

TESTGROUP("testharness::assert_context")
{
    TESTCASE("can be constructed")
    {
	    const th::detail::assert_context ac{ std::string("call"), th::location("file", 42) };
        ASSERT_EQUAL("call", ac.call());
        ASSERT_EQUAL(42, ac.located().line_);
        ASSERT_EQUAL("file", ac.located().file_);
    }

    TESTCASE("can be copy-constructed")
    {
        const th::detail::assert_context ac{ std::string("call"), th::location("file", 42) };
        const th::detail::assert_context& ac_copy{ ac };
        ASSERT_EQUAL("call", ac_copy.call());
        ASSERT_EQUAL(42, ac_copy.located().line_);
        ASSERT_EQUAL("file", ac_copy.located().file_);
    }

    TESTCASE("can have expected value which can be overwritten")
    {
        th::detail::assert_context ac{ std::string(), th::location() };
        ac.expected(std::string("expected"));
        ASSERT_EQUAL("expected", ac.expected());
        ac.expected(std::string("new expected"));
        ASSERT_EQUAL("new expected", ac.expected());
    }

    TESTCASE("can have actual value which can be overwritten")
    {
        th::detail::assert_context ac{ std::string(), th::location() };
        ac.actual(std::string("actual"));
        ASSERT_EQUAL("actual", ac.actual());
        ac.actual(std::string("new actual"));
        ASSERT_EQUAL("new actual", ac.actual());
    }

    TESTCASE("can have custom_message which can be overwritten")
    {
        th::detail::assert_context ac{ std::string(), th::location() };
        ac.custom_message(std::string("custom message"));
        ASSERT_EQUAL("custom message", ac.custom_message());
        ac.custom_message(std::string("foobar"));
        ASSERT_EQUAL("foobar", ac.custom_message());
    }

    TESTCASE("can have user_message which can be appended")
    {
        th::detail::assert_context ac{ std::string(), th::location() };
        ac.append_user_message("foo");
        ac.append_user_message(std::string("bar"));
        ASSERT_EQUAL("foobar", ac.user_message());
    }

    TESTCASE("can be converted to string")
    {
        th::detail::assert_context ac{ std::string("foobar"), th::location("file", 42) };
        ASSERT_EQUAL("foobar failure", ac.to_string().c_str());
        ac.expected(std::string("exp_val"));
        ASSERT_EQUAL("foobar failure\nexpected: exp_val\nactual  : ", ac.to_string().c_str());
        ac.actual(std::string("act_val"));
        ASSERT_EQUAL("foobar failure\nexpected: exp_val\nactual  : act_val", ac.to_string().c_str());
        ac.custom_message(std::string("custom message"));
        ASSERT_EQUAL("foobar failure: custom message\nexpected: exp_val\nactual  : act_val", ac.to_string().c_str());
        ac.append_user_message(std::string("user message"));
        ASSERT_EQUAL("foobar failure: user message\ncustom message\nexpected: exp_val\nactual  : act_val", ac.to_string().c_str());
        ac.custom_message(std::string());
        ASSERT_EQUAL("foobar failure: user message\nexpected: exp_val\nactual  : act_val", ac.to_string().c_str());
    }
}

TESTGROUP("testharness::failure")
{
    TESTCASE("can be constructed")
    {
        {
            th::detail::failure f{
                th::detail::assert_context{ std::string("call"), th::location("file", 42) },
                [](const th::detail::assert_context& a)
                {
                    ASSERT_EQUAL("call", a.call());
                    ASSERT_EQUAL(42, a.located().line_);
                    ASSERT_EQUAL("file", a.located().file_);
                } };
        }
    }

    TESTCASE("can be move-constructed")
    {
        {
            th::detail::failure f{ th::detail::failure{
                th::detail::assert_context{ std::string("call"), th::location("file", 42) },
                [](const th::detail::assert_context& a)
                {
                    ASSERT_EQUAL("call", a.call());
                    ASSERT_EQUAL(42, a.located().line_);
                    ASSERT_EQUAL("file", a.located().file_);
                } } };
        }
    }

    TESTCASE("can have user_message which can be appended")
    {
        {
            th::detail::failure f{ th::detail::assert_context{ std::string(), th::location() },
                [](const th::detail::assert_context& a)
            {
                ASSERT_EQUAL("foobar", a.user_message());
            } };
            f << "foo" << std::string("bar");
        }
    }
}

TESTGROUP("testharness::assert::equal")
{
    TESTCASE("compares <double, double, ulp> using given ulp and default comparer")
    {
        th::assert.equal(-26946.839140521406, -26946.839140521435, 8, LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(-26946.839140521406, -26946.839140521435, 7, LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL_FP(-26946.839140521406, -26946.839140521435, 8) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_FP(-26946.839140521406, -26946.839140521435, 7) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <float, float, ulp> using given ulp and default comparer")
    {
        th::assert.equal(1.8391405f, 1.8391406f, 1, LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(1.8391405f, 1.8391406f, 0, LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL_FP(1.8391405f, 1.8391406f, 1) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_FP(1.8391405f, 1.8391406f, 0) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <double, float, ulp> using given ulp and default comparer")
    {
        th::assert.equal(1.839140521406, 1.8391406f, 1, LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(1.839140521406, 1.8391406f, 0, LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL_FP(1.839140521406, 1.8391406f, 1) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_FP(1.839140521406, 1.8391406f, 0) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <float, double, ulp> using given ulp and default comparer")
    {
        th::assert.equal(1.8391406f, 1.839140521406, 1, LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(1.8391406f, 1.839140521406, 0, LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL_FP(1.8391406f, 1.839140521406, 1) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_FP(1.8391406f, 1.839140521406, 0) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <int, int> using default comparer")
    {
        th::assert.equal(42, 42, LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(42, 43, LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL(42, 42) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL(42, 43) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <int, int> using custom comparer")
    {
        auto comparer = [](int&& exp, int&& act) { return exp == (act / 10); };

        th::assert.equal(4, 42, comparer, LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(42, 42,
                [](int&& exp, int&& act) { return exp == (act / 10); }, LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL_CMP(4, 42, comparer) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_CMP(42, 42,
            [](int&& exp _Comma int&& act){ return exp == (act / 10); }) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <int, long> using default comparer")
    {
        th::assert.equal(42, 42l, LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(42, 43l, LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL(42, 42l) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL(42, 43l) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <unsigned int, unsigned long> using default comparer")
    {
        th::assert.equal(42u, 42ul, LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(42u, 43ul, LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL(42u, 42ul) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL(42u, 43ul) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <int, char> using default comparer")
    {
        th::assert.equal(0x61, 'a', LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(0x61, 'b', LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL(0x61, 'a') << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL(0x61, 'b') << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <const char*, const char*> using default comparer")
    {
        th::assert.equal("foobar", "foobar", LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal("foobar", "foobar*", LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL("foobar", "foobar") << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL("foobar", "foobar*") << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <std::string, const char*> using default comparer")
    {
        th::assert.equal(std::string("foobar"), "foobar", LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(std::string("foobar"), "foobar*", LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL(std::string("foobar"), "foobar") << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL(std::string("foobar"), "foobar*") << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <std::string, std::string> using default comparer")
    {
        th::assert.equal(std::string("foobar"), std::string("foobar"), LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(std::string("foobar"), std::string("foobar*"), LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL(std::string("foobar"), std::string("foobar")) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL(std::string("foobar"), std::string("foobar*")) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <bool, bool> using default comparer")
    {
        th::assert.equal(true, true, LOC) << "does not throw if equal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.equal(true, false, LOC) << "throws if not equal";
        }, LOC);

        ASSERT_EQUAL(true, true) << "does not throw if equal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL(true, false) << "throws if not equal (using macro)";
        );
    }

    TESTCASE("compares <std::vector, std::vector> sequences using default comparer")
    {
        std::vector<int> v0{ 1, 2, 3, 4, 5 };
        std::vector<int> v1{ 1, 2, 3, 4, 5 };
        th::assert.equal_seq(v0.begin(), v0.end(), v1.begin(), v1.end(), LOC) << "does not throw if equal";

        std::vector<int> v3{ 1, 2, 3, 4, 5 };
        std::vector<short> v4{ 1, 2, 3, 7, 5 };
        auto a1 = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.equal_seq(v3.begin(), v3.end(), v4.begin(), v4.end(), LOC) << "throws if not equal";
        }, LOC);
        th::assert.contains(a1.custom_message(), "unequal at location 3", LOC);

        std::vector<int> v5{ 1, 2, 3, 4, 5 };
        std::vector<long> v6{ 1, 2, 3, 4 };
        auto a2 = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.equal_seq(v5.begin(), v5.end(), v6.begin(), v6.end(), LOC) << "throws if not equal";
        }, LOC);
        th::assert.contains(a2.custom_message(), "unequal at location 4", LOC);

        ASSERT_EQUAL_SEQ(v0.begin(), v0.end(), v1.begin(), v1.end()) << "does not throw if equal (using macro)";

        auto b1 = ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_SEQ(v3.begin(), v3.end(), v4.begin(), v4.end()) << "throws if not equal (using macro)";
        );
        ASSERT_CONTAINS(b1.custom_message(), "unequal at location 3");

        auto b2 = ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_SEQ(v5.begin(), v5.end(), v6.begin(), v6.end()) << "throws if not equal (using macro)";
        );
        ASSERT_CONTAINS(b2.custom_message(), "unequal at location 4");
    }

    TESTCASE("compares <std::vector, std::vector> sequences using custom comparer")
    {
        std::vector<int> v0{ 1, 2, 3, 4, 5 };
        std::vector<int> v1{ 11, 12, 13, 14, 15 };
        th::assert.equal_seq(v0.begin(), v0.end(), v1.begin(), v1.end(),
            [](int exp, int act) { return exp == (act - 10); },
            LOC) << "does not throw if equal";

        std::vector<int> v3{ 1, 2, 3, 4, 5 };
        std::vector<short> v4{ 11, 12, 13, 17, 15 };
        auto a1 = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.equal_seq(v3.begin(), v3.end(), v4.begin(), v4.end(),
                [](int exp, short act) { return exp == (act - 10); },
                LOC) << "throws if not equal";
        }, LOC);
        th::assert.contains(a1.custom_message(), "unequal at location 3", LOC);

        std::vector<int> v5{ 1, 2, 3, 4, 5 };
        std::vector<long> v6{ 11, 12, 13, 14 };
        auto a2 = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.equal_seq(v5.begin(), v5.end(), v6.begin(), v6.end(),
                [](int exp, long act) { return exp == (act - 10); },
                LOC) << "throws if not equal";
        }, LOC);
        th::assert.contains(a2.custom_message(), "unequal at location 4", LOC);

        ASSERT_EQUAL_SEQ_CMP(v0.begin(), v0.end(), v1.begin(), v1.end(),
            [](int exp, int act) { return exp == (act - 10); }) << "does not throw if equal (using macro)";

        auto b1 = ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_SEQ_CMP(v3.begin(), v3.end(), v4.begin(), v4.end(),
            [](int exp _Comma short act) { return exp == (act - 10); }) << "throws if not equal (using macro)";
        );
        ASSERT_CONTAINS(b1.custom_message(), "unequal at location 3");

        auto b2 = ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_SEQ_CMP(v5.begin(), v5.end(), v6.begin(), v6.end(),
            [](int exp _Comma long act) { return exp == (act - 10); }) << "throws if not equal (using macro)";
        );
        ASSERT_CONTAINS(b2.custom_message(), "unequal at location 4");
    }

    TESTCASE("compares <array of int> arrays using default comparer")
    {
        const int len{ 5 };
        const int v1[]{ 1, 2, 3, 4, 5 };
        const int v2[]{ 1, 2, 3, 4, 5 };
        th::assert.equal_array(len, v1, v2, LOC) << "does not throw if equal";

        const int v3[]{ 1, 2, 3, 4, 5 };
        const int v4[]{ 1, 2, 7, 4, 8 };
        auto a = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.equal_array(len, v3, v4, LOC) << "throws if not equal";
        }, LOC);
        th::assert.contains(a.custom_message(), "failed 2 out of 5", LOC);

        ASSERT_EQUAL_ARRAY(len, v1, v2) << "does not throw if equal (using macro)";

        auto b = ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_ARRAY(len, v3, v4) << "throws if not equal (using macro)";
        );
        ASSERT_CONTAINS(b.custom_message(), "failed 2 out of 5");
    }

    TESTCASE("compares <array of int> arrays using custom comparer")
    {
        const int len{ 5 };
        const int v1[]{ 1, 2, 3, 4, 5 };
        const int v2[]{ 11, 12, 13, 14, 15 };
        th::assert.equal_array(len, v1, v2,
            [](int exp, int act) { return exp == (act - 10); },
            LOC) << "does not throw if equal";

        const int v3[]{ 1, 2, 3, 4, 5 };
        const int v4[]{ 11, 12, 17, 14, 18 };
        auto a = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.equal_array(len, v3, v4,
                [](int exp, int act) { return exp == (act - 10); },
                LOC) << "throws if not equal";
        }, LOC);
        th::assert.contains(a.custom_message(), "failed 2 out of 5", LOC);

        ASSERT_EQUAL_ARRAY_CMP(len, v1, v2,
            [](int exp _Comma int act) { return exp == (act - 10); }
        ) << "does not throw if equal (using macro)";

        auto b = ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_ARRAY_CMP(len, v3, v4,
            [](int exp _Comma int act) { return exp == (act - 10); }
        ) << "throws if not equal (using macro)";
        );
        ASSERT_CONTAINS(b.custom_message(), "failed 2 out of 5");
    }

    TESTCASE("compares <array of double> arrays using given ulp and default comparer")
    {
        const int len{ 3 };
        const double v1[]{ 0.1751528236650919900, 0.0511652155111260030, 0.3067856089049090200 };
        const double v2[]{ 0.1751528236650917700, 0.0511652155111257460, 0.3067856089049094700 };
        th::assert.equal_array(len, v1, v2, 37, LOC) << "does not throw if equal";

        auto a = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.equal_array(len, v1, v2, 36, LOC) << "throws if not equal";
        }, LOC);
        th::assert.contains(a.custom_message(), "failed 1 out of 3", LOC);

        const double v3[]{ 1.0, 2.0, 3.0, 4.0 };
        const double v4[]{ 1.0, 2.0, 3.0 };
        auto a2 = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.equal_array(4, v3, v4, 4, LOC) << "throws if not equal";
        }, LOC);
        th::assert.contains(a2.custom_message(), "failed 1 out of 4", LOC);

        auto a3 = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.equal_array(4, v4, v3, 4, LOC) << "throws if not equal";
        }, LOC);
        th::assert.contains(a3.custom_message(), "failed 1 out of 4", LOC);

        ASSERT_EQUAL_FP_ARRAY(len, v1, v2, 38) << "does not throw if equal (using macro)";

        auto b = ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_FP_ARRAY(len, v1, v2, 4) << "throws if not equal (using macro)";
        );
        ASSERT_CONTAINS(b.custom_message(), "failed 3 out of 3");

        auto b2 = ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_FP_ARRAY(4, v3, v4, 4) << "throws if not equal (using macro)";
        );
        ASSERT_CONTAINS(b2.custom_message(), "failed 1 out of 4");

        auto b3 = ASSERT_THROWS(thd::assert_context,
            ASSERT_EQUAL_FP_ARRAY(4, v4, v3, 4) << "throws if not equal (using macro)";
        );
        ASSERT_CONTAINS(b3.custom_message(), "failed 1 out of 4");
    }
}

TESTGROUP("testharness::assert::unequal")
{
    TESTCASE("compares <double, double, ulp> using given ulp and default comparer")
    {
        th::assert.unequal(-26946.839140521406, -26946.839140521435, 7, LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(-26946.839140521406, -26946.839140521435, 8, LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL_FP(-26946.839140521406, -26946.839140521435, 7) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_FP(-26946.839140521406, -26946.839140521435, 8) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <float, float, ulp> using given ulp and default comparer")
    {
        th::assert.unequal(1.8391405f, 1.8391406f, 0, LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(1.8391405f, 1.8391406f, 1, LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL_FP(1.8391405f, 1.8391406f, 0) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_FP(1.8391405f, 1.8391406f, 1) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <double, float, ulp> using given ulp and default comparer")
    {
        th::assert.unequal(1.839140521406, 1.8391406f, 0, LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(1.839140521406, 1.8391406f, 1, LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL_FP(1.839140521406, 1.8391406f, 0) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_FP(1.839140521406, 1.8391406f, 1) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <float, double, ulp> using given ulp and default comparer")
    {
        th::assert.unequal(1.8391406f, 1.839140521406, 0, LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(1.8391406f, 1.839140521406, 1, LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL_FP(1.8391406f, 1.839140521406, 0) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_FP(1.8391406f, 1.839140521406, 1) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <int, int> using default comparer")
    {
        th::assert.unequal(42, 43, LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(42, 42, LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL(42, 43) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL(42, 42) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <int, int> using custom comparer")
    {
        auto comparer = [](int&& exp, int&& act) { return exp == (act / 10); };

        th::assert.unequal(4, 4, comparer, LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(4, 42,
                [](int&& exp, int&& act) { return exp == (act / 10); }, LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL_CMP(4, 4, comparer) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_CMP(4, 42,
            [](int&& exp _Comma int&& act){ return exp == (act / 10); }) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <int, long> using default comparer")
    {
        th::assert.unequal(42, 43l, LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(42, 42l, LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL(42, 43l) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL(42, 42l) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <unsigned int, unsigned long> using default comparer")
    {
        th::assert.unequal(42u, 43ul, LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(42u, 42ul, LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL(42u, 43ul) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL(42u, 42ul) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <int, char> using default comparer")
    {
        th::assert.unequal(0x61, 'b', LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(0x61, 'a', LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL(0x61, 'b') << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL(0x61, 'a') << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <const char*, const char*> using default comparer")
    {
        th::assert.unequal("foobar", "foobar*", LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal("foobar", "foobar", LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL("foobar", "foobar*") << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL("foobar", "foobar") << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <std::string, const char*> using default comparer")
    {
        th::assert.unequal(std::string("foobar"), "foobar*", LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(std::string("foobar"), "foobar", LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL(std::string("foobar"), "foobar*") << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL(std::string("foobar"), "foobar") << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <std::string, std::string> using default comparer")
    {
        th::assert.unequal(std::string("foobar"), std::string("foobar*"), LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(std::string("foobar"), std::string("foobar"), LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL(std::string("foobar"), std::string("foobar*")) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL(std::string("foobar"), std::string("foobar")) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <bool, bool> using default comparer")
    {
        th::assert.unequal(true, false, LOC) << "does not throw if unequal";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.unequal(true, true, LOC) << "throws if equal";
        }, LOC);

        ASSERT_UNEQUAL(true, false) << "does not throw if unequal (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL(true, true) << "throws if equal (using macro)";
        );
    }

    TESTCASE("compares <std::vector, std::vector> sequences using default comparer")
    {
        std::vector<int> v0{ 1, 2, 3, 4, 5 };
        std::vector<int> v1{ 11, 12, 13, 14, 15 };
        th::assert.unequal_seq(v0.begin(), v0.end(), v1.begin(), v1.end(), LOC) << "does not throw if unequal";

        std::vector<int> v3{ 1, 2, 3, 4, 5 };
        std::vector<short> v4{ 11, 12, 13, 4, 15 };
        auto a1 = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.unequal_seq(v3.begin(), v3.end(), v4.begin(), v4.end(), LOC) << "throws if equal";
        }, LOC);
        th::assert.contains(a1.custom_message(), "equal at location 3", LOC);

        std::vector<int> v5{ 1, 2, 3, 4, 5 };
        std::vector<long> v6{ 11, 12, 13, 14 };
        auto a2 = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.unequal_seq(v5.begin(), v5.end(), v6.begin(), v6.end(), LOC) << "throws if equal";
        }, LOC);
        th::assert.contains(a2.custom_message(), "equal at location 4", LOC);

        ASSERT_UNEQUAL_SEQ(v0.begin(), v0.end(), v1.begin(), v1.end()) << "does not throw if unequal (using macro)";

        auto b1 = ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_SEQ(v3.begin(), v3.end(), v4.begin(), v4.end()) << "throws if equal (using macro)";
        );
        ASSERT_CONTAINS(b1.custom_message(), "equal at location 3");

        auto b2 = ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_SEQ(v5.begin(), v5.end(), v6.begin(), v6.end()) << "throws if equal (using macro)";
        );
        ASSERT_CONTAINS(b2.custom_message(), "equal at location 4");
    }

    TESTCASE("compares <std::vector, std::vector> sequences using custom comparer")
    {
        std::vector<int> v0{ 1, 2, 3, 4, 5 };
        std::vector<int> v1{ 21, 22, 23, 24, 25 };
        th::assert.unequal_seq(v0.begin(), v0.end(), v1.begin(), v1.end(),
            [](int exp, int act) { return exp == (act - 10); },
            LOC) << "does not throw if unequal";

        std::vector<int> v3{ 1, 2, 3, 4, 5 };
        std::vector<short> v4{ 21, 22, 23, 14, 25 };
        auto a = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.unequal_seq(v3.begin(), v3.end(), v4.begin(), v4.end(),
                [](int exp, short act) { return exp == (act - 10); },
                LOC) << "throws if equal";
        }, LOC);
        th::assert.contains(a.custom_message(), "equal at location 3", LOC);

        ASSERT_UNEQUAL_SEQ_CMP(v0.begin(), v0.end(), v1.begin(), v1.end(),
            [](int exp, int act) { return exp == (act - 10); }) << "does not throw if unequal (using macro)";

        auto b = ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_SEQ_CMP(v3.begin(), v3.end(), v4.begin(), v4.end(),
            [](int exp _Comma short act) { return exp == (act - 10); }) << "throws if equal (using macro)";
        );
        ASSERT_CONTAINS(b.custom_message(), "equal at location 3");
    }

    TESTCASE("compares <array of int> arrays using default comparer")
    {
        const int len{ 5 };
        const int v1[]{ 1, 2, 3, 4, 5 };
        const int v2[]{ 11, 12, 13, 14, 15 };
        th::assert.unequal_array(len, v1, v2, LOC) << "does not throw if unequal";

        const int v3[]{ 1, 2, 3, 4, 5 };
        const int v4[]{ 11, 2, 13, 4, 15 };
        auto a = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.unequal_array(len, v3, v4, LOC) << "throws if equal";
        }, LOC);
        th::assert.contains(a.custom_message(), "failed 2 out of 5", LOC);

        ASSERT_UNEQUAL_ARRAY(len, v1, v2) << "does not throw if equal (using macro)";

        auto b = ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_ARRAY(len, v3, v4) << "throws if equal (using macro)";
        );
        ASSERT_CONTAINS(b.custom_message(), "failed 2 out of 5");
    }

    TESTCASE("compares <array of int> arrays using custom comparer")
    {
        const int len{ 5 };
        const int v1[]{ 1, 2, 3, 4, 5 };
        const int v2[]{ 21, 22, 23, 24, 25 };
        th::assert.unequal_array(len, v1, v2,
            [](int exp, int act) { return exp == (act - 10); },
            LOC) << "does not throw if unequal";

        const int v3[]{ 1, 2, 3, 4, 5 };
        const int v4[]{ 21, 12, 27, 14, 25 };
        auto a = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.unequal_array(len, v3, v4,
                [](int exp, int act) { return exp == (act - 10); },
                LOC) << "throws if equal";
        }, LOC);
        th::assert.contains(a.custom_message(), "failed 2 out of 5", LOC);

        ASSERT_UNEQUAL_ARRAY_CMP(len, v1, v2,
            [](int exp _Comma int act) { return exp == (act - 10); }
        ) << "does not throw if unequal (using macro)";

        auto b = ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_ARRAY_CMP(len, v3, v4,
            [](int exp _Comma int act) { return exp == (act - 10); }
            ) << "throws if equal (using macro)";
        );
        ASSERT_CONTAINS(b.custom_message(), "failed 2 out of 5");
    }

    TESTCASE("compares <array of double> arrays using given ulp and default comparer")
    {
        const int len{ 3 };
        const double v1[]{ 0.1751528236650919900, 0.0511652155111260030, 0.3067856089049090200 };
        const double v2[]{ 0.1751528236650917700, 0.0511652155111257460, 0.3067856089049094700 };
        th::assert.unequal_array(len, v1, v2, 4, LOC) << "does not throw if unequal";

        auto a = th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.unequal_array(len, v1, v2, 36, LOC) << "throws if equal";
        }, LOC);
        th::assert.contains(a.custom_message(), "failed 2 out of 3", LOC);

        ASSERT_UNEQUAL_FP_ARRAY(len, v1, v2, 4) << "does not throw if unequal (using macro)";

        auto b = ASSERT_THROWS(thd::assert_context,
            ASSERT_UNEQUAL_FP_ARRAY(len, v1, v2, 38) << "throws if equal (using macro)";
        );
        ASSERT_CONTAINS(b.custom_message(), "failed 3 out of 3");
    }
}

TESTGROUP("testharness::assert::is_true")
{
    TESTCASE("does not throw if true, throws if false")
    {
        th::assert.is_true(true, LOC) << "does not throw if true";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.is_true(false, LOC) << "throws if false";
        }, LOC);

        ASSERT_IS_TRUE(true) << "does not throw if true (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_IS_TRUE(false) << "throws if false (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::is_false")
{
    TESTCASE("does not throw if false, throws if true")
    {
        th::assert.is_false(false, LOC) << "does not throw if false";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.is_false(true, LOC) << "throws if true";
        }, LOC);

        ASSERT_IS_FALSE(false) << "does not throw if false (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_IS_FALSE(true) << "throws if true (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::empty")
{
    TESTCASE("does not throw if empty, throws if not")
    {
        std::vector<int> v0;
        std::vector<int> v1{ 1, 2, 3, 4, 5 };

        th::assert.empty(v0, LOC) << "does not throw if empty";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.empty(v1, LOC) << "throws if not";
        }, LOC);

        ASSERT_EMPTY(v0) << "does not throw if empty (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_EMPTY(v1) << "throws if not (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::not_empty")
{
    TESTCASE("does not throw if not empty, throws if is")
    {
        std::vector<int> v0{ 1, 2, 3, 4, 5 };
        std::vector<int> v1;

        th::assert.not_empty(v0, LOC) << "does not throw if not empty";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.not_empty(v1, LOC) << "throws if is";
        }, LOC);

        ASSERT_NOT_EMPTY(v0) << "does not throw if not empty (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_NOT_EMPTY(v1) << "throws if is (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::null")
{
    TESTCASE("does not throw if nullptr, throws if not")
    {
        int v{ 1 };

        th::assert.null(nullptr, LOC) << "does not throw if nullptr";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.null(&v, LOC) << "throws if not";
        }, LOC);

        ASSERT_NULL(nullptr) << "does not throw if nullptr (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_NULL(&v) << "throws if not (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::not_null")
{
    TESTCASE("does not throw if not nullptr, throws if is")
    {
        int v{ 1 };

        th::assert.not_null(&v, LOC) << "does not throw if not empty";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.not_null(nullptr, LOC) << "throws if is";
        }, LOC);

        ASSERT_NOT_NULL(&v) << "does not throw if not empty (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_NOT_NULL(nullptr) << "throws if is (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::same")
{
    TESTCASE("does not throw if the same object references, throws if not")
    {
        const int v{ 1 }, w{ 1 };

        th::assert.same(v, v, LOC) << "does not throw if the same references";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.same(v, w, LOC) << "throws if not";
        }, LOC);

        ASSERT_SAME(v, v) << "does not throw if the same references (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_SAME(v, w) << "throws if not (using macro)";
        );
    }

    TESTCASE("does not throw if the same object pointers, throws if not")
    {
        const int v{ 1 }, w{ 1 };

        th::assert.same(&v, &v, LOC) << "does not throw if the same pointers";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.same(&v, &w, LOC) << "throws if not";
        }, LOC);

        ASSERT_SAME(&v, &v) << "does not throw if the same pointers (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_SAME(&v, &w) << "throws if not (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::not_same")
{
    TESTCASE("does not throw if different object references, throws if the same")
    {
        const int v{ 1 }, w{ 1 };

        th::assert.not_same(v, w, LOC) << "does not throw if different references";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.not_same(v, v, LOC) << "throws if the same";
        }, LOC);

        ASSERT_NOT_SAME(v, w) << "does not throw if different references (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_NOT_SAME(v, v) << "throws if the same (using macro)";
        );
    }

    TESTCASE("does not throw if different object pointers, throws if the same")
    {
        const int v{ 1 }, w{ 1 };

        th::assert.not_same(&v, &w, LOC) << "does not throw if different pointers";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.not_same(&v, &v, LOC) << "throws if the same";
        }, LOC);

        ASSERT_NOT_SAME(&v, &w) << "does not throw if different pointers (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_NOT_SAME(&v, &v) << "throws if the same (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::in_range")
{
    TESTCASE("throws if min is greater or equal to max")
    {
        th::assert.throws<std::invalid_argument>([]()  // NOLINT(bugprone-throw-keyword-missing)
        {
            th::assert.in_range(1.9, 2.1, 1.1, LOC) << "throws if min is greater than max";
        }, LOC);
        th::assert.throws<std::invalid_argument>([]()  // NOLINT(bugprone-throw-keyword-missing)
        {
            th::assert.in_range(2.1, 2.1, 2.1, LOC) << "throws if min is equal to max";
        }, LOC);

        ASSERT_THROWS(std::invalid_argument,
            ASSERT_IN_RANGE(1.9, 2.1, 1.1) << "throws if min is greater than max (using macro)";
        );
        ASSERT_THROWS(std::invalid_argument,
            ASSERT_IN_RANGE(2.1, 2.1, 2.1) << "throws if min is equal to max (using macro)";
        );
    }

    TESTCASE("does not throw if in range, throws if not")
    {
        th::assert.in_range(1.9, 1.1, 2.1, LOC) << "does not throw if in range";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.in_range(1.0, 1.1, 2.1, LOC) << "throws if less than min";
        }, LOC);
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.in_range(2.2, 1.1, 2.1, LOC) << "throws if greater than max";
        }, LOC);
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.in_range(2.1, 1.1, 2.1, LOC) << "throws if equal to max";
        }, LOC);

        ASSERT_IN_RANGE(1.9, 1.1, 2.1) << "does not throw if in range (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_IN_RANGE(1.0, 1.1, 2.1) << "throws if less than min (using macro)";
        );
        ASSERT_THROWS(thd::assert_context,
            ASSERT_IN_RANGE(2.2, 1.1, 2.1) << "throws if greater than max (using macro)";
        );
        ASSERT_THROWS(thd::assert_context,
            ASSERT_IN_RANGE(2.1, 1.1, 2.1) << "throws if equal to max (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::not_in_range")
{
    TESTCASE("throws if min is greater or equal to max")
    {
        th::assert.throws<std::invalid_argument>([]()  // NOLINT(bugprone-throw-keyword-missing)
        {
            th::assert.not_in_range(1.9, 2.1, 1.1, LOC) << "throws if min is greater than max";
        }, LOC);
        th::assert.throws<std::invalid_argument>([]()  // NOLINT(bugprone-throw-keyword-missing)
        {
            th::assert.not_in_range(2.1, 2.1, 2.1, LOC) << "throws if min is equal to max";
        }, LOC);

        ASSERT_THROWS(std::invalid_argument,
            ASSERT_NOT_IN_RANGE(1.9, 2.1, 1.1) << "throws if min is greater than max (using macro)";
        );
        ASSERT_THROWS(std::invalid_argument,
            ASSERT_NOT_IN_RANGE(2.1, 2.1, 2.1) << "throws if min is equal to max (using macro)";
        );
    }

    TESTCASE("does not throw if out of range, throws if in")
    {
        th::assert.not_in_range(1.0, 1.1, 2.1, LOC) << "does not throw if less than min";
        th::assert.not_in_range(2.2, 1.1, 2.1, LOC) << "does not throw if greater than max";
        th::assert.not_in_range(2.1, 1.1, 2.1, LOC) << "does not throw if equal to max";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.not_in_range(1.9, 1.1, 2.1, LOC) << "throws if in range";
        }, LOC);

        ASSERT_NOT_IN_RANGE(1.0, 1.1, 2.1) << "does not throw if less than min (using macro)";
        ASSERT_NOT_IN_RANGE(2.2, 1.1, 2.1) << "does not throw if greater than max (using macro)";
        ASSERT_NOT_IN_RANGE(2.1, 1.1, 2.1) << "does not throw if equal to max (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_NOT_IN_RANGE(1.9, 1.1, 2.1) << "throws if in range (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::contains")
{
    TESTCASE("does not throw if a sequence contains a pattern, throws if not")
    {
        std::string v("foobar");
        const std::string f("foo");
        const char* b{ "boo" };

        th::assert.contains(v, f, LOC) << "does not throw if a sequence contains a pattern";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.contains(v, b, LOC) << "throws if not";
        }, LOC);

        ASSERT_CONTAINS(v, f) << "does not throw if a sequence contains a pattern (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_CONTAINS(v, b) << "throws if not (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::does_not_contain")
{
    TESTCASE("does not throw if a sequence does not contain a pattern, throws if it does")
    {
        std::string v("foobar"), f("foo");
        const char* b{ "boo" };

        th::assert.does_not_contain(v, b, LOC) << "does not throw if a sequence does not contain a pattern";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.does_not_contain(v, f, LOC) << "throws if it does";
        }, LOC);

        ASSERT_DOES_NOT_CONTAIN(v, b) << "does not throw if a sequence does not contain a pattern (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_DOES_NOT_CONTAIN(v, f) << "throws if it does (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::contains_predicate")
{
    TESTCASE("does not throw if a sequence contains a predicate, throws if not")
    {
        std::vector<int> v{1, 2, 3, 4, 5};

        th::assert.contains_predicate(v, 3, LOC) << "does not throw if a sequence contains a predicate";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.contains_predicate(v, 13, LOC) << "throws if not";
        }, LOC);

        ASSERT_CONTAINS_PREDICATE(v, 3) << "does not throw if a sequence contains a predicate (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_CONTAINS_PREDICATE(v, 13) << "throws if not (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::does_not_contain_predicate")
{
    TESTCASE("does not throw if a sequence does not contain a predicate, throws if it does")
    {
        std::vector<int> v{1, 2, 3, 4, 5};

        th::assert.does_not_contain_predicate(v, 13, LOC) << "does not throw if a sequence does not contain a predicate";
        th::assert.throws<thd::assert_context>([&]()
        {
            th::assert.does_not_contain_predicate(v, 3, LOC) << "throws if it does";
        }, LOC);

        ASSERT_DOES_NOT_CONTAIN_PREDICATE(v, 13) << "does not throw if a sequence does not contain a predicate (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_DOES_NOT_CONTAIN_PREDICATE(v, 3) << "throws if it does (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::throws")
{
    TESTCASE("does not throw if a specified function throws the specified exception, throws if not")
    {
        th::assert.throws<thd::assert_context>([](){ th::assert.is_true(false); }, LOC);
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.throws<std::exception>([](){th::assert.is_true(false); }, LOC);
        }, LOC);

        ASSERT_THROWS(thd::assert_context, ASSERT_IS_TRUE(false););
        ASSERT_THROWS(thd::assert_context,
            th::assert.throws<std::exception>([](){th::assert.is_true(false); } _Comma LOC);
        );
    }
}

TESTGROUP("testharness::assert::does_not_throw")
{
    TESTCASE("does not throw if a specified function does not throw, throws if it does")
    {
        th::assert.does_not_throw([](){ return; }, LOC) << "does not throw if a sequence contains a pattern";
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.does_not_throw([](){th::assert.is_true(false); }, LOC) << "throws if it does";
        }, LOC);

        ASSERT_DOES_NOT_THROW([](){ return; }) << "does not throw if a sequence contains a pattern (using macro)";
        ASSERT_THROWS(thd::assert_context,
            ASSERT_DOES_NOT_THROW([](){th::assert.is_true(false); }) << "throws if not (using macro)";
        );
    }
}

TESTGROUP("testharness::assert::fail")
{
    TESTCASE("fails the active test")
    {
        th::assert.throws<thd::assert_context>([]()
        {
            th::assert.fail(LOC) << "always throws";
        }, LOC);

        ASSERT_THROWS(thd::assert_context,
            ASSERT_FAIL() << "always throws (using macro)";
        );
    }
}

TESTGROUP("testharness::event")
{
    TESTCASE("can be constructed from a severity, message text and location")
    {
	    const thd::event evt{ thd::severity::assert, std::string("message"), th::location("file", 42) };

        ASSERT_EQUAL("", evt.assert_call());
        ASSERT_EQUAL("", evt.assert_actual());
        ASSERT_EQUAL("", evt.assert_expected());
        ASSERT_EQUAL("", evt.assert_custom_message());
        ASSERT_EQUAL("", evt.assert_user_message());
        ASSERT_EQUAL("", evt.assert_call());
        ASSERT_EQUAL("message", evt.message());
        ASSERT_EQUAL(thd::severity::assert, evt.severity_level());
        ASSERT_IS_FALSE(evt.is_assert());
        ASSERT_IS_TRUE(evt.is_failure());
        ASSERT_EQUAL(42, evt.located().line_);
        ASSERT_EQUAL("file", evt.located().file_);
    }

    TESTCASE("can be constructed from a severity, and an assert context")
    {
        const thd::assert_context ac{ std::string("call"), th::location("file", 42) };
        const thd::event evt{ thd::severity::expect, ac };

        ASSERT_EQUAL("call", evt.assert_call());
        ASSERT_EQUAL("", evt.assert_actual());
        ASSERT_EQUAL("", evt.assert_expected());
        ASSERT_EQUAL("", evt.assert_custom_message());
        ASSERT_EQUAL("", evt.assert_user_message());
        ASSERT_EQUAL("", evt.message());
        ASSERT_EQUAL(thd::severity::expect, evt.severity_level());
        ASSERT_IS_TRUE(evt.is_assert());
        ASSERT_IS_TRUE(evt.is_failure());
        ASSERT_EQUAL(42, evt.located().line_);
        ASSERT_EQUAL("file", evt.located().file_);
    }

    TESTCASE("can be constructed from an std::exception")
    {
	    const thd::event evt{ std::invalid_argument("message") };

        ASSERT_EQUAL("", evt.assert_call());
        ASSERT_EQUAL("", evt.assert_actual());
        ASSERT_EQUAL("", evt.assert_expected());
        ASSERT_EQUAL("", evt.assert_custom_message());
        ASSERT_EQUAL("", evt.assert_user_message());
        ASSERT_EQUAL("uncaught std::exception: message", evt.message());
        ASSERT_EQUAL(thd::severity::fatal, evt.severity_level());
        ASSERT_IS_FALSE(evt.is_assert());
        ASSERT_IS_TRUE(evt.is_failure());
        ASSERT_EQUAL(0, evt.located().line_);
        ASSERT_EQUAL("", evt.located().file_);
    }

    TESTCASE("can be move-constructed")
    {
        const thd::assert_context ac{ std::string("call"), th::location("file", 42) };
        const thd::event evt{ thd::event{thd::severity::expect, ac} };

        ASSERT_EQUAL("call", evt.assert_call());
        ASSERT_EQUAL("", evt.assert_actual());
        ASSERT_EQUAL("", evt.assert_expected());
        ASSERT_EQUAL("", evt.assert_custom_message());
        ASSERT_EQUAL("", evt.assert_user_message());
        ASSERT_EQUAL("", evt.message());
        ASSERT_EQUAL(thd::severity::expect, evt.severity_level());
        ASSERT_IS_TRUE(evt.is_assert());
        ASSERT_IS_TRUE(evt.is_failure());
        ASSERT_EQUAL(42, evt.located().line_);
        ASSERT_EQUAL("file", evt.located().file_);
    }
}

TESTGROUP("testharness::expect")
{
    TESTCASE("appends multiple events which fail the test")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.expect.equal(1, 2, LOC) << "event #" << "1";
            context.expect.is_true(false, LOC) << "event #" << "2";
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "always a failure";
        ASSERT_EQUAL(2, tc.events().size()) << "two events";

#ifdef _MSC_VER
        ASSERT_EQUAL("expect.equal<int, int>", tc.events()[0].assert_call());
#else
        ASSERT_EQUAL("expect.equal<i, i>", tc.events()[0].assert_call());
#endif
        ASSERT_EQUAL("1", tc.events()[0].assert_expected());
        ASSERT_EQUAL("2", tc.events()[0].assert_actual());
        ASSERT_EQUAL("event #1", tc.events()[0].assert_user_message());
        ASSERT_IS_TRUE(tc.events()[0].is_failure());
        ASSERT_EQUAL(thd::severity::expect, tc.events()[0].severity_level());

        ASSERT_EQUAL("expect.is_true", tc.events()[1].assert_call());
        ASSERT_EQUAL("true", tc.events()[1].assert_expected());
        ASSERT_EQUAL("false", tc.events()[1].assert_actual());
        ASSERT_EQUAL("event #2", tc.events()[1].assert_user_message());
        ASSERT_IS_TRUE(tc.events()[1].is_failure());
        ASSERT_EQUAL(thd::severity::expect, tc.events()[1].severity_level());
    }

    TESTCASE("expect.equal does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            // 1
            context.expect.equal(-26946.839140521406, -26946.839140521435, 7, LOC);
            EXPECT_EQUAL_FP(-26946.839140521406, -26946.839140521435, 7);
            // 2
            context.expect.equal(1.8391405f, 1.8391406f, 0, LOC);
            EXPECT_EQUAL_FP(1.8391405f, 1.8391406f, 0);
            // 3
            context.expect.equal(1.839140521406, 1.8391406f, 0, LOC);
            EXPECT_EQUAL_FP(1.839140521406, 1.8391406f, 0);
            // 4
            context.expect.equal(1.8391406f, 1.839140521406, 0, LOC);
            EXPECT_EQUAL_FP(1.8391406f, 1.839140521406, 0);
            // 5
            context.expect.equal(42, 43, LOC);
            EXPECT_EQUAL(42, 43);
            // 6
            context.expect.equal(42, 42, [](int&& exp, int&& act) { return exp == (act / 10); }, LOC);
            EXPECT_EQUAL_CMP(42, 42, [](int&& exp _Comma int&& act){ return exp == (act / 10); });
            // 7
            context.expect.equal(42, 43l, LOC);
            EXPECT_EQUAL(42, 43l);
            // 8
            context.expect.equal(42u, 43ul, LOC);
            EXPECT_EQUAL(42u, 43ul);
            // 9
            context.expect.equal(0x61, 'b', LOC);
            EXPECT_EQUAL(0x61, 'b');
            // 10
            context.expect.equal("foobar", "foobar*", LOC);
            EXPECT_EQUAL("foobar", "foobar*");
            // 11
            context.expect.equal(std::string("foobar"), std::string("foobar*"), LOC);
            EXPECT_EQUAL(std::string("foobar"), std::string("foobar*"));
            // 12
            context.expect.equal(true, false, LOC);
            EXPECT_EQUAL(true, false);
            // 13
            std::vector<int> v0{ 1, 2, 3, 4, 5 };
            std::vector<short> v1{ 1, 2, 3, 7, 5 };
            context.expect.equal_seq(v0.begin(), v0.end(), v1.begin(), v1.end(), LOC);
            EXPECT_EQUAL_SEQ(v0.begin(), v0.end(), v1.begin(), v1.end());
            // 14
            std::vector<int> v2{ 1, 2, 3, 4, 5 };
            std::vector<short> v3{ 11, 12, 13, 17, 15 };
            context.expect.equal_seq(v2.begin(), v2.end(), v3.begin(), v3.end(),
                [](int exp, short act) { return exp == (act - 10); }, LOC);
            EXPECT_EQUAL_SEQ_CMP(v2.begin(), v2.end(), v3.begin(), v3.end(),
                [](int exp _Comma short act) { return exp == (act - 10); });
            // 15
            const int v4[]{ 1, 2, 3, 4, 5 };
            const int v5[]{ 1, 2, 7, 4, 8 };
            context.expect.equal_array(5, v4, v5, LOC);
            EXPECT_EQUAL_ARRAY(5, v4, v5);
            // 16
            const int v6[]{ 1, 2, 3, 4, 5 };
            const int v7[]{ 11, 12, 17, 14, 18 };
            context.expect.equal_array(5, v6, v7, [](int exp, int act) { return exp == (act - 10); }, LOC);
            EXPECT_EQUAL_ARRAY_CMP(5, v6, v7, [](int exp _Comma int act) { return exp == (act - 10); });
            // 17
            const double v8[]{ 0.1751528236650919900, 0.0511652155111260030, 0.3067856089049090200 };
            const double v9[]{ 0.1751528236650917700, 0.0511652155111257460, 0.3067856089049094700 };
            context.expect.equal_array(3, v8, v9, 36, LOC);
            EXPECT_EQUAL_FP_ARRAY(3, v8, v9, 36);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2 * 17, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.unequal does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            // 1
            context.expect.unequal(-26946.839140521406, -26946.839140521435, 8, LOC);
            EXPECT_UNEQUAL_FP(-26946.839140521406, -26946.839140521435, 8);
            // 2
            context.expect.unequal(1.8391405f, 1.8391406f, 1, LOC);
            EXPECT_UNEQUAL_FP(1.8391405f, 1.8391406f, 1);
            // 3
            context.expect.unequal(1.839140521406, 1.8391406f, 1, LOC);
            EXPECT_UNEQUAL_FP(1.839140521406, 1.8391406f, 1);
            // 4
            context.expect.unequal(1.8391406f, 1.839140521406, 1, LOC);
            EXPECT_UNEQUAL_FP(1.8391406f, 1.839140521406, 1);
            // 5
            context.expect.unequal(42, 42, LOC);
            EXPECT_UNEQUAL(42, 42);
            // 6
            context.expect.unequal(4, 42, [](int&& exp, int&& act) { return exp == (act / 10); }, LOC);
            EXPECT_UNEQUAL_CMP(4, 42, [](int&& exp _Comma int&& act){ return exp == (act / 10); });
            // 7
            context.expect.unequal(42, 42l, LOC);
            EXPECT_UNEQUAL(42, 42l);
            // 8
            context.expect.unequal(42u, 42ul, LOC);
            EXPECT_UNEQUAL(42u, 42ul);
            // 9
            context.expect.unequal(0x61, 'a', LOC);
            EXPECT_UNEQUAL(0x61, 'a');
            // 10
            context.expect.unequal("foobar", "foobar", LOC);
            EXPECT_UNEQUAL("foobar", "foobar");
            // 11
            context.expect.unequal(std::string("foobar"), std::string("foobar"), LOC);
            EXPECT_UNEQUAL(std::string("foobar"), std::string("foobar"));
            // 12
            context.expect.unequal(true, true, LOC);
            EXPECT_UNEQUAL(true, true);
            // 13
            std::vector<int> v0{ 1, 2, 3, 4, 5 };
            std::vector<short> v1{ 11, 12, 13, 4, 15 };
            context.expect.unequal_seq(v0.begin(), v0.end(), v1.begin(), v1.end(), LOC);
            EXPECT_UNEQUAL_SEQ(v0.begin(), v0.end(), v1.begin(), v1.end());
            // 14
            std::vector<int> v2{ 1, 2, 3, 4, 5 };
            std::vector<short> v3{ 21, 22, 23, 14, 25 };
            context.expect.unequal_seq(v2.begin(), v2.end(), v3.begin(), v3.end(),
                [](int exp, short act) { return exp == (act - 10); }, LOC);
            EXPECT_UNEQUAL_SEQ_CMP(v2.begin(), v2.end(), v3.begin(), v3.end(),
                [](int exp _Comma short act) { return exp == (act - 10); });
            // 15
            const int v4[]{ 1, 2, 3, 4, 5 };
            const int v5[]{ 11, 2, 13, 4, 15 };
            context.expect.unequal_array(5, v4, v5, LOC);
            EXPECT_UNEQUAL_ARRAY(5, v4, v5);
            // 16
            const int v6[]{ 1, 2, 3, 4, 5 };
            const int v7[]{ 21, 12, 27, 14, 25 };
            context.expect.unequal_array(5, v6, v7, [](int exp, int act) { return exp == (act - 10); }, LOC);
            EXPECT_UNEQUAL_ARRAY_CMP(5, v6, v7, [](int exp _Comma int act) { return exp == (act - 10); });
            // 17
            const double v8[]{ 0.1751528236650919900, 0.0511652155111260030, 0.3067856089049090200 };
            const double v9[]{ 0.1751528236650917700, 0.0511652155111257460, 0.3067856089049094700 };
            context.expect.unequal_array(3, v8, v9, 46, LOC);
            EXPECT_UNEQUAL_FP_ARRAY(3, v8, v9, 46);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2 * 17, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.is_true does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.expect.is_true(false, LOC);
            EXPECT_IS_TRUE(false);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.is_false does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.expect.is_false(true, LOC);
            EXPECT_IS_FALSE(true);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.empty does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            std::vector<int> v{ 1, 2, 3, 4, 5 };
            context.expect.empty(v, LOC);
            EXPECT_EMPTY(v);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.not_empty does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            std::vector<int> v;
            context.expect.not_empty(v, LOC);
            EXPECT_NOT_EMPTY(v);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.null does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            int v{ 1 };
            context.expect.null(&v, LOC);
            EXPECT_NULL(&v);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.not_null does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.expect.not_null(nullptr, LOC);
            EXPECT_NOT_NULL(nullptr);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.same does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            char const *v1 = "foo", *v2 = "bar";
            context.expect.same(v1, v2, LOC);
            EXPECT_SAME(v1, v2);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.not_same does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const int v{ 1 };
            context.expect.not_same(v, v, LOC);
            context.expect.not_same(&v, &v, LOC);
            EXPECT_NOT_SAME(v, v);
            EXPECT_NOT_SAME(&v, &v);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(4, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.in_range does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.expect.in_range(1.0, 1.1, 2.1, LOC);
            context.expect.in_range(2.2, 1.1, 2.1, LOC);
            //context.expect.in_range(2.1, 1.1, 2.1, LOC);
            //context.expect.in_range(1.9, 2.1, 1.1, LOC);
            //context.expect.in_range(2.1, 2.1, 2.1, LOC);
            EXPECT_IN_RANGE(1.0, 1.1, 2.1);
            EXPECT_IN_RANGE(2.2, 1.1, 2.1);
            //EXPECT_IN_RANGE(2.1, 1.1, 2.1);
            //EXPECT_IN_RANGE(1.9, 2.1, 1.1);
            //EXPECT_IN_RANGE(2.1, 2.1, 2.1);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(4, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.not_in_range does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.expect.not_in_range(1.9, 1.1, 2.1, LOC);
            //context.expect.not_in_range(1.9, 2.1, 1.1, LOC);
            //context.expect.not_in_range(2.1, 2.1, 2.1, LOC);
            EXPECT_NOT_IN_RANGE(1.9, 1.1, 2.1);
            //EXPECT_NOT_IN_RANGE(1.9, 2.1, 1.1);
            //EXPECT_NOT_IN_RANGE(2.1, 2.1, 2.1);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.contains does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const std::string v("foobar");
            const char* b{ "boo" };
            context.expect.contains(v, b, LOC);
            EXPECT_CONTAINS(v, b);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.does_not_contain does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const std::string v("foobar"), f("foo");
            context.expect.does_not_contain(v, f, LOC);
            EXPECT_DOES_NOT_CONTAIN(v, f);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.contains_predicate does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const std::vector<int> v{ 1, 2, 3, 4, 5 };
            context.expect.contains_predicate(v, 13, LOC);
            EXPECT_CONTAINS_PREDICATE(v, 13);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.does_not_contain_predicate does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const std::vector<int> v{ 1, 2, 3, 4, 5 };
            context.expect.does_not_contain_predicate(v, 3, LOC);
            EXPECT_DOES_NOT_CONTAIN_PREDICATE(v, 3);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.does_not_throw does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.expect.does_not_throw([](){th::assert.is_true(false); }, LOC);
            EXPECT_DOES_NOT_THROW([](){th::assert.is_true(false); });
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("expect.fail does not throw but makes test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.expect.fail(LOC);
            EXPECT_FAIL();
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_FALSE(tc.run(o)) << "test is failed";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }
}

TESTGROUP("testharness::warn")
{
    TESTCASE("appends multiple events which do not fail the test")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.warn.equal(1, 2, LOC) << "event #" << "1";
            context.warn.is_true(false, LOC) << "event #" << "2";
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "not a failure";
        ASSERT_EQUAL(2, tc.events().size()) << "two events";

#ifdef _MSC_VER
        ASSERT_EQUAL("warn.equal<int, int>", tc.events()[0].assert_call());
#else
        ASSERT_EQUAL("warn.equal<i, i>", tc.events()[0].assert_call());
#endif
        ASSERT_EQUAL("1", tc.events()[0].assert_expected());
        ASSERT_EQUAL("2", tc.events()[0].assert_actual());
        ASSERT_EQUAL("event #1", tc.events()[0].assert_user_message());
        ASSERT_IS_FALSE(tc.events()[0].is_failure());
        ASSERT_EQUAL(thd::severity::warning, tc.events()[0].severity_level());

        ASSERT_EQUAL("warn.is_true", tc.events()[1].assert_call());
        ASSERT_EQUAL("true", tc.events()[1].assert_expected());
        ASSERT_EQUAL("false", tc.events()[1].assert_actual());
        ASSERT_EQUAL("event #2", tc.events()[1].assert_user_message());
        ASSERT_IS_FALSE(tc.events()[1].is_failure());
        ASSERT_EQUAL(thd::severity::warning, tc.events()[1].severity_level());
    }

    TESTCASE("warn.equal does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            // 1
            context.warn.equal(-26946.839140521406, -26946.839140521435, 7, LOC);
            WARN_EQUAL_FP(-26946.839140521406, -26946.839140521435, 7);
            // 2
            context.warn.equal(1.8391405f, 1.8391406f, 0, LOC);
            WARN_EQUAL_FP(1.8391405f, 1.8391406f, 0);
            // 3
            context.warn.equal(1.839140521406, 1.8391406f, 0, LOC);
            WARN_EQUAL_FP(1.839140521406, 1.8391406f, 0);
            // 4
            context.warn.equal(1.8391406f, 1.839140521406, 0, LOC);
            WARN_EQUAL_FP(1.8391406f, 1.839140521406, 0);
            // 5
            context.warn.equal(42, 43, LOC);
            WARN_EQUAL(42, 43);
            // 6
            context.warn.equal(42, 42, [](int&& exp, int&& act) { return exp == (act / 10); }, LOC);
            WARN_EQUAL_CMP(42, 42, [](int&& exp _Comma int&& act){ return exp == (act / 10); });
            // 7
            context.warn.equal(42, 43l, LOC);
            WARN_EQUAL(42, 43l);
            // 8
            context.warn.equal(42u, 43ul, LOC);
            WARN_EQUAL(42u, 43ul);
            // 9
            context.warn.equal(0x61, 'b', LOC);
            WARN_EQUAL(0x61, 'b');
            // 10
            context.warn.equal("foobar", "foobar*", LOC);
            WARN_EQUAL("foobar", "foobar*");
            // 11
            context.warn.equal(std::string("foobar"), std::string("foobar*"), LOC);
            WARN_EQUAL(std::string("foobar"), std::string("foobar*"));
            // 12
            context.warn.equal(true, false, LOC);
            WARN_EQUAL(true, false);
            // 13
            std::vector<int> v0{ 1, 2, 3, 4, 5 };
            std::vector<short> v1{ 1, 2, 3, 7, 5 };
            context.warn.equal_seq(v0.begin(), v0.end(), v1.begin(), v1.end(), LOC);
            WARN_EQUAL_SEQ(v0.begin(), v0.end(), v1.begin(), v1.end());
            // 14
            std::vector<int> v2{ 1, 2, 3, 4, 5 };
            std::vector<short> v3{ 11, 12, 13, 17, 15 };
            context.warn.equal_seq(v2.begin(), v2.end(), v3.begin(), v3.end(),
                [](int exp, short act) { return exp == (act - 10); }, LOC);
            WARN_EQUAL_SEQ_CMP(v2.begin(), v2.end(), v3.begin(), v3.end(),
                [](int exp _Comma short act) { return exp == (act - 10); });
            // 15
            const int v4[]{ 1, 2, 3, 4, 5 };
            const int v5[]{ 1, 2, 7, 4, 8 };
            context.warn.equal_array(5, v4, v5, LOC);
            WARN_EQUAL_ARRAY(5, v4, v5);
            // 16
            const int v6[]{ 1, 2, 3, 4, 5 };
            const int v7[]{ 11, 12, 17, 14, 18 };
            context.warn.equal_array(5, v6, v7, [](int exp, int act) { return exp == (act - 10); }, LOC);
            WARN_EQUAL_ARRAY_CMP(5, v6, v7, [](int exp _Comma int act) { return exp == (act - 10); });
            // 17
            const double v8[]{ 0.1751528236650919900, 0.0511652155111260030, 0.3067856089049090200 };
            const double v9[]{ 0.1751528236650917700, 0.0511652155111257460, 0.3067856089049094700 };
            context.warn.equal_array(3, v8, v9, 36, LOC);
            WARN_EQUAL_FP_ARRAY(3, v8, v9, 36);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2 * 17, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.unequal does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            // 1
            context.warn.unequal(-26946.839140521406, -26946.839140521435, 8, LOC);
            WARN_UNEQUAL_FP(-26946.839140521406, -26946.839140521435, 8);
            // 2
            context.warn.unequal(1.8391405f, 1.8391406f, 1, LOC);
            WARN_UNEQUAL_FP(1.8391405f, 1.8391406f, 1);
            // 3
            context.warn.unequal(1.839140521406, 1.8391406f, 1, LOC);
            WARN_UNEQUAL_FP(1.839140521406, 1.8391406f, 1);
            // 4
            context.warn.unequal(1.8391406f, 1.839140521406, 1, LOC);
            WARN_UNEQUAL_FP(1.8391406f, 1.839140521406, 1);
            // 5
            context.warn.unequal(42, 42, LOC);
            WARN_UNEQUAL(42, 42);
            // 6
            context.warn.unequal(4, 42, [](int&& exp, int&& act) { return exp == (act / 10); }, LOC);
            WARN_UNEQUAL_CMP(4, 42, [](int&& exp _Comma int&& act){ return exp == (act / 10); });
            // 7
            context.warn.unequal(42, 42l, LOC);
            WARN_UNEQUAL(42, 42l);
            // 8
            context.warn.unequal(42u, 42ul, LOC);
            WARN_UNEQUAL(42u, 42ul);
            // 9
            context.warn.unequal(0x61, 'a', LOC);
            WARN_UNEQUAL(0x61, 'a');
            // 10
            context.warn.unequal("foobar", "foobar", LOC);
            WARN_UNEQUAL("foobar", "foobar");
            // 11
            context.warn.unequal(std::string("foobar"), std::string("foobar"), LOC);
            WARN_UNEQUAL(std::string("foobar"), std::string("foobar"));
            // 12
            context.warn.unequal(true, true, LOC);
            WARN_UNEQUAL(true, true);
            // 13
            std::vector<int> v0{ 1, 2, 3, 4, 5 };
            std::vector<short> v1{ 11, 12, 13, 4, 15 };
            context.warn.unequal_seq(v0.begin(), v0.end(), v1.begin(), v1.end(), LOC);
            WARN_UNEQUAL_SEQ(v0.begin(), v0.end(), v1.begin(), v1.end());
            // 14
            std::vector<int> v2{ 1, 2, 3, 4, 5 };
            std::vector<short> v3{ 21, 22, 23, 14, 25 };
            context.warn.unequal_seq(v2.begin(), v2.end(), v3.begin(), v3.end(),
                [](int exp, short act) { return exp == (act - 10); }, LOC);
            WARN_UNEQUAL_SEQ_CMP(v2.begin(), v2.end(), v3.begin(), v3.end(),
                [](int exp _Comma short act) { return exp == (act - 10); });
            // 15
            const int v4[]{ 1, 2, 3, 4, 5 };
            const int v5[]{ 11, 2, 13, 4, 15 };
            context.warn.unequal_array(5, v4, v5, LOC);
            WARN_UNEQUAL_ARRAY(5, v4, v5);
            // 16
            const int v6[]{ 1, 2, 3, 4, 5 };
            const int v7[]{ 21, 12, 27, 14, 25 };
            context.warn.unequal_array(5, v6, v7, [](int exp, int act) { return exp == (act - 10); }, LOC);
            WARN_UNEQUAL_ARRAY_CMP(5, v6, v7, [](int exp _Comma int act) { return exp == (act - 10); });
            // 17
            const double v8[]{ 0.1751528236650919900, 0.0511652155111260030, 0.3067856089049090200 };
            const double v9[]{ 0.1751528236650917700, 0.0511652155111257460, 0.3067856089049094700 };
            context.warn.unequal_array(3, v8, v9, 46, LOC);
            WARN_UNEQUAL_FP_ARRAY(3, v8, v9, 46);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2 * 17, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.is_true does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.warn.is_true(false, LOC);
            WARN_IS_TRUE(false);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.is_false does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.warn.is_false(true, LOC);
            WARN_IS_FALSE(true);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.empty does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            std::vector<int> v{ 1, 2, 3, 4, 5 };
            context.warn.empty(v, LOC);
            WARN_EMPTY(v);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.not_empty does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            std::vector<int> v;
            context.warn.not_empty(v, LOC);
            WARN_NOT_EMPTY(v);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.null does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            int v{ 1 };
            context.warn.null(&v, LOC);
            WARN_NULL(&v);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.not_null does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.warn.not_null(nullptr, LOC);
            WARN_NOT_NULL(nullptr);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.same does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            char const *v1 = "foo", *v2 = "bar";
            context.warn.same(v1, v2, LOC);
            WARN_SAME(v1, v2);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.not_same does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const int v{ 1 };
            context.warn.not_same(v, v, LOC);
            context.warn.not_same(&v, &v, LOC);
            WARN_NOT_SAME(v, v);
            WARN_NOT_SAME(&v, &v);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(4, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.in_range does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.warn.in_range(1.0, 1.1, 2.1, LOC);
            context.warn.in_range(2.2, 1.1, 2.1, LOC);
            //context.warn.in_range(2.1, 1.1, 2.1, LOC);
            //context.warn.in_range(1.9, 2.1, 1.1, LOC);
            //context.warn.in_range(2.1, 2.1, 2.1, LOC);
            WARN_IN_RANGE(1.0, 1.1, 2.1);
            WARN_IN_RANGE(2.2, 1.1, 2.1);
            //WARN_IN_RANGE(2.1, 1.1, 2.1);
            //WARN_IN_RANGE(1.9, 2.1, 1.1);
            //WARN_IN_RANGE(2.1, 2.1, 2.1);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(4, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.not_in_range does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.warn.not_in_range(1.9, 1.1, 2.1, LOC);
            //context.warn.not_in_range(1.9, 2.1, 1.1, LOC);
            //context.warn.not_in_range(2.1, 2.1, 2.1, LOC);
            WARN_NOT_IN_RANGE(1.9, 1.1, 2.1);
            //WARN_NOT_IN_RANGE(1.9, 2.1, 1.1);
            //WARN_NOT_IN_RANGE(2.1, 2.1, 2.1);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.contains does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const std::string v("foobar");
            const char* b{ "boo" };
            context.warn.contains(v, b, LOC);
            WARN_CONTAINS(v, b);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.does_not_contain does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const std::string v("foobar"), f("foo");
            context.warn.does_not_contain(v, f, LOC);
            WARN_DOES_NOT_CONTAIN(v, f);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.contains_predicate does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const std::vector<int> v{ 1, 2, 3, 4, 5 };
            context.warn.contains_predicate(v, 13, LOC);
            WARN_CONTAINS_PREDICATE(v, 13);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.does_not_contain_predicate does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            const std::vector<int> v{ 1, 2, 3, 4, 5 };
            context.warn.does_not_contain_predicate(v, 3, LOC);
            WARN_DOES_NOT_CONTAIN_PREDICATE(v, 3);
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.does_not_throw does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.warn.does_not_throw([](){th::assert.is_true(false); }, LOC);
            WARN_DOES_NOT_THROW([](){th::assert.is_true(false); });
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }

    TESTCASE("warn.fail does not throw and does not make test failed")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.warn.fail(LOC);
            WARN_FAIL();
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "test succeeded";
        ASSERT_EQUAL(2, tc.events().size()) << "event count is correct";
    }
}

TESTGROUP("testharness::log")
{
    TESTCASE("appends multiple log events with severity 'debug' which do not fail the test")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.log.debug(LOC) << "event #" << "1";
            LOG_DEBUG() << "event #" << "2";
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "not a failure";
        ASSERT_EQUAL(2, tc.events().size()) << "two events";

        ASSERT_EQUAL("", tc.events()[0].assert_call());
        ASSERT_EQUAL("", tc.events()[0].assert_expected());
        ASSERT_EQUAL("", tc.events()[0].assert_actual());
        ASSERT_EQUAL("", tc.events()[0].assert_user_message());
        ASSERT_EQUAL("event #1", tc.events()[0].message());
        ASSERT_IS_FALSE(tc.events()[0].is_failure());
        ASSERT_EQUAL(thd::severity::debug, tc.events()[0].severity_level());

        ASSERT_EQUAL("", tc.events()[1].assert_call());
        ASSERT_EQUAL("", tc.events()[1].assert_expected());
        ASSERT_EQUAL("", tc.events()[1].assert_actual());
        ASSERT_EQUAL("", tc.events()[1].assert_user_message());
        ASSERT_EQUAL("event #2", tc.events()[1].message());
        ASSERT_IS_FALSE(tc.events()[1].is_failure());
        ASSERT_EQUAL(thd::severity::debug, tc.events()[1].severity_level());
    }

    TESTCASE("appends multiple log events with severity 'warning' which do not fail the test")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.log.warn(LOC) << "event #" << "1";
            LOG_WARN() << "event #" << "2";
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "not a failure";
        ASSERT_EQUAL(2, tc.events().size()) << "two events";

        ASSERT_EQUAL("", tc.events()[0].assert_call());
        ASSERT_EQUAL("", tc.events()[0].assert_expected());
        ASSERT_EQUAL("", tc.events()[0].assert_actual());
        ASSERT_EQUAL("", tc.events()[0].assert_user_message());
        ASSERT_EQUAL("event #1", tc.events()[0].message());
        ASSERT_IS_FALSE(tc.events()[0].is_failure());
        ASSERT_EQUAL(thd::severity::warning, tc.events()[0].severity_level());

        ASSERT_EQUAL("", tc.events()[1].assert_call());
        ASSERT_EQUAL("", tc.events()[1].assert_expected());
        ASSERT_EQUAL("", tc.events()[1].assert_actual());
        ASSERT_EQUAL("", tc.events()[1].assert_user_message());
        ASSERT_EQUAL("event #2", tc.events()[1].message());
        ASSERT_IS_FALSE(tc.events()[1].is_failure());
        ASSERT_EQUAL(thd::severity::warning, tc.events()[1].severity_level());
    }

    TESTCASE("appends multiple log events with severity 'info' which do not fail the test")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            context.log.info(LOC) << "event #" << "1";
            LOG_INFO() << "event #" << "2";
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "not a failure";
        ASSERT_EQUAL(2, tc.events().size()) << "two events";

        ASSERT_EQUAL("", tc.events()[0].assert_call());
        ASSERT_EQUAL("", tc.events()[0].assert_expected());
        ASSERT_EQUAL("", tc.events()[0].assert_actual());
        ASSERT_EQUAL("", tc.events()[0].assert_user_message());
        ASSERT_EQUAL("event #1", tc.events()[0].message());
        ASSERT_IS_FALSE(tc.events()[0].is_failure());
        ASSERT_EQUAL(thd::severity::info, tc.events()[0].severity_level());

        ASSERT_EQUAL("", tc.events()[1].assert_call());
        ASSERT_EQUAL("", tc.events()[1].assert_expected());
        ASSERT_EQUAL("", tc.events()[1].assert_actual());
        ASSERT_EQUAL("", tc.events()[1].assert_user_message());
        ASSERT_EQUAL("event #2", tc.events()[1].message());
        ASSERT_IS_FALSE(tc.events()[1].is_failure());
        ASSERT_EQUAL(thd::severity::info, tc.events()[1].severity_level());
    }

    TESTCASE("appends multiple log events with different severities which do not fail the test")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context& context)
        {
            LOG_DEBUG() << "event #" << "1";
            LOG_INFO() << "event #" << "2";
            LOG_WARN() << "event #" << "3";
        }, th::auto_timeout, LOC);
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o)) << "not a failure";
        ASSERT_EQUAL(3, tc.events().size()) << "two events";

        ASSERT_EQUAL("", tc.events()[0].assert_call());
        ASSERT_EQUAL("", tc.events()[0].assert_expected());
        ASSERT_EQUAL("", tc.events()[0].assert_actual());
        ASSERT_EQUAL("", tc.events()[0].assert_user_message());
        ASSERT_EQUAL("event #1", tc.events()[0].message());
        ASSERT_IS_FALSE(tc.events()[0].is_failure());
        ASSERT_EQUAL(thd::severity::debug, tc.events()[0].severity_level());

        ASSERT_EQUAL("", tc.events()[1].assert_call());
        ASSERT_EQUAL("", tc.events()[1].assert_expected());
        ASSERT_EQUAL("", tc.events()[1].assert_actual());
        ASSERT_EQUAL("", tc.events()[1].assert_user_message());
        ASSERT_EQUAL("event #2", tc.events()[1].message());
        ASSERT_IS_FALSE(tc.events()[1].is_failure());
        ASSERT_EQUAL(thd::severity::info, tc.events()[1].severity_level());

        ASSERT_EQUAL("", tc.events()[2].assert_call());
        ASSERT_EQUAL("", tc.events()[2].assert_expected());
        ASSERT_EQUAL("", tc.events()[2].assert_actual());
        ASSERT_EQUAL("", tc.events()[2].assert_user_message());
        ASSERT_EQUAL("event #3", tc.events()[2].message());
        ASSERT_IS_FALSE(tc.events()[2].is_failure());
        ASSERT_EQUAL(thd::severity::warning, tc.events()[2].severity_level());
    }

    //TESTCASE_WITH_CONTEXT("example log output")
    //{
    //    LOG_DEBUG() << "example debug message";
    //    LOG_INFO() << "example info message";
    //    LOG_WARN() << "example warn message";
    //}
}

TESTGROUP("testharness::testcase")
{
    TESTCASE("can be constructed")
    {
	    const thd::testcase tc(std::string("name"), std::string("group"), [](const th::test_context&){},
            th::auto_timeout, th::location("file", 42));

        ASSERT_EQUAL("name", tc.name());
        ASSERT_EQUAL("group", tc.group());
        ASSERT_IS_TRUE(tc.id() >= 0);
        ASSERT_EQUAL(42, tc.located().line_);
        ASSERT_EQUAL("file", tc.located().file_);
    }

    TESTCASE("id always grows")
    {
        thd::testcase tc1(std::string(), std::string(), [](const th::test_context&){},
            th::auto_timeout, th::location());

        thd::testcase tc2(std::string(), std::string(), [](const th::test_context&){},
            th::auto_timeout, th::location());

        ASSERT_EQUAL(tc2.id(), tc1.id() + 1);
    }

    TESTCASE("an event can be added")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context&){},
            th::auto_timeout, th::location());
        tc.add_event(thd::event{ thd::severity::info, std::string("message"), th::location() });

        ASSERT_EQUAL(1, tc.events().size());
        ASSERT_EQUAL("", tc.events()[0].assert_call());
        ASSERT_EQUAL("", tc.events()[0].assert_expected());
        ASSERT_EQUAL("", tc.events()[0].assert_actual());
        ASSERT_EQUAL("", tc.events()[0].assert_user_message());
        ASSERT_EQUAL("message", tc.events()[0].message());
        ASSERT_IS_FALSE(tc.events()[0].is_failure());
        ASSERT_EQUAL(thd::severity::info, tc.events()[0].severity_level());
    }

    TESTCASE("can be run")
    {
        thd::testcase tc(std::string(), std::string(), [](const th::test_context&){}, 12345, th::location());
        const thd::options o;
        ASSERT_IS_TRUE(tc.run(o));
        ASSERT_EQUAL(12345, tc.timeout_milliseconds());
        ASSERT_IS_TRUE(tc.duration() >= std::chrono::nanoseconds(0));
    }
}

TESTGROUP("testharness::testcollection")
{
    TESTCASE("is not empty since we are running this test")
    {
        ASSERT_IS_TRUE(!thd::testcollection::instance().tests.empty());
    }
}

}
