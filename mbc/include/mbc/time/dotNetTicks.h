#pragma once

#include <ctime>

namespace mbc::time {

    //! Encapsulates operations with .Net's DateTime ticks.
    //! A single tick represents one hundred nanoseconds or one ten-millionth of a second. There are 10,000 ticks in a millisecond.
    //! The .Net's DateTime represents the number of 100-nanosecond intervals that have elapsed since 12:00:00 midnight, January 1, 0001, which represents minimum value.
    //! Note that it does not include the number of ticks that are attributable to leap seconds.
    class DotNetTicks final
    {
    public: 
        //! The number of days in the month given by the \a year and \p month arguments.
        //! \param year The year in range [1, 9999].
        //! \param month The month in range [1, 12].
        static constexpr int daysInMonth(const int year, const int month) noexcept
        {
            return isLeapYear(year) ?
                daysToMonth366[month] - daysToMonth366[month - 1] :
                daysToMonth365[month] - daysToMonth365[month - 1];
        }

        //! Checks whether a given \p year is a leap year.
        //! \param year The year in range [1, 9999].
        static constexpr bool isLeapYear(const int year) noexcept
        {
            return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
        }

        //! The number of 100-nanosecond ticks corresponding to the given \p year, \p month, and \p day.
        //! \param year The year in range [1, 9999].
        //! \param month The month in range [1, 12].
        //! \param day The day in range [1, 31]. 
        static long long fromDate(const int year, const int month, const int day) noexcept
        {
            const int *days{ isLeapYear(year) ? daysToMonth366 : daysToMonth365 };
            const int y{ year - 1 };
            const int n{ y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1 };
            return n * ticksPerDay;
        }

        //! The date part of 100-nanosecond \p ticks.
        //! The resulting value corresponds to the date with the time-of-day part set to zero (midnight).
        //! \param ticks The number of 100-nanosecond intervals that have elapsed since 1/1/0001 12:00am.
        static constexpr long long datePartTicks(const long long ticks) noexcept
        {
            return ticks - ticks % ticksPerDay;
        }

        //! Converts 100-nanosecond \p ticks to the \p tm structure.
        //! \param ticks The number of 100-nanosecond intervals that have elapsed since 1/1/0001 12:00am.
        //! \param tm A reference to the \p tm structure to be filled.
        //! \param nanoseconds100 A reference to be filled with the number of 100-nanosecond ticks representing a fraction of the second.
        static void toTm(const long long ticks, struct tm &tm, int &nanoseconds100) noexcept
        {
            nanoseconds100 = static_cast<int>(ticks % 10000000);
            // Number of days since 1/1/0001.
            int n{ static_cast<int>(ticks / ticksPerDay) };
            // Number of whole 400-year periods since 1/1/0001.
            const int y400{ n / daysPer400Years };
            // Day number within 400-year period.
            n -= y400 * daysPer400Years;
            // Number of whole 100-year periods within 400-year period.
            int y100{ n / daysPer100Years };
            // Last 100-year period has an extra day, so decrement result if 4.
            if (y100 == 4) y100 = 3;
            // Day number within 100-year period.
            n -= y100 * daysPer100Years;
            // Number of whole 4-year periods within 100-year period.
            const int y4{ n / daysPer4Years };
            // Day number within 4-year period.
            n -= y4 * daysPer4Years;
            // Number of whole years within 4-year period.
            int y1{ n / daysPerYear };
            // Last year has an extra day, so decrement result if 4.
            if (y1 == 4) y1 = 3;
            // Years since 1900.
            tm.tm_year = y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1 - 1900;
            // Day number within year.
            n -= y1 * daysPerYear;
            // [0, 365].
            tm.tm_yday = n; // +1;
            // Leap year calculation looks different from isLeapYear() since y1, y4, and y100 are relative to year 1, not year 0.
            const bool leapYear{ y1 == 3 && (y4 != 24 || y100 == 3) };
            const int *days{ leapYear ? daysToMonth366 : daysToMonth365 };
            // All months have less than 32 days, so n >> 5 is a good conservative estimate for the month.
            int m{ (n >> 5) + 1 };
            // M is 1-based month number.
            while (n >= days[m]) m++;
            // [0, 11].
            tm.tm_mon = m - 1;
            // 1-based day-of-month, [1, 31].
            tm.tm_mday = n - days[m - 1] + 1;
            // [0, 6], Sunday=0, Monday=1, Tuesday=2, Wednesday=3, Thursday=4, Friday=5, Saturday=6.
            tm.tm_wday = static_cast<int>(ticks / ticksPerDay + 1);
            // [0, 23].
            tm.tm_hour = static_cast<int>((ticks / ticksPerHour) % 24);
            // [0, 59].
            tm.tm_min = static_cast<int>((ticks / ticksPerMinute) % 60);
            // [0, 59].
            tm.tm_sec = static_cast<int>((ticks / ticksPerSecond) % 60);
            tm.tm_isdst = -1;
            // [0, 999].
            //int milliseconds{ static_cast<int>((ticks / ticksPerMillisecond) % 10) };
        }

