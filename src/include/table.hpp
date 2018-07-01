#ifndef SCPPDB_TABLE_HPP
#define SCPPDB_TABLE_HPP

#include <error.hpp>
#include <config.hpp>
#include <data_types.hpp>

#include <set>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>
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
    class Table {
    public:
        using PrimaryKey = vector<string>;
        using RecordStoragePtr = unique_ptr<RecordStorage>;

        struct OperationResult {
            optional<Table> table;
            optional<Error> error;
        };

    private:
        friend class Record;

        string                name;
        vector<Field>         fields;
        size_t                pk_size;
        PrimaryKey            primary_key;

        RecordStoragePtr      storage;

    public:
        Table(): name(""), pk_size(0UL), primary_key(), storage(nullptr) {}
        Table(const Table&) = default;
        Table(string name): name(move(name)), pk_size(0UL), primary_key(), storage(nullptr) {}
        Table(string name, const initializer_list<Field>& fields_list): name(move(name)), pk_size(0UL), primary_key(), storage(nullptr) {
            for (auto& field : fields_list) {
                fields.push_back(Field { 
                    .name = field.name,
                    .type = field.type,
                });
            }
        }

        PrimaryKey get_primary_key() const { return primary_key; }
        optional<Error> set_primary_key(PrimaryKey pk) { 
            for (auto& field: pk) {
                auto it = find(fields.begin(), fields.end(), field);
                if (it == fields.end())
                    return Error(ErrorKind::INCORRECT_PARAMS, "Given primary key is invalid, some fields weren't found for this table");
            }

            pk_size = pk.size();
            primary_key = move(pk); 

            return nullopt;
        }

        [[nodiscard]] // You shouldn't discard this return value
        OperationResult projection(vector<Field> sel_fields) {
            // Simple cases where we don't return a result, just an error
            if (!storage)
                return { nullopt, Error(ErrorKind::NULL_STORAGE, "The storage doesn't exist for this table") };

            if (storage->is_empty()) 
                return { nullopt, Error(ErrorKind::EMPTY_STORAGE, "The storage exists but it's empty for this table") };

            if (fields.size() < sel_fields.size())
                return { nullopt, Error(ErrorKind::INCORRECT_PARAMS, "The vector of fields is bigger than the actual table fields size") };

            // Three conditions that also need to be checked
            bool both_are_the_same;
            bool sel_fields_is_valid;
            bool present_primary_key;

            { // inner scope to check the three above conditions
              // also used to updated selected fields types and visibility
                both_are_the_same = (fields.size() == sel_fields.size());

                auto itr1 = fields.begin();
                auto itr2 = sel_fields.begin();
                auto end2 = sel_fields.end();

                size_t found_keys = 0;
                
                sel_fields_is_valid = true;
                for (; itr2 != end2; ++itr1, ++itr2) {
                    if (*itr1 != *itr2) {
                        sel_fields_is_valid = false;
                        break;
                    }

                    itr2->type    = itr1->type;
                    itr2->visible = true;
                    auto it = find(primary_key.begin(), primary_key.end(), *itr2);
                    if (it != primary_key.end()) found_keys += 1;
                }

                both_are_the_same = (sel_fields_is_valid && both_are_the_same);
                present_primary_key = (found_keys == pk_size) && pk_size;
            }

            // handle results 
            if (!sel_fields_is_valid)
                return { nullopt, Error(ErrorKind::INCORRECT_PARAMS, "Some selected fields are invalid for this table") };

            if (both_are_the_same)
                return { *this, nullopt };

            // Above conditions passed so we can finally create an empty table 
            // that will hold the result
            Table result;

            // if the primary key isn't complete in the selected fields, then we
            // need to use an autocounter has primary key
            if (!present_primary_key)
                result.fields.push_back(Field {
                    .name = "_counter_",
                    .type = ULONG,
                    .visible = false
                });

            // set the step for the autocounter if needed
            size_t step_counter = present_primary_key? 0UL : 1UL;
            size_t auto_counter = 0UL;

            auto end = storage->end();
            for (auto itr = storage->begin(); itr != end; ++itr) {
                vector<DataType> vals;
                if (step_counter)
                    vals.reserve(sel_fields.size() + 1);
                else
                    vals.reserve(sel_fields.size());

                if (step_counter) {
                    vals.push_back(auto_counter);
                    auto_counter += step_counter;
                }

                for (auto& field : sel_fields) {
                    auto it = find(fields.begin(), fields.end(), field);
                    auto pos = (it - fields.begin());
                    vals.push_back(itr->values[pos]);
                }
            }

            result.fields = sel_fields;
            if (present_primary_key) {
                result.set_primary_key(primary_key);
            } else {
                vector<string> pk;
                pk.push_back("_counter_");
                result.set_primary_key(pk);
            }

            return { result, nullopt }
        }
    };

}

#endif
