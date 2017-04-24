// Table.cpp
//
#include "monik/log/third_party/amqp/Table.h"
#include "monik/log/third_party/amqp/TableImpl.h"

namespace monik { namespace AmqpClient {

TableValue::TableValue()
    : m_impl(new Detail::TableValueImpl(Detail::void_t())) {}

TableValue::TableValue(bool value)
    : m_impl(new Detail::TableValueImpl(value)) {}

TableValue::TableValue(std::int8_t value)
    : m_impl(new Detail::TableValueImpl(value)) {}

TableValue::TableValue(std::int16_t value)
    : m_impl(new Detail::TableValueImpl(value)) {}

TableValue::TableValue(std::int32_t value)
    : m_impl(new Detail::TableValueImpl(value)) {}

TableValue::TableValue(std::int64_t value)
    : m_impl(new Detail::TableValueImpl(value)) {}

TableValue::TableValue(float value)
    : m_impl(new Detail::TableValueImpl(value)) {}

TableValue::TableValue(double value)
    : m_impl(new Detail::TableValueImpl(value)) {}

TableValue::TableValue(const char *value)
    : m_impl(new Detail::TableValueImpl(std::string(value))) {}

TableValue::TableValue(const std::string &value)
    : m_impl(new Detail::TableValueImpl(value)) {}

TableValue::TableValue(const std::vector<TableValue> &values)
    : m_impl(new Detail::TableValueImpl(values)) {}

TableValue::TableValue(const Table &value)
    : m_impl(new Detail::TableValueImpl(value)) {}

TableValue::TableValue(const TableValue &l)
    : m_impl(new Detail::TableValueImpl(l.m_impl->m_value)) {}

TableValue &TableValue::operator=(const TableValue &l) {
  if (this == &l) {
    return *this;
  }

  m_impl->m_value = l.m_impl->m_value;

  return *this;
}

bool operator==(const Array &l, const Array &r) {
  if (l.size() == r.size()) {
    return std::equal(l.begin(), l.end(), r.begin());
  }
  return false;
}

bool operator==(const Table &l, const Table &r) {
  if (l.size() == r.size()) {
    return std::equal(l.begin(), l.end(), r.begin());
  }
  return false;
}

bool TableValue::operator==(const TableValue &l) const {
  if (this == &l) {
    return true;
  }

  return m_impl->m_value == l.m_impl->m_value;
}

bool TableValue::operator!=(const TableValue &l) const {
  if (this == &l) {
    return false;
  }

  return !(m_impl->m_value == l.m_impl->m_value);
}

TableValue::~TableValue() {}

TableValue::ValueType TableValue::GetType() const {
  return static_cast<ValueType>(m_impl->m_value.which());
}

bool TableValue::GetBool() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_bool);
  return m_impl->m_value.variant.boolean;
}

std::int8_t TableValue::GetInt8() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_int8);
  return m_impl->m_value.variant.i8;
}

std::int16_t TableValue::GetInt16() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_int16);
  return m_impl->m_value.variant.i16;
}

std::int32_t TableValue::GetInt32() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_int32);
  return m_impl->m_value.variant.i32;
}

std::int64_t TableValue::GetInt64() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_int64);
  return m_impl->m_value.variant.i64;
}

std::int64_t TableValue::GetInteger() const {
  switch (m_impl->m_value.which()) {
    case VT_int8:
      return GetInt8();
    case VT_int16:
      return GetInt16();
    case VT_int32:
      return GetInt32();
    case VT_int64:
      return GetInt64();
    default:
      throw std::runtime_error("bad_get");
  }
}

float TableValue::GetFloat() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_float);
  return m_impl->m_value.variant.f32;
}

double TableValue::GetDouble() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_double);
  return m_impl->m_value.variant.f64;
}

double TableValue::GetReal() const {
  switch (m_impl->m_value.which()) {
    case VT_float:
      return GetFloat();
    case VT_double:
      return GetDouble();
    default:
      throw std::runtime_error("bad_get");
  }
}

std::string TableValue::GetString() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_string);
  return m_impl->m_value.string_;
}

std::vector<TableValue> TableValue::GetArray() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_array);
  return m_impl->m_value.array_;
}

Table TableValue::GetTable() const {
  MONIK_ASSERT(m_impl->m_value.kind == VT_table);
  return m_impl->m_value.table_;
}

void TableValue::Set() { m_impl->m_value = Detail::void_t(); }

void TableValue::Set(bool value) { m_impl->m_value = value; }

void TableValue::Set(std::int8_t value) { m_impl->m_value = value; }

void TableValue::Set(std::int16_t value) { m_impl->m_value = value; }

void TableValue::Set(std::int32_t value) { m_impl->m_value = value; }

void TableValue::Set(std::int64_t value) { m_impl->m_value = value; }

void TableValue::Set(float value) { m_impl->m_value = value; }

void TableValue::Set(double value) { m_impl->m_value = value; }

void TableValue::Set(const char *value) {
  m_impl->m_value = std::string(value);
}

void TableValue::Set(const std::string &value) { m_impl->m_value = value; }

void TableValue::Set(const std::vector<TableValue> &value) {
  m_impl->m_value = value;
}

void TableValue::Set(const Table &value) { m_impl->m_value = value; }

}}  // AmqpClient
