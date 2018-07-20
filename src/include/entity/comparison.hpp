#pragma once
#include <core/data_types.hpp>

namespace comparison{

  using namespace data_types;

  enum TypeFunction : size_t {
    Equal,
    Different;
    Less;
    Higher;
    EqualLess;
    EqualHigher;
    SubString;
  };

  // order comparison functions T to Constant
  static bool is_equal(DataType &c1, DataType &c2)
    switch (sel_field.type) {
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
  static bool is_different(DataType &c1, DataType &c2)
  switch (sel_field.type) {
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
  static bool is_less(DataType &c1, DataType &c2)
    switch (sel_field.type) {
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
  static bool is_higher(DataType &c1, DataType &c2)
    switch (sel_field.type) {
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
  static bool is_equal_less(DataType &c1, DataType &c2)
    switch (sel_field.type) {
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
  static bool is_equal_higher(DataType &c1, DataType &c2)
    switch (sel_field.type) {
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
  static bool sub_string(std::string _string, std::string _substring) {return _string.find(_substring) != _string.npos;}*/
};
