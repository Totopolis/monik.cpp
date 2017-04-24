// remote_log.cpp
//
#include "monik/log/remote_log.h"
#include "monik/log/log_thread.h"

#if MONIK_INCLUDE_AMQP
#include "monik/log/third_party/amqp/MessageSender.h"

namespace sdl { namespace log {

#if MONIK_DEBUG
void traceParams(remote_log::Params const & p) {
    MONIK_TRACE("host = ", p.host);
    MONIK_TRACE("port = ", p.port);
    MONIK_TRACE("user = ", p.user);
    MONIK_TRACE("password = ", p.password);
    MONIK_TRACE("exchange = ", p.exchange);
    MONIK_TRACE("source = ", p.source);
    MONIK_TRACE("instance = ", p.instance);
    MONIK_TRACE("tags = ", p.tags);
    MONIK_TRACE("durable = ", p.durable);
    MONIK_TRACE("bufsize = ", p.bufsize);
    MONIK_TRACE("retrytimeout = ", p.retrytimeout);
}
#endif

class remote_log::data_type final : noncopyable {
    using remote_log_error = sdl_exception_t<remote_log>;
    using MessageSender = AmqpClient::MessageSender;
public:
    explicit data_type(Params &&);
    ~data_type() {}
    bool empty() const {
        return m_thread->empty();
    }
    void log(message_with_severity && s) {
        m_thread->log(std::move(s));
    }
    const Params & params() const {
        return m_params;
    }
private:
    void write(const message_with_severity & s);      // called in thread
    void overflow(const message_with_severity & s);   // called in thread
    bool connect();
    bool try_connect();
private:
    const Params m_params;
    std::unique_ptr<log_thread> m_thread;
    std::unique_ptr<MessageSender> m_sender;
};

remote_log::data_type::data_type(Params && p)
    : m_params(std::move(p))
{
#if MONIK_DEBUG
    traceParams(m_params);
#endif
    m_thread.reset(new log_thread(std::launch::async, 
        m_params.bufsize, m_params.retrytimeout,
        log_thread::overflow_policy::pop_front,
        log_thread::write_exception_policy::place_back,
        [this](message_with_severity const & s) {
            this->write(s);
        },
        [this](message_with_severity const & s) {
            this->overflow(s);
        }
    ));
    MONIK_ASSERT(m_thread->running());
}

bool remote_log::data_type::connect()
{
    if (m_sender) {
        return true;
    }
    try {
        return try_connect();
    }
    catch (std::exception & e) {
        MONIK_TRACE("remote_log exception: ", e.what()); (void)e;
        MONIK_WARNING(0); // some data may be lost
        throw;
    }
}

bool remote_log::data_type::try_connect()
{
    auto const & p = m_params;
    auto sender = std::make_unique<MessageSender>(
        p.host,
        p.port,
        p.user,
        p.password,
        p.exchange,
        p.durable
    );
    sender->set_message_type("application/octet-stream", nullptr);
    if (sender->connect()) {
        m_sender.swap(sender);
        return true;
    }
    MONIK_WARNING(!"connect");
    return false;
}

void remote_log::data_type::write(const message_with_severity & s)
{
    MONIK_ASSERT(!s.m_message.empty());
    if (connect()) {
        const bool persistent = (s.m_severity > severity::debug);
        m_sender->send(s.m_message.c_str(), s.m_message.size(), 
           persistent);
    }
}

void remote_log::data_type::overflow(const message_with_severity & s)
{
    MONIK_TRACE(s.m_message);
    MONIK_ASSERT(!"overflow");
}

remote_log::remote_log(Params && p)
    : m_data(new data_type(std::move(p)))
{
}

remote_log::~remote_log()
{
}

bool remote_log::empty() const
{
    return m_data->empty();
}

void remote_log::log(message_with_severity && s)
{
    m_data->log(std::move(s));
}

const remote_log::Params &
remote_log::params() const
{
    return m_data->params();
}

} // log
} // sdl

#endif // MONIK_INCLUDE_AMQP
