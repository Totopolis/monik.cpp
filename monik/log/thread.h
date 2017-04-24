// thread.h
//
#pragma once
#ifndef __MONIK_LOG_THREAD_H__
#define __MONIK_LOG_THREAD_H__

#include "monik/common/common.h"
#include <thread>
#include <future>
#include <condition_variable>

namespace sdl { namespace log {

class joinable_thread : noncopyable {
    std::thread m_thread;
public:
    template<typename... Ts>
    explicit joinable_thread(Ts&&... params)
        : m_thread(std::forward<Ts>(params)...)
    {}
    ~joinable_thread() {
        m_thread.join();
    }
};

class joinable_thread_ex : noncopyable {
    std::function<void()> m_exit;
    std::thread m_thread;
public:
    template<class on_exit, typename... Ts>
    joinable_thread_ex(on_exit fun, Ts&&... params)
        : m_exit(fun)
        , m_thread(std::forward<Ts>(params)...)
    {}
    ~joinable_thread_ex() {
        m_exit();
        m_thread.join();
    }
};

inline void sleep_for_milliseconds(size_t v) {
    std::this_thread::sleep_for(std::chrono::milliseconds(v));
}

template<class T> 
class scoped_shutdown : noncopyable {
    T & m_obj;
    std::future<void> m_fut; // wait for thread to join in std::future desructors
public:
    scoped_shutdown(T & obj, std::future<void> && f)
        : m_obj(obj), m_fut(std::move(f))
    {}
    ~scoped_shutdown() {
        m_obj.shutdown();
    }
    void get() { // can be used to process std::future exception
        m_fut.get();
    }
};

} // log
} // sdl

#endif // __MONIK_LOG_THREAD_H__
