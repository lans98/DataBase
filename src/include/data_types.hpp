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
        char >;

    enum Type : int {
        SHORT,
        UINT,
        ULONG,
        FLOAT,
        DOUBLE, 
        INT,
        LONG,
        STRING,
        BOOL,
        CHAR
    };
}

#endif
