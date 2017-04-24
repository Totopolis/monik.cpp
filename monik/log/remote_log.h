// remote_log.h
//
#pragma once
#ifndef __MONIK_LOG_REMOTE_LOG_H__
#define __MONIK_LOG_REMOTE_LOG_H__

#include "monik/log/message.h"

#if MONIK_INCLUDE_AMQP
namespace sdl { namespace log { 

class remote_log final : public channel { // thread safe
public:
    struct Params final {
        std::string host;
        std::uint16_t port = 0;
        std::string user;
        std::string password;
        std::string exchange;
        std::string source;
        std::string instance;
        std::string tags;
        bool durable = false;
        buf_size_t bufsize = 0;
        second_t retrytimeout = 30;
    };
    explicit remote_log(Params &&);
    ~remote_log() override;
    bool empty() const override;
    void log(message_with_severity &&) override;
    const Params & params() const;
private:
    class data_type;
    const std::unique_ptr<data_type> m_data;
};

} // log
} // sdl

#endif // MONIK_INCLUDE_AMQP
#endif // __MONIK_LOG_REMOTE_LOG_H__

