// AmqpException.h
//
#pragma once
#ifndef SIMPLEAMQPCLIENT_AMQPEXCEPTION_H
#define SIMPLEAMQPCLIENT_AMQPEXCEPTION_H

#include "dataserver/log/third_party/amqp/amqp_config.h"

struct amqp_rpc_reply_t_;
struct amqp_channel_close_t_;
struct amqp_connection_close_t_;

namespace AmqpClient {

class AmqpException : public std::runtime_error {
 public:
  static void Throw(const amqp_rpc_reply_t_ &reply);
  static void Throw(const amqp_channel_close_t_ &reply);
  static void Throw(const amqp_connection_close_t_ &reply);

  explicit AmqpException(const std::string &what,
                         const std::string &reply_text,
                         std::uint16_t class_id,
                         std::uint16_t method_id) AMQP_NOEXCEPT;
  virtual ~AmqpException() AMQP_NOEXCEPT {}

  virtual bool is_soft_error() const AMQP_NOEXCEPT = 0;
  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT = 0;
  virtual std::uint16_t class_id() const AMQP_NOEXCEPT { return m_class_id; }
  virtual std::uint16_t method_id() const AMQP_NOEXCEPT { return m_method_id; }
  virtual std::string reply_text() const { return m_reply_text; }

 protected:
  std::string m_reply_text;
  std::uint16_t m_class_id;
  std::uint16_t m_method_id;
};

class ConnectionException : public AmqpException {
 public:
  explicit ConnectionException(const std::string &what,
                               const std::string &reply_text,
                               std::uint16_t class_id,
                               std::uint16_t method_id) AMQP_NOEXCEPT
      : AmqpException(what, reply_text, class_id, method_id) {}

  virtual bool is_soft_error() const AMQP_NOEXCEPT { return false; }
};

class ChannelException : public AmqpException {
 public:
  explicit ChannelException(const std::string &what,
                            const std::string &reply_text,
                            std::uint16_t class_id,
                            std::uint16_t method_id) AMQP_NOEXCEPT
      : AmqpException(what, reply_text, class_id, method_id) {}

