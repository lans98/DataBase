#include <iostream>

#include <entity/field.hpp>

using namespace std;
using namespace field;

int main() {
    Field first_name("first_name", STRING, true);
    Field last_name("last_name", STRING, true);
    Field phone_num("phone_num", STRING, true);
    Field address("address", STRING, true);
    Field salary("salary", UINT, true);

    debug(cerr, first_name) << '\n';
    debug(cerr, last_name)  << '\n';
    debug(cerr, phone_num)  << '\n';
    debug(cerr, address)    << '\n';
    debug(cerr, salary)     << '\n';
}
