#ifndef SCPPDB_TABLE_HPP
#define SCPPDB_TABLE_HPP

#include <config.hpp>
#include <data_types.hpp>

#include <set>
#include <vector>
#include <initializer_list>

namespace table {

    using namespace std;
    using namespace config;
    using namespace data_types;

    struct Field {
        string name;
        Type   type;
        size_t index;

        bool operator<(const Field& r) { return name < r.name; }
        bool operator>(const Field& r) { return name > r.name; }
        bool operator==(const Field& r) { return name == r.name; }
        bool operator!=(const Field& r) { return name == r.name; }
    };

    class Record {
    private:
        vector<DataType> values;

    public:
        Record() = default;

    };

    class RecordStorage {
    private:
        // B+ goes here 
    public:
    };

    class Table {
    private:
        friend class Record;

        string      name;
        string      primary_key;
        set<Field>  fields;

        RecordStorage*  storage;

    public:
        Table(string name): name(move(name)), primary_key("") {}
        Table(string name, const initializer_list<Field>& fields_list): name(move(name)), primary_key("") {
            size_t index = 0;

            for (auto& field : fields_list) {
                fields.insert(Field { 
                    .name = field.name,
                    .type = field.type,
                    .index = index
                });

                index += 1;
            }
        }

        string get_primary_key() const { return primary_key; }
        void set_primary_key(string pk) { primary_key = move(pk); }
    };

}

#endif