  virtual bool is_soft_error() const AMQP_NOEXCEPT { return true; }
};

class ConnectionForcedException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit ConnectionForcedException(const std::string &what,
                                     const std::string &reply_text,
                                     std::uint16_t class_id,
                                     std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class InvalidPathException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit InvalidPathException(const std::string &what,
                                const std::string &reply_text,
                                std::uint16_t class_id,
                                std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class FrameErrorException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit FrameErrorException(const std::string &what,
                               const std::string &reply_text,
                               std::uint16_t class_id,
                               std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class SyntaxErrorException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit SyntaxErrorException(const std::string &what,
                                const std::string &reply_text,
                                std::uint16_t class_id,
                                std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class CommandInvalidException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit CommandInvalidException(const std::string &what,
                                   const std::string &reply_text,
                                   std::uint16_t class_id,
                                   std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class ChannelErrorException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit ChannelErrorException(const std::string &what,
                                 const std::string &reply_text,
                                 std::uint16_t class_id,
                                 std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class UnexpectedFrameException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit UnexpectedFrameException(const std::string &what,
                                    const std::string &reply_text,
                                    std::uint16_t class_id,
                                    std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class ResourceErrorException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit ResourceErrorException(const std::string &what,
                                  const std::string &reply_text,
                                  std::uint16_t class_id,
                                  std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class NotAllowedException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit NotAllowedException(const std::string &what,
                               const std::string &reply_text,
                               std::uint16_t class_id,
                               std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class NotImplementedException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit NotImplementedException(const std::string &what,
                                   const std::string &reply_text,
                                   std::uint16_t class_id,
                                   std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class InternalErrorException : public ConnectionException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit InternalErrorException(const std::string &what,
                                  const std::string &reply_text,
                                  std::uint16_t class_id,
                                  std::uint16_t method_id) AMQP_NOEXCEPT
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class ContentTooLargeException : public ChannelException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit ContentTooLargeException(const std::string &what,
                                    const std::string reply_text,
                                    std::uint16_t class_id,
                                    std::uint16_t method_id) AMQP_NOEXCEPT
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class NoRouteException : public ChannelException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit NoRouteException(const std::string &what,
                            const std::string reply_text,
                            std::uint16_t class_id,
                            std::uint16_t method_id) AMQP_NOEXCEPT
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};
class NoConsumersException : public ChannelException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit NoConsumersException(const std::string &what,
                                const std::string reply_text,
                                std::uint16_t class_id,
                                std::uint16_t method_id) AMQP_NOEXCEPT
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class AccessRefusedException : public ChannelException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit AccessRefusedException(const std::string &what,
                                  const std::string reply_text,
                                  std::uint16_t class_id,
                                  std::uint16_t method_id) AMQP_NOEXCEPT
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class NotFoundException : public ChannelException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit NotFoundException(const std::string &what,
                             const std::string reply_text,
                             std::uint16_t class_id,
                             std::uint16_t method_id) AMQP_NOEXCEPT
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class ResourceLockedException : public ChannelException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit ResourceLockedException(const std::string &what,
                                   const std::string reply_text,
                                   std::uint16_t class_id,
                                   std::uint16_t method_id) AMQP_NOEXCEPT
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

class PreconditionFailedException : public ChannelException {
 public:
  static const std::uint16_t REPLY_CODE;
  explicit PreconditionFailedException(const std::string &what,
                                       const std::string reply_text,
                                       std::uint16_t class_id,
                                       std::uint16_t method_id) AMQP_NOEXCEPT
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual std::uint16_t reply_code() const AMQP_NOEXCEPT { return REPLY_CODE; }
};

//----------------------------------------------------------------------------

class AmqpLibraryException : public std::runtime_error {
 public:
  static AmqpLibraryException CreateException(int error_code);
  static AmqpLibraryException CreateException(int error_code,
                                              const std::string &context);

  int ErrorCode() const { return m_errorCode; }

 protected:
  explicit AmqpLibraryException(const std::string &message,
                                int error_code) AMQP_NOEXCEPT;

 private:
  int m_errorCode;
};

//----------------------------------------------------------------------------

class AmqpResponseLibraryException : public std::runtime_error {
 public:
  static AmqpResponseLibraryException CreateException(
      const amqp_rpc_reply_t_ &reply, const std::string &context);

 protected:
  explicit AmqpResponseLibraryException(const std::string &message) AMQP_NOEXCEPT;
};

//----------------------------------------------------------------------------

class BadUriException : public std::runtime_error {
 public:
  explicit BadUriException() : std::runtime_error("URI is malformed") {}
};

//----------------------------------------------------------------------------

class ConnectionClosedException : public std::runtime_error {
 public:
  explicit ConnectionClosedException()
      : std::runtime_error("Connection is closed") {}
};

//----------------------------------------------------------------------------

class ConsumerCancelledException : public std::runtime_error {
 public:
  explicit ConsumerCancelledException(const std::string &consumer_tag) AMQP_NOEXCEPT
      : std::runtime_error(
            std::string("Consumer was cancelled: ").append(consumer_tag)),
        m_consumer_tag(consumer_tag) {}

  virtual ~ConsumerCancelledException() AMQP_NOEXCEPT {}

  std::string GetConsumerTag() const { return m_consumer_tag; }

 private:
  std::string m_consumer_tag;
};

//----------------------------------------------------------------------------

class ConsumerTagNotFoundException : public std::runtime_error {
 public:
  ConsumerTagNotFoundException() AMQP_NOEXCEPT
      : std::runtime_error("The specified consumer tag is unknown") {}
  virtual ~ConsumerTagNotFoundException() AMQP_NOEXCEPT {}
};

//----------------------------------------------------------------------------

}  // namespace AmqpClient

#endif  // SIMPLEAMQPCLIENT_AMQPEXCEPTION_H
