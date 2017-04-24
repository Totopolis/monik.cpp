// monik_log.h
//
#pragma once
#ifndef __MONIK_LOG_MONIK_LOG_H__
#define __MONIK_LOG_MONIK_LOG_H__

#include "monik/log/remote_log.h"

#if MONIK_INCLUDE_AMQP

namespace monik { namespace log { 

class monik_log final : public channel { // thread safe
public:
    using Params = remote_log::Params;
    explicit monik_log(Params &&);
    ~monik_log() override;
    bool empty() const override;
    void log(message_with_severity &&) override; 
    void log(message_with_severity &&, message_source_ptr) override; 
#if MONIK_DEBUG
    void test();
#endif
private:
    std::string to_string(const message_with_severity &, const char * source) const;
    remote_log m_log;
};

} // log
} // monik

#endif // MONIK_INCLUDE_AMQP
#endif // __MONIK_LOG_MONIK_LOG_H__

