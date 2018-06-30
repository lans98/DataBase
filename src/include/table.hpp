#ifndef SCPPDB_TABLE_HPP
#define SCPPDB_TABLE_HPP

#include <error.hpp>
#include <config.hpp>
#include <data_types.hpp>

#include <set>
#include <array>
#include <vector>
#include <memory>
#include <initializer_list>

namespace table {

    using namespace std;
    using namespace error;
    using namespace config;
    using namespace data_types;

    // abstraction for fields (i.e. id for columns)
    struct Field {
        string name;
        Type   type;
        size_t index;
        bool   visible;

        static Field new_field(string name) {
            return Field {
                .name = name,
                .type = UNKNOWN,
                .index = 0,
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

    // abstraction for manipulate rows in disk
    class RecordStorage {
    private:
        // B+ object (impl is in btree.hpp)
        
    public:
        // TODO
        bool insert(Record) {
            return false;
        }

        // TODO
        bool remove(Record) {
            return false;
        }

        // TODO
        bool is_empty() {
            return true;
        }

        // TODO
        class Iterator {
        private:

        public:
        
            Record /*& (reference)*/ operator*() { return Record(); }
            Record /*& (reference)*/ operator->() { return Record(); }
           
            bool operator==(const Iterator& rhs) { return false; }
            bool operator!=(const Iterator& rhs) { return false; }
            
            Iterator operator++() { return *this; }
            Iterator operator++(int) { return *this; }
        };

        // TODO
        Iterator begin() {  return Iterator(); }

        // TODO
        Iterator end() { return Iterator(); }

        // TODO
        Iterator last() { return Iterator(); }

        // TODO 
        Iterator search(Record) { return Iterator(); }
    };

    // abstraction for a table, holding fields and 
    // a record storage manipulating posible records (i.e. rows)
    template <size_t no_pk>
    class Table {
    public:
        using PrimaryKey = array<string, no_pk>;
        using RecordStoragePtr = unique_ptr<RecordStorage>;

        struct OperationResult {
            optional<Table> table;
            optional<Error> error;
        };

    private:
        friend class Record;

        string                name;
        vector<Field>         fields;
        PrimaryKey            primary_key;

        RecordStoragePtr      storage;

    public:
        Table(): name(""), primary_key(), storage(nullptr) {}
        Table(const Table&) = default;
        Table(string name): name(move(name)), primary_key(), storage(nullptr) {}
        Table(string name, const initializer_list<Field>& fields_list): name(move(name)), primary_key(), storage(nullptr) {
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
