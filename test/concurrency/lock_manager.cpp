#include <entity/table.hpp>
#include <concurrency/lock_manager.hpp>

using namespace lock_manager;
using namespace table;

int main() {
    LockManager lm;
    auto a = new Record();
    auto b = new Record();
}
