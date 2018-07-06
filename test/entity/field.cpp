#include <iostream>

#include <entity/field.hpp>

using namespace std;
using namespace field;

int main() {
    Field first_name("first_name", STRING);
    Field last_name("last_name", STRING);
    Field phone_num("phone_num", STRING);
    Field address("address", STRING);
    Field salary("salary", UINT);

    debug(cerr, first_name) << '\n';
    debug(cerr, last_name)  << '\n';
    debug(cerr, phone_num)  << '\n';
    debug(cerr, address)    << '\n';
    debug(cerr, salary)     << '\n';
}
