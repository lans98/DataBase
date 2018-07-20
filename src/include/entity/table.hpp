#ifndef SCPPDB_TABLE_HPP
#define SCPPDB_TABLE_HPP

#include <config.hpp>
#include <core/data_types.hpp>
#include <core/address.hpp>
#include <core/storage.hpp>
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
#include <iostream>

namespace table {

    using namespace std;
    using namespace config;
    using namespace address;
    using namespace storage;
    using namespace entity;
    using namespace field;
    using namespace record;
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
        using RecordStoragePtr = shared_ptr<RecordStorage>;

    public:
        friend class Record;

        string                name;
        vector<Field>         fields;
        size_t                pk_size;
        PrimaryKey            primary_key;

        map<string, RecordStoragePtr> storage;

    public:
        Table():
            Entity(),
            name("_temp_"),
            pk_size(0UL),
            primary_key(),
            storage() {}

        Table(Table&&) = default;
        Table(const Table&) = default;

        Table(string name, optional<EntityID> opt_parent = nullopt, optional<EntityID> opt_id = nullopt):
            Entity(EntityType::TABLE, opt_id, opt_parent),
            name(move(name)),
            pk_size(0UL),
            primary_key() {}
            //storage(nullptr) {}

<<<<<<< HEAD
        Table(string name, vector<Field> fields_list, optional<EntityID> opt_parent = nullopt, optional<EntityID> opt_id = nullopt): 
            Entity(EntityType::TABLE, opt_id, opt_parent), 
            name(move(name)), 
            pk_size(0UL), 
            primary_key()
            //storage(nullptr) 
        { 
=======
        Table(string name, vector<Field> fields_list, optional<EntityID> opt_parent = nullopt, optional<EntityID> opt_id = nullopt):
            Entity(EntityType::TABLE, opt_id, opt_parent),
            name(move(name)),
            pk_size(0UL),
            primary_key()
            //storage(nullptr)
        {
>>>>>>> 54286d3add23b731752eecfd07320d775a480d51
            //copy(fields_list.begin(), fields_list.end(), fields.begin());
            for(int i=0; i<fields_list.size();i++){
                fields.push_back(fields_list[i]);
            }
        }

        PrimaryKey get_primary_key() const { return primary_key; }
        void set_primary_key(PrimaryKey pk) {
            for (auto& field: pk) {
                auto it = find(fields.begin(), fields.end(), field);
                if (it == fields.end())
                    throw runtime_error("Given primary key is invalid, some fields weren't found for this table");
            }

            pk_size = pk.size();
            primary_key = move(pk);
        }

        string get_name() const { return name; }
        vector<Field> get_fields() const { return fields; }

        bool index_field(const string& field) {
            //si el mapa no tiene un row_storage del campo
            if(this->storage.find(field)==this->storage.end()){
                //indexamos campo
                //RecordStorage es la clase interfaz del b+
                //es decir, acá creamos un b+
                this->storage[field] = make_shared<RecordStorage>();
                auto row_storage = this->storage[field];

                //leo el archivo relacionado con esta tabla y voy insertando en índice, bajo hash
                //se hace flush al terminar, ojo flush solo escribe en disco el árbol, pero este sigue estando en memoria

                string tabla_path = this->name + ".tabla";
                fstream(archivo_tabla);
                archivo_tabla.open("/home/vlue/SCPPDB/test/core/persona.tabla", fstream::in);
                //cerr << tabla.get_fields()[2].get_name() << "\n";
                vector<Field> v(this->get_fields());
                unsigned int posicion_campo = distance(v.begin(), find(v.begin(), v.end(), field));

                string linea, string_campo;
                int begin_valor_campo, end_valor_campo;
                int posicion_de_inicio_de_linea=0;
                while(archivo_tabla >> linea){
                    int posicion_comas=0, find_;
                    for(int i_=0; i_<= posicion_campo-1; i_++){
                        find_ = linea.find(",",posicion_comas,1);
                        posicion_comas = find_+1;
                    }
                    //ubicado justo en el primer caracter del campo a leer
                    begin_valor_campo = posicion_comas;
                    end_valor_campo = linea.find(",",posicion_comas,1)-1;
                    string_campo =  linea.substr(begin_valor_campo, end_valor_campo-begin_valor_campo+1);
                    //aplicamos hash
                    size_t valor_hash = hash<string>{}(string_campo);
                    //nPointers = 200
                    //insertamos
                    Address<size_t> reg(posicion_de_inicio_de_linea, nullptr);
                    row_storage->get_bplus().insert(valor_hash,reg);
                    posicion_de_inicio_de_linea += linea.length() +1;   
                }

                archivo_tabla.close();
                cerr << "Listo, se indexó el campo "<< field << ".\n";
                return true;
            }

            cerr << "Ya está indexado el campo "<< field << ".\n";
            return false;
        }

        bool index_pk() {
            string field("");
            for(int i=0; i<primary_key.size();i++){
                field += primary_key[i];
            }

            //si el mapa no tiene un row_storage de pk
            if(this->storage.find(field)==this->storage.end()){
                //indexamos campo
                //RecordStorage es la clase interfaz del b+
                //es decir, acá creamos un b+
                this->storage[field] = make_shared<RecordStorage>();
                auto row_storage = this->storage[field];

                //leo el archivo relacionado con esta tabla y voy insertando en índice, bajo hash
                //se hace flush al terminar, ojo flush solo escribe en disco el árbol, pero este sigue estando en memoria

                string tabla_path = this->name + ".tabla";
                fstream(archivo_tabla);
                archivo_tabla.open("/home/vlue/SCPPDB/test/core/persona.tabla", fstream::in);
                //cerr << tabla.get_fields()[2].get_name() << "\n";
                vector<Field> v(this->get_fields());
                unsigned int posicion_campo = distance(v.begin(), find(v.begin(), v.end(), field));

                string linea, string_campo;
                int begin_valor_campo, end_valor_campo;
                int posicion_de_inicio_de_linea=0;
                while(archivo_tabla >> linea){
                    int posicion_comas=0, find_;
                    //asumimos que las pk estan en los primeros campos
                    find_ = linea.find(",",posicion_comas,1);
                    posicion_comas = find_+1;
                    find_ = linea.find(",",posicion_comas,1);
                    posicion_comas = find_+1;
                    //ubicado justo en el primer caracter del campo a leer
                    begin_valor_campo = 0;
                    end_valor_campo = posicion_comas-2;
                    string_campo =  linea.substr(begin_valor_campo, end_valor_campo-begin_valor_campo+1);
                    //aplicamos hash
                    size_t valor_hash = hash<string>{}(string_campo);
                    //nPointers = 200
                    //insertamos
                    Address<size_t> reg(posicion_de_inicio_de_linea, nullptr);
                    row_storage->get_bplus().insert(valor_hash,reg);
                    posicion_de_inicio_de_linea += linea.length() +1;
                }
                archivo_tabla.close();
                cerr << "Listo, se indexó el pk "<<field<<"\n";
                return true;
            }
            cerr << "Ya está indexado el pk.\n";
            return false;
        }

<<<<<<< HEAD
        bool index_pk() {
            string field("");
            for(int i=0; i<primary_key.size();i++){
                field += primary_key[i];
            }

            //si el mapa no tiene un row_storage de pk
            if(this->storage.find(field)==this->storage.end()){
                //indexamos campo
                //RecordStorage es la clase interfaz del b+
                //es decir, acá creamos un b+
                this->storage[field] = make_shared<RecordStorage>();
                auto row_storage = this->storage[field];    

                //leo el archivo relacionado con esta tabla y voy insertando en índice, bajo hash
                //se hace flush al terminar, ojo flush solo escribe en disco el árbol, pero este sigue estando en memoria

                string tabla_path = this->name + ".tabla";
                fstream(archivo_tabla);
                archivo_tabla.open("/home/vlue/SCPPDB/test/core/persona.tabla", fstream::in);
                //cerr << tabla.get_fields()[2].get_name() << "\n";
                vector<Field> v(this->get_fields());
                unsigned int posicion_campo = distance(v.begin(), find(v.begin(), v.end(), field));

                string linea, string_campo;
                int begin_valor_campo, end_valor_campo;
                int posicion_de_inicio_de_linea=0;
                while(archivo_tabla >> linea){
                    int posicion_comas=0, find_;    
                    //asumimos que las pk estan en los primeros campos                
                    find_ = linea.find(",",posicion_comas,1);
                    posicion_comas = find_+1;
                    find_ = linea.find(",",posicion_comas,1);
                    posicion_comas = find_+1;                 
                    //ubicado justo en el primer caracter del campo a leer
                    begin_valor_campo = 0;
                    end_valor_campo = posicion_comas-2;
                    string_campo =  linea.substr(begin_valor_campo, end_valor_campo-begin_valor_campo+1);
                    //aplicamos hash
                    size_t valor_hash = hash<string>{}(string_campo);
                    //nPointers = 200
                    //insertamos
                    Address<size_t> reg(posicion_de_inicio_de_linea, nullptr);
                    row_storage->get_bplus().insert(valor_hash,reg);
                    posicion_de_inicio_de_linea += linea.length() +1;   
                }
                archivo_tabla.close();
                cerr << "Listo, se indexó el pk "<<field<<"\n";
                return true;
            }
            cerr << "Ya está indexado el pk.\n";
            return false; 
        }

        /*
=======
>>>>>>> 54286d3add23b731752eecfd07320d775a480d51
        Table projection(vector<Field> sel_fields) {
            // Simple cases where we don't return a result, just an error
            if (storage.empty())
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
                    auto it = find(primary_key.begin(), primary_key.end(), itr->get_name());
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
            if (!present_primary_key){
              result.fields.push_back(Field("_counter_", ULONG, false));
              for(auto& it : sel_fields)
                result.fields.push_back(it);
            }
            else
              result.fields = sel_fields;

            // set the step for the autocounter if needed
            size_t step_counter = present_primary_key? 0UL : 1UL;
            size_t auto_counter = 0UL;

            for(auto& it : sel_fields){
              result.storage[it.get_name()] = storage[it.get_name()];
            }


            if (present_primary_key) {
                result.set_primary_key(primary_key);
            } else {
                vector<string> pk;
                pk.push_back("_counter_");
                result.set_primary_key(pk);
            }

            return result;
        }

        Table selection(Field sel_field, Field other_field, bool func(DataType,DataType,Type)){
          if (storage.empty())
            throw runtime_error("The storage exists but it's empty for this table");

          if (sel_field.get_type() != other_field.get_type())
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


          return result;
        }
        template<typename T>
        Table selection(Field sel_field, T constant, bool func(DataType,DataType,Type) ){
          if (storage.empty())
              throw runtime_error("The storage exists but it's empty for this table");
          //big space for big switch

          auto fields_end = fields.end();
          auto fields_begin = fields.begin();
          auto itr_sel_field = find(fields_begin,fields_end,sel_field);

          if (itr_sel_field == fields_end)
            throw runtime_error("Select field is not found in current table");

          Table result;


          return result;
        }
        Table unionTable(Table _table){
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
        Table instersectionTable(Table _table){
          // empty storage
          //verifiyng same fields and same primary keys  fields
        }
        bool Insert(){}
        bool Delete(){}
        bool Update(){}
        void print(){
        }
    };

}

#endif
