// log_define.cpp
//
#include "monik/log/log_define.h"

#if MONIK_DEBUG
namespace sdl { namespace log { namespace {

inline constexpr const char * maybe_constexpr(const char * s) { 
    return s;
}

#define is_constexpr(e) noexcept(maybe_constexpr(e))

const char * foo_runtime(const char * s){
    return "foo_runtime";
}

constexpr const char * foo_compiletime(const char * s) {
    return "foo_compiletime";
}

#define foo(X) (is_constexpr(X)?foo_compiletime(X):foo_runtime(X))

class unit_test {
public:
    unit_test()
    {
        if (0) {
            MONIK_LOG_TRACE(__FUNCTION__, " at line ", __LINE__);
            MONIK_LOG_TRACE(__FUNCTION__);
            MONIK_LOG_DEBUG(__FUNCTION__);
            MONIK_LOG_INFO(__FUNCTION__);
            MONIK_LOG_WARNING(__FUNCTION__);
            MONIK_LOG_ERROR(__FUNCTION__);
            MONIK_LOG_FATAL(__FUNCTION__);
        }
        if (0) {
            MONIK_LOG_TRACE_SOURCE(monik_source::application, " at line ", __LINE__);
            MONIK_LOG_TRACE_SOURCE(monik_source::application, "message");
            MONIK_LOG_TRACE_SOURCE(monik_source::application);
            //
            MONIK_LOG_TRACE_SOURCE(__FUNCTION__);
            MONIK_LOG_DEBUG_SOURCE(__FUNCTION__);
            MONIK_LOG_INFO_SOURCE(__FUNCTION__);
            MONIK_LOG_WARNING_SOURCE(__FUNCTION__);
            MONIK_LOG_ERROR_SOURCE(__FUNCTION__);
            MONIK_LOG_FATAL_SOURCE(__FUNCTION__);
            //
            MONIK_LOG_TRACE_SYSTEM(__FUNCTION__);
            MONIK_LOG_TRACE_APPLICATION(__FUNCTION__);
            MONIK_LOG_TRACE_LOGIC(__FUNCTION__);
            MONIK_LOG_TRACE_SECURITY(__FUNCTION__);
            //
            MONIK_LOG_DEBUG_SYSTEM(__FUNCTION__);
            MONIK_LOG_INFO_SYSTEM(__FUNCTION__);
            MONIK_LOG_WARNING_SYSTEM(__FUNCTION__);
            MONIK_LOG_ERROR_SYSTEM(__FUNCTION__);
            MONIK_LOG_FATAL_SYSTEM(__FUNCTION__);
        }
        if (0) {
            const char * a = "a";
            constexpr const char * c = "c";
            MONIK_TRACE(a, " = ", foo(a));
            MONIK_TRACE(c, " = ", foo(c));
            MONIK_TRACE(__FUNCTION__, " = ", foo(__FUNCTION__));
            static_assert(!is_constexpr(a), "");
            static_assert(is_constexpr(c), "");
            static_assert(is_constexpr(__FUNCTION__), "");
        }
    }
};
static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG



