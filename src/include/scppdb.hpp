#pragma once

#include <config.hpp>
#include <entity/database.hpp>

namespace scppdb {

    using namespace config;
    using namespace database;

    class DataBaseLoader {
    private:
        ConfigInstance cfg;

        static DataBaseLoader* instance;
        DataBaseLoader(): cfg(Config::get_instance()) {}

    public:
        static DataBaseLoader& get_instance() {
            if (!instance) 
                instance = new DataBaseLoader;
            return *instance;
        }

        void load(const string& config_file) {
            Config::load_config(config_file);
            ConfigInstance  cfg = Config::get_instance();
            DataBaseInstance db = DataBase::get_instance();
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();

            string prefix = cfg.get_as<string>("files.prefix").value_or(".scppdb");

            auto table_file = cfg.get_as<string>("files.table_file");
            db.load_tables(prefix + table_file.value_or("tables"));

            auto ent_ids_file = cfg.get_as<string>("files.entity_ids_file");
            id_manager.load_ids(prefix + ent_ids_file.value_or("ent_ids"));
        }
    };

    DataBaseLoader* DataBaseLoader::instance = nullptr;
    using DataBaseLoaderInstance = DataBaseLoader&;
}
