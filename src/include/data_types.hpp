#ifndef SCPPDB_DATA_TYPES_HPP
#define SCPPDB_DATA_TYPES_HPP

#include <string>
#include <variant>

namespace data_types {
  using namespace std;

  using DataType = variant<
    short, unsigned int, unsigned long, float, double, int, long, string>;
}

#endif
