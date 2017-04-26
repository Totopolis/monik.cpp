// algorithm.h
//
#pragma once
#ifndef __MONIK_COMMON_ALGORITHM_H__
#define __MONIK_COMMON_ALGORITHM_H__

#include "monik/common/common.h"

namespace monik { namespace algo { namespace scope_exit { 

template<typename fun_type>
struct scope_guard : noncopyable {
    scope_guard(fun_type && f) : fun(std::move(f)) {}
    ~scope_guard() {
        fun();
    }
private:
    fun_type fun;
};

#if MONIK_DEBUG
template<typename fun_type>
struct assert_guard : noncopyable {
    assert_guard(fun_type && f) : fun(std::move(f)) {}
    ~assert_guard() { 
        MONIK_ASSERT(fun());
    }
private:
    fun_type fun;
};
#else
template<typename fun_type>
struct assert_guard {
    explicit assert_guard(fun_type &&){}
};
#endif

} // scope_exit

#define _MONIK_LINENAME_CAT(name, line) name##line
#define _MONIK_LINENAME(name, line) _MONIK_LINENAME_CAT(name, line)
#define _MONIK_UTILITY_MAKE_GUARD(guard, ...) \
    auto _MONIK_LINENAME(scope_exit_fun_, __LINE__) = __VA_ARGS__; \
    monik::algo::scope_exit::guard<decltype(_MONIK_LINENAME(scope_exit_fun_, __LINE__))> \
    _MONIK_LINENAME(scope_exit_line_, __LINE__)(std::move(_MONIK_LINENAME(scope_exit_fun_, __LINE__)));
#define MONIK_UTILITY_SCOPE_EXIT(...) _MONIK_UTILITY_MAKE_GUARD(scope_guard, __VA_ARGS__)
#define MONIK_UTILITY_ASSERT_EXIT(...) _MONIK_UTILITY_MAKE_GUARD(assert_guard, __VA_ARGS__)

#if MONIK_DEBUG
#define ASSERT_SCOPE_EXIT(...)  MONIK_UTILITY_ASSERT_EXIT(__VA_ARGS__)
#else
#define ASSERT_SCOPE_EXIT(...)  ((void)0)
#endif

#if MONIK_DEBUG > 1
#define ASSERT_SCOPE_EXIT_DEBUG_2(...)  ASSERT_SCOPE_EXIT(__VA_ARGS__)
#else
#define ASSERT_SCOPE_EXIT_DEBUG_2(...)  ((void)0)
#endif

template<class T, class key_type>
inline bool is_find(T const & result, key_type const & value) {
    return std::find(std::begin(result), std::end(result), value) != std::end(result);
}

template<class T>
inline bool is_sorted(T const & result) {
    return std::is_sorted(std::begin(result), std::end(result));
}

template<class T, class fun_type>
inline bool is_sorted(T const & result, fun_type && compare) {
    return std::is_sorted(std::begin(result), std::end(result), compare);
}

template<class T>
inline bool is_unique(T const & result) {
    MONIK_ASSERT(is_sorted(result));
    return std::adjacent_find(std::begin(result), std::end(result)) == std::end(result);
}

template<class T>
inline void erase_unique(T & result) {
    MONIK_ASSERT(is_sorted(result));
    result.erase(std::unique(result.begin(), result.end()), result.end());
}

template<class T>
inline void sort_erase_unique(T & result) {
    std::sort(result.begin(), result.end());
    erase_unique(result);
}

namespace detail {

inline size_t strlen_t(std::string const & s) {
    return s.size();
}

template<class T>
inline size_t strlen_t(T const & s) { // avoid array decay to pointer
    MONIK_ASSERT(s);
    return strlen(s);
}

template<size_t buf_size>
inline size_t strlen_t(char const(&s)[buf_size]) {
    MONIK_ASSERT(s[buf_size - 1] == 0);
    MONIK_ASSERT(strlen(s) == buf_size - 1);
    return buf_size - 1;
}

inline const char * c_str(std::string const & s) {
    return s.c_str();
}

inline const char * c_str(const char * s) {
    MONIK_ASSERT(s);
    return s;
}

} // detail

//--------------------------------------------------------------
// compare two strings ignoring case

bool iequal_range(const char * first1, const char * last1, const char * first2); 

template<class T1, class T2>
inline bool iequal_n(T1 const & str1, T2 const & str2, const size_t N) {
    MONIK_ASSERT(detail::strlen_t(str2) >= N);
    if (detail::strlen_t(str1) >= N) {
        const char * const first1 = detail::c_str(str1);
        return iequal_range(first1, first1 + N, detail::c_str(str2));
    }
    return false;
}

template<class T1, class T2>
inline bool iequal_n(T1 const & str1, T2 const & str2) {
    const size_t N = detail::strlen_t(str2);
    if (detail::strlen_t(str1) >= N) {
        const char * const first1 = detail::c_str(str1);
        return iequal_range(first1, first1 + N, detail::c_str(str2));
    }
    return false;
}

template<class T1, class T2>
inline bool iequal(T1 const & str1, T2 const & str2) {
    const size_t N = detail::strlen_t(str2);
    if (detail::strlen_t(str1) == N) {
        const char * const first1 = detail::c_str(str1);
        return iequal_range(first1, first1 + N, detail::c_str(str2));
    }
    return false;
}

//--------------------------------------------------------------
// http://en.cppreference.com/w/cpp/algorithm/iota
template<typename T>
inline void iota(T & index, typename T::value_type i) { // Index generator
    for (auto & it : index) {
        it = i++;
    }
}
//--------------------------------------------------------------

struct lex : is_static {
    static std::vector<std::string> split(std::string const &, const char delim);
};

} // algo
} // monik

#endif // __MONIK_COMMON_ALGORITHM_H__
