#include <cstdio>
#include <iostream>

#include <core/data_types.hpp>

using namespace std;
using namespace data_types;

int main() {
    DataType a(3);
    DataType b(string("hola"));
    DataType c(3.4);

    auto a_value = a.get_int();
    auto b_value = b.get_string();
    auto c_value = c.get_double();

    cerr << *a_value << '\n';
    cerr << *b_value << '\n';
    cerr << *c_value << '\n';

    *a_value = 8;

    cerr << *a_value << '\n';
    auto after = a.get_int();
    cerr << *after << '\n';
}
