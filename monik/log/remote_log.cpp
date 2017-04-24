// remote_log.cpp
//
#include "monik/log/remote_log.h"
#include "monik/log/log_thread.h"

#if SDL_INCLUDE_AMQP
#include "monik/log/third_party/amqp/MessageSender.h"

namespace sdl { namespace log {

#if SDL_DEBUG
void traceParams(remote_log::Params const & p) {
    SDL_TRACE("host = ", p.host);
    SDL_TRACE("port = ", p.port);
    SDL_TRACE("user = ", p.user);
    SDL_TRACE("password = ", p.password);
    SDL_TRACE("exchange = ", p.exchange);
    SDL_TRACE("source = ", p.source);
    SDL_TRACE("instance = ", p.instance);
    SDL_TRACE("tags = ", p.tags);
    SDL_TRACE("durable = ", p.durable);
    SDL_TRACE("bufsize = ", p.bufsize);
    SDL_TRACE("retrytimeout = ", p.retrytimeout);
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
#if SDL_DEBUG
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
    SDL_ASSERT(m_thread->running());
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
        SDL_TRACE("remote_log exception: ", e.what()); (void)e;
        SDL_WARNING(0); // some data may be lost
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
    SDL_WARNING(!"connect");
    return false;
}

void remote_log::data_type::write(const message_with_severity & s)
{
    SDL_ASSERT(!s.m_message.empty());
    if (connect()) {
        const bool persistent = (s.m_severity > severity::debug);
        m_sender->send(s.m_message.c_str(), s.m_message.size(), 
           persistent);
    }
}

void remote_log::data_type::overflow(const message_with_severity & s)
{
    SDL_TRACE(s.m_message);
    SDL_ASSERT(!"overflow");
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

#endif // SDL_INCLUDE_AMQP
