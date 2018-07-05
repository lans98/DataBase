#include <entity/field.hpp>

using namespace field;

int main() {
    Field first_name("first_name", STRING);
    Field last_name("last_name", STRING);
    Field phone_num("phone_num", STRING);
    Field address("address", STRING);
    Field salary("salary", UINT);
}
