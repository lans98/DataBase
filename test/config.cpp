#include <config.hpp>

using namespace config;

int main() {
    Config::load_config("scppdb.toml");
    ConfigInstance cfg = Config::get_instance();
    cout << *cfg.get_as<string>("files.prefix") << '\n';
}
