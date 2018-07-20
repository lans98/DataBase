#pragma once

#include <config.hpp>
#include <core/error.hpp>
#include <core/storage.hpp>
#include <core/data_types.hpp>
#include <entity/entity.hpp>
#include <entity/field.hpp>
#include <entity/record.hpp>
#include <entity/comparison.hpp>

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
    using namespace comparison;

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
        using RecordStoragePtr = shared_ptr<RecordStorage>;

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

        Table projection(vector<Field> sel_fields) {
            // Simple cases where we don't return a result, just an error
            if (!storage)
                throw runtime_error("The storage doesn't exist for this table");

            if (storage->is_empty())
                throw runtime_error("The storage exists but it's empty for this table");

            if (fields.size() < sel_fields.size())
                throw runtime_error("The vector of fields is bigger than the actual table fields size");

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
                throw runtime_error("Some selected fields are invalid for this table");

            // Nothing to query
            if (both_are_the_same)
                return *this;

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
                    vals.push_back(DataType(auto_counter));
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

            return result;
        }
        Table selection(Field sel_field, Field other_field, TypeFuncion func){
          if (!storage)
            throw runtime_error("The storage doesn't exist for this table");

          if (storage->is_empty())
            throw runtime_error("The storage exists but it's empty for this table");

          if (sel_field.type != other_field.type)
            throw runtime_error("Can't operate a comparions between two different Field's types");

          auto fields_end = fields.end();
          auto fields_begin = fields.begin();
          auto itr_sel_field = find(fields_begin,fields_end,sel_field);
          auto itr_other_field = find(fields_begin,fields_end,other_field);

          if (itr_sel_field == fields_end)
            throw runtime_error("Select field is not found in current table");

          if (itr_other_field == fields_end)
            throw runtime_error("Select field for comparison is not found in current table");

          Table result;

          auto storage_end = storage->end();
          size_t size = fields.size();
          size_t pos1 = itr_sel_field - fields_end;
          size_t pos2 = itr_other_field - fields_end;
          /*
          for(auto itr = storage->begin(); itr != storage_end; ++itr){
            vector<DataType> row(size);
            if( compare(itr->values[pos1],itr->values[pos2],func) )
              row = itr->values;
          }
          */
          return Table;
        }
        template<typename T>
        Table selection(Field sel_field, T constant, bool func(DataType,DataType) ){
          if (!storage)
              throw runtime_error("The storage doesn't exist for this table");

          if (storage->is_empty())
              throw runtime_error("The storage exists but it's empty for this table");

          switch (sel_field.type) {
            case Type::SHORT:
              if( typeid(a.data.get_short()) != typeid(T) )
                  throw runtime_error("Can't compare two different types");
            case Type::UINT:
              if( a.data.get_uint() != typeid(T) )
                  throw runtime_error("Can't compare two different types");
            case Type::ULONG:
              if( a.data.get_ulong() != typeid(T) )
                  throw runtime_error("Can't compare two different types");
            case Type::DOUBLE:
              if( a.data.get_double() != typeid(T) )
                  throw runtime_error("Can't compare two different types");
            case Type::INT:
              if( a.data.get_int() != typeid(T) )
                  throw runtime_error("Can't compare two different types");
            case Type::LONG:
              if( a.data.get_long() != typeid(T) )
                  throw runtime_error("Can't compare two different types");
            case Type::STRING:
              if( a.data.get_string() != typeid(T) )
                  throw runtime_error("Can't compare two different types");
            case Type::BOOL:
              if( a.data.get_bool() != typeid(T) )
                  throw runtime_error("Can't compare two different types");
            case Type::CHAR:
              if( a.data.get_char() != typeid(T) )
                  throw runtime_error("Can't compare two different types");
          }

          auto fields_end = fields.end();
          auto fields_begin = fields.begin();
          auto itr_sel_field = find(fields_begin,fields_end,sel_field);

          if (itr_sel_field == fields_end)
            throw runtime_error("Select field is not found in current table");

          Table result;

          //auto storage_end = storage->end();
          size_t size = fields.size();
          size_t pos1 = itr_sel_field - fields_end;
          size_t pos2 = itr_other_field - fields_end;
          /*
          for(){
          

            }
          */
          return Table;
        }

        Table union(Table _table){
          // empty storage
          //verifiyng same fields and same primary keys  fields

          auto current_end = fields.end();
          auto parameter_end = _table.fields.end();

          bool same_fields = true;

          for(auto cur_it = fields.begin(), par_it = _table.fields.begin(); par_it != parameter_end && cur_it != current_end; ++cur_it, ++par_it){
            if(cur_it != par_it){
              same_fields = false;
              break;
            }
          }

          if(!same_fields)
            throw runtime_error("Can't union tables with differents fields");

          Table result;

          return result;
        }
        Table instersection(Table _table){
          // empty storage
          //verifiyng same fields and same primary keys  fields



        }

        bool Insert(vector<Datafields> Tupla){}
        bool Delete(vector<Datafields> Tupla){}
        bool Update(vector<Datafields> Tupla){}

        void print(){

        }
    };

}
