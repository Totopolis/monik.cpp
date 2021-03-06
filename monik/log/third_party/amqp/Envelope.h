// Envelope.h
//
#pragma once
#ifndef SIMPLEAMQPCLIENT_ENVELOPE_H
#define SIMPLEAMQPCLIENT_ENVELOPE_H

#include "monik/log/third_party/amqp/BasicMessage.h"

namespace monik { namespace AmqpClient {

class Envelope final : noncopyable {
 public:
  typedef std::shared_ptr<Envelope> ptr_t;

  /**
    * Creates an new envelope object
    * @param message the payload
    * @param consumer_tag the consumer tag the message was delivered to
    * @param delivery_tag the delivery tag that the broker assigned to the
   * message
    * @param exchange the name of the exchange that the message was published to
    * @param redelivered a flag indicating whether the message consumed as a
   * result of a redelivery
    * @param routing_key the routing key that the message was published with
    * @returns a std::shared_ptr to an envelope object
    */
  static ptr_t Create(const BasicMessage::ptr_t message,
                      const std::string &consumer_tag,
                      const std::uint64_t delivery_tag,
                      const std::string &exchange, bool redelivered,
                      const std::string &routing_key,
                      const std::uint16_t delivery_channel) {
    return std::make_shared<Envelope>(message, consumer_tag, delivery_tag,
                                        exchange, redelivered, routing_key,
                                        delivery_channel);
  }

  explicit Envelope(const BasicMessage::ptr_t message,
                    const std::string &consumer_tag,
                    const std::uint64_t delivery_tag,
                    const std::string &exchange, bool redelivered,
                    const std::string &routing_key,
                    const std::uint16_t delivery_channel);

 public:
  /**
    * destructor
    */
  virtual ~Envelope();

  /**
    * Get the payload of the envelope
    *
    * @returns the message
    */
  inline BasicMessage::ptr_t Message() const { return m_message; }

  /**
    * Get the consumer tag for the consumer that delivered the message
    *
    * @returns the consumer that delivered the message
    */
  inline std::string ConsumerTag() const { return m_consumerTag; }

  /**
    * Get the delivery tag for the message.
    *
    * The delivery tag is a unique tag for a given message assigned by the
   * broker
    * This tag is used when Ack'ing a message
    *
    * @returns the delivery tag for a message
    */
  inline std::uint64_t DeliveryTag() const { return m_deliveryTag; }

  /**
    * Get the name of the exchange that the message was published to
    *
    * @returns the name of the exchange the message was published to
    */
  inline std::string Exchange() const { return m_exchange; }

  /**
    * Get the flag that indicates whether the message was redelivered
    *
    * A flag that indicates whether the message was redelievered means
    * the broker tried to deliver the message and the client did not Ack
    * the message, so the message was requeued, or the client asked the broker
    * to Recover which forced all non-Acked messages to be redelivered
    *
    * @return a boolean flag indicating whether the message was redelivered
    */
  inline bool Redelivered() const { return m_redelivered; }

  /**
    * Get the routing key that the message was published with
    *
    * @returns a string containing the routing key the message was published
   * with
    */
  inline std::string RoutingKey() const { return m_routingKey; }

  inline std::uint16_t DeliveryChannel() const { return m_deliveryChannel; }

  struct DeliveryInfo {
    std::uint64_t delivery_tag;
    std::uint16_t delivery_channel;
  };

  inline DeliveryInfo GetDeliveryInfo() const {
    DeliveryInfo info;
    info.delivery_tag = m_deliveryTag;
    info.delivery_channel = m_deliveryChannel;

    return info;
  }

 private:
  const BasicMessage::ptr_t m_message;
  const std::string m_consumerTag;
  const std::uint64_t m_deliveryTag;
  const std::string m_exchange;
  const bool m_redelivered;
  const std::string m_routingKey;
  const std::uint16_t m_deliveryChannel;
};

}}  // AmqpClient

#endif  // SIMPLEAMQPCLIENT_ENVELOPE_H