        //! Determines if the given number of ticks is within the allowed range.
        //! \param ticks The number of 100-nanosecond ticks.
        static constexpr bool isValid(const long long ticks) noexcept
        {
            return ticks <= maxTicks && ticks >= minTicks;
        }

        //! The number of 100-nanosecond ticks resulting from addition of two tick given tick values.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param value The number of 100-nanosecond ticks to add. This argument is permitted to be negative.
        static constexpr long long addTicks(const long long ticks, const long long value) noexcept
        {
            return ticks + value;
        }

        //! The number of 100-nanosecond ticks resulting from adding a fractional number of \p milliseconds to the given number of \p ticks.
        //! The result is computed by rounding the number of milliseconds given by value to the nearest integer, and adding that interval.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param milliseconds A fractional number of milliseconds to add. This argument is permitted to be negative.
        static constexpr long long addMilliseconds(const long long ticks, const double milliseconds) noexcept
        { 
            return ticks + ticksPerMillisecond *
                static_cast<long long>(milliseconds + (milliseconds >= 0 ? 0.5 : -0.5));
        }

        //! The number of 100-nanosecond ticks resulting from adding a fractional number of \p seconds to the given number of \p ticks.
        //! The result is computed by rounding the fractional number of seconds given by value to the nearest millisecond, and adding that interval.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param seconds A fractional number of seconds to add. This argument is permitted to be negative.
        static constexpr long long addSeconds(const long long ticks, const double seconds) noexcept
        { 
            return ticks + ticksPerMillisecond *
                static_cast<long long>(seconds * millisPerSecond + (seconds >= 0 ? 0.5 : -0.5));
        }
 
        //! The number of 100-nanosecond ticks resulting from adding a fractional number of \p minutes to the given number of \p ticks.
        //! The result is computed by rounding the fractional number of minutes given by value to the nearest millisecond, and adding that interval.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param minutes A fractional number of minutes to add. This argument is permitted to be negative.
        static constexpr long long addMinutes(const long long ticks, const double minutes) noexcept
        { 
            return ticks + ticksPerMillisecond *
                static_cast<long long>(minutes * millisPerMinute + (minutes >= 0 ? 0.5 : -0.5));
        }

        //! The number of 100-nanosecond ticks resulting from adding a fractional number of \p hours to the given number of \p ticks.
        //! The result is computed by rounding the fractional number of hours given by value to the nearest millisecond, and adding that interval.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param hours A fractional number of hours to add. This argument is permitted to be negative.
        static constexpr long long addHours(const long long ticks, const double hours) noexcept
        { 
            return ticks + ticksPerMillisecond *
                static_cast<long long>(hours * millisPerHour + (hours >= 0 ? 0.5 : -0.5));
        }

        //! The number of 100-nanosecond ticks resulting from adding a fractional number of \p days to the given number of \p ticks.
        //! The result is computed by rounding the fractional number of days given by value to the nearest millisecond, and adding that interval.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param days A fractional number of days to add. This argument is permitted to be negative.
        static constexpr long long addDays(const long long ticks, const double days) noexcept
        { 
            return ticks + ticksPerMillisecond *
                static_cast<long long>(days * millisPerDay + (days >= 0 ? 0.5 : -0.5));
        }

