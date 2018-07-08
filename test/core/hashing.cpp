#include <iostream>
#include <core/extra.hpp>

using namespace std;
using namespace extra;

int main() {
    string a = "hola";  
    int b = 3;
    double c = 1.92;

    size_t result;
    hash_combine(result, a, b, c);
    cout << result << '\n';
}
