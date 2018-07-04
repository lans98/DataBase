#include <core/address.hpp>
#include <address/foo.hpp>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

using namespace foo;
using namespace fmt;
using namespace address;

void test1() {
    print("Test 1\n");
    auto foo = new Foo();
    Address<Foo> address(0UL, nullptr);
    cout << *address.read("foo_data") << '\n';
}

int main() {
    test1();
}
