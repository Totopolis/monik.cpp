// timer_thread.h
//
#pragma once
#ifndef __MONIK_LOG_TIMER_THREAD_H__
#define __MONIK_LOG_TIMER_THREAD_H__

#include "monik/log/message.h"
#include "monik/log/thread.h"

namespace sdl { namespace log { 

class timer_thread : noncopyable {
    using this_error = sdl_exception_t<timer_thread>;
    using function_timer = std::function<break_or_continue()>;
public:
    timer_thread(std::launch, second_t, function_timer &&);
    virtual ~timer_thread();
    bool running() const {
        return m_running;
    }
    bool launch();
    void shutdown();
    second_t period() const;
    void period(second_t); // set period
private:
    void worker_thread();
    std::atomic_bool m_shutdown;
    std::atomic_bool m_ready;
    std::atomic_bool m_running;
    std::atomic<int> m_period; // in seconds
    const function_timer m_timer;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::unique_ptr<joinable_thread> m_thread;
};

inline second_t timer_thread::period() const {
    return second_t(m_period.load());
}

inline void timer_thread::period(second_t s) {
    m_period = s.value();
}

} // log
} // sdl

#endif // __MONIK_LOG_TIMER_THREAD_H__

