#include <iostream>

#include <entity/field.hpp>
#include <entity/record.hpp>

using namespace std;
using namespace field;
using namespace record;

int main() {
    Field first_name("first_name", STRING, true);
    Field last_name("last_name", STRING, true);

    vector<DataType> data;
    data.push_back(DataType(string("Kevin")));
    data.push_back(DataType(string("Del Castillo")));

    Record some_row(data);
    cout << *some_row[0].get_string() << '\n';
    cout << *some_row[1].get_string() << '\n';
}
