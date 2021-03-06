// message.h
//
#pragma once
#ifndef __MONIK_LOG_MESSAGE_H__
#define __MONIK_LOG_MESSAGE_H__

#include "monik/common/common.h"
#include "monik/common/array_enum.h"

namespace monik { namespace log {

enum class severity : uint8 {
    verbose = 0,
    info,           // 1
    warning,        // 2
    error,          // 3
    fatal,          // 4
    //
    _end,
};

inline std::initializer_list<severity> severity_all() {
    static const std::initializer_list<severity> all {
        severity::verbose,     // 0
        severity::info,        // 1
        severity::warning,     // 2
        severity::error,       // 3
        severity::fatal        // 4
    };
    return all;
}

inline constexpr size_t severity_size() {
    return static_cast<size_t>(severity::_end);
}

template<typename T>
using array_severity = array_enum<T, severity_size(), severity>;

class message_with_severity final {
public:
    std::string m_message;
    severity m_severity;
    explicit message_with_severity(severity l) noexcept : m_severity(l) {}
    template<class T>
    message_with_severity(severity l, T && m)
        : m_message(std::forward<T>(m))
        , m_severity(l) 
    {}
    message_with_severity(message_with_severity && src) noexcept
        : m_message(std::move(src.m_message))
        , m_severity(src.m_severity) 
    {}
    message_with_severity(message_with_severity const & src)
        : m_message(src.m_message)
        , m_severity(src.m_severity) 
    {}
#if 0 //MONIK_DEBUG
    void trace() const {
        std::cout << m_message << std::endl;
    }
#endif
    size_t empty() const {
        return m_message.empty();
    }
    size_t mem_size() const {
        return sizeof(*this) + m_message.size() + 1; // + 1 for tail zero
    }
    void clear() {
        m_message.clear();
    }
};

using message_source_ptr = const char * const;

struct monik_source : is_static {
    static message_source_ptr system;
    static message_source_ptr application;
    static message_source_ptr logic;
    static message_source_ptr security;
};

class channel {
public:
    virtual ~channel(){}
    virtual bool empty() const = 0;
    virtual void log(message_with_severity &&) = 0;
    virtual void log(message_with_severity && s, message_source_ptr){
        this->log(std::move(s));
    } 
};

using shared_channel = std::shared_ptr<channel>;

namespace unit {
    struct buf_size{};
    struct log_size{};
    struct second{};
}
typedef quantity<unit::buf_size, size_t> buf_size_t;
typedef quantity<unit::log_size, size_t> log_size_t;
typedef quantity<unit::second, int> second_t;

} // log
} // monik

namespace monik {
MONIK_QUANTITY_ALLOW_DEFAULT_CTOR(log::buf_size_t);
MONIK_QUANTITY_ALLOW_DEFAULT_CTOR(log::log_size_t);
MONIK_QUANTITY_ALLOW_DEFAULT_CTOR(log::second_t);
}

#endif // __MONIK_LOG_MESSAGE_H__

