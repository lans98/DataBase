#pragma once

#include <map>
#include <entity/entity.hpp>
#include <entity/table.hpp>

namespace database {

    using namespace entity;
    using namespace table;

    class DataBase : public Entity {
    private:
        map<string, Table> tables;

        static DataBase* instance;
        DataBase(): Entity(EntityType::DATABASE, 1U, 0U) {} 

    public:
        static DataBase& get_instance() { 
            if (!instance)
                instance = new DataBase;
            return *instance;
        }

        Table& operator[](const string& name) {
            auto it = tables.find(name);
            if (it == tables.end())
                throw runtime_error("Table " + name + " doesn't exists");

            return it->second;
        }

        void create_table(const Table& table) {
            auto it = tables.find(table.get_name());
            if (it != tables.end())
                throw runtime_error("Table " + table.get_name() + " already exists");

            tables.emplace(table.get_name(), table);
        }
    };

    DataBase* DataBase::instance = nullptr;
    using DataBaseInstance = DataBase&;
}
