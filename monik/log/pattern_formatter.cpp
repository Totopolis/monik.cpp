// logger.cpp
//
#include "monik/log/pattern_formatter.h"
#include "monik/log/logger.h"
#include <thread>
#include <chrono>
#include <ctime>

namespace sdl { namespace log { namespace pattern_formatter_ {

/* struct tm {
    int tm_sec;   // seconds after the minute - [0, 60] including leap second
    int tm_min;   // minutes after the hour - [0, 59]
    int tm_hour;  // hours since midnight - [0, 23]
    int tm_mday;  // day of the month - [1, 31]
    int tm_mon;   // months since January - [0, 11]
    int tm_year;  // years since 1900
    int tm_wday;  // days since Sunday - [0, 6]
    int tm_yday;  // days since January 1 - [0, 365]
    int tm_isdst; // daylight savings time flag
}; */

const char * const WEEKDAY_NAMES[] = {
	"Sunday",       // 0
	"Monday",       // 1
	"Tuesday",      // 2
	"Wednesday",    // 3
	"Thursday",     // 4
	"Friday",       // 5
	"Saturday"      // 6
};

const char * const ABBREVIATED_WEEKDAY[] = {
	"Sun",  // 0
	"Mon",  // 1
	"Tue",  // 2
	"Wed",  // 3
	"Thu",  // 4
	"Fri",  // 5
	"Sat"   // 6
};

const char * const MONTH_NAMES[] = {
	"January",      // 0
	"February",     // 1
	"March",        // 2
	"April",        // 3
	"May",          // 4
	"June",         // 5
	"July",         // 6
	"August",       // 7
	"September",    // 8
	"October",      // 9
	"November",     // 10
	"December"      // 11
};

const char * const ABBREVIATED_MONTH[] = {
	"Jan",  // 0
	"Feb",  // 1
	"Mar",  // 2
	"Apr",  // 3
	"May",  // 4
	"Jun",  // 5
	"Jul",  // 6
	"Aug",  // 7
	"Sep",  // 8
	"Oct",  // 9
	"Nov",  // 10
	"Dec"   // 11
};

template<class T>
void convert_time_since_epoch(const T & tt, ::tm * utc_tm, ::tm * local_tm = nullptr) {
    A_STATIC_ASSERT_TYPE(std::time_t, time_t);
    MONIK_ASSERT(utc_tm || local_tm);
    static std::mutex m_mutex;
    std::unique_lock<std::mutex> lock(m_mutex);
    if (utc_tm) {
        * utc_tm = * ::gmtime(&tt);
    }
    if (local_tm) {
        * local_tm = * ::localtime(&tt);
    }
}

template< class Type, size_t n >
Type get_name_t(Type const(&names)[n], const int index) {
    if ((index >= 0) && (index < (int) count_of(names))) {
        return names[index];
    }
    MONIK_ASSERT(0);
    return {};
}

inline const char * get_month(const int tm_mon) {
    static_assert(count_of(MONTH_NAMES) == 12, "");
    return get_name_t(MONTH_NAMES, tm_mon);
}

inline const char * abbreviated_month(const int tm_mon) {
    static_assert(count_of(ABBREVIATED_MONTH) == 12, "");
    return get_name_t(ABBREVIATED_MONTH, tm_mon);
}

inline const char * get_weekday(const int tm_wday) {
    static_assert(count_of(WEEKDAY_NAMES) == 7, "");
    return get_name_t(WEEKDAY_NAMES, tm_wday);
}

inline const char * abbreviated_weekday(const int tm_wday) {
    static_assert(count_of(ABBREVIATED_WEEKDAY) == 7, "");
    return get_name_t(ABBREVIATED_WEEKDAY, tm_wday);
}

inline const char * get_month(::tm const & t) {
    return get_month(t.tm_mon);
}

inline const char * abbreviated_month(::tm const & t) {
    return abbreviated_month(t.tm_mon);
}

inline const char * get_weekday(::tm const & t) {
    return get_weekday(t.tm_wday);
}

inline const char * abbreviated_weekday(::tm const & t) {
    return abbreviated_weekday(t.tm_wday);
}

template<char pad>
inline void append_padded_10(std::string & text, const int number) {
    MONIK_ASSERT(number >= 0);
    if (number < 10)
        text += pad;
    text += std::to_string(number);
}

inline int hour_AMPM(const int tm_hour) {
    MONIK_ASSERT(tm_hour >= 0);
    MONIK_ASSERT(tm_hour < 24);
	if (tm_hour < 1)
		return 12;
	else if (tm_hour > 12)
		return tm_hour - 12;
	else
		return tm_hour;
}

inline bool hour_is_AM(const int tm_hour) {
    MONIK_ASSERT(tm_hour >= 0);
    MONIK_ASSERT(tm_hour < 24);
	return tm_hour < 12;
}

inline bool hour_is_PM(const int tm_hour) {
    MONIK_ASSERT(tm_hour >= 0);
    MONIK_ASSERT(tm_hour < 24);
	return tm_hour >= 12;
}

typedef long long long_long;

template<class duration_type, class T>
inline long_long cast_duration(const T & now) {
    return std::chrono::duration_cast<duration_type>(
        now.time_since_epoch()).count();
}

template<class T>
inline long_long time_seconds(const T & now) {
    return cast_duration<std::chrono::seconds>(now);
}

template<class T>
inline long_long time_milliseconds(const T & now) {
    return cast_duration<std::chrono::milliseconds>(now) % 1000;
}

template<class T>
inline long_long time_microseconds(const T & now) {
    return cast_duration<std::chrono::microseconds>(now) % 1000000;
}

template<size_t len>
std::string pad_zero(std::string && s) {
    MONIK_ASSERT(s.size() <= len);
    if (s.size() < len) {
        return std::string(len - s.size(), '0') + s;
    }
    return std::move(s);
}

class time_state final : noncopyable {
public:
    using now_type = decltype(std::chrono::system_clock::now());
    now_type now;
    ::tm utc_tm;
    ::tm local_tm;
    int current_year = 0;
    mutable bool use_local_tm = false;
    explicit time_state(now_type && t): now(std::move(t)) {
        const auto tt = std::chrono::system_clock::to_time_t(now);
        A_STATIC_CHECK_TYPE(const std::time_t, tt);
        convert_time_since_epoch(tt, &utc_tm, &local_tm);
        current_year = 1900 + utc_tm.tm_year;
    }
    time_state(): time_state(std::chrono::system_clock::now()) {}
};

using unique_time_state = std::unique_ptr<time_state>;

void format_with_time(std::string & text, const char key, 
    message_with_severity const & msg,
    const char * const source,
    unique_time_state & pstate)
{
    switch (key) {
    case 's': // %s - message source
        if (is_str_valid(source))
            text += source;
        break;
    case 't': // %t - message text
        text += msg.m_message;
        break;
    case '%': // %% - percent sign
        text += '%';
        break;
    case 'l': // %l - message priority level (1 .. 7)
        text += std::to_string(static_cast<int>(msg.m_severity));
        break;
    case 'p': // %p - message priority (Fatal, Critical, Error, Warning, Notice, Information, Debug, Trace)
        text += logger::to_string(msg.m_severity);
        break;
    case 'q': // %q - abbreviated message priority (F, C, E, W, N, I, D, T)
        text += logger::abbreviated(msg.m_severity);
        break;
    case 'I': // %I - message thread identifier (numeric)
        {
            std::stringstream ss;
            ss << std::this_thread::get_id();
            text += ss.str();
        }
        break;
    default:
        {
            if (!pstate) {
                pstate.reset(new time_state);
            }
            auto const & state = *pstate;
            auto const & utc_tm = state.use_local_tm ? state.local_tm : state.utc_tm;
            switch (key) {
            case 'w': // %w - message date/time abbreviated weekday (Mon, Tue, ...)
                text += abbreviated_weekday(utc_tm);
                break;
            case 'W': // %W - message date/time full weekday (Monday, Tuesday, ...)
                text += get_weekday(utc_tm);
                break;
            case 'b': // %b - message date/time abbreviated month (Jan, Feb, ...)
                text += abbreviated_month(utc_tm);
                break;
            case 'B': // %B - message date/time full month (January, February, ...)
                text += get_month(utc_tm);
                break;
            case 'd': // %d - message date/time zero-padded day of month (01 .. 31)
                append_padded_10<'0'>(text, utc_tm.tm_mday);
                break;
            case 'e': // %e - message date/time day of month (1 .. 31)
                text += std::to_string(utc_tm.tm_mday);
                break;
            case 'f': // %f - message date/time space-padded day of month ( 1 .. 31)
                append_padded_10<' '>(text, utc_tm.tm_mday);
                break;
            case 'm': // %m - message date/time zero-padded month (01 .. 12)
                append_padded_10<'0'>(text, utc_tm.tm_mon + 1);
                break;
            case 'n': // %n - message date/time month (1 .. 12)
                text += std::to_string(utc_tm.tm_mon + 1);
                break;
            case 'o': // %o - message date/time space-padded month ( 1 .. 12)
                append_padded_10<' '>(text, utc_tm.tm_mon + 1);
                break;
            case 'y': // %y - message date/time year without century (70)
                text += std::to_string(state.current_year % 100);
                break;
            case 'Y': // %Y - message date/time year with century (1970)
                text += std::to_string(state.current_year);
                break;
            case 'H': // %H - message date/time hour (00 .. 23)
                text += std::to_string(utc_tm.tm_hour);
                break;
            case 'h': // %h - message date/time hour (00 .. 12)
                text += std::to_string(hour_AMPM(utc_tm.tm_hour));
                break;
            case 'a': // %a - message date/time am/pm
                text += hour_is_AM(utc_tm.tm_hour) ? "am" : "pm";
                break;
            case 'A': // %A - message date/time AM/PM
                text += hour_is_AM(utc_tm.tm_hour) ? "AM" : "PM";
                break;
            case 'M': // %M - message date/time minute (00 .. 59)
                text += pad_zero<2>(std::to_string(utc_tm.tm_min));
                break;
            case 'S': // %S - message date/time second (00 .. 59)
                text += pad_zero<2>(std::to_string(utc_tm.tm_sec % 60));
                break;
            case 'i': // %i - message date/time millisecond (000 .. 999)
                text += pad_zero<3>(std::to_string(time_milliseconds(state.now)));
                break;
            case 'c': // %c - message date/time centisecond (0 .. 9)
                text += std::to_string(time_milliseconds(state.now) / 100);
                break;
            case 'F': // %F - message date/time fractional seconds/microseconds (000000 - 999999)
                text += pad_zero<6>(std::to_string(time_microseconds(state.now)));
                break;
#if 0
            case 'z': // %z - time zone differential in ISO 8601 format (Z or +NN.NN)
                break;
            case 'Z': // %Z - time zone differential in RFC format (GMT or +NNNN)
                break;
#endif
            case 'E': // %E - epoch time (UTC, seconds since midnight, January 1, 1970)
                text += std::to_string(time_seconds(state.now));
                break;
            case 'L': // %L - convert time to local time (must be specified before any date/time specifier; does not itself output anything)
                state.use_local_tm = true;
                break;
            case 'U': // %U - convert time to UTC time (must be specified before any date/time specifier; does not itself output anything)
                state.use_local_tm = false;
                break;
            default:
                MONIK_ASSERT_DEBUG_2(0);
                text += '%';
                text += key;
                break;
            }
        }
        break;
    }
}


} // pattern_formatter_

void pattern_formatter::format(std::string & text, const std::string & pattern,
    message_with_severity const & msg, const char * const source) 
{
    MONIK_ASSERT(!msg.m_message.empty() || source);
    using namespace pattern_formatter_;
    unique_time_state state;
    const size_t pattern_size = pattern.size();
    MONIK_ASSERT(pattern_size);
    size_t start = 0;
    while (start < pattern_size) {
        size_t pos = pattern.find('%', start);
        if (pos != std::string::npos) {
            if (pos != start) {
                text += pattern.substr(start, pos - start);
            }
            if (++pos != pattern_size) {
                format_with_time(text, pattern[pos], msg, source, state);
                start = pos + 1;
            }
            else {
                MONIK_ASSERT(0);
                break;
            }
        }
        else {
            text += pattern.substr(start);
            break;
        }
    }
}

} // log
} // sdl

