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
    public:
        using TypeWrapper = variant<
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

    private:
        TypeWrapper data;

    public:
        DataType() = default;
        DataType(TypeWrapper data): data(move(data)) {}
        DataType(const DataType&) = default;

        define_getter(get_short, short, SHORT)
        define_getter(get_uint, unsigned int, UINT)
        define_getter(get_ulong, unsigned long, ULONG)
        define_getter(get_double, double, DOUBLE)
        define_getter(get_int, int, INT)
        define_getter(get_long, long, LONG)
        define_getter(get_string, string, STRING)
        define_getter(get_bool, bool, BOOL)
        define_getter(get_char, char, CHAR)
    };


}
