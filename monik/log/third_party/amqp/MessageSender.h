// MessageSender.h
//
#pragma once
#ifndef __MONIK_LOG_AMQP_MESSAGESENDER_H__
#define __MONIK_LOG_AMQP_MESSAGESENDER_H__

#include "monik/log/third_party/amqp/amqp_config.h"

namespace AmqpClient {

class MessageSenderImpl;
class MessageSender final : noncopyable {
public:
    MessageSender(
        const std::string &host,
        uint16_t port,
        const std::string &user_name,
        const std::string &password,
        const std::string &exchange_name,
        bool durable);

    ~MessageSender();

    /*
    * throw: std::runtime_error
    */
    bool connect();

    /*
    * Must be called after connect().
    */
    void create_queue(const char *queue_name);

    /*
    * mime_type: can be nullptr or empty string
    * user_type: can be nullptr or empty string
    */
    void set_message_type(const char *mime_type, const char *user_type);

    /*
    * throw: std::runtime_error
    */
    void send(const void *data, size_t data_size, bool persistent);

private:
    std::unique_ptr<MessageSenderImpl> impl_;
};

} // AmqpClient

#endif // __MONIK_LOG_AMQP_MESSAGESENDER_H__

