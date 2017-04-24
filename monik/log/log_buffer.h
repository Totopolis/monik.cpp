// log_buffer.h
//
#pragma once
#ifndef __SDL_LOG_LOG_BUFFER_H__
#define __SDL_LOG_LOG_BUFFER_H__

#include "monik/common/forward_queue.h"
#include "monik/log/message.h"

namespace sdl { namespace log { 

class log_buffer : noncopyable { // thread safe
public:
    enum class overflow_policy {
        refuse,
        pop_front,
    };
    enum class push_t {
        false_ = 0,
        true_,
        true_pop_front
    };
    static const size_t default_capacity = megabyte<1>::value; // 1048576 
    using value_type = message_with_severity;
    using container_type = forward_queue<value_type>;
public:
    explicit log_buffer(buf_size_t, overflow_policy = overflow_policy::refuse);
    size_t capacity() const {
        return m_capacity;
    }
    bool empty() const;
    bool full() const;
    container_type pop();
    push_t push_back_ex(value_type &&);
    bool push_back(value_type &&);
    size_t append(container_type &&, container_type::iterator);
private:
    push_t push_back_nolock(value_type &&);
private:
    const size_t m_capacity = 0;
    const overflow_policy m_policy;
    mutable std::mutex m_mutex;
    container_type m_messages;
    size_t m_messages_size = 0;
};

} // log
} // sdl

#endif // __SDL_LOG_LOG_BUFFER_H__

