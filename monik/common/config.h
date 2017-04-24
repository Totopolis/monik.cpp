// config.h
//
#pragma once
#ifndef __MONIK_COMMON_CONFIG_H__
#define __MONIK_COMMON_CONFIG_H__

#include "monik/common/stdcommon.h"

namespace sdl {
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
            std::cout << value; trace(params...);
        }
        static void warning(const char * message, const char * fun, const int line) {
            if (warning_level()) {
                std::cout << "\nwarning (" << message << ") in " << fun << " at line " << line << std::endl; 
                assert(warning_level() < 2);
            }
        }
    };
#endif
} // sdl

#if MONIK_DEBUG
#define MONIK_TRACE(...)              sdl::debug::trace(__VA_ARGS__)
#define MONIK_TRACE_FILE              ((void)0)
#define MONIK_TRACE_FUNCTION          MONIK_TRACE(__FUNCTION__)
#define MONIK_DEBUG_SETPRECISION(...) std::cout << std::setprecision(__VA_ARGS__)
#else
#define MONIK_TRACE(...)              ((void)0)
#define MONIK_TRACE_FILE              ((void)0)
#define MONIK_TRACE_FUNCTION          ((void)0)
#define MONIK_DEBUG_SETPRECISION(...) ((void)0)
#endif

#if defined(MONIK_OS_WIN32) && MONIK_DEBUG && defined(NDEBUG) 
#define MONIK_NDEBUG_ASSERT(x) (void)(!!(x) || (sdl::debug::warning(#x, __FUNCTION__, __LINE__), __debugbreak(), 0))
#endif

#if MONIK_DEBUG
#if defined(MONIK_OS_WIN32) && defined(NDEBUG) 
inline void MONIK_ASSERT_1(bool x)    { MONIK_NDEBUG_ASSERT(x); }
#define MONIK_ASSERT(...)             MONIK_NDEBUG_ASSERT(__VA_ARGS__)
#else
inline void MONIK_ASSERT_1(bool x)    { assert(x); }
#define MONIK_ASSERT(...)             assert(__VA_ARGS__)
#endif
#define MONIK_WARNING(x)              (void)(!!(x) || (sdl::debug::warning(#x, __FUNCTION__, __LINE__), 0))
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

#define CURRENT_BYTE_ORDER          (*(uint32 *)"\x01\x02\x03\x04")
#define LITTLE_ENDIAN_BYTE_ORDER    0x04030201
#define BIG_ENDIAN_BYTE_ORDER       0x01020304
#define PDP_ENDIAN_BYTE_ORDER       0x02010403

#define IS_LITTLE_ENDIAN (CURRENT_BYTE_ORDER == LITTLE_ENDIAN_BYTE_ORDER)
#define IS_BIG_ENDIAN    (CURRENT_BYTE_ORDER == BIG_ENDIAN_BYTE_ORDER)
#define IS_PDP_ENDIAN    (CURRENT_BYTE_ORDER == PDP_ENDIAN_BYTE_ORDER)

#define A_STATIC_ASSERT_IS_POD(...)         static_assert(std::is_pod<__VA_ARGS__>::value, "std::is_pod")
#define A_STATIC_CHECK_IS_POD(...)          static_assert(std::is_pod<decltype(__VA_ARGS__)>::value, "std::is_pod")
#define A_STATIC_ASSERT_IS_INTEGRAL(...)    static_assert(std::is_integral<__VA_ARGS__>::value, "std::is_integral")
#define A_STATIC_ASSERT_TYPE(...)           static_assert(std::is_same<__VA_ARGS__>::value, "std::is_same")
#define A_STATIC_ASSERT_NOT_TYPE(...)       static_assert(!std::is_same<__VA_ARGS__>::value, "!std::is_same")
#define A_STATIC_SAME_TYPE(x1, x2)          static_assert(std::is_same<decltype(x1), decltype(x2)>::value, "std::is_same decltype")
#define A_STATIC_NOT_SAME_TYPE(x1, x2)      static_assert(!std::is_same<decltype(x1), decltype(x2)>::value, "!std::is_same decltype")
#define A_STATIC_CHECK_TYPE(T, x)           static_assert(std::is_same<T, decltype(x)>::value, "std::is_same")
#define A_STATIC_CHECK_NOT_TYPE(T, x)       static_assert(!std::is_same<T, decltype(x)>::value, "!std::is_same")

// require clang version 3.5.0 or later
#define static_assert_is_nothrow_move_assignable(x) static_assert(std::is_nothrow_move_assignable<x>::value, "std::is_nothrow_move_assignable")
#define static_check_is_nothrow_move_assignable(x)  static_assert(std::is_nothrow_move_assignable<decltype(x)>::value, "std::is_nothrow_move_assignable")
#define static_assert_is_nothrow_copy_assignable(x) static_assert(std::is_nothrow_copy_assignable<x>::value, "std::is_nothrow_copy_assignable")
#define static_check_is_nothrow_copy_assignable(x)  static_assert(std::is_nothrow_copy_assignable<decltype(x)>::value, "std::is_nothrow_copy_assignable")

#if defined(MONIK_OS_WIN32) // FIXME: clang support on Linux
#define static_assert_is_trivially_copyable(x)      static_assert(std::is_trivially_copyable<x>::value, "std::is_trivially_copyable")
#else
#define static_assert_is_trivially_copyable(x)      ((void)0)
#endif

//#define MONIK_DECAY_DECLTYPE(expr)   std::decay<decltype(expr)>::type

#define A_STATIC_ASSERT_64_BIT \
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
