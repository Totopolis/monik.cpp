// log_util.h
//
#pragma once
#ifndef __MONIK_LOG_LOG_UTIL_H__
#define __MONIK_LOG_LOG_UTIL_H__

#include "monik/common/common.h"

namespace monik { namespace log {

class is_std_to_string_ {
    template<typename T> static auto check(T && x) -> decltype(std::to_string(x));
    template<typename T> static void check(...);
    template<typename T> static T && make();
public:
    template<typename T> static auto test() -> decltype(check<T>(make<T>()));
};

template<typename T> 
using is_std_to_string_t = decltype(is_std_to_string_::test<T>());

template<typename T> 
using is_std_to_string = bool_constant<!std::is_same<void,is_std_to_string_t<T>>::value>;

//------------------------------------------------------------

class log_util : is_static {
    template<typename T>
    static std::string impl_to_string(T && value, bool_constant<true>) {
        return std::to_string(value);
    }
    template<typename T>
    static std::string impl_to_string(T && value, bool_constant<false>) {
        std::stringstream ss; 
        ss << value;
        return ss.str();
    }
public:
    static std::string to_string(std::string const & value) {
        return value;
    }
    static std::string to_string(std::string && value) {
        return std::move(value);
    }
    static std::string to_string(const char * value) {
        MONIK_ASSERT(value);
        return is_str_valid(value) ? std::string(value) : std::string();
    }
    template<typename T>
    static std::string to_string(T && value) {
        return impl_to_string(std::forward<T>(value), 
            bool_constant<is_std_to_string<T>::value>());
    }
    template<typename T, typename... Ts>
    static std::string to_string(T && value, Ts&&... params) {
        return log_util::to_string(std::forward<T>(value)) + 
            log_util::to_string(std::forward<Ts>(params)...);
    }
};

} // log
} // monik

#endif // __MONIK_LOG_LOG_UTIL_H__

