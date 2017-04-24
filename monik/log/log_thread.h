// log_thread.h
//
#pragma once
#ifndef __MONIK_LOG_LOG_THREAD_H__
#define __MONIK_LOG_LOG_THREAD_H__

#include "monik/log/message.h"
#include "monik/log/log_buffer.h"
#include "monik/log/thread.h"

namespace sdl { namespace log { 

class log_thread : noncopyable {
    using value_type = message_with_severity;
    using log_thread_error = sdl_exception_t<log_thread>;
    using function_write = std::function<void(value_type const &)>;
    using function_overflow = std::function<void(value_type const &)>;
public:
    enum class write_exception_policy {
        ignore,
        place_back
    };
    using overflow_policy = log_buffer::overflow_policy;
    log_thread(std::launch, 
        buf_size_t, second_t retrytimeout,
        overflow_policy,
        write_exception_policy,
        function_write && _write,
        function_overflow && _overflow);
    virtual ~log_thread();
    size_t capacity() const {
        return m_buf.capacity();
    }
    bool running() const {
        return m_running;
    }
    bool empty() const {
        return m_buf.empty();
    }
    void log(value_type &&);
    bool launch();
    void shutdown();
private:
    void worker_thread();
    bool write_value(value_type const &) const;
private:
    log_buffer m_buf;
    std::atomic_bool m_shutdown;
    std::atomic_bool m_ready;
    std::atomic_bool m_running;
    const second_t m_retrytimeout;
    const function_write m_write_callback;
    const function_overflow m_overflow_callback;
    const write_exception_policy m_write_policy;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::unique_ptr<joinable_thread> m_thread;
};

} // log
} // sdl

#endif // __MONIK_LOG_LOG_THREAD_H__

