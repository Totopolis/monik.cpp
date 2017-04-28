// config.h
//
#pragma once
#ifndef __MONIK_COMMON_CONFIG_H__
#define __MONIK_COMMON_CONFIG_H__

#include "monik/common/stdcommon.h"

namespace monik {
#if MONIK_DEBUG
    struct debug {
        static int & warning_level() { 
            static int value = 1; 
            return value;
        }
        static bool & is_unit_test() {
            static bool value = false;
            return value;
        }
        static void trace() { std::cout << std::endl; }
        template<typename T, typename... Ts>
        static void trace(T && value, Ts&&... params) {
            std::cout << value;
            trace(std::forward<Ts>(params)...);
        }
        template<typename T, typename... Ts>
        static void trace_if(const bool condition, T && value, Ts&&... params) {
            if (condition) {
                trace(std::forward<T>(value), std::forward<Ts>(params)...);
            }
        }
        static void warning(const char * message, const char * fun, const int line) {
            if (warning_level()) {
                std::cout << "\nwarning (" << message << ") in " << fun << " at line " << line << std::endl; 
                assert(warning_level() < 2);
            }
        }
    };
#endif
} // monik

#if MONIK_DEBUG
#define MONIK_TRACE(...)              monik::debug::trace(__VA_ARGS__)
#define MONIK_TRACE_IF(...)           monik::debug::trace_if(__VA_ARGS__)
#define MONIK_TRACE_FILE              ((void)0)
#define MONIK_TRACE_FUNCTION          MONIK_TRACE(__FUNCTION__)
#define MONIK_DEBUG_SETPRECISION(...) std::cout << std::setprecision(__VA_ARGS__)
#else
#define MONIK_TRACE(...)              ((void)0)
#define MONIK_TRACE_IF(...)           ((void)0)
#define MONIK_TRACE_FILE              ((void)0)
#define MONIK_TRACE_FUNCTION          ((void)0)
#define MONIK_DEBUG_SETPRECISION(...) ((void)0)
#endif

#if defined(MONIK_OS_WIN32) && MONIK_DEBUG && defined(NDEBUG) 
#define MONIK_NDEBUG_ASSERT(x) (void)(!!(x) || (monik::debug::warning(#x, __FUNCTION__, __LINE__), __debugbreak(), 0))
#endif

#if MONIK_DEBUG
#if defined(MONIK_OS_WIN32) && defined(NDEBUG) 
inline void MONIK_ASSERT_1(bool x)    { MONIK_NDEBUG_ASSERT(x); }
#define MONIK_ASSERT(...)             MONIK_NDEBUG_ASSERT(__VA_ARGS__)
#else
inline void MONIK_ASSERT_1(bool x)    { assert(x); }
#define MONIK_ASSERT(...)             assert(__VA_ARGS__)
#endif
#define MONIK_WARNING(x)              (void)(!!(x) || (monik::debug::warning(#x, __FUNCTION__, __LINE__), 0))
#define MONIK_VERIFY(expr)            (void)(!!(expr) || (assert(false), 0))
#define MONIK_DEBUG_CODE(expr)        expr
#else
#define MONIK_ASSERT_1(...)           ((void)0)
#define MONIK_ASSERT(...)             ((void)0)
#define MONIK_WARNING(...)            ((void)0)
#define MONIK_VERIFY(...)             ((void)(expr))
#define MONIK_DEBUG_CODE(...)         
#endif

#if MONIK_DEBUG > 1
#define MONIK_ASSERT_DEBUG_2(...)     MONIK_ASSERT(__VA_ARGS__)
#define MONIK_WARNING_DEBUG_2(...)    MONIK_WARNING(__VA_ARGS__)
#define MONIK_TRACE_DEBUG_2(...)      MONIK_TRACE(__VA_ARGS__)
#else
#define MONIK_ASSERT_DEBUG_2(...)     ((void)0)
#define MONIK_WARNING_DEBUG_2(...)    ((void)0)
#define MONIK_TRACE_DEBUG_2(...)      ((void)0)
#endif

#if defined(MONIK_OS_WIN32)
#define MONIK_ASSERT_WIN32(...)       MONIK_ASSERT(__VA_ARGS__)
#else
#define MONIK_ASSERT_WIN32(...)       ((void)0)
#endif

