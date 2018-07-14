#include <scppdb.hpp>

using namespace scppdb;

#define loop while(true)

int main() {
    ConfigInstance cfg = Config::get_instance();
    cfg.load_config("scppdb.toml");

    DataBaseLoaderInstance loader = DataBaseLoader::get_instance();
    loader.load();

    loop {
    
    }
}
