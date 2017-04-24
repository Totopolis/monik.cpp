// MessageSender.cpp
//
#include "monik/log/third_party/amqp/MessageSender.h"
#include "monik/log/third_party/amqp/Channel.h"

namespace monik { namespace AmqpClient {

class MessageSenderImpl final : noncopyable {
public:
    MessageSenderImpl(
        const std::string &host,
        uint16_t port,
        const std::string &user_name,
        const std::string &password,
        const std::string &exchange_name,
        bool durable);

    bool connect();
    void create_queue(const char *queue_name);
    void set_message_type(const char *mime_type, const char *user_type);
    void send(const void *data, size_t data_size, bool persistent);
private:
    const std::string host_;
    const uint16_t port_;
    const std::string user_name_;
    const std::string password_;
    const std::string exchange_name_;
    const bool durable_;

    Channel::ptr_t channel_; // std::shared_ptr<Channel>
    amqp_basic_properties_t msg_props_;

    std::string mime_type_;
    std::string user_type_;
};

MessageSenderImpl::MessageSenderImpl(const std::string &host,
                                     const uint16_t port,
                                     const std::string &user_name,
                                     const std::string &password,
                                     const std::string &exchange_name,
                                     const bool durable)
    : host_(host)
    , port_(port)
    , user_name_(user_name)
    , password_(password)
    , exchange_name_(exchange_name)
    , durable_(durable)
{
    set_message_type(nullptr, nullptr);
}

void MessageSenderImpl::set_message_type(const char * const mime_type,
                                         const char * const user_type)
{
    msg_props_._flags = AMQP_BASIC_DELIVERY_MODE_FLAG;

    if (mime_type) {
        mime_type_ = mime_type;
    }
    else {
        mime_type_.clear();
    }
    if (user_type) {
        user_type_ = user_type;
    }
    else {
        user_type_.clear();
    }
    if (!mime_type_.empty()) {
        msg_props_._flags |= AMQP_BASIC_CONTENT_TYPE_FLAG;
        msg_props_.content_type.bytes = &mime_type_[0];
        msg_props_.content_type.len = mime_type_.size();
    }
    if (!user_type_.empty()) {
        msg_props_._flags |= AMQP_BASIC_TYPE_FLAG;
        msg_props_.type.bytes = &user_type_[0];
        msg_props_.type.len = user_type_.size();
    }
}

void MessageSenderImpl::send(const void * const data, const size_t data_size, const bool persistent)
{
    MONIK_ASSERT(!persistent || durable_);

    msg_props_.delivery_mode = persistent ? 2 : 1;

    amqp_bytes_t b = amqp_bytes_malloc(data_size);
    if (!b.bytes) {
        throw std::runtime_error("amqp_bytes_malloc");
    }
    memcpy(b.bytes, data, data_size);
    channel_->BasicPublish(exchange_name_,
        "",                                     // routing_key
        BasicMessage::Create(b, &msg_props_),   // b is managed by BasicMessage
        false,                                  // mandatory
        false);                                 // immediate
}

bool MessageSenderImpl::connect()
{
    channel_.reset(); // disconnect
    if (auto channel = Channel::Create(host_, port_, user_name_, password_)) {
        channel->DeclareExchange(
                    exchange_name_,
                    Channel::EXCHANGE_TYPE_FANOUT,
                    false,          // passive
                    durable_,       // durable
                    false           // auto_delete
                    );
        channel_.swap(channel);
        return true;
    }
    return false;
}

void MessageSenderImpl::create_queue(const char * const queue_name)
{
    MONIK_ASSERT(channel_);
    monik::sdl_throw_error_if(monik::is_str_empty(queue_name), "queue name is empty");

/*
 * See:
 * http://www.rabbitmq.com/amqp-0-9-1-reference.html
 */
    const std::string qname = channel_->DeclareQueue(queue_name,
                false,  // passive
/*
 * If set when creating a new queue, the queue will be marked as durable.
 * Durable queues remain active when a server restarts.
 * Non-durable queues (transient queues) are purged if/when a server restarts.
 * Note that durable queues do not necessarily hold persistent messages,
 * although it does not make sense to send persistent messages to a transient
 * queue.
 */
                durable_, // durable
/*
 * Exclusive queues may only be accessed by the current connection,
 * and are deleted when that connection closes.
 * Passive declaration of an exclusive queue by other connections
 * are not allowed.
 */
                false,  // exclusive
/*
 * If set, the queue is deleted when all consumers have finished using it.
 * The last consumer can be cancelled either explicitly or because its channel
 * is closed. If there was no consumer ever on the queue, it won't be deleted.
 * Applications can explicitly delete auto-delete queues using the Delete
 * method as normal.
 */
                false   // auto_delete
                );
    MONIK_ASSERT(qname == queue_name);
    channel_->BindQueue(qname, exchange_name_);
}

//------------------------------------------------------------------------

MessageSender::MessageSender(
    const std::string &host,
    const uint16_t port,
    const std::string &user_name,
    const std::string &password,
    const std::string &exchange_name,
    const bool durable)
    : impl_(new MessageSenderImpl(
        host, port, user_name, password, exchange_name, durable))
{
}

MessageSender::~MessageSender()
{
}

void MessageSender::set_message_type(const char * const mime_type, const char * const user_type)
{
    impl_->set_message_type(mime_type, user_type);
}

void MessageSender::send(const void * const data, const size_t data_size, const bool persistent)
{
    impl_->send(data, data_size, persistent);
}

bool MessageSender::connect()
{
    return impl_->connect();
}

void MessageSender::create_queue(const char * const queue_name)
{
    impl_->create_queue(queue_name);
}

}} // AmqpClient
