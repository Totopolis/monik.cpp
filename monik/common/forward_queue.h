// forward_queue.h
//
#pragma once
#ifndef __SDL_LOG_COMMON_FORWARD_QUEUE_H__
#define __SDL_LOG_COMMON_FORWARD_QUEUE_H__

#include "monik/common/common.h"
#include <forward_list>

namespace sdl { namespace log {

template<class T>
class forward_queue : noncopyable { // movable
    using data_type = std::forward_list<T>;
public:
    using value_type = T;
    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;
    forward_queue() = default;
    forward_queue(forward_queue && src) noexcept 
        : m_data(std::move(src.m_data))
        , m_size(src.m_size)
        , m_last(src.m_last)
    {
        SDL_ASSERT(empty() || (m_last != m_data.end()));
        SDL_ASSERT(m_size == std::distance(m_data.begin(), m_data.end()));
    }
    forward_queue & operator=(forward_queue && src) noexcept {
        this->swap(src);
        return *this;
    }
    size_t size() const noexcept {
        return m_size;
    }
    bool empty() const noexcept {
        SDL_ASSERT((!m_size) == m_data.empty());
        return !m_size;
    }
    explicit operator bool() const noexcept {
        return !empty();
    }
    void clear() {
        forward_queue().swap(*this);
    }
    void swap(forward_queue & src) noexcept {
        m_data.swap(src.m_data);
        std::swap(m_size, src.m_size);
        std::swap(m_last, src.m_last);
    }
    value_type & front() {
        SDL_ASSERT(!empty());
        return * m_data.begin();
    }
    value_type const & front() const {
        SDL_ASSERT(!empty());
        return * m_data.begin();
    }
    value_type & back() {
        SDL_ASSERT(!empty());
        SDL_ASSERT(m_last != m_data.end());
        return * m_last;
    }
    value_type const & back() const {
        SDL_ASSERT(!empty());
        SDL_ASSERT(m_last != m_data.end());
        return * m_last;
    }
    iterator begin() {
        return m_data.begin();
    }
    iterator end() {
        return m_data.end();
    }
    const_iterator begin() const {
        return m_data.begin();
    }
    const_iterator end() const {
        return m_data.end();
    }
    void pop_front() {
        SDL_ASSERT(!empty());
        SDL_ASSERT(m_last != m_data.end());
        m_data.pop_front();
        if (!(--m_size)) {
            SDL_ASSERT(empty());
            m_last = m_data.end();
        }
    }
    template<class other_type> void push_front(other_type &&);
    template<class other_type> void push_back(other_type &&);
private:
    data_type m_data;
    size_t m_size = 0;
    iterator m_last;
};

template<class T>
template<class other_type>
void forward_queue<T>::push_front(other_type && v) {
    m_data.push_front(std::forward<other_type>(v));
    if (!m_size++) {
        SDL_ASSERT(1 == std::distance(m_data.begin(), m_data.end()));
        m_last = m_data.begin();
    }
}

template<class T>
template<class other_type>
void forward_queue<T>::push_back(other_type && v) {
    if (m_size++) { // not empty
        SDL_ASSERT(m_last != m_data.end());
        m_last = m_data.insert_after(m_last, std::forward<other_type>(v));
    }
    else {
        m_data.push_front(std::forward<other_type>(v));
        m_last = m_data.begin();
    }
}

} // log
} // sdl

#endif // __SDL_LOG_COMMON_FORWARD_QUEUE_H__

