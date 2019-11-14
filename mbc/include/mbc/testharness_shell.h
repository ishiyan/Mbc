#pragma once

#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <queue>
#include <random>
#include <regex>
#include <thread>
#include <unordered_map>
#include <utility>

#include "testharness.h"

#if defined(_WIN32)
#include <Windows.h>
#else
#define __declspec(x)
#endif

namespace mbc::testharness::shell {

    namespace detail
    {
        // ReSharper disable once CppInconsistentNaming
        class shared_output
        {
        public:
            shared_output(output_interface& test_reporter) : output_{ test_reporter } {}

            shared_output() = delete;
            shared_output(const shared_output&) = delete;
            shared_output(shared_output&&) = delete;
            shared_output& operator =(const shared_output&) = delete;
            shared_output& operator =(shared_output&&) = delete;
            ~shared_output() = default;

            void report_start(const th::detail::testcase& testcase)
            {
                std::lock_guard<std::mutex> guard(lock_);
                output_.get().report_start(testcase);
            }

            void report_event(const th::detail::testcase& testcase, const th::detail::event_interface& evt)
            {
                std::lock_guard<std::mutex> guard(lock_);
                output_.get().report_event(testcase, evt);
            }

            void report_skip(const th::detail::testcase& testcase, const std::string& reason)
            {
                std::lock_guard<std::mutex> guard(lock_);
                output_.get().report_skip(testcase, reason.c_str());
            }

            void report_finish(const th::detail::testcase& detail, const std::chrono::nanoseconds nanoseconds)
            {
                std::lock_guard<std::mutex> guard(lock_);
                output_.get().report_finish(detail, nanoseconds);
            }

            void report_complete(const int total, const int skipped, const int failed, const std::chrono::nanoseconds nanoseconds_total) const
            {
                output_.get().report_complete(total, skipped, failed, nanoseconds_total);
            }

        private:
            std::mutex lock_;
            std::reference_wrapper<output_interface> output_;
        };

        class attached_output
        {
        public:
            attached_output(shared_output& output) : attached_(true), output_(std::ref(output)) {}

            attached_output() = delete;
            attached_output(const attached_output&) = delete;
            attached_output(attached_output&&) = delete;
            attached_output& operator =(const attached_output&) = delete;
            attached_output& operator =(attached_output&&) = delete;
            ~attached_output() = default;

            void detach()
            {
                std::lock_guard<std::mutex> guard(lock_);
                attached_ = false;
            }

            void report_start(const th::detail::testcase& testcase)
            {
                std::lock_guard<std::mutex> guard(lock_);
                if (attached_)
                    output_.get().report_start(testcase);
            }

            void report_event(const th::detail::testcase& testcase, const th::detail::event_interface& evt)
            {
                std::lock_guard<std::mutex> guard(lock_);
                if (attached_)
                    output_.get().report_event(testcase, evt);
            }

            void report_skip(const th::detail::testcase& testcase, const std::string& reason)
            {
                std::lock_guard<std::mutex> guard(lock_);
                if (attached_)
                    output_.get().report_skip(testcase, reason);
            }

            void report_finish(const th::detail::testcase& testcase, const std::chrono::nanoseconds nanoseconds)
            {
                std::lock_guard<std::mutex> guard(lock_);
                if (attached_)
                    output_.get().report_finish(testcase, nanoseconds);
            }

            static void report_complete(size_t, size_t, size_t, std::chrono::nanoseconds)
            {
                throw std::logic_error("No one holding an attached_output object should be calling report_complete().");
            }

        private:
            std::mutex lock_;
            bool attached_;
            std::reference_wrapper<shared_output> output_;
        };

        inline int run_tests(output_interface& output, filter_callback filter, const th::detail::options& opt,
            std::chrono::milliseconds max_run_time_milliseconds, int max_concurrent)
        {
            const std::vector<std::shared_ptr<th::detail::testcase>>
                tests{ th::detail::testcollection::instance().tests };
            const auto time_start = std::chrono::high_resolution_clock::now();
            if (max_concurrent < 1)
                max_concurrent = 1;

            class thread_counter
            {
            public:
                thread_counter(const int max_threads) :
                    max_threads_{ max_threads }, active_threads_{ 0 } {}

                void operator++()
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    condition_.wait(lock, [&]() { return active_threads_ < max_threads_; });
                    ++active_threads_;
                }

                void operator--()
                {
                    --active_threads_;
                    condition_.notify_one();
                }

            private:
                const int max_threads_;
                std::atomic<int> active_threads_;
                std::mutex mutex_;
                std::condition_variable condition_;
            } thread_counter_{ max_concurrent };

            std::atomic<int> failed_tests{ 0 };
            const int skipped_tests{ 0 };
            shared_output shared_output{ output };
            std::vector<std::shared_ptr<th::detail::testcase>> active_tests;

