// timer_thread.cpp
//
#include "monik/log/timer_thread.h"

namespace monik { namespace log {

timer_thread::timer_thread(
        const std::launch policy, 
        const second_t period,
        function_timer && fun)
    : m_shutdown(false)
    , m_running(false)
    , m_period(period.value())
    , m_timer(std::move(fun))
{
    MONIK_TRACE_FUNCTION;
    throw_error_if<this_error>(!m_timer, "bad params");
    if (policy != std::launch::deferred) {
        launch();
    }
}

bool timer_thread::launch()
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

timer_thread::~timer_thread()
{
    MONIK_TRACE_FUNCTION;
    shutdown();
    m_thread.reset();
}

void timer_thread::shutdown()
{
    MONIK_WARNING(!m_shutdown);
    m_shutdown = true;
    m_cv.notify_one();
}

void timer_thread::worker_thread()
{
    MONIK_TRACE("timer_thread id = ", std::this_thread::get_id());
    while (!m_shutdown) {
        {
            const int period = m_period;
            if (period > 0) {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cv.wait_for(lock, std::chrono::seconds(period), [this]{
                    return m_shutdown.load();
                });
            }
        }
        if (m_shutdown) {
            MONIK_TRACE(__FUNCTION__, " shutdown");
            break;
        }
        try {
            if (is_break(m_timer())) {
                break;
            }
        }
        catch (std::exception & e) {
            MONIK_TRACE("timer_thread exception: ", e.what()); (void)e;
            MONIK_WARNING(0); // some data may be lost
        }
    }
}

} // log
} // monik

#if MONIK_DEBUG
namespace monik { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
            if (0) {
                static std::unique_ptr<timer_thread> test;
                static int count = 0;
                enum { max_count = 10 };
                test = std::make_unique<timer_thread>(std::launch::async, 1, [](){
                        MONIK_TRACE("count = ", count);
                        return (++count < max_count) ? bc::continue_ : bc::break_;
                });
                while (count < max_count) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                MONIK_TRACE_FUNCTION;
            }
        }
    };
    static unit_test s_test;

}}} // monik
#endif //#if SV_DEBUG
