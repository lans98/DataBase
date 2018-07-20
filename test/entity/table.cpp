#include <iostream>

#include <entity/table.hpp>

using namespace std;
using namespace table;
using namespace field;

int main() {
  Field first_name("first_name", STRING, true);
  Field last_name("last_name", STRING, true);
  Field phone_num("phone_num", STRING, true);
  Field address("address", STRING, true);
  Field salary("salary", UINT, true);
  vector<Field> campos = {first_name,last_name,phone_num,address,salary};
  Table("Alumnos",campos);
}