            std::copy_if(tests.begin(), tests.end(), std::back_inserter(active_tests),
                [&filter](const std::shared_ptr<th::detail::testcase>& test) { return filter(*test); });

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(active_tests.begin(), active_tests.end(), g);
            std::vector<std::future<void>> futures;
            for (auto& test : active_tests)
            {
                futures.push_back(std::async([&]()
                {
                    // ReSharper disable once CppInconsistentNaming
                    struct counter_guard
                    {
                        counter_guard(thread_counter& tc) : tc_(tc) { ++tc_; }
                        ~counter_guard() { --tc_; }

                        counter_guard(const counter_guard&) = delete;
                        counter_guard(counter_guard&&) = delete;
                        counter_guard& operator =(const counter_guard&) const = delete;
                        counter_guard& operator =(counter_guard&&) = delete;

                    private:
                        thread_counter& tc_;
                    } counter_guard{ thread_counter_ };

                    // We are deliberately not capturing any values by reference, since the thread running
                    // this lambda may be detached and abandoned by a timed test. If that were to happen,
                    // variables on the stack would get destroyed out from underneath us. Instead, we're
                    // going to make copies that are guaranteed to outlive our method, and return the test
                    // status. If the running thread is still valid, it can manage updating the count of
                    // failed threads if necessary.
                    const auto actual_test = [&opt](const std::shared_ptr<th::detail::testcase>& running_test,
                        const std::shared_ptr<attached_output>& shared_ptr) -> bool
                    {
                        shared_ptr->report_start(*running_test);
                        const auto result = running_test->run(opt);
                        for (auto& event : running_test->events())
                            shared_ptr->report_event(*running_test, event);
                        return result;
                    };

                    long long timeout_milliseconds{ test->timeout_milliseconds() };
                    if (timeout_milliseconds == auto_timeout)
                        timeout_milliseconds = max_run_time_milliseconds.count();

                    if (timeout_milliseconds != no_timeout)
                    {
                        const std::chrono::nanoseconds timeout_nanoseconds
                        {
                            std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(timeout_milliseconds))
                        };

                        // Note that forcing a test to run in under a certain amount of time
                        // is inherently fragile there's no guarantee that a thread, once
                        // started, actually gets `maxTestRunTime` nanoseconds of CPU.
                        auto m = std::make_shared<std::mutex>();
                        std::unique_lock<std::mutex> gate(*m);
                        auto attached_output = std::make_shared<detail::attached_output>(shared_output);
                        auto thread_started = std::make_shared<std::condition_variable>();
                        const auto result = std::make_shared<bool>();
                        std::thread timed_runner([=]()
                        {
                            m->lock();
                            m->unlock();
                            *result = actual_test(test, attached_output);
                            thread_started->notify_all();
                        });
                        timed_runner.detach();
                        if (thread_started->wait_for(gate, std::chrono::milliseconds(timeout_milliseconds)) == std::cv_status::timeout)
                        {
                            attached_output->detach();
                            shared_output.report_event(*test, th::detail::event(th::detail::severity::fatal,
                                "test failed to complete within " + th::to_string(timeout_nanoseconds)));
                            shared_output.report_finish(*test, timeout_nanoseconds);
                            ++failed_tests;
                        }
                        else
                        {
                            shared_output.report_finish(*test, test->duration());
                            if (!*result)
                                ++failed_tests;
                        }
                    }
                    else
                    {
                        const bool result{ actual_test(test, std::make_shared<attached_output>(shared_output)) };
                        shared_output.report_finish(*test, test->duration());
                        if (!result)
                            ++failed_tests;
                    }
                }));
            }

