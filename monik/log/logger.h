// logger.h
//
#pragma once
#ifndef __MONIK_LOG_LOGGER_H__
#define __MONIK_LOG_LOGGER_H__

#include "monik/log/message.h"
#include "monik/common/singleton.h"
#if MONIK_INCLUDE_AMQP
#include "monik/log/keepalive_log.h"
#endif

namespace monik { namespace log {

class logger : noncopyable {
public:
    using ST = singleton<logger>;
    logger();
    ~logger();
    using ex_handler = std::function<void(severity, std::exception_ptr)>;
    void reset();
    void set_handler(ex_handler &&);
    severity get_filter() const;
    void set_filter(severity);
    bool is_pass(severity) const;
    void set_console(std::initializer_list<severity>, const std::string & format = {});
    void set_channel(severity, shared_channel const &, const std::string & format = {});
    void add_channel(severity, shared_channel const &, const std::string & format = {});
#if MONIK_INCLUDE_AMQP
    bool add_keepalive(shared_keepalive const &);
#endif
    void log(severity, std::string const &) const;
    void log(severity, std::string const &, message_source_ptr) const;
    void log(severity, std::string &&) const;
    void log(severity, std::string &&, message_source_ptr) const;
    void log(message_with_severity &&) const;
    void log(message_with_severity &&, message_source_ptr) const;
    static char abbreviated(severity);
    static const char * to_string(severity);
    static severity from_string(const char *);
    static severity from_string(std::string const & s) {
        return from_string(s.c_str());
    }
private:
    class impl;
    std::unique_ptr<impl> m_data;
};

using init_logger_t = std::function<logger *()>;
init_logger_t & init_logger(); // singleton<logger> by default

inline logger* get_logger() {
    const auto & p = init_logger();
    return p ? p() : nullptr;
}

} // log
} // monik

#endif // __MONIK_LOG_LOGGER_H__

