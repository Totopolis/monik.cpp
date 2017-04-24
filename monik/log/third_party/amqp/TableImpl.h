// TableImpl.h
//
#pragma once
#ifndef SIMPLEAMQPCLIENT_TABLEIMPL_H
#define SIMPLEAMQPCLIENT_TABLEIMPL_H

#include "monik/log/third_party/amqp/Table.h"

namespace monik { namespace AmqpClient { namespace Detail {

typedef std::shared_ptr<amqp_pool_t> amqp_pool_ptr_t;

struct void_t {};

inline bool operator==(const void_t &, const void_t &) { return true; }

typedef std::vector<TableValue> array_t;

struct value_t final {
    uint8_t kind = TableValue::VT_void;
    uint8_t which() const { return kind; }
    union union_t {
        bool boolean;       // 1
        std::int8_t i8;     // 2
        std::int16_t i16;   // 3
        std::int32_t i32;   // 4
        std::int64_t i64;   // 5
        float f32;          // 6
        double f64;         // 7
    } variant;
    std::string string_;    // 8
    array_t array_;         // 9
    Table table_;           // 10
    value_t(void_t)                 : kind(TableValue::VT_void) {}
    value_t(bool v)                 : kind(TableValue::VT_bool) { variant.boolean = v; }
    value_t(std::int8_t v)          : kind(TableValue::VT_int8) { variant.i8 = v; }
    value_t(std::int16_t v)         : kind(TableValue::VT_int16) { variant.i16 = v; }
    value_t(std::int32_t v)         : kind(TableValue::VT_int32) { variant.i32 = v; }
    value_t(std::int64_t v)         : kind(TableValue::VT_int64) { variant.i64 = v; }
    value_t(float v)                : kind(TableValue::VT_float) { variant.f32 = v; }
    value_t(double v)               : kind(TableValue::VT_double) { variant.f64 = v; }
    value_t(std::string const & v)  : kind(TableValue::VT_string) { string_ = v; }
    value_t(array_t const & v)      : kind(TableValue::VT_array) { array_ = v; }
    value_t(Table const & v)        : kind(TableValue::VT_table) { table_ = v; }
    bool operator == (value_t const &) const;
};

class TableValueImpl final : noncopyable {
 public:
  explicit TableValueImpl(const value_t &v) : m_value(v) {}
  virtual ~TableValueImpl() {}

  value_t m_value;

  static amqp_table_t CreateAmqpTable(const Table &table,
                                      amqp_pool_ptr_t &pool);

  static Table CreateTable(const amqp_table_t &table);

  static amqp_table_t CopyTable(const amqp_table_t &table,
                                amqp_pool_ptr_t &pool);

 private:
  static amqp_table_t CreateAmqpTableInner(const Table &table,
                                           amqp_pool_t &pool);
  static TableValue CreateTableValue(const amqp_field_value_t &entry);
  static amqp_table_t CopyTableInner(const amqp_table_t &table,
                                     amqp_pool_t &pool);
  static amqp_field_value_t CopyValue(const amqp_field_value_t value,
                                      amqp_pool_t &pool);

 public:
  class generate_field_value final {
   public:
    explicit generate_field_value(amqp_pool_t &p) : pool(p) {}
    amqp_field_value_t operator()(const void_t) const;
    amqp_field_value_t operator()(const bool value) const;
    amqp_field_value_t operator()(const std::int8_t value) const;
    amqp_field_value_t operator()(const std::int16_t value) const;
    amqp_field_value_t operator()(const std::int32_t value) const;
    amqp_field_value_t operator()(const std::int64_t value) const;
    amqp_field_value_t operator()(const float value) const;
    amqp_field_value_t operator()(const double value) const;
    amqp_field_value_t operator()(const std::string &value) const;
    amqp_field_value_t operator()(const array_t &value) const;
    amqp_field_value_t operator()(const Table &value) const;
    amqp_field_value_t operator()(const value_t& value) const;
   private:
    amqp_pool_t &pool;
  };
};

} // namespace Detail
}} // AmqpClient

#endif  // SIMPLEAMQPCLIENT_TABLEIMPL_H
