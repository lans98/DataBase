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

  bool compare(DataType d1, DataType d2, TypeFunction type){
    switch (type) {
      case TypeFunction::Equal:
        return == ;
      case TypeFunction::Different:
        return != ;
      case TypeFunction::Less:
        return < ;
      case TypeFunction::Higher:
        return > ;
      case TypeFunction::EqualLess:
        return <= ;
      case TypeFunction::EqualHigher:
        return >= ;
      case TypeFunction::SubString:
        if(*std::get_if<std::string>(&d1) && *std::get_if<std::string>(&d2))
          return d1.get_string().find(d2.get_string()) != d1.get_string().npos;
      default:
        return false;
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
