#pragma once

#include <config.hpp>
#include <core/error.hpp>
#include <core/storage.hpp>
#include <core/data_types.hpp>
#include <entity/entity.hpp>
#include <entity/field.hpp>
#include <entity/record.hpp>

#include <set>
#include <array>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <initializer_list>

namespace table {

    using namespace std;
    using namespace error;
    using namespace config;
    using namespace entity;
    using namespace field;
    using namespace record;
    using namespace storage;
    using namespace data_types;

    /**
     * Table is used to hold a vector of fields than 
     * are the ID's for each of its colums, when we 
     * are handling with records we can easily know 
     * the type of each column, because fields tell
     * us the type.
     */
    class Table : public Entity {
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
        Table(): 
            Entity(), 
            name("_temp_"), 
            pk_size(0UL), 
            primary_key(), 
            storage(nullptr) {}

        Table(Table&&) = default;
        Table(const Table&) = default;

        Table(string name, optional<EntityID> opt_parent = nullopt, optional<EntityID> opt_id = nullopt): 
            Entity(EntityType::TABLE, opt_id, opt_parent), 
            name(move(name)), 
            pk_size(0UL), 
            primary_key(), 
            storage(nullptr) {}

        Table(string name, const initializer_list<Field>& fields_list, optional<EntityID> opt_parent = nullopt, optional<EntityID> opt_id = nullopt): 
            Entity(EntityType::TABLE, opt_id, opt_parent), 
            name(move(name)), 
            pk_size(0UL), 
            primary_key(), 
            storage(nullptr) 
        {
            copy(fields_list.begin(), fields_list.end(), fields.begin());
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

        string get_name() const { return name; }

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

                auto itr = sel_fields.begin();
                auto end = sel_fields.end();

                size_t found_keys = 0;
                
                sel_fields_is_valid = true;
                for (; itr != end; ++itr) {
                    auto search = find(fields.begin(), fields.end(), *itr);
                    if (search == fields.end()) {
                        sel_fields_is_valid = false;
                        break;
                    }

                    itr->set_type(search->get_type());
                    itr->set_visible(true);
                    auto it = find(primary_key.begin(), primary_key.end(), *itr);
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
                result.fields.push_back(Field("_counter_", ULONG, false));

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