#if MONIK_DEBUG
#define MONIK_TRACE_ERROR(...)        (MONIK_TRACE(__VA_ARGS__), MONIK_ASSERT_WIN32(false), 0)
#else
#define MONIK_TRACE_ERROR(...)        ((void)0)
#endif

#define M_CURRENT_BYTE_ORDER          (*(uint32 *)"\x01\x02\x03\x04")
#define M_LITTLE_ENDIAN_BYTE_ORDER    0x04030201
#define M_BIG_ENDIAN_BYTE_ORDER       0x01020304
#define M_PDP_ENDIAN_BYTE_ORDER       0x02010403

#define M_IS_LITTLE_ENDIAN (M_CURRENT_BYTE_ORDER == M_LITTLE_ENDIAN_BYTE_ORDER)
#define M_IS_BIG_ENDIAN    (M_CURRENT_BYTE_ORDER == M_BIG_ENDIAN_BYTE_ORDER)
#define M_IS_PDP_ENDIAN    (M_CURRENT_BYTE_ORDER == M_PDP_ENDIAN_BYTE_ORDER)

#define M_STATIC_ASSERT_IS_POD(...)         static_assert(std::is_pod<__VA_ARGS__>::value, "std::is_pod")
#define M_STATIC_CHECK_IS_POD(...)          static_assert(std::is_pod<decltype(__VA_ARGS__)>::value, "std::is_pod")
#define M_STATIC_ASSERT_IS_INTEGRAL(...)    static_assert(std::is_integral<__VA_ARGS__>::value, "std::is_integral")
#define M_STATIC_ASSERT_TYPE(...)           static_assert(std::is_same<__VA_ARGS__>::value, "std::is_same")
#define M_STATIC_ASSERT_NOT_TYPE(...)       static_assert(!std::is_same<__VA_ARGS__>::value, "!std::is_same")
#define M_STATIC_SAME_TYPE(x1, x2)          static_assert(std::is_same<decltype(x1), decltype(x2)>::value, "std::is_same decltype")
#define M_STATIC_NOT_SAME_TYPE(x1, x2)      static_assert(!std::is_same<decltype(x1), decltype(x2)>::value, "!std::is_same decltype")
#define M_STATIC_CHECK_TYPE(T, x)           static_assert(std::is_same<T, decltype(x)>::value, "std::is_same")
#define M_STATIC_CHECK_NOT_TYPE(T, x)       static_assert(!std::is_same<T, decltype(x)>::value, "!std::is_same")

// require clang version 3.5.0 or later
#define monik_static_assert_is_nothrow_move_assignable(x) static_assert(std::is_nothrow_move_assignable<x>::value, "std::is_nothrow_move_assignable")
#define monik_static_check_is_nothrow_move_assignable(x)  static_assert(std::is_nothrow_move_assignable<decltype(x)>::value, "std::is_nothrow_move_assignable")
#define monik_static_assert_is_nothrow_copy_assignable(x) static_assert(std::is_nothrow_copy_assignable<x>::value, "std::is_nothrow_copy_assignable")
#define monik_static_check_is_nothrow_copy_assignable(x)  static_assert(std::is_nothrow_copy_assignable<decltype(x)>::value, "std::is_nothrow_copy_assignable")

#if defined(MONIK_OS_WIN32) // FIXME: clang support on Linux
#define monik_static_assert_is_trivially_copyable(x)      static_assert(std::is_trivially_copyable<x>::value, "std::is_trivially_copyable")
#else
#define monik_static_assert_is_trivially_copyable(x)      ((void)0)
#endif

#define M_STATIC_ASSERT_64_BIT \
    static_assert(sizeof(void *) == sizeof(std::int64_t), "64-bit only"); \
    static_assert(sizeof(size_t) == sizeof(std::int64_t), "64-bit only")

#if defined(MONIK_OS_WIN32) // Level4 (/W4)
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4512) // assignment operator could not be generated
#pragma warning(disable: 4706) // assignment within conditional expression
#endif

#if !defined(MONIK_OS_WIN32)
    #if defined(NDEBUG) && MONIK_DEBUG
        #error MONIK_DEBUG
    #endif
#endif

#if !defined(_MSC_VER)
#ifndef __cplusplus
  #error C++ is required
#elif __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#elif __cplusplus < 201402L
  //FIXME: #error C++14 is required
#endif
#endif

#endif // __MONIK_COMMON_CONFIG_H__