        //! The number of 100-nanosecond ticks resulting from adding an integer number of \p months to the given number of \p ticks.
        //! The result is computed by incrementing (or decrementing) the year and month parts by \p months months, and, if required, adjusting the day part of the resulting date downwards to the last day of the resulting month in the resulting year.
        //! The time-of-day part of the result is the same as the original time-of-day part.
        //! In more precise terms, considering this date and time to be of the form \code y / m / d + t\endcode, where \c y is the year, \c m is the month, \c d is the day, and \c t is the time-of-day, the result is \code y1 / m1 / d1 + t\endcode , where \c y1 and \c m1 are computed by adding months months to \c y and \c m, and \c d1 is the largest value less than or equal to \c d that denotes a valid day in month \c m1 of year \c y1.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param months The number of months to add. This argument is permitted to be negative. The valid range is [-120000, 120000].
        static long long addMonths(const long long ticks, const int months) noexcept
        {
            struct tm t{};
            int nanoseconds100;
            toTm(ticks, t, nanoseconds100);
            int y{ t.tm_year + 1900 };
            int m{ t.tm_mon + 1 };
            int d{ t.tm_mday };
            const int i{ m - 1 + months };
            if (i >= 0)
            {
                m = i % 12 + 1;
                y += i / 12; 
            }
            else
            { 
                m = 12 + (i + 1) % 12; 
                y += (i - 11) / 12;
            }
            const int days{ daysInMonth(y, m) };
            if (d > days) d = days;
            return fromDate(y, m, d) + ticks % ticksPerDay; 
        } 

        //! The number of 100-nanosecond ticks resulting from adding an integer number of \p years to the given number of \p ticks.
        //! The result is computed by incrementing (or decrementing) the year part by \p years years.
        //! If the month and day of this date is 2 / 29, and if the resulting year is not a leap year, the month and day of the resulting date becomes 2 / 28.
        //! Otherwise, the month, day, and time - of - day parts of the result are the same as those of this date.
        //! In more precise terms, considering this date and time to be of the form \code y / m / d + t\endcode, where \c y is the year, \c m is the month, \c d is the day, and \c t is the time-of-day, the result is \code y1 / m1 / d1 + t\endcode , where \c y1 and \c m1 are computed by adding months months to \c y and \c m, and \c d1 is the largest value less than or equal to \c d that denotes a valid day in month \c m1 of year \c y1.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param years The number of years to add. This argument is permitted to be negative. The valid range is [-10000, 10000].
        static long long addYears(const long long ticks, const int years) noexcept
        {
            return addMonths(ticks, years * 12);
        } 

        //! The millisecond part of this date and time. The returned value is an integer between 0 and 999. 
        //! \param ticks The number of 100-nanosecond ticks.
        static constexpr int millisecond(const long long ticks) noexcept
        {
            return static_cast<int>((ticks / ticksPerMillisecond) % 1000);
        }

        //! The second part of this date and time. The returned value is an integer between 0 and 59. 
        //! \param ticks The number of 100-nanosecond ticks.
        static constexpr int second(const long long ticks) noexcept
        {
            return static_cast<int>((ticks / ticksPerSecond) % 60);
        }

        //! The minute part of this date and time. The returned value is an integer between 0 and 59. 
        //! \param ticks The number of 100-nanosecond ticks.
        static constexpr int minute(const long long ticks) noexcept
        {
            return static_cast<int>((ticks / ticksPerMinute) % 60);
        }

        //! The hour part of this date and time. The returned value is an integer between 0 and 23. 
        //! \param ticks The number of 100-nanosecond ticks.
        static constexpr int hour(const long long ticks) noexcept
        {
            return static_cast<int>((ticks / ticksPerHour) % 24);
        }

        //! The minimal length of the buffer required for DotNetTicks::toStringFull to represent ticks as "yyyy-MM-dd HH:mm:ss.fffffff".
        static constexpr int toStringFullBufferLength{ 28 };

        //! The text representation of the ticks as "yyyy-MM-dd HH:mm:ss.fffffff". Returns the \p buffer.
        //! \param buffer The character buffer.
        //! \param bufferLength The character buffer length, must be at least DotNetTicks::toStringFullBufferLength characters, including trailing zero.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param dateDelimiter The date delimiter character, default value is dash.
        static const char* toStringFull(char* buffer, const int bufferLength, const long long ticks, const char dateDelimiter = '-') noexcept
        {
            char *ptr = buffer;
            if (toStringFullBufferLength > bufferLength)
                return buffer;
            int nanoseconds100;
            struct tm tm{};
            toTm(ticks, tm, nanoseconds100);
            int i = tm.tm_year + 1900;
            *ptr = '0' + static_cast<char>(i / 1000);                  // [0]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 1000) / 100);        // [1]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 100) / 10);          // [2]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [3]  // NOLINT
            *++ptr = dateDelimiter;                                    // [4]
            i = tm.tm_mon + 1;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [5]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [6]  // NOLINT
            *++ptr = dateDelimiter;                                    // [7]
            i = tm.tm_mday;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [8]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [9]  // NOLINT
            *++ptr = ' ';                                              // [10]
            i = tm.tm_hour;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [11] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [12] // NOLINT
            *++ptr = ':';                                              // [13]
            i = tm.tm_min;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [14] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [15] // NOLINT
            *++ptr = ':';                                              // [16]
            i = tm.tm_sec;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [17] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [18] // NOLINT
            *++ptr = '.';                                              // [19]
            i = nanoseconds100;
            *++ptr = '0' + static_cast<char>(i / 1000000);             // [20] // NOLINT
            *++ptr = '0' + static_cast<char>((i % 1000000) / 100000);  // [21] // NOLINT
            *++ptr = '0' + static_cast<char>((i % 100000) / 10000);    // [22] // NOLINT
            *++ptr = '0' + static_cast<char>((i % 10000) / 1000);      // [23] // NOLINT
            *++ptr = '0' + static_cast<char>((i % 1000) / 100);        // [24] // NOLINT
            *++ptr = '0' + static_cast<char>((i % 100) / 10);          // [25] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [26] // NOLINT
            *++ptr = '\0';                                             // [27]
            // Total length is 28.
            return buffer;
        }

        //! The minimal length of the buffer required for DotNetTicks::toStringSeconds to represent ticks as "yyyy-MM-dd HH:mm:ss".
        static constexpr int toStringSecondsBufferLength{ 20 };

        //! The text representation of the ticks as "yyyy-MM-dd HH:mm:ss". Returns the \p buffer.
        //! \param buffer The character buffer.
        //! \param bufferLength The character buffer length, must be at least DotNetTicks::toStringFullBufferLength characters, including trailing zero.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param dateDelimiter The date delimiter character, default value is dash.
        static const char* toStringSeconds(char* buffer, const int bufferLength, const long long ticks, const char dateDelimiter = '-') noexcept
        {
            char* ptr = buffer;
            if (toStringSecondsBufferLength > bufferLength)
                return buffer;
            int nanoseconds100;
            struct tm tm {};
            toTm(ticks, tm, nanoseconds100);
            int i = tm.tm_year + 1900;
            *ptr = '0' + static_cast<char>(i / 1000);                  // [0]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 1000) / 100);        // [1]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 100) / 10);          // [2]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [3]  // NOLINT
            *++ptr = dateDelimiter;                                    // [4]
            i = tm.tm_mon + 1;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [5]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [6]  // NOLINT
            *++ptr = dateDelimiter;                                    // [7]
            i = tm.tm_mday;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [8]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [9]  // NOLINT
            *++ptr = ' ';                                              // [10]
            i = tm.tm_hour;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [11] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [12] // NOLINT
            *++ptr = ':';                                              // [13]
            i = tm.tm_min;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [14] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [15] // NOLINT
            *++ptr = ':';                                              // [16]
            i = tm.tm_sec;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [17] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [18] // NOLINT
            *++ptr = '\0';                                             // [19]
            // Total length is 20.
            return buffer;
        }

        //! The minimal length of the buffer required for DotNetTicks::toStringMinutes to represent ticks as "yyyy-MM-dd HH:mm".
        static constexpr int toStringMinutesBufferLength{ 17 };

