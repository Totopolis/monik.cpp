// format.h
//
#pragma once
#ifndef __MONIK_COMMON_FORMAT_H__
#define __MONIK_COMMON_FORMAT_H__

#include "monik/common/common.h"

namespace sdl {

inline size_t strcount(const char * str1, const char * str2) {
    MONIK_ASSERT(str1 && str2);
    const size_t len2 = strlen(str2);
    size_t count = 0;
    while ((str1 = strstr(str1, str2)) != nullptr) {
        ++count;
        str1 += len2;
    }
    return count;
}

template<size_t buf_size> inline
const char * format_s(char(&buf)[buf_size], const char * const str) {
    static_assert(buf_size > 20, "");
    if (str) {
        const size_t len = strlen(str);
        if (len < buf_size) {
            memcpy(buf, str, len);
            buf[len] = 0;
            return buf;
        }
    }
    MONIK_ASSERT(!"format_s");
    buf[0] = 0;
    return buf;
}

template<size_t buf_size, typename... Ts> inline
const char * format_s(char(&buf)[buf_size], const char * const str, Ts&&... params) {
    static_assert(buf_size > 20, "");
    if (is_str_valid(str)) {
        MONIK_ASSERT(strlen(str) < buf_size);
        MONIK_ASSERT(strcount(str, "%") == sizeof...(params));
        if (snprintf(buf, buf_size, str, std::forward<Ts>(params)...) > 0) {
            buf[buf_size-1] = 0;
            return buf;
        }
    }
    MONIK_ASSERT(!"format_s");
    buf[0] = 0;
    return buf;
}

// precision - number of digits after dot; trailing zeros are discarded
template<int buf_size>
const char * format_double(char(&buf)[buf_size], const double value, const int precision) {
    static_assert(buf_size > 20, "");
    static_assert(buf_size > limits::double_max_digits10, "");
    static_assert(limits::double_max_digits10 == 17, "");
#if MONIK_DEBUG
    memset_zero(buf);
#endif
    MONIK_ASSERT((precision > 0) || debug::is_unit_test());
    MONIK_ASSERT((precision < buf_size - 1) || debug::is_unit_test());
    int c = snprintf(buf, buf_size, "%#.*f", 
        a_min_max<int, 0, limits::double_max_digits10>(precision),
        value); // print '.' char even if the value is integer
    if (c > 0) { 
        MONIK_ASSERT(c < buf_size);
        c = a_min<int, limits::double_max_digits10>(a_min<int, buf_size - 1>(c));
        char * p = buf + c - 1; // pointer to last meaning char 
        while ((p > buf) && (*p == '0')) {
            --p;
        }
        MONIK_ASSERT(p >= buf);
        if ((*p == '.') || (*p == ',')) {
            --p;
        }
        MONIK_ASSERT((p - buf + 1) >= 0);
        p[1] = 0;
        while(--p >= buf) {
            if (*p == ',') {
                *p = '.';
                break;
            }
        }
        return buf;
    }
    MONIK_ASSERT(0);
    buf[0] = 0;
    return buf;
}

// remove leading and trailing spaces
template<class string_type, typename string_type::value_type const space>
string_type impl_to_string_trim(const string_type & s)
{
    if (!s.empty()) {
        size_t const size = s.size();
        size_t s1 = 0;
        while ((s1 < size) && (s[s1] == space)) {
            ++s1;
        }
        if (s1 < size) {
            MONIK_ASSERT(s[s1] != space);
            size_t s2 = size - 1;
            while (s[s2] == space) {
                --s2;
            }
            MONIK_ASSERT(s1 <= s2);
            if ((s1 == 0) && (s2 == size - 1)) {
                return s;
            }
            return s.substr(s1, s2 - s1 + 1);
        }
    }
    return {};
}

inline std::string trim_string(const std::string & s) {
    return impl_to_string_trim<std::string, ' '>(s);
}

inline std::wstring trim_string(const std::wstring & s) {
    return impl_to_string_trim<std::wstring, L' '>(s);
}

} // sdl

#endif // __MONIK_COMMON_FORMAT_H__
