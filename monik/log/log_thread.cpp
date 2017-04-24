// log_thread.cpp
//
#include "monik/log/log_thread.h"

namespace sdl { namespace log {

log_thread::log_thread(const std::launch policy,
        const buf_size_t max_size,
        const second_t retrytimeout,
        const overflow_policy op,
        const write_exception_policy wp,
        function_write && _write,
        function_overflow && _overflow)
    : m_buf(max_size, op)
    , m_shutdown(false)
    , m_ready(false)
    , m_running(false)
    , m_retrytimeout(retrytimeout)
    , m_write_callback(std::move(_write))
    , m_overflow_callback(std::move(_overflow))
    , m_write_policy(wp)
{
    MONIK_ASSERT((m_retrytimeout.value() > 0) || (m_write_policy == write_exception_policy::ignore));
    throw_error_if<log_thread_error>(!m_write_callback, "bad params");
    if (policy != std::launch::deferred) {
        launch();
    }
}

bool log_thread::launch()
{
    if (m_running) {
        MONIK_ASSERT(0);
        return false;
    }
    m_running = true;
    m_thread.reset(new joinable_thread([this](){
        this->worker_thread();
    }));
    return true;
}

log_thread::~log_thread()
{
    shutdown();
    m_thread.reset();
}

void log_thread::shutdown()
{
    MONIK_WARNING(!m_shutdown);
    m_shutdown = true;
    m_ready = true;
    m_cv.notify_one();
}

void log_thread::log(value_type && s)
{
    if (m_shutdown) {
        return;
    }
    if (m_buf.push_back(std::move(s))) {
        m_ready = true;
        m_cv.notify_one();
    }
    else if (m_overflow_callback != nullptr) {
        m_overflow_callback(std::move(s));
    }
}

void log_thread::worker_thread()
{
    MONIK_TRACE("log_thread id = ", std::this_thread::get_id());
    while (!m_shutdown) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this]{
                return m_ready.load();
            });
            MONIK_ASSERT(m_ready);
            m_ready = false;
        }
        if (auto data = m_buf.pop()) { // data can be empty
            const auto end = data.end();
            auto first = data.begin();
            for (; first != end; ++first) { 
                if (write_value(*first)) {
                    first->clear();
                }
                else {
                    MONIK_WARNING(!"write_value");
                    break;
                }
            }
            if (first != end) {
                if (m_write_policy == write_exception_policy::place_back) {
                    MONIK_TRACE("place_back = ", std::distance(first, end));
                    m_buf.append(std::move(data), first); // place data back to buffer
                    if (m_retrytimeout.value() > 0) {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        m_cv.wait_for(lock, std::chrono::seconds(m_retrytimeout.value()), [this]{
                            return m_ready.load();
                        });
                    }
                    else {
                        MONIK_ASSERT(0);
                    }
                    m_ready = true;
                    m_cv.notify_one();
                }
            }
        }
    }
}

bool log_thread::write_value(value_type const & value) const
{
    try {
        m_write_callback(value); // remote_log may throw !
    }
    catch (std::exception & e) { (void)e; 
        MONIK_TRACE("log_thread exception: ", e.what()); // possible case: hostname lookup failed
        return false;
    }
    return true;
}

} // log
} // sdl

#if MONIK_DEBUG
namespace sdl { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
        }
    };
    static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG
