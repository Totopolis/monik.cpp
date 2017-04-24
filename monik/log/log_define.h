// log_define.h
//
#pragma once
#ifndef __MONIK_LOG_LOG_DEFINE_H__
#define __MONIK_LOG_LOG_DEFINE_H__

#include "monik/log/logger.h"
#include "monik/log/log_util.h"

#if !defined(MONIK_LOG_ENABLE)
#define MONIK_LOG_ENABLE    1
#endif

#if MONIK_LOG_ENABLE

namespace sdl { namespace log {
class log_define : is_static {
public:
    template<typename T, typename... Ts>
    static void write(const severity prio, T && value, Ts&&... params) {
        if (logger const * const dest = get_logger()) {
            if (dest->is_pass(prio)) {
                dest->log(message_with_severity(prio, log_util::to_string(
                    std::forward<T>(value), 
                    std::forward<Ts>(params)...)));
            }
        }
    }
    static void write(const severity prio, const char * const s) {
        if (logger const * const dest = get_logger()) {
            if (dest->is_pass(prio)) {
                dest->log(message_with_severity(prio, s));
            }
        }
    }
    template<typename T, typename... Ts>
    static void write_with_source(const severity prio, message_source_ptr source, T && value, Ts&&... params) {
        if (logger const * const dest = get_logger()) {
            if (dest->is_pass(prio)) {
                dest->log(message_with_severity(prio, log_util::to_string(
                    std::forward<T>(value), 
                    std::forward<Ts>(params)...)), 
                    source);
            }
        }
    }
    static void write_with_source(const severity prio, message_source_ptr source, const char * const s) {
        if (logger const * const dest = get_logger()) {
            if (dest->is_pass(prio)) {
                dest->log(message_with_severity(prio, s), source);
            }
        }
    }
    static void write_with_source(const severity prio, message_source_ptr source) {
        if (logger const * const dest = get_logger()) {
            if (dest->is_pass(prio)) {
                dest->log(message_with_severity(prio), source);
            }
        }
    }
};

} // log
} // sdl

#if 0 // todo
#define MONIK_LOG_TRACE(...)      ((void)0)
#define MONIK_LOG_DEBUG(...)      ((void)0)
#define MONIK_LOG_INFO(...)       ((void)0)
#define MONIK_LOG_WARNING(...)    ((void)0)
#define MONIK_LOG_ERROR(...)      ((void)0)
#define MONIK_LOG_FATAL(...)      ((void)0)
#endif

#define MONIK_LOG_TRACE(...)      sdl::log::log_define::write(sdl::log::severity::trace, __VA_ARGS__)
#define MONIK_LOG_DEBUG(...)      sdl::log::log_define::write(sdl::log::severity::debug, __VA_ARGS__)
#define MONIK_LOG_INFO(...)       sdl::log::log_define::write(sdl::log::severity::info, __VA_ARGS__)
#define MONIK_LOG_WARNING(...)    sdl::log::log_define::write(sdl::log::severity::warning, __VA_ARGS__)
#define MONIK_LOG_ERROR(...)      sdl::log::log_define::write(sdl::log::severity::error, __VA_ARGS__)
#define MONIK_LOG_FATAL(...)      sdl::log::log_define::write(sdl::log::severity::fatal, __VA_ARGS__)

#define MONIK_LOG_TRACE_SOURCE(...)      sdl::log::log_define::write_with_source(sdl::log::severity::trace, __VA_ARGS__)
#define MONIK_LOG_DEBUG_SOURCE(...)      sdl::log::log_define::write_with_source(sdl::log::severity::debug, __VA_ARGS__)
#define MONIK_LOG_INFO_SOURCE(...)       sdl::log::log_define::write_with_source(sdl::log::severity::info, __VA_ARGS__)
#define MONIK_LOG_WARNING_SOURCE(...)    sdl::log::log_define::write_with_source(sdl::log::severity::warning, __VA_ARGS__)
#define MONIK_LOG_ERROR_SOURCE(...)      sdl::log::log_define::write_with_source(sdl::log::severity::error, __VA_ARGS__)
#define MONIK_LOG_FATAL_SOURCE(...)      sdl::log::log_define::write_with_source(sdl::log::severity::fatal, __VA_ARGS__)

#define MONIK_LOG_TRACE_SYSTEM(...)       sdl::log::log_define::write_with_source(sdl::log::severity::trace, monik_source::system, __VA_ARGS__)
#define MONIK_LOG_TRACE_APPLICATION(...)  sdl::log::log_define::write_with_source(sdl::log::severity::trace, monik_source::application, __VA_ARGS__)
#define MONIK_LOG_TRACE_LOGIC(...)        sdl::log::log_define::write_with_source(sdl::log::severity::trace, monik_source::logic, __VA_ARGS__)
#define MONIK_LOG_TRACE_SECURITY(...)     sdl::log::log_define::write_with_source(sdl::log::severity::trace, monik_source::security, __VA_ARGS__)

#define MONIK_LOG_DEBUG_SYSTEM(...)       sdl::log::log_define::write_with_source(sdl::log::severity::debug, monik_source::system, __VA_ARGS__)
#define MONIK_LOG_DEBUG_APPLICATION(...)  sdl::log::log_define::write_with_source(sdl::log::severity::debug, monik_source::application, __VA_ARGS__)
#define MONIK_LOG_DEBUG_LOGIC(...)        sdl::log::log_define::write_with_source(sdl::log::severity::debug, monik_source::logic, __VA_ARGS__)
#define MONIK_LOG_DEBUG_SECURITY(...)     sdl::log::log_define::write_with_source(sdl::log::severity::debug, monik_source::security, __VA_ARGS__)

#define MONIK_LOG_INFO_SYSTEM(...)       sdl::log::log_define::write_with_source(sdl::log::severity::info, monik_source::system, __VA_ARGS__)
#define MONIK_LOG_INFO_APPLICATION(...)  sdl::log::log_define::write_with_source(sdl::log::severity::info, monik_source::application, __VA_ARGS__)
#define MONIK_LOG_INFO_LOGIC(...)        sdl::log::log_define::write_with_source(sdl::log::severity::info, monik_source::logic, __VA_ARGS__)
#define MONIK_LOG_INFO_SECURITY(...)     sdl::log::log_define::write_with_source(sdl::log::severity::info, monik_source::security, __VA_ARGS__)

#define MONIK_LOG_WARNING_SYSTEM(...)       sdl::log::log_define::write_with_source(sdl::log::severity::warning, monik_source::system, __VA_ARGS__)
#define MONIK_LOG_WARNING_APPLICATION(...)  sdl::log::log_define::write_with_source(sdl::log::severity::warning, monik_source::application, __VA_ARGS__)
#define MONIK_LOG_WARNING_LOGIC(...)        sdl::log::log_define::write_with_source(sdl::log::severity::warning, monik_source::logic, __VA_ARGS__)
#define MONIK_LOG_WARNING_SECURITY(...)     sdl::log::log_define::write_with_source(sdl::log::severity::warning, monik_source::security, __VA_ARGS__)

#define MONIK_LOG_ERROR_SYSTEM(...)       sdl::log::log_define::write_with_source(sdl::log::severity::error, monik_source::system, __VA_ARGS__)
#define MONIK_LOG_ERROR_APPLICATION(...)  sdl::log::log_define::write_with_source(sdl::log::severity::error, monik_source::application, __VA_ARGS__)
#define MONIK_LOG_ERROR_LOGIC(...)        sdl::log::log_define::write_with_source(sdl::log::severity::error, monik_source::logic, __VA_ARGS__)
#define MONIK_LOG_ERROR_SECURITY(...)     sdl::log::log_define::write_with_source(sdl::log::severity::error, monik_source::security, __VA_ARGS__)

#define MONIK_LOG_FATAL_SYSTEM(...)       sdl::log::log_define::write_with_source(sdl::log::severity::fatal, monik_source::system, __VA_ARGS__)
#define MONIK_LOG_FATAL_APPLICATION(...)  sdl::log::log_define::write_with_source(sdl::log::severity::fatal, monik_source::application, __VA_ARGS__)
#define MONIK_LOG_FATAL_LOGIC(...)        sdl::log::log_define::write_with_source(sdl::log::severity::fatal, monik_source::logic, __VA_ARGS__)
#define MONIK_LOG_FATAL_SECURITY(...)     sdl::log::log_define::write_with_source(sdl::log::severity::fatal, monik_source::security, __VA_ARGS__)

#else // !MONIK_LOG_ENABLE

#define MONIK_LOG_TRACE(...)      ((void)0)
#define MONIK_LOG_DEBUG(...)      ((void)0)
#define MONIK_LOG_INFO(...)       ((void)0)
#define MONIK_LOG_WARNING(...)    ((void)0)
#define MONIK_LOG_ERROR(...)      ((void)0)
#define MONIK_LOG_FATAL(...)      ((void)0)

#define MONIK_LOG_TRACE_SOURCE(...)      ((void)0)
#define MONIK_LOG_DEBUG_SOURCE(...)      ((void)0)
#define MONIK_LOG_INFO_SOURCE(...)       ((void)0)
#define MONIK_LOG_WARNING_SOURCE(...)    ((void)0)
#define MONIK_LOG_ERROR_SOURCE(...)      ((void)0)
#define MONIK_LOG_FATAL_SOURCE(...)      ((void)0)

#define MONIK_LOG_TRACE_SYSTEM(...)       ((void)0)
#define MONIK_LOG_TRACE_APPLICATION(...)  ((void)0)
#define MONIK_LOG_TRACE_LOGIC(...)        ((void)0)
#define MONIK_LOG_TRACE_SECURITY(...)     ((void)0)

#define MONIK_LOG_DEBUG_SYSTEM(...)       ((void)0)
#define MONIK_LOG_DEBUG_APPLICATION(...)  ((void)0)
#define MONIK_LOG_DEBUG_LOGIC(...)        ((void)0)
#define MONIK_LOG_DEBUG_SECURITY(...)     ((void)0)

#define MONIK_LOG_INFO_SYSTEM(...)       ((void)0)
#define MONIK_LOG_INFO_APPLICATION(...)  ((void)0)
#define MONIK_LOG_INFO_LOGIC(...)        ((void)0)
#define MONIK_LOG_INFO_SECURITY(...)     ((void)0)

#define MONIK_LOG_WARNING_SYSTEM(...)       ((void)0)
#define MONIK_LOG_WARNING_APPLICATION(...)  ((void)0)
#define MONIK_LOG_WARNING_LOGIC(...)        ((void)0)
#define MONIK_LOG_WARNING_SECURITY(...)     ((void)0)

#define MONIK_LOG_ERROR_SYSTEM(...)       ((void)0)
#define MONIK_LOG_ERROR_APPLICATION(...)  ((void)0)
#define MONIK_LOG_ERROR_LOGIC(...)        ((void)0)
#define MONIK_LOG_ERROR_SECURITY(...)     ((void)0)

#define MONIK_LOG_FATAL_SYSTEM(...)       ((void)0)
#define MONIK_LOG_FATAL_APPLICATION(...)  ((void)0)
#define MONIK_LOG_FATAL_LOGIC(...)        ((void)0)
#define MONIK_LOG_FATAL_SECURITY(...)     ((void)0)

#endif // MONIK_LOG_ENABLE
#endif // __MONIK_LOG_LOG_DEFINE_H__