        //! The text representation of the ticks as "yyyy-MM-dd HH:mm". Returns the \p buffer.
        //! \param buffer The character buffer.
        //! \param bufferLength The character buffer length, must be at least DotNetTicks::toStringFullBufferLength characters, including trailing zero.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param dateDelimiter The date delimiter character, default value is dash.
        static const char* toStringMinutes(char* buffer, const int bufferLength, const long long ticks, const char dateDelimiter = '-') noexcept
        {
            char* ptr = buffer;
            if (toStringMinutesBufferLength > bufferLength)
                return buffer;
            int nanoseconds100;
            struct tm tm {};
            toTm(ticks, tm, nanoseconds100);
            int i = tm.tm_year + 1900;
            *ptr = '0' + static_cast<char>(i / 1000);                  // [0]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 1000) / 100);        // [1]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 100) / 10);          // [2]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [3]  // NOLINT
            *++ptr = dateDelimiter;                                    // [4]
            i = tm.tm_mon + 1;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [5]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [6]  // NOLINT
            *++ptr = dateDelimiter;                                    // [7]
            i = tm.tm_mday;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [8]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [9]  // NOLINT
            *++ptr = ' ';                                              // [10]
            i = tm.tm_hour;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [11] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [12] // NOLINT
            *++ptr = ':';                                              // [13]
            i = tm.tm_min;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [14] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [15] // NOLINT
            *++ptr = '\0';                                             // [16]
            // Total length is 17.
            return buffer;
        }

        //! The minimal length of the buffer required for DotNetTicks::toStringHours to represent ticks as "yyyy-MM-dd HH".
        static constexpr int toStringHoursBufferLength{ 14 };

        //! The text representation of the ticks as "yyyy-MM-dd HH". Returns the \p buffer.
        //! \param buffer The character buffer.
        //! \param bufferLength The character buffer length, must be at least DotNetTicks::toStringFullBufferLength characters, including trailing zero.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param dateDelimiter The date delimiter character, default value is dash.
        static const char* toStringHours(char* buffer, const int bufferLength, const long long ticks, const char dateDelimiter = '-') noexcept
        {
            char* ptr = buffer;
            if (toStringHoursBufferLength > bufferLength)
                return buffer;
            int nanoseconds100;
            struct tm tm {};
            toTm(ticks, tm, nanoseconds100);
            int i = tm.tm_year + 1900;
            *ptr = '0' + static_cast<char>(i / 1000);                  // [0]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 1000) / 100);        // [1]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 100) / 10);          // [2]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [3]  // NOLINT
            *++ptr = dateDelimiter;                                    // [4]
            i = tm.tm_mon + 1;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [5]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [6]  // NOLINT
            *++ptr = dateDelimiter;                                    // [7]
            i = tm.tm_mday;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [8]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [9]  // NOLINT
            *++ptr = ' ';                                              // [10]
            i = tm.tm_hour;
            *++ptr = '0' + static_cast<char>(i / 10);                  // [11] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);                  // [12] // NOLINT
            *++ptr = '\0';                                             // [13]
            // Total length is 14.
            return buffer;
        }

        //! The minimal length of the buffer required for DotNetTicks::toStringDate to represent ticks as "yyyy-MM-dd".
        static constexpr int toStringDateBufferLength{ 11 };

        //! The text representation of the ticks as "yyyy-MM-dd". Returns the \p buffer.
        //! \param buffer The character buffer.
        //! \param bufferLength The character buffer length, must be at least DotNetTicks::toStringDateBufferLength characters, including trailing zero.
        //! \param ticks The number of 100-nanosecond ticks.
        //! \param dateDelimiter The date delimiter character, default value is dash.
        static const char* toStringDate(char* buffer, const int bufferLength, const long long ticks, const char dateDelimiter = '-') noexcept
        {
            char *ptr = buffer;
            if (toStringDateBufferLength > bufferLength)
                return buffer;
            int nanoseconds100;
            struct tm tm{};
            toTm(ticks, tm, nanoseconds100);
            int i{ tm.tm_year + 1900 };
            *ptr = '0' + static_cast<char>(i / 1000);           // [0]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 1000) / 100); // [1]  // NOLINT
            *++ptr = '0' + static_cast<char>((i % 100) / 10);   // [2]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);           // [3]  // NOLINT
            *++ptr = dateDelimiter;                             // [4]
            i = tm.tm_mon + 1;
            *++ptr = '0' + static_cast<char>(i / 10);           // [5]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);           // [6]  // NOLINT
            *++ptr = dateDelimiter;                             // [7]
            i = tm.tm_mday;
            *++ptr = '0' + static_cast<char>(i / 10);           // [8]  // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10);           // [9]  // NOLINT
            *++ptr = '\0';                                      // [10]
            // Total length is 11.
            return buffer;
        }