            for (auto& test : futures)
                test.get();
            shared_output.report_complete(static_cast<int>(futures.size()), skipped_tests, failed_tests,
                std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - time_start));
            return failed_tests;
        }
    }

    namespace util
    {
        extern "C" inline __declspec(dllexport) void enumerate_tests(const detail::enumerate_testcase_callback& callback)
        {
            for (const auto& test : th::detail::testcollection::instance().tests)
                callback(*test);
        }

        extern "C" inline __declspec(dllexport) int run_tests(detail::output_interface& test_reporter,
            const detail::filter_callback& filter, const th::detail::options& opt,
            const std::chrono::milliseconds max_run_time_milliseconds, const int max_concurrent)
        {
            return detail::run_tests(test_reporter, filter, opt, max_run_time_milliseconds, max_concurrent);
        }

        inline std::string usage(const std::string& exe)
        {
            static const std::string usage =
                " [option]+\n"
                "\n"
                "options:\n\n"
                "  -v --verbose                   : verbose mode: include successful test timing\n"
                "     --list                      : do not run tests, just list the ones that pass the filters\n"
                "  -g --group <GROUP>+            : test groups to run (regex match)\n"
                "  -t --test <TEST>+              : tests to run (regex match)\n"
                "  -l --timelimit <milliseconds>  : Set the default test time limit\n"
                "  -x --xml [FILENAME]            : xml output to optional file named FILENAME\n"
                "  -c --concurrent <max tests>    : set maximum number of concurrent tests\n"
                "  -s --sort                      : Sort tests by test group and then by test name\n"
                "  -a --aggregate                 : group test output under group headers (implies --sort)\n"
                "     --roff                      : enables roundoff errors, by default roundoff errors are ignored\n"
                "  -m --monochrome                : produce monochrome report, do not colorize it\n"
                "\n"
                "Sorting and grouping test output causes test results to be cached until after all tests have completed.\n"
                "Normally, test results are printed as soon as the test is complete.\n";

            return "\nusage: " + exe + usage;
        }

        inline std::string take_front(std::queue<std::string>& queue)
        {
            auto s = queue.front();
            queue.pop();
            return s;
        }

        inline bool get_int(std::queue<std::string>& queue, int& value)
        {
            std::istringstream stream{ take_front(queue) };
            stream >> value;
            return std::istringstream::failbit;
        }

        inline std::string parse_arguments(const int argc, char** argv, th::detail::options& options)
        {
            const auto exe = [=]() -> std::string
            {
                std::string path{ argv[0] };
                std::replace(path.begin(), path.end(), '\\', '/');
                const auto idx = path.find_last_of('/');
                if (idx != std::string::npos)
                    return path.substr(idx + 1);
                return path;
            };

            std::queue<std::string> arguments;
            for (int i = 1; i != argc; ++i)
                arguments.emplace(argv[i]);

            while (!arguments.empty())
            {
                std::string opt{ util::take_front(arguments) };
                if (opt[0] == '-')
                {
                    if (opt == "-v" || opt == "--verbose")
                        options.verbose = true;
                    else if (opt == "--list")
                        options.list = true;
                    else if (opt == "-g" || opt == "--group")
                    {
                        if (arguments.empty())
                            return opt + " expects a following test group name argument."
                            + util::usage(exe());
                        options.groups.push_back(util::take_front(arguments));
                    }
                    else if (opt == "-t" || opt == "--test")
                    {
                        if (arguments.empty())
                            return opt + " expects a following test name argument."
                            + util::usage(exe());
                        options.testcases.push_back(util::take_front(arguments));
                    }
                    else if (opt == "-x" || opt == "--xml")
                    {
                        if (arguments.empty() || arguments.front().front() == '-')
                            options.xml_output = "."; // "." is a special filename meaning "use stdout".
                        else
                            options.xml_output = util::take_front(arguments);
                    }
                    else if (opt == "-l" || opt == "--timelimit")
                    {
                        if (arguments.empty() || !util::get_int(arguments, options.time_limit_milliseconds))
                            return opt + " expects a following timelimit specified in milliseconds."
                            + util::usage(exe());
                    }
                    else if (opt == "-c" || opt == "--concurrent")
                    {
                        if (arguments.empty() || !util::get_int(arguments, options.thread_limit))
                            return opt + " expects a following test limit count."
                            + util::usage(exe());
                    }
                    else if (opt == "-s" || opt == "--sort")
                        options.sort = true;
                    else if (opt == "-a" || opt == "--aggregate")
                    {
                        options.sort = true;
                        options.aggregate = true;
                    }
                    else if (opt == "--roff")
                        options.ignore_round_off_errors = false;
                    else if (opt == "-m" || opt == "--monochrome")
                        options.color_output = false;
                    else if (opt == "-?" || opt == "--?")
                        return util::usage(exe());
                    else if (opt == "-help" || opt == "--help")
                        return util::usage(exe());
                    else
                        return "Unrecognized option " + opt + "." + util::usage(exe());
                }
                else
                    return util::usage(exe());
            }
            return "";
        }
    }

    namespace util
    {
        inline std::string file_and_line(const th::detail::testcase_interface& testcase, const th::location& location)
        {
            std::string result = th::to_string<const th::location&>(location);
            if (result.empty())
                result = th::to_string<const th::location&>(testcase.located());
            return result;
        }

        inline std::string safe_str(const char* s)
        {
            return s == nullptr ? "" : s;
        }
    }

    namespace monochrome
    {
        class reporter : public detail::output_interface
        {
        public:
            reporter(const bool verbose, const bool sort, const bool aggregated) :
                cache_(new report_cache(verbose, sort, aggregated)) {}

            reporter(const reporter&) = delete;
            reporter(reporter&&) = delete;
            reporter& operator =(const reporter&) = delete;
            reporter& operator =(reporter&&) = delete;
            ~reporter() = default;

            virtual void report_start(const th::detail::testcase_interface&) override
            {
            }
            virtual void report_event(const th::detail::testcase_interface& testcase,
                const th::detail::event_interface& evt) override
            {
                cache_->cache(testcase) << evt;
            }
            virtual void report_skip(const th::detail::testcase_interface& testcase,
                const char* reason) override
            {
                cache_->skip(testcase, reason);
            }
            virtual void report_finish(const th::detail::testcase_interface& testcase,
                const std::chrono::nanoseconds nanoseconds) override
            {
                cache_->cache(testcase) << nanoseconds;
                cache_->finish(testcase);
            }
            virtual void report_complete(const int total, const int skipped, const int failed,
                const std::chrono::nanoseconds nanoseconds_total) override
            {
                cache_->finish();
                if (failed > 0)
                    std::cout << "\nFAILURE";
                else if (skipped > 0)
                    std::cout << "\nWARNING";
                else
                    std::cout << "\nSUCCESS";
                std::cout << ": " + std::to_string(total) + " tests, " +
                    std::to_string(failed) + " failed, " + std::to_string(skipped) + " skipped;";
                std::cout << " test time: " + th::to_string(nanoseconds_total) + "\n";
            }

        private:
            // ReSharper disable once CppInconsistentNaming
            class report_cache
            {
                // ReSharper disable once CppInconsistentNaming
                class test_output
                {
                public:
                    test_output(const th::detail::testcase_interface& testcase, const bool verbose) :
                        testcase_(testcase), failed_(false), verbose_(verbose), skipped_(false) {}

                    test_output() = delete;
                    test_output(const test_output&) = delete;
                    test_output(test_output&&) = delete;
                    test_output& operator =(const test_output&) = delete;
                    test_output& operator =(test_output&&) = delete;
                    ~test_output() = default;

                    [[nodiscard]] bool will_print() const { return failed_ || verbose_; }

                    void print(const bool aggregated) const
                    {
                        if (will_print())
                        {
                            if (!aggregated)
                                std::cout << "\n";

                            if (failed_)
                                std::cout << "[failure] ";
                            else if (skipped_)
                                std::cout << "[skipped] ";
                            else
                                std::cout << "[success] ";

                            if (!aggregated)
                            {
                                const std::string group = util::safe_str(testcase_.group());
                                if (!group.empty())
                                    std::cout << "[" + group + "] ";
                            }
                            std::cout << util::safe_str(testcase_.name()) + "\n";
                        }
                    }

                    void skip(const std::string& reason)
                    {
                        std::cout << th::to_string<const th::location&>(testcase_.located()) + ": " + reason + "\n";
                        skipped_ = true;
                    }

                    test_output& operator <<(const th::detail::event_interface& event)
                    {
                        if (event.is_failure())
                            failed_ = true;
                        std::cout << util::file_and_line(testcase_, event.located()) + ": " + th::to_string(event.severity_level()) + ": ";
                        if (event.is_assert())
                        {
                            std::cout << event.assert_call() + std::string("()");
                            const std::string user_message{ event.assert_user_message() };
                            const std::string custom_message{ event.assert_custom_message() };
                            if (!user_message.empty())
                            {
                                std::cout << " failure: " + user_message;
                                if (!custom_message.empty())
                                    std::cout << "\n     " + custom_message;
                            }
                            else if (!custom_message.empty())
                                std::cout << " failure: " + custom_message;
                            else
                                std::cout << " failure.";

                            const std::string expected{ event.assert_expected() };
                            const std::string actual{ event.assert_actual() };
                            if (!expected.empty() || !actual.empty())
                                std::cout << "\n     expected: " + expected + "\n       actual: " + actual;
                        }
                        else
                            std::cout << event.message();
                        std::cout << "\n";
                        return *this;
                    }

                    test_output& operator <<(const std::chrono::nanoseconds nanoseconds)
                    {
                        if (verbose_)
                            std::cout << "test completed in " + th::to_string(nanoseconds) + ".\n";
                        return *this;
                    }

                    [[nodiscard]] const th::detail::testcase_interface& testcase() const { return testcase_; }

                private:
                    const th::detail::testcase_interface& testcase_;
                    bool failed_;
                    bool verbose_;
                    bool skipped_;
                };

                typedef std::unordered_map<int, std::shared_ptr<test_output>> output_cache;

            public:
                report_cache(const bool verbose, const bool sort, const bool aggregate) :
                    verbose_(verbose), sort_(sort), aggregate_(aggregate) {}

                report_cache() = delete;
                report_cache(const report_cache&) = delete;
                report_cache(report_cache&&) = delete;
                report_cache& operator =(const report_cache&) = delete;
                report_cache& operator =(report_cache&&) = delete;
                ~report_cache() = default;

                void skip(const th::detail::testcase_interface& testcase, const std::string& reason)
                {
                    if (!sort_)
                    {
                        std::cout << "\n[skipped] ";
                        const std::string group{ util::safe_str(testcase.group()) };
                        if (!group.empty())
                            std::cout << "[" + group + "] ";
                        std::cout << util::safe_str(testcase.name()) + "\n" +
                            th::to_string<const th::location&>(testcase.located()) + ": " + reason + "\n";
                    }
                    else
                        cache(testcase).skip(reason);
                }

                test_output& cache(const th::detail::testcase_interface& testcase)
                {
                    int id{ testcase.id() };
                    const auto it = cache_.find(id);
                    if (it == cache_.end())
                        cache_.insert(std::make_pair(id, std::make_shared<test_output>(testcase, verbose_)));
                    return *cache_[id];
                }

                void finish(const th::detail::testcase_interface& testcase)
                {
                    if (!sort_)
                    {
                        const auto it = cache_.find(testcase.id());
                        if (it != cache_.end())
                        {
                            it->second->print(false);
                            cache_.erase(it);
                        }
                    }
                }

                void finish()
                {
                    if (sort_)
                    {
                        std::vector<std::shared_ptr<test_output>> final_results;
                        final_results.reserve(cache_.size());
                        for (const auto& x : cache_)
                            final_results.push_back(x.second);

                        std::sort(final_results.begin(), final_results.end(),
                            [](const std::shared_ptr<test_output>& lhs, const std::shared_ptr<test_output>& rhs)
                        {
                            const std::string lhs_group{ util::safe_str(lhs->testcase().group()) };
                            const std::string rhs_group{ util::safe_str(rhs->testcase().group()) };
                            if (lhs_group != rhs_group)
                                return lhs_group < rhs_group;
                            const std::string lhs_name{ lhs->testcase().name() };
                            const std::string rhs_name{ rhs->testcase().name() };
                            return lhs_name < rhs_name;
                        });

                        std::string cur_group;
                        for (auto it = final_results.begin(); it != final_results.end(); ++it)
                        {
                            auto& result = *it;
                            if (result->will_print())
                            {
                                if (aggregate_)
                                {
                                    const std::string group{ util::safe_str(result->testcase().group()) };
                                    if (cur_group != group)
                                    {
                                        cur_group = group;
                                        const std::string sep(cur_group.length() + 2, '=');
                                        std::string out("\n\n");
                                        out.append(sep);
                                        out.append("\n[");
                                        out.append(cur_group);
                                        out.append("]\n");
                                        out.append(sep);
                                        out.append("\n");
                                        std::cout << out;
                                    }
                                }
                                result->print(aggregate_);
                            }
                        }
                    }
                }

            private:
                output_cache cache_;
                bool verbose_;
                bool sort_;
                bool aggregate_;
            };
            std::unique_ptr<report_cache> cache_;
        };
    }

    namespace colorized
    {
#if defined (_WIN32)
        // ReSharper disable once CppInconsistentNaming
        class reset_console_colors
        {
        public:
            reset_console_colors() : std_out_(GetStdHandle(STD_OUTPUT_HANDLE))
            {
                CONSOLE_SCREEN_BUFFER_INFO info;
                GetConsoleScreenBufferInfo(std_out_, &info);
                attributes_ = info.wAttributes;
            }

            ~reset_console_colors() { reset(); }

            reset_console_colors(const reset_console_colors&) = delete;
            reset_console_colors(reset_console_colors&&) = delete;
            reset_console_colors& operator =(const reset_console_colors&) = delete;
            reset_console_colors& operator =(reset_console_colors&&) = delete;

            void reset() const { SetConsoleTextAttribute(std_out_, attributes_); }

        private:
            HANDLE std_out_;
            WORD attributes_;
        };
#endif

        enum class color : unsigned short
        {
            default_color = 65535,

            black = 0,
            dark_blue,
            dark_green,
            dark_cyan,
            dark_red,
            dark_magenta,
            dark_yellow,
            light_gray,
            dark_gray,
            blue,
            green,
            cyan,
            red,
            magenta,
            yellow,
            white,

            debug = dark_magenta,
            info = dark_cyan,
            warning = yellow,
            expect = red,
            assert = red,
            skip = yellow,
            group = white,
            separator = dark_gray,
            test_name = white,
            file_and_line = default_color,
            success = green,
            failure = red,
            time_summary = dark_gray,
            call = white,
            expected = cyan,
            actual = cyan,

            // This value is to match the Win32 value
            // there is a special test in to_ansi_code
            fatal = (0x40 + white),
        };

        inline color to_color(const th::detail::severity severity)
        {
            switch (severity)
            {
                case th::detail::severity::debug:   return color::debug;
                case th::detail::severity::info:    return color::info;
                case th::detail::severity::warning: return color::warning;
                case th::detail::severity::expect:  return color::expect;
                case th::detail::severity::assert:  return color::assert;
                case th::detail::severity::fatal:   return color::fatal;
            }
            return color::default_color;
        }

#ifdef _WIN32
        template<typename T>
        T& operator <<(T& stream, color color)
        {
            stream.flush();
            if (color != color::default_color)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<unsigned short>(color));
            else
            {
                CONSOLE_SCREEN_BUFFER_INFO info;
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), info.wAttributes);
            }
            return stream;
        }
