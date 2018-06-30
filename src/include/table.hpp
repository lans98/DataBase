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

        PrimaryKey get_primary_key() const { return primary_key; }
        void set_primary_key(PrimaryKey pk) const { primary_key = move(pk); }

        [[nodiscard]] // You shouldn't discard this return value
        OperationResult project(const vector<Field>& sel_fields) {
            // Simple cases where we don't return a result, just an error
            if (!storage)
                return { nullopt, Error(ErrorKind::NULL_STORAGE, "The storage doesn't exist for this table") };

            if (storage->is_empty()) 
                return { nullopt, Error(ErrorKind::EMPTY_STORAGE, "The storage exist but it's empty for this table") };

            if (fields.size() < sel_fields.size())
                return { nullopt, Error(ErrorKind::INCORRECT_PARAMS, "The vector of fields is bigger than the actual table fields size") };


            // check if fields is equal to sel_fields
            bool both_are_the_same = [&](){ 
                if (fields.size() != sel_fields.size())
                    return false;

                auto itr1 = fields.begin();
                auto itr2 = sel_fields.begin();
                for (; itr1 != fields.end(); ++itr1, ++itr2)
                    if (*itr1 != *itr2)                
                        return false;

                return true;
            }();

            if (both_are_the_same)
                return { *this, nullopt };

            // check if sel_fields are valid
            bool sel_fields_are_valid = [&](){
                auto itr1 = fields.begin();
                auto itr2 = sel_fields.begin();
                for (; itr1 != fields.end() || itr2 != sel_fields.end(); ++itr1, ++itr2)
                    if (*itr1 != *itr2)
                        return false;

                return true;            
            }();

            if (!sel_fields_are_valid) 
                return { nullopt, Error(ErrorKind::INCORRECT_PARAMS, "Some selected fields are invalid for this table") };

            // check if the primary_key is present on the sel_fields if it isn't 
            // or it's but partially (compound primary key) then use an auto counter as
            // primary key for the result (this may change the order)
            size_t counter_step = [&](){

            }();
                
            if (counter_step)
                result.fields.push_back(Field {
                    .name = "_counter_",
                    .type = ULONG,
                    .index = 0,
                    .visible = false
                });

            // If we are here, then we know that sel_fields size is less than fields size
            // and that all selected fields are valid for this table now start iterating 
            // over all the records and select just specified fields
            Table result;

            size_t counter = 0;
            auto end = storage->end();
            for (auto itr = storage->begin(); itr != end; ++itr) {
                //Record& record = *itr;
                vector<DataType> vals;
                if (counter_step)
                    vals.reserve(sel_fields.size() + 1);
                else
                    vals.reserve(sel_fields.size());

                if (counter_step) {
                    vals.push_back(counter);
                    counter += counter_step;
                }

                for (auto& field : sel_fields) {
                    auto it = find(fields.begin(), fields.end(), field);
                    vals.push_back(itr->values[it->index]);
                }
            }

            result.fields = sel_fields;
            return result;
        }
    };

}

#endif
