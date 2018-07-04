#include <entity/table.hpp>

using namespace table;

int main() {
    Table table("test", {
        Field("id", SHORT),
        Field("name", STRING),
        Field("sex", CHAR)
    });
}
