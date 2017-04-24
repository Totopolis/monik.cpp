// algorithm.h
//
#pragma once
#ifndef __MONIK_COMMON_ALGORITHM_H__
#define __MONIK_COMMON_ALGORITHM_H__

#include "monik/common/common.h"
#include "monik/common/time_util.h"

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

template<class T, class key_type>
bool binary_insertion(T & result, key_type && unique_key) {
    ASSERT_SCOPE_EXIT_DEBUG_2([&result]{
        return is_sorted(result);
    });
    if (!result.empty()) {
        const auto pos = std::lower_bound(result.begin(), result.end(), unique_key);
        if (pos != result.end()) {
            if (*pos == unique_key) {
                return false;
            }
            result.insert(pos, std::forward<key_type>(unique_key));
            return true;
        }
    }
    result.push_back(std::forward<key_type>(unique_key)); 
    return true;
}

template<class T, class key_type, class fun_type>
bool binary_insertion(T & result, key_type && unique_key, fun_type && compare) {
    ASSERT_SCOPE_EXIT_DEBUG_2([&result, compare]{
        return is_sorted(result, compare);
    });
    if (!result.empty()) {
        const auto pos = std::lower_bound(result.begin(), result.end(), unique_key, compare);
        if (pos != result.end()) {
            if (*pos == unique_key) {
                MONIK_ASSERT(!compare(*pos, unique_key) && !compare(unique_key, *pos));
                return false;
            }
            result.insert(pos, std::forward<key_type>(unique_key));
            return true;
        }
    }
    result.push_back(std::forward<key_type>(unique_key)); 
    return true;
}

template<class T, class key_type>
void insertion_sort(T & result, const key_type & value) {
    ASSERT_SCOPE_EXIT_DEBUG_2([&result]{
        return is_sorted(result);
    });
    result.push_back(value);
    auto const left = result.begin();
    auto right = result.end(); --right;
    while (right > left) {
        auto const old = right--;
        if (*old < *right) {
            std::swap(*old, *right);
        }
        else {
            break;
        }
    }
    MONIK_ASSERT(left <= right);
}

template<class T, class key_type, class fun_type>
void insertion_sort(T & result, const key_type & value, fun_type && compare) {
    ASSERT_SCOPE_EXIT_DEBUG_2([&result, compare]{
        return is_sorted(result, compare);
    });
    result.push_back(value);
    auto const left = result.begin();
    auto right = result.end(); --right;
    while (right > left) {
        auto const old = right--;
        if (compare(*old, *right)) {
            std::swap(*old, *right);
        }
        else {
            break;
        }
    }
    MONIK_ASSERT(left <= right);
}

template<class T, class value_type>
bool unique_insertion(T & result, value_type const & value)
{
    ASSERT_SCOPE_EXIT_DEBUG_2([&result]{
        return std::is_sorted(result.begin(), result.end());
    });
    if (result.empty()) {
        result.push_back(value);
        return true;
    }
    auto const left = result.begin();
    auto right = result.end(); --right;
    for (;;) {        
        MONIK_ASSERT(!(right < left));
        if (value < *right) {
            if (left == right) {
                break;
            }
            --right;
        }
        else if (value == *right) {
            return false;
        }
        else {
            MONIK_ASSERT(*right < value);
            ++right;
            break;
        }
    } 
    result.insert(right, value);
    return true;
}

template<class T, class fun_type>
void for_reverse(T && data, fun_type && fun) {
    auto const last = data.begin();
    auto it = data.end();
    if (it != last) {
        do {
            --it;
            fun(*it);
        } while (it != last);
    }
}

template<class T>
bool is_same(T const & v1, T const & v2)
{
    size_t size = v1.size();
    if (size != v2.size()) {
        return false;
    }
    if (size) {
        auto p1 = v1.begin();
        auto p2 = v2.begin();
        for (;;) {
            if (*p1 != *p2)
                return false;
            if (--size == 0)
                break;
            ++p1;
            ++p2;
        }
    }
    return true;
}

template<typename T>
inline size_t number_of_1(T n) {
    static_assert(std::is_integral<T>::value, "");
    size_t count = 0;
    while (n) {
        ++count;
        n = (n - 1) & n;
    }
    return count;
}

//--------------------------------------------------------------

enum class stable_sort { false_, true_ };

template<stable_sort stable>
struct sort_t {
    template<class T, class Compare>
    static void sort(T & data, Compare && comp) {
        static_assert(stable == stable_sort::false_, "");
        std::sort(data.begin(), data.end(), std::forward<Compare>(comp));
    }
};

template<> struct sort_t<stable_sort::true_> {
    template<class T, class Compare>
    static void sort(T & data, Compare && comp) {
        std::stable_sort(data.begin(), data.end(), std::forward<Compare>(comp));
    }
};

//--------------------------------------------------------------

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

#if MONIK_DEBUG
#define MONIK_DEBUG_TIMER_SEC(timer, message) \
    monik::time_span timer; \
    MONIK_UTILITY_SCOPE_EXIT([&timer](){ \
        MONIK_TRACE(message, timer.now()); })
#else
#define MONIK_DEBUG_TIMER_SEC(...)      ((void)0)
#endif

} // monik

#endif // __MONIK_COMMON_ALGORITHM_H__
