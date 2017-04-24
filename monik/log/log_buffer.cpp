// log_buffer.cpp
//
#include "monik/log/log_buffer.h"

namespace sdl { namespace log {

log_buffer::log_buffer(const buf_size_t max_size, const overflow_policy p)
    : m_capacity(max_size.value() > 0 ? max_size.value() : default_capacity)
    , m_policy(p)
{
    SDL_ASSERT(m_capacity);
}

bool log_buffer::empty() const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_messages.empty();
}

bool log_buffer::full() const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_messages_size >= m_capacity;
}

log_buffer::container_type
log_buffer::pop()
{
    container_type result;
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_messages.swap(result);
        m_messages_size = 0;
    }
    return result;
}

bool log_buffer::push_back(value_type && v)
{
    return push_back_ex(std::move(v)) != push_t::false_;
}

log_buffer::push_t
log_buffer::push_back_ex(value_type && s)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return push_back_nolock(std::move(s));
}

log_buffer::push_t
log_buffer::push_back_nolock(value_type && s)
{
    SDL_ASSERT(!s.m_message.empty());
    if (m_messages_size < m_capacity) {
        m_messages.push_back(std::move(s));
        m_messages_size += s.mem_size();
        return push_t::true_;
    }
    else if (m_policy == overflow_policy::pop_front) { // free space for new value
        const size_t mem_size = s.mem_size();
        size_t free_size = 0;
        while ((free_size < mem_size) && !m_messages.empty()) {
            free_size += m_messages.front().mem_size();
            m_messages.pop_front();
        }
        if (free_size <= m_messages_size) {
            m_messages_size -= free_size;
        }
        else {
            SDL_ASSERT(0);
            m_messages.clear();
            m_messages_size = 0;
        }
        m_messages.push_back(std::move(s));
        m_messages_size += mem_size;
        return push_t::true_pop_front;
    }
    return push_t::false_;
}

size_t log_buffer::append(container_type && data, container_type::iterator first)
{
    SDL_ASSERT(!data.empty());
    SDL_ASSERT(first != data.end());
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_messages.empty() && (first == data.begin())) {
        SDL_ASSERT(!m_messages_size);
        m_messages.swap(data);
        m_messages_size = 0;
        for (const auto & s : m_messages) {
            m_messages_size += s.mem_size();
        }
        SDL_ASSERT(m_messages_size);
        return m_messages.size();
    }
    else {
        size_t count = 0;
        const auto end = data.end();
        for (; first != end; ++first) {
            if (push_back_nolock(std::move(*first)) == push_t::false_) {
                SDL_ASSERT(0);
                break;
            }
            ++count;
        }
        data.clear();
        return count;
    }
}

} // log
} // sdl

#if SDL_DEBUG
namespace sdl { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
            if (1) {
                log_buffer buf(megabyte<1>::value);
                SDL_ASSERT(buf.empty());
                size_t count = 0;
                while (buf.push_back(message_with_severity(severity::trace, __FUNCTION__))) {
                    ++count;
                }
                SDL_ASSERT(buf.full());
                auto data = buf.pop();
                SDL_ASSERT(data.size() == count);
                SDL_ASSERT(buf.empty());
            }
            if (1) {
                message_with_severity s1(severity::trace, std::string(__FUNCTION__));
                message_with_severity s2 = std::move(s1);
                SDL_ASSERT(!s2.m_message.empty());
                SDL_ASSERT(s1.m_message.empty());
            }
            if (1) {
                log_buffer buf(megabyte<1>::value, log_buffer::overflow_policy::pop_front);
                while (true) {
                    message_with_severity msg(severity::trace, __FUNCTION__);
                    const auto result = buf.push_back_ex(std::move(msg));
                    if (result == log_buffer::push_t::true_pop_front) {
                        break;
                    }
                    SDL_ASSERT(result == log_buffer::push_t::true_);
                }
            }
        }
    };
    static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG
