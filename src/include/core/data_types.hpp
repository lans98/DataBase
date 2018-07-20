#pragma once

#include <string>
#include <variant>
#include <utility>
#include <optional>
#include <stdexcept>

#define define_getter(name, type, type_id) \
    type* name() {\
        try {\
            return &get<type_id>(data);\
        } catch(...) {\
            return nullptr; \
        }\
    }\

#define define_case_strify(var, type)\
    case type: \
        var = #type;\
        break;\

namespace data_types {

    using namespace std;

    enum Type : size_t {
        // known types
        SHORT,
        UINT,
        ULONG,
        DOUBLE,
        INT,
        LONG,
        STRING,
        BOOL,
        CHAR,
        // aggregate new types starting here

        // don't agreggate nothing after here
        UNKNOWN
    };

    class DataType {
    private:

        using TypeWrapper = variant
        <
            short,
            unsigned int,
            unsigned long,
            double,
            int,
            long,
            string,
            bool,
            char
            // agreggate new types starting here
        >;

        TypeWrapper data;

    public:
        DataType() = default;
        DataType(TypeWrapper data): data(move(data)) {}
        DataType(const DataType&) = default;

        define_getter(get_short  , short         , Type::SHORT  )
        define_getter(get_uint   , unsigned int  , Type::UINT   )
        define_getter(get_ulong  , unsigned long , Type::ULONG  )
        define_getter(get_double , double        , Type::DOUBLE )
        define_getter(get_int    , int           , Type::INT    )
        define_getter(get_long   , long          , Type::LONG   )
        define_getter(get_string , string        , Type::STRING )
        define_getter(get_bool   , bool          , Type::BOOL   )
        define_getter(get_char   , char          , Type::CHAR   )
    };


}

namespace comparison{

  using namespace data_types;

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
  static bool is_equal(data_types::DataType &c1, data_types::DataType &c2, data_types::Type type){
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
  }
  static bool is_different(data_types::DataType &c1, data_types::DataType &c2, data_types::Type type){
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
  }
  static bool is_less(data_types::DataType &c1, data_types::DataType &c2, data_types::Type type){
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
  }
  static bool is_higher(data_types::DataType &c1, data_types::DataType &c2, data_types::Type type){
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
  }
  static bool is_equal_less(data_types::DataType &c1, data_types::DataType &c2, data_types::Type type){
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
  }
  static bool is_equal_higher(data_types::DataType &c1, data_types::DataType &c2, data_types::Type type){
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
  }
  // non-order comparison functions T to T
  static bool sub_string(data_types::DataType _string, data_types::DataType _substring) {
    return _string.get_string()->find(*_substring.get_string()) != _string.get_string()->npos;
  }

}
