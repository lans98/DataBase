#include <iostream>
#include <data_types.hpp>

using namespace std;
using namespace data_types;

int main() {
    DataType a = "hello world";
    cout << get<STRING>(a) << '\n';
}
