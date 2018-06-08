#ifndef SCPPDB_CONFIG_HPP
#define SCPPDB_CONFIG_HPP

#include <string>

namespace config {
    using namespace std;
    using namespace std::literals::string_literals;

    // Main prefix where needed data will be saved
    const string data_prefix = ".scppdb"s;

    // Filenames 
    const string table_file = data_prefix + "/tables"s;
    const string index_file = data_prefix + "/indexes"s;
}

#endif
