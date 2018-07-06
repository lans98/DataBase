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

    debug(cerr, first_name);
    debug(cerr, last_name);
    debug(cerr, phone_num);
    debug(cerr, address);
    debug(cerr, salary);
}
