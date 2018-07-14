#pragma once

#include <string>
#include <iostream>
#include <cpptoml.h>

namespace config {
    using namespace std;
    using namespace cpptoml;

    class Config {
    private:
        shared_ptr<cpptoml::table> config;

        static Config* instance;

        Config(const string& config_file) {
            try {
                config = parse_file(config_file);
            } catch (const exception& e) {
                cerr << e.what() << '\n';
                throw e;
            }
        }

    public:
        static void load_config(const string& name) {
            delete instance;
            instance = new Config(name);
        }

        static Config& get_instance() {
            if (!instance)
                throw runtime_error("Please load a config file first");

            return *instance;
        }

        template <class T> 
        optional<T> get_as(const string& id) {
            auto dot = id.find(".");
            if (dot == string::npos) {
                auto val = config->get_as<T>(id);
                return val? optional<T>(*val) : nullopt;
            } else {
                auto val = config->get_qualified_as<T>(id);
                return val? optional<T>(*val) : nullopt;
            }
        }
    };

    Config* Config::instance = nullptr;
    using ConfigInstance = Config&;
}