#else
        inline std::string to_ansi_code(color color)
        {
            static const std::string codes[] =
            {
                "\33[0;30m",
                "\33[0;34m",
                "\33[0;32m",
                "\33[0;36m",
                "\33[0;31m",
                "\33[0;35m",
                "\33[0;33m",
                "\33[0;37m",
                "\33[0;1;30m", // The 0 at the start clears the background color
                "\33[0;1;34m", // then the 1 sets the color to bold.
                "\33[0;1;32m",
                "\33[0;1;36m",
                "\33[0;1;31m",
                "\33[0;1;35m",
                "\33[0;1;33m",
                "\33[0;1;37m",
            };

            if (color == color::fatal)
                return "\33[1;37;41m";

            if (color != color::default_color)
                return codes[static_cast<int>(color)];

            return "\33[m";
        }

        template<typename T>
        T& operator <<(T& stream, color color)
        {
            stream << to_ansi_code(color);
            return stream;
        }
#endif

        class reporter final : public detail::output_interface
        {
        public:
            reporter(const bool verbose, const bool sort, const bool aggregated) :
                cache_(new report_cache(verbose, sort, aggregated)) {}

            reporter() = delete;
            reporter(const reporter&) = delete;
            reporter(reporter&&) = delete;
            reporter& operator =(const reporter&) = delete;
            reporter& operator =(reporter&&) = delete;
            ~reporter() = default;

            void report_start(const th::detail::testcase_interface&) override
            {
            }
            void report_event(const th::detail::testcase_interface& testcase, const th::detail::event_interface& evt) override
            {
                cache_->cache(testcase) << evt;
            }
            void report_skip(const th::detail::testcase_interface& testcase, const char* reason) override
            {
                cache_->skip(testcase, reason);
            }
            void report_finish(const th::detail::testcase_interface& testcase, const std::chrono::nanoseconds nanoseconds) override
            {
                cache_->cache(testcase) << nanoseconds;
                cache_->finish(testcase);
            }
            void report_complete(const int total, const int skipped, const int failed, const std::chrono::nanoseconds nanoseconds_total) override
            {
                cache_->finish();
                color fail_color = color::default_color;
                color skip_color = color::default_color;
                if (failed > 0)
                {
                    fail_color = color::failure;
                    cache_->instant(fail_color, "\nFAILURE");
                }
                else if (skipped > 0)
                {
                    skip_color = color::warning;
                    cache_->instant(skip_color, "\nWARNING");
                }
                else
                    cache_->instant(color::success, "\nSUCCESS");
                cache_->instant(color::default_color, ": " + std::to_string(total) + " tests, ");
                cache_->instant(fail_color, std::to_string(failed) + " failed");
                cache_->instant(color::default_color, ", ");
                cache_->instant(skip_color, std::to_string(skipped) + " skipped");
                cache_->instant(color::default_color, ";");

                const std::string report{ " test time: " + th::to_string(nanoseconds_total) };
                cache_->instant(color::time_summary, report);
                cache_->instant(color::default_color, "\n");
            }

        private:
            // ReSharper disable once CppInconsistentNaming
            class report_cache
            {
                // ReSharper disable once CppInconsistentNaming
                class test_output
                {
                public:
                    struct fragment
                    {
                        fragment(const color color, std::string message) : color_(color), message_(std::move(message)) {}

                        fragment() = delete;
                        fragment(const fragment&) = delete;
                        fragment(fragment&& other) noexcept :
                            color_(other.color_), message_(std::move(other.message_)) {}
                        fragment& operator =(const fragment&) = delete;
                        fragment& operator =(fragment&&) = delete;
                        ~fragment() = default;

                        friend std::ostream& operator <<(std::ostream& stream, const fragment& fragment)
                        {
                            return stream << fragment.color_ << fragment.message_;
                        }

                        color color_;
                        std::string message_;
                    };

                    test_output(const th::detail::testcase_interface& testcase, const bool verbose) :
                        testcase_(testcase), failed_(false), verbose_(verbose), skipped_(false) {}

                    test_output() = delete;
                    test_output(const test_output&) = delete;
                    test_output(test_output&&) = delete;
                    test_output& operator =(const test_output&) = delete;
                    test_output& operator =(test_output&&) = delete;
                    ~test_output() = default;

                    [[nodiscard]] bool will_print() const
                    {
                        return failed_ || verbose_ || !fragments_.empty();
                    }

                    void print(const bool aggregated)
                    {
                        if (will_print())
                        {
                            //if (!aggregated)
                            //    std::cout << fragment(color::default_color, "\n");

                            if (failed_)
                                std::cout << fragment(color::failure, "[failure] ");
                            else if (skipped_)
                                std::cout << fragment(color::skip, "[skipped] ");
                            else
                                std::cout << fragment(color::success, "[success] ");

                            if (!aggregated)
                            {
                                const std::string group = util::safe_str(testcase_.group());
                                if (!group.empty())
                                    std::cout << fragment(color::group, "[" + group + "] ");
                            }

                            const std::string test_name = util::safe_str(testcase_.name());
                            std::cout << fragment(color::test_name, test_name);
                            std::cout << fragment(color::default_color, "\n");

                            for (auto&& msg : fragments_)
                                std::cout << msg;
                        }
                    }

                    void skip(const std::string& reason)
                    {
                        fragments_.emplace_back(color::file_and_line, th::to_string<const th::location&>(testcase_.located()));
                        fragments_.emplace_back(color::separator, ": ");
                        fragments_.emplace_back(color::skip, reason);
                        fragments_.emplace_back(color::default_color, "\n");
                        skipped_ = true;
                    }

                    test_output& operator <<(const th::detail::event_interface& event)
                    {
                        if (event.is_failure())
                            failed_ = true;

                        fragments_.emplace_back(color::file_and_line, util::file_and_line(testcase_, event.located()));
                        fragments_.emplace_back(color::separator, ": ");
                        fragments_.emplace_back(to_color(event.severity_level()), th::to_string(event.severity_level()));
                        fragments_.emplace_back(color::separator, ": ");

                        if (event.is_assert())
                        {
                            fragments_.emplace_back(color::call, event.assert_call() + std::string("()"));

                            std::string message{ " failure" };
                            const std::string user_message{ event.assert_user_message() };
                            const std::string custom_message{ event.assert_custom_message() };
                            if (!user_message.empty())
                            {
                                message += ": " + user_message;
                                if (!custom_message.empty())
                                    message += "\n     " + custom_message;
                            }
                            else if (!custom_message.empty())
                                message += ": " + custom_message;

                            fragments_.emplace_back(color::default_color, message);
                            std::string expected{ event.assert_expected() };
                            std::string actual{ event.assert_actual() };
                            if (!expected.empty() || !actual.empty())
                            {
                                fragments_.emplace_back(color::expected, "\n     expected: ");
                                fragments_.emplace_back(color::default_color, expected);
                                fragments_.emplace_back(color::actual, "\n       actual: ");
                                fragments_.emplace_back(color::default_color, actual);
                            }
                        }
                        else
                        {
                            std::string message{ event.message() };
                            fragments_.emplace_back(color::default_color, message);
                        }

                        fragments_.emplace_back(color::default_color, "\n");
                        return *this;
                    }

                    test_output& operator <<(const std::chrono::nanoseconds nanoseconds)
                    {
                        if (verbose_)
                            fragments_.emplace_back(color::time_summary,
                                "test completed in " + th::to_string(nanoseconds) + "\n");
                        return *this;
                    }

                    [[nodiscard]] const th::detail::testcase_interface& testcase() const { return testcase_; }

                private:
                    const th::detail::testcase_interface& testcase_;
                    std::vector<fragment> fragments_;
                    bool failed_;
                    bool verbose_;
                    bool skipped_;
                };

                typedef std::unordered_map<int, std::shared_ptr<test_output>> output_cache;

            public:
                report_cache(const bool verbose, const bool sort, const bool aggregate) :
                    verbose_(verbose), sort_(sort), aggregate_(aggregate) {}

                report_cache() = delete;
                report_cache(const report_cache&) = delete;
                report_cache(report_cache&&) = delete;
                report_cache& operator =(const report_cache&) = delete;
                report_cache& operator =(report_cache&&) = delete;
                ~report_cache() = default;

                static void instant(const color color, const std::string& message)
                {
                    std::cout << test_output::fragment(color, message);
                }

                void skip(const th::detail::testcase_interface& testcase, const std::string& reason)
                {
                    if (!sort_)
                    {
                        instant(color::skip, "\n[skipped] ");

                        const std::string group{ util::safe_str(testcase.group()) };
                        if (!group.empty())
                        {
                            instant(color::group, "[" + group + "]");
                            instant(color::separator, " ");
                        }

                        const std::string test_name{ util::safe_str(testcase.name()) };
                        instant(color::test_name, test_name + "\n");
                        instant(color::file_and_line, th::to_string<const th::location&>(testcase.located()) + ": ");
                        instant(color::skip, reason);
                        instant(color::default_color, "\n");
                    }
                    else
                        cache(testcase).skip(reason);
                }

                test_output& cache(const th::detail::testcase_interface& testcase)
                {
                    int id{ testcase.id() };
                    const auto it = cache_.find(id);
                    if (it == cache_.end())
                        cache_.insert(std::make_pair(id, std::make_shared<test_output>(testcase, verbose_)));
                    return *cache_[id];
                }

                void finish(const th::detail::testcase_interface& testcase)
                {
                    if (!sort_)
                    {
                        auto it = cache_.find(testcase.id());
                        if (it != cache_.end())
                        {
                            it->second->print(false);
                            cache_.erase(it);
                        }
                    }
                }

                void finish()
                {
                    if (sort_)
                    {
                        std::vector<std::shared_ptr<test_output>> final_results;
                        final_results.reserve(cache_.size());
                        for (const auto& x : cache_)
                            final_results.push_back(x.second);

                        std::sort(final_results.begin(), final_results.end(),
                            [](const std::shared_ptr<test_output>& lhs, const std::shared_ptr<test_output>& rhs)
                        {
                            const std::string lhs_group{ util::safe_str(lhs->testcase().group()) };
                            const std::string rhs_group{ util::safe_str(rhs->testcase().group()) };
                            if (lhs_group != rhs_group)
                                return lhs_group < rhs_group;
                            const std::string lhs_name{ lhs->testcase().name() };
                            const std::string rhs_name{ rhs->testcase().name() };
                            return lhs_name < rhs_name;
                        });

                        std::string cur_group;
                        for (auto it = final_results.begin(); it != final_results.end(); ++it)
                        {
                            auto& result = *it;
                            if (result->will_print())
                            {
                                if (aggregate_)
                                {
                                    const std::string group{ util::safe_str(result->testcase().group()) };
                                    if (cur_group != group)
                                    {
                                        cur_group = group;
                                        const std::string sep(cur_group.length(), '-');
                                        std::string out("\n\n");
                                        out.append(sep);
                                        out.append("\n");
                                        out.append(cur_group);
                                        out.append("\n");
                                        out.append(sep);
                                        out.append("\n");
                                        std::cout << test_output::fragment(color::group, out);
                                    }
                                }
                                result->print(aggregate_);
                            }
                        }
                    }
                }

            private:
                output_cache cache_;
                bool verbose_;
                bool sort_;
                bool aggregate_;
            };
            std::unique_ptr<report_cache> cache_;
        };
    }

    namespace util
    {
        inline int process(th::detail::options& options)
        {
            int total_failures{ 0 };
            const bool forced_failure{ false };

            std::vector<int> active_test_ids;
            auto on_list = [&](const th::detail::testcase_interface& testcase)
            {
                if (options.list)
                    std::cout << "\n" << testcase.group() << " :: " << testcase.name() << "\n";
                else
                    active_test_ids.push_back(testcase.id());
            };

            util::enumerate_tests([&](const th::detail::testcase_interface& testcase)
            {
                // If any groups are specified, a test has to belong to one of them to be run.
                if (!options.groups.empty())
                {
                    bool included{ false };
                    for (const auto& group : options.groups)
                    {
                        const std::regex regex(group, std::regex_constants::icase);

                        const std::string testgroup{ testcase.group() == nullptr ? "" : testcase.group() };
                        if (std::regex_search(testgroup, regex))
                        {
                            included = true;
                            break;
                        }
                    }
                    if (!included)
                        return;
                }
                if (!options.testcases.empty())
                {
                    bool included = false;
                    for (const auto& name : options.testcases)
                    {
                        const std::regex regex(name, std::regex_constants::icase);

                        if (std::regex_search(testcase.name(), regex))
                        {
                            included = true;
                            break;
                        }
                    }
                    if (!included)
                        return;
                }

                on_list(testcase);
            });

            if (!active_test_ids.empty())
            {
                std::sort(active_test_ids.begin(), active_test_ids.end());
                const auto run_tests = [&](detail::output_interface& reporter)
                {
                    total_failures += util::run_tests(reporter,
                        [&](const th::detail::testcase_interface& testcase)
                    {
                        return std::binary_search(active_test_ids.begin(), active_test_ids.end(), testcase.id());
                    },
                        options, std::chrono::milliseconds(options.time_limit_milliseconds), options.thread_limit);
                };

                if (options.xml_output.empty())
                {
                    if (options.color_output)
                    {
                        colorized::reporter reporter(options.verbose, options.sort, options.aggregate);
                        run_tests(reporter);
                    }
                    else
                    {
                        monochrome::reporter reporter(options.verbose, options.sort, options.aggregate);
                        run_tests(reporter);
                    }
                }
                // ReSharper disable CommentTypo
                /*else if (options.xml_output == ".")
                {
                    //util::xml_reporter reporter(std::cout);
                    //run_tests(reporter);
                }
                else
                {
                    std::ofstream file(options.xml_output, std::ios::binary);
                    if (!file)
                        std::cerr << "Unable to open " << options.xmlOutput << " for writing.\n\n";
                    xUnitpp::util::XmlReporter reporter(!file ? std::cerr : file);
                    run_tests(reporter);
                }*/
                // ReSharper restore CommentTypo
            }
            return forced_failure ? 1 : -total_failures;
        }
    }

    inline int run_tests(const int argc, char* argv[])
    {
        th::detail::options options;
        const std::string result = util::parse_arguments(argc, argv, options);
        if (!result.empty())
        {
            std::cerr << result;
            return -1;
        }
        return shell::util::process(options);
    }
}
