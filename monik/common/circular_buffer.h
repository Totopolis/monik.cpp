// circular_buffer.h
//
#pragma once
#ifndef __MONIK_LOG_COMMON_CIRCULAR_BUFFER_H__
#define __MONIK_LOG_COMMON_CIRCULAR_BUFFER_H__

#include "monik/common/common.h"

namespace monik {

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
        MONIK_ASSERT(empty());
        MONIK_ASSERT(s <= capacity());
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
        MONIK_ASSERT(m_size);
        return m_data[m_index];
    }
    const T & front() const {
        MONIK_ASSERT(m_size);
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
            MONIK_ASSERT(!full());
            ++m_size;
        }
    }
    void pop_back() {
        MONIK_ASSERT(m_size);
        --m_size;
    }
    void pop_back(T & value) {
        MONIK_ASSERT(m_size);
        value = std::move(back());
        --m_size;
    }
    template<class fun_type>
    T * find_if(const size_t first, const size_t last, fun_type && fun) {
        MONIK_ASSERT(first <= m_size);
        MONIK_ASSERT(first <= last);
        size_t count = a_min(last - first, m_size - first);
        size_t i = m_index;
        increment(i, first);
        while (count--) {
            T & item = m_data[i];
            if (fun(item)) {
                return &item;
            }
            increment(i);
        }
        return nullptr;
    }
#if MONIK_DEBUG
    template<class fun_type>
    T * find_if(fun_type && fun) {
        return find_if(0, capacity(), std::forward<fun_type>(fun));
    }
#else
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
#endif
private:
    static void increment(size_t & i) {
        MONIK_ASSERT(i < N);
        if (++i == N) 
            i = 0;
    }
    static void increment(size_t & i, const size_t count) {
        MONIK_ASSERT(i < N);
        i += count;
        if (i >= N) {
            i %= N;
        }
    }
    static void decrement(size_t & i) {
        MONIK_ASSERT(i < N);
        if (i)
            --i;
        else
            i = N - 1;
    }
    size_t back_index() const {
        MONIK_ASSERT(m_size);
        size_t i = m_index;
        increment(i, m_size - 1);
        return i;
    }
};

} // monik

#endif // __MONIK_LOG_COMMON_CIRCULAR_BUFFER_H__
