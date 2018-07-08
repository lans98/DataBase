#pragma once

#include <string>

namespace config {
    using namespace std;
    using namespace std::literals::string_literals;

    // Main prefix where needed data will be saved
    const string data_prefix = ".scppdb"s;

    // Filenames 
    const string table_file = data_prefix + "/tables"s;
    const string btree_file = data_prefix + "/btree"s;
    const string entity_ids_file = data_prefix + "/eids"s;
}
