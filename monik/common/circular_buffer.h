// circular_buffer.h
//
#pragma once
#ifndef __SDL_LOG_COMMON_CIRCULAR_BUFFER_H__
#define __SDL_LOG_COMMON_CIRCULAR_BUFFER_H__

#include "monik/common/common.h"

namespace sdl { namespace log {

template<class T, size_t N>
class circular_buffer {
    using data_type = std::array<T, N>;
    data_type m_data;
    size_t m_index = 0; // front position
    size_t m_size = 0;
public:
    typedef T value_type;
    static constexpr size_t capacity() { return N; }

    circular_buffer() noexcept {
        static_assert(N, "");
    }
    void resize(const size_t s) { // init size
        SDL_ASSERT(empty());
        SDL_ASSERT(s <= capacity());
        m_size = s;
    }
    bool full() const noexcept {
        return N == m_size;
    }
    bool empty() const noexcept {
        return 0 == m_size;
    }
    size_t size() const noexcept {
        return m_size;
    }
    void clear() {
        m_size = 0;
    }
    T & front() {
        SDL_ASSERT(m_size);
        return m_data[m_index];
    }
    const T & front() const {
        SDL_ASSERT(m_size);
        return m_data[m_index];
    }
    T & back() {
        return m_data[back_index()];
    }
    const T & back() const {
        return m_data[back_index()];
    }
    void push_front(T && value) {
        decrement(m_index);
        m_data[m_index] = std::move(value);
        if (m_size < N) {
            SDL_ASSERT(!full());
            ++m_size;
        }
    }
    void pop_back() {
        SDL_ASSERT(m_size);
        --m_size;
    }
    void pop_back(T & value) {
        SDL_ASSERT(m_size);
        value = std::move(back());
        --m_size;
    }
    template<class fun_type>
    void for_each(size_t count, fun_type && fun) {
        SDL_ASSERT(count <= m_size);
        if (count > m_size)
            count = m_size;
        size_t i = m_index;
        while (count--) {
            fun(m_data[i]);
            increment(i);
        }
    }
    template<class fun_type>
    T * find_if(fun_type && fun) {
        size_t count = m_size;
        size_t i = m_index;
        while (count--) {
            T & item = m_data[i];
            if (fun(item)) {
                return &item;
            }
            increment(i);
        }
        return nullptr;
    }
private:
    static void increment(size_t & i) {
        SDL_ASSERT(i < N);
        if (++i == N) 
            i = 0;
    }
    static void increment(size_t & i, const size_t count) {
        SDL_ASSERT(i < N);
        i += count;
        if (i >= N) {
            i %= N;
        }
    }
    static void decrement(size_t & i) {
        SDL_ASSERT(i < N);
        if (i)
            --i;
        else
            i = N - 1;
    }
    size_t back_index() const {
        SDL_ASSERT(m_size);
        size_t i = m_index;
        increment(i, m_size - 1);
        return i;
    }
};

} // log
} // sdl

#endif // __SDL_LOG_COMMON_CIRCULAR_BUFFER_H__
