// time_util.h
//
#pragma once
#ifndef __MONIK_COMMON_TIME_UTIL_H__
#define __MONIK_COMMON_TIME_UTIL_H__

#include "monik/common/common.h"
#include <time.h>  /* time_t, struct tm, time, localtime, strftime */
#include <chrono>

namespace monik {

class time_span {
    time_t m_start;
public:
    time_span() { reset(); }
    explicit time_span(time_t t): m_start(t) {}
    void reset();
    time_t start() const { return m_start; }
    time_t now() const; // time in seconds elapsed since reset()
    time_t now_reset(); // now() and reset()
};

inline void time_span::reset()
{
    ::time(&m_start);
}

inline time_t time_span::now() const
{
    time_t value;
    ::time(&value);
    return (value - m_start);
}

inline time_t time_span::now_reset()
{
    time_t res = now();
    reset();
    return res;
}

struct time_util: is_static {
    static bool safe_gmtime(struct tm & dest, time_t);
};

inline auto time_since_epoch_seconds() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

inline auto time_since_epoch_milliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

} // monik

#endif // __MONIK_COMMON_TIME_UTIL_H__
