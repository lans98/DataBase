#ifndef SCPPDB_RECORD_HPP
#define SCPPDB_RECORD_HPP

#include <data_types.hpp>

#include <string>
#include <vector>

namespace record {

    using namespace std;
    using namespace data_types;

    // abstraction for fields (i.e. id for columns)
    struct Field {
        string name;
        Type   type;
        bool   visible;

        static Field new_field(string name) {
            return Field {
                .name = name,
                .type = UNKNOWN,
                .visible = true
            };
        } 

        bool operator<(const Field& r) const { return name < r.name; }
        bool operator>(const Field& r) const { return name > r.name; }
        bool operator==(const Field& r) const { return name == r.name; }
        bool operator!=(const Field& r) const { return name == r.name; }
    };

    // abstraction for rows
    class Record {
    private:
        vector<DataType> values;

    public:
        Record() = default;
        Record(const Record&) = default;
        Record(vector<DataType> vals): values(move(vals)) {}
    };
}

#endif
