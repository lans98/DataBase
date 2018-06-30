#ifndef SCPPDB_DATA_TYPES_HPP
#define SCPPDB_DATA_TYPES_HPP

#include <string>
#include <variant>
#include <utility>
#include <stdexcept>

namespace data_types {
    using namespace std;

    using DataType = variant<
        short, 
        unsigned int, 
        unsigned long, 
        float, 
        double, 
        int, 
        long, 
        string, 
        bool,
        char
        // agreggate new types starting here
    >;

    enum Type : size_t {
        // known types
        SHORT,
        UINT,
        ULONG,
        FLOAT,
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
}

#endif
