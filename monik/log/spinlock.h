// spinlock.h
//
#pragma once
#ifndef __SDL_LOG_SPINLOCK_H__
#define __SDL_LOG_SPINLOCK_H__

#include "monik/common/common.h"
#include <thread>

namespace sdl { namespace log {

template<typename T, bool one_writer>
class writelock_t: noncopyable { // spinlock
public:
    using value_type = T;
    using atomic_counter = std::atomic<value_type>;
    static const value_type locked_0 = 0;
private:
    atomic_counter volatile & counter;
    value_type old = 0;

    void yield(size_t) {
        std::this_thread::yield();
    }
    void lock() {
        size_t i = 0;
        while (!(old = counter.exchange(locked_0))) { // try lock
            SDL_ASSERT(!one_writer);
            yield(++i);
        }
        SDL_ASSERT(old);
    }
    void unlock() {
        if (!(++old)) ++old;
        counter = old; // atomic store
    }
public:
    explicit writelock_t(atomic_counter volatile & c)
        : counter(c)
    {
        lock();
    }
    ~writelock_t(){
        unlock();
    }
};

template<typename T>
class readlock_t: noncopyable {
public:
    using value_type = T;
    using atomic_counter = std::atomic<value_type>;
private:
    atomic_counter volatile & counter;
    value_type old = 0;

    void yield(size_t) {
        std::this_thread::yield();
    }
public:
    explicit readlock_t(atomic_counter volatile & c)
        : counter(c)
    {}
    bool try_lock() {
        return (old = counter) != 0;  // atomic load
    }
    bool commit() {
        return old && (old == counter); // atomic load
    }
    template<class fun_type>
    void work(fun_type && fun) {
        size_t i = 0;
        for (;;) {
            if (try_lock()) {
                fun();
                if (commit())
                    return;
            }
            yield(++i);
        }
    }
    template<class result_type, class fun_type>
    void work(result_type & result, fun_type && fun) {
        size_t i = 0;
        for (;;) {
            if (try_lock()) {
                fun(result);
                if (commit())
                    return;
            }
            yield(++i);
        }
    }
};

using writelock = writelock_t<uint32, false>;
using readlock = readlock_t<uint32>;

} // log
} // sdl

#endif // __SDL_LOG_SPINLOCK_H__