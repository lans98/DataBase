#pragma once

#include <config.hpp>

namespace scppdb {

    using namespace config;

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

        void load() {
                    
        }
    };

    DataBaseLoader* DataBaseLoader::instance = nullptr;
    using DataBaseLoaderInstance = DataBaseLoader&;
}