        //! The minimal length of the buffer required for DotNetTicks::toStringTime to represent ticks as "hh:mm:ss".
        static constexpr int toStringTimeBufferLength{ 9 };

        //! The text representation of the ticks as "hh:mm:ss". Returns the \p buffer.
        //! \param buffer The character buffer.
        //! \param bufferLength The character buffer length, must be at least DotNetTicks::toStringTimeBufferLength characters, including trailing zero.
        //! \param ticks The number of 100-nanosecond ticks.
        static const char* toStringTime(char* buffer, const int bufferLength, const long long ticks) noexcept
        {
            char *ptr = buffer;
            if (toStringTimeBufferLength > bufferLength)
                return buffer;
            int nanoseconds100;
            struct tm tm{};
            toTm(ticks, tm, nanoseconds100);
            int i{ tm.tm_hour };
            *ptr = '0' + static_cast<char>(i / 10);   // [0] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10); // [1] // NOLINT
            *++ptr = ':';                             // [2]
            i = tm.tm_min;
            *++ptr = '0' + static_cast<char>(i / 10); // [3] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10); // [4] // NOLINT
            *++ptr = ':';                             // [5]
            i = tm.tm_sec;
            *++ptr = '0' + static_cast<char>(i / 10); // [6] // NOLINT
            *++ptr = '0' + static_cast<char>(i % 10); // [7] // NOLINT
            *++ptr = '\0';                            // [8]
            // Total length is 9.
            return buffer;
        }

    private:
        // Number of 100ns ticks per time unit.
        static constexpr long long ticksPerMillisecond{ 10000 };
        static constexpr long long ticksPerSecond{ ticksPerMillisecond * 1000 };
        static constexpr long long ticksPerMinute{ ticksPerSecond * 60 };
        static constexpr long long ticksPerHour{ ticksPerMinute * 60 };
        static constexpr long long ticksPerDay{ ticksPerHour * 24 };
 
        // Number of milliseconds per time unit.
        static constexpr int millisPerSecond{ 1000 };
        static constexpr int millisPerMinute{ millisPerSecond * 60 };
        static constexpr int millisPerHour{ millisPerMinute * 60 };
        static constexpr int millisPerDay{ millisPerHour * 24 };

        // Number of days in a non-leap year.
        static constexpr int daysPerYear{ 365 };
        // Number of days in 4 years.
        static constexpr int daysPer4Years{ daysPerYear * 4 + 1 };
        // Number of days in 100 years.
        static constexpr int daysPer100Years{ daysPer4Years * 25 - 1 };
        // Number of days in 400 years.
        static constexpr int daysPer400Years{ daysPer100Years * 4 + 1 };

        // Number of days from 1/1/0001 to 12/31/1600.
        static constexpr int daysTo1601{ daysPer400Years * 4 };
        // Number of days from 1/1/0001 to 12/30/1899.
        static constexpr int daysTo1899{ daysPer400Years * 4 + daysPer100Years * 3 - 367 };
        // Number of days from 1/1/0001 to 12/31/9999.
        static constexpr int daysTo10000{ daysPer400Years * 25 - 366 };

        static constexpr long long maxMillis{ static_cast<long long>(daysTo10000) * millisPerDay };
        static constexpr long long minTicks{ 0 };
        static constexpr long long maxTicks{ daysTo10000 * ticksPerDay - 1 };

        static constexpr int daysToMonth365[] { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
        static constexpr int daysToMonth366[] { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
    };
}
