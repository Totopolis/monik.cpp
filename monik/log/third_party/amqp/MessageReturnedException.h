// MessageReturnedException.h
//
#pragma once
#ifndef SIMPLEAMQPCLIENT_MESSAGERETURNEDEXCEPTION_H
#define SIMPLEAMQPCLIENT_MESSAGERETURNEDEXCEPTION_H

#include "dataserver/log/third_party/amqp/BasicMessage.h"

namespace AmqpClient {

class MessageReturnedException : public std::runtime_error {
 public:
  explicit MessageReturnedException(BasicMessage::ptr_t message,
                                    std::uint32_t reply_code,
                                    const std::string &reply_text,
                                    const std::string &exchange,
                                    const std::string &routing_key) AMQP_NOEXCEPT;

  virtual ~MessageReturnedException() AMQP_NOEXCEPT {}

  BasicMessage::ptr_t message() const AMQP_NOEXCEPT { return m_message; }
  std::uint32_t reply_code() const AMQP_NOEXCEPT { return m_reply_code; }
  std::string reply_text() const AMQP_NOEXCEPT { return m_reply_text; }
  std::string exchange() const AMQP_NOEXCEPT { return m_exchange; }
  std::string routing_key() const AMQP_NOEXCEPT { return m_routing_key; }

 private:
  BasicMessage::ptr_t m_message;
  std::uint32_t m_reply_code;
  std::string m_reply_text;
  std::string m_exchange;
  std::string m_routing_key;
  std::string m_what;
};

}  // namespace AmqpClient

#endif  // SIMPLEAMQPCLIENT_MESSAGERETURNEDEXCEPTION_H
