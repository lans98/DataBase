#pragma once
#include <core/data_types.hpp>

namespace comparison{
  /*
  enum TypeFunction : size_t {
    Equal,
    Different,
    Less,
    Higher,
    EqualLess,
    EqualHigher,
    SubString
  };

  // order comparison functions T to Constant
  static bool is_equal(data_type::DataType &c1, data_type::DataType &c2, data_type::Type type)
    switch (type) {
      case Type::SHORT:
        return c1.get_short() == c2.get_short();
      case Type::UINT:
        return c1.get_uint() == c2.get_uint();
      case Type::ULONG:
        return c1.get_ulong() == c2.get_ulong();
      case Type::DOUBLE:
        return c1.get_double() == c2.get_double();
      case Type::INT:
        return c1.get_int() == c2.get_int();
      case Type::LONG:
        return c1.get_long() == c2.get_long();
      case Type::STRING:
        return c1.get_string() == c2.get_string();
      case Type::BOOL:
        return c1.get_bool() == c2.get_bool();
      case Type::CHAR:
        return c1.get_char() == c2.get_char();
    }
  static bool is_different(data_type::DataType &c1, data_type::DataType &c2, data_type::Type type)
  switch (type) {
    case Type::SHORT:
      return c1.get_short() != c2.get_short();
    case Type::UINT:
      return c1.get_uint() != c2.get_uint();
    case Type::ULONG:
      return c1.get_ulong() != c2.get_ulong();
    case Type::DOUBLE:
      return c1.get_double() != c2.get_double();
    case Type::INT:
      return c1.get_int() != c2.get_int();
    case Type::LONG:
      return c1.get_long() != c2.get_long();
    case Type::STRING:
      return c1.get_string() != c2.get_string();
    case Type::BOOL:
      return c1.get_bool() != c2.get_bool();
    case Type::CHAR:
      return c1.get_char() != c2.get_char();
  }
  static bool is_less(data_type::DataType &c1, data_type::DataType &c2, data_type::Type type)
    switch (type) {
      case Type::SHORT:
        return c1.get_short() < c2.get_short();
      case Type::UINT:
        return c1.get_uint() < c2.get_uint();
      case Type::ULONG:
        return c1.get_ulong() < c2.get_ulong();
      case Type::DOUBLE:
        return c1.get_double() < c2.get_double();
      case Type::INT:
        return c1.get_int() < c2.get_int();
      case Type::LONG:
        return c1.get_long() < c2.get_long();
      case Type::STRING:
        return c1.get_string() < c2.get_string();
      case Type::BOOL:
        return c1.get_bool() < c2.get_bool();
      case Type::CHAR:
        return c1.get_char() < c2.get_char();
    }
  static bool is_higher(data_type::DataType &c1, data_type::DataType &c2, data_type::Type type)
    switch (type) {
      case Type::SHORT:
        return c1.get_short() > c2.get_short();
      case Type::UINT:
        return c1.get_uint() > c2.get_uint();
      case Type::ULONG:
        return c1.get_ulong() > c2.get_ulong();
      case Type::DOUBLE:
        return c1.get_double() > c2.get_double();
      case Type::INT:
        return c1.get_int() > c2.get_int();
      case Type::LONG:
        return c1.get_long() > c2.get_long();
      case Type::STRING:
        return c1.get_string() > c2.get_string();
      case Type::BOOL:
        return c1.get_bool() > c2.get_bool();
      case Type::CHAR:
        return c1.get_char() > c2.get_char();
    }
  static bool is_equal_less(data_type::DataType &c1, data_type::DataType &c2, data_type::Type type)
    switch (type) {
      case Type::SHORT:
        return c1.get_short() <= c2.get_short();
      case Type::UINT:
        return c1.get_uint() <= c2.get_uint();
      case Type::ULONG:
        return c1.get_ulong() <= c2.get_ulong();
      case Type::DOUBLE:
        return c1.get_double() <= c2.get_double();
      case Type::INT:
        return c1.get_int() <= c2.get_int();
      case Type::LONG:
        return c1.get_long() <= c2.get_long();
      case Type::STRING:
        return c1.get_string() <= c2.get_string();
      case Type::BOOL:
        return c1.get_bool() <= c2.get_bool();
      case Type::CHAR:
        return c1.get_char() <= c2.get_char();
    }
  static bool is_equal_higher(data_type::DataType &c1, data_type::DataType &c2, data_type::Type type)
    switch (type) {
      case Type::SHORT:
        return c1.get_short() >= c2.get_short();
      case Type::UINT:
        return c1.get_uint() >= c2.get_uint();
      case Type::ULONG:
        return c1.get_ulong() >= c2.get_ulong();
      case Type::DOUBLE:
        return c1.get_double() >= c2.get_double();
      case Type::INT:
        return c1.get_int() >= c2.get_int();
      case Type::LONG:
        return c1.get_long() >= c2.get_long();
      case Type::STRING:
        return c1.get_string() >= c2.get_string();
      case Type::BOOL:
        return c1.get_bool() >= c2.get_bool();
      case Type::CHAR:
        return c1.get_char() >= c2.get_char();
    }

  // non-order comparison functions T to T
  static bool sub_string(data_type::DataType _string, data_type::DataType _substring) {
    return _string.get_string().find(_substring.get_string()) != _string.get_string().npos;
  }
  */
};