#if MONIK_DEBUG
namespace sdl { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
            {
                using namespace pattern_formatter_;
                ::tm utc_tm, local_tm;
                const auto now = std::chrono::system_clock::now();
                const auto tt = std::chrono::system_clock::to_time_t(now);
                A_STATIC_CHECK_TYPE(const std::time_t, tt);
                convert_time_since_epoch(tt, &utc_tm, &local_tm);
                MONIK_ASSERT(is_str_valid(get_month(utc_tm)));
                MONIK_ASSERT(is_str_valid(get_weekday(utc_tm)));
                MONIK_ASSERT(is_str_valid(abbreviated_weekday(utc_tm)));
                MONIK_ASSERT(is_str_valid(abbreviated_month(utc_tm)));
            }
            {
                const auto s = pattern_formatter::format("%t [%s]", 
                    message_with_severity(severity::trace, "msg"),
                    "source");
                MONIK_ASSERT(s == "msg [source]");
            }
            {
                const auto s = pattern_formatter::format(
                    "year %Y, %y-%m-%d %h-%H:%M:%S, centisecond %c,"
                    "microseconds %F, millisecond %i, thread %I, priority %q: %t [%s], day %e", 
                    message_with_severity(severity::trace, "msg"),
                    "source");
                MONIK_ASSERT(!s.empty());
            }
            {
                const auto s = pattern_formatter::format(
                    "timestamp %E, year %Y, %y-%m-%d, UTC %h-%H:%M:%S, Local %L%h-%H:%M:%S, UTC again %U%h-%H:%M:%S, centisecond %c,"
                    "microseconds %F, millisecond %i, thread %I, priority %q: %t [%s], day %e", 
                    message_with_severity(severity::trace, "msg"),
                    "source");
                MONIK_ASSERT(!s.empty());
            }
        }
    };
    static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG
