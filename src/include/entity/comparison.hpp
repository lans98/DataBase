#pragma once
#include <core/data_types.hpp>

namespace Comparison{

  using namespace data_types;

  enum Operations : size_t {
    Equal,
    Different;
    Less;
    Higher;
    EqualLess;
    EqualHigher;
    SubString;

  };

  auto getType(DataType a, Type type){
    switch (type) {
      case Type::SHORT:
        return &a.get_short();
        break;
      case Type::UINT:
        return &a.get_uint();
        break;
      case Type::ULONG:
        return &a.get_ulong();
        break;
      case Type::DOUBLE:
        return &a.get_double();
        break;
      case Type::INT:
        return &a.get_int();
        break;
      case Type::LONG:
        return &a.get_long();
        break;
      case Type::STRING:
        return &a.get_string();
        break;
      case Type::BOOL:
        return &a.get_bool();
        break;
      case Type::CHAR:
        return &a.get_char();
        break;
      default
        return nullptr;
        break;
    }
  }

  // order comparison functions T to Constant
  /*static bool is_equal(T c1, T c2)       {return c1 == c2;}
  static bool is_different(T c1, T c2)   {return c1 != c2;}
  static bool is_less(T c1, T c2)        {return c1 <  c2;}
  static bool is_higher(T c1, T c2)      {return c1 >  c2;}
  static bool is_equal_less(T c1, T c2)  {return c1 <= c2;}
  static bool is_equal_higher(T c1, T c2){return c1 >= c2;}

  // non-order comparison functions T to T
  static bool sub_string(std::string _string, std::string _substring) {return _string.find(_substring) != _string.npos;}*/
};
