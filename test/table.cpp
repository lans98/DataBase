#include <table.hpp>
#include <iostream>

using namespace std;
using namespace table;

int main() {
    Table table("test", {
        { "id", SHORT },
        { "name", STRING },
        { "sex", CHAR }
    });
}
