// keepalive_log.h
//
#pragma once
#ifndef __SDL_LOG_KEEPALIVE_LOG_H__
#define __SDL_LOG_KEEPALIVE_LOG_H__

#include "monik/log/remote_log.h"

#if SDL_INCLUDE_AMQP

namespace sdl { namespace log { 

class keepalive_log final : noncopyable {
public:
    using Params = remote_log::Params;
    keepalive_log(severity, second_t, Params &&);
    ~keepalive_log();
    bool running() const;
    bool launch();
    severity get_severity() const;
    second_t get_period() const;
private:
    class data_type;
    const std::unique_ptr<data_type> m_data;
};

using shared_keepalive = std::shared_ptr<keepalive_log>;

} // log
} // sdl

#endif // SDL_INCLUDE_AMQP
#endif // __SDL_LOG_KEEPALIVE_LOG_H__

