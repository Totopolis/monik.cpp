// MessageReturnedException.cpp
//
#include "dataserver/log/third_party/amqp/MessageReturnedException.h"

namespace AmqpClient {
MessageReturnedException::MessageReturnedException(
    BasicMessage::ptr_t message, std::uint32_t reply_code,
    const std::string &reply_text, const std::string &exchange,
    const std::string &routing_key) AMQP_NOEXCEPT
    : std::runtime_error(
          std::string("Message returned. Reply code: ")
              .append(std::to_string(reply_code))
              .append(" ")
              .append(reply_text)),
      m_message(message),
      m_reply_code(reply_code),
      m_reply_text(reply_text),
      m_exchange(exchange),
      m_routing_key(routing_key) {}

}  // namespace AmqpClient
