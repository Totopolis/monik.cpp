// keepalive_log.cpp
//
#include "monik/log/keepalive_log.h"
#include "monik/log/timer_thread.h"
#include "monik/common/time_util.h"

#if MONIK_INCLUDE_AMQP

#if defined(MONIK_OS_WIN32) && defined(ERROR)
#undef ERROR // windows may conflict with monik.proto
#endif
#include "monik/log/protobuf/monik.pb.h"

namespace sdl { namespace log {

class keepalive_log::data_type : noncopyable {
    using this_error = sdl_exception_t<keepalive_log>;
    break_or_continue on_timer();
public:
    data_type(severity, second_t, Params &&);
    ~data_type();
    const severity m_severity;
    const second_t m_period;
    remote_log m_log;
    std::unique_ptr<timer_thread> m_thread;
};

keepalive_log::data_type::data_type(const severity s, const second_t period, Params && p)
    : m_severity(s)
    , m_period(period)
    , m_log(std::move(p))
{
    throw_error_if<this_error>(m_period.value() <= 0, "bad period");
    m_thread.reset(new timer_thread(std::launch::deferred, m_period,
        [this]() {
            return this->on_timer();
        }
    ));
    MONIK_ASSERT(!m_thread->running());
}

keepalive_log::data_type::~data_type()
{
    m_thread.reset();
}

break_or_continue keepalive_log::data_type::on_timer()
{
    tutorial::Event event;
    event.set_created(time_since_epoch_milliseconds());
    event.set_source(m_log.params().source);
    event.set_instance(m_log.params().instance);
    if (tutorial::KeepAlive * const ka = event.mutable_ka()) {
        ka->set_interval(static_cast<uint32>(m_period.value()));
        std::string text;
        if (event.SerializeToString(&text)) {
            m_log.log(message_with_severity(m_severity, text));
            return bc::continue_;
        }
    }
    MONIK_ASSERT(!"keepalive_log");
    return bc::break_;
}

//------------------------------------------------

keepalive_log::keepalive_log(severity s, second_t period, Params && p)
    : m_data(new data_type(s, period, std::move(p)))
{
    MONIK_TRACE_FUNCTION;
}

keepalive_log::~keepalive_log()
{
    MONIK_TRACE_FUNCTION;
}

bool keepalive_log::running() const
{
    return m_data->m_thread->running();
}

bool keepalive_log::launch()
{
    return m_data->m_thread->launch();
}

severity keepalive_log::get_severity() const
{
    return m_data->m_severity;
}

second_t keepalive_log::get_period() const
{
    return m_data->m_period;
}

} // log
} // sdl

#endif // MONIK_INCLUDE_AMQP

