#include <core/address.hpp>
#include <address/foo.hpp>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

using namespace foo;
using namespace fmt;
using namespace address;

void test1() {
    print("Test 1\n");
    auto foo = new Foo(1, 2.3, true);    
    Address<Foo> address(foo);
    address.write("foo_data");

    print("{}\n", *address.get_disk_address());
}

void test2() {
    print("Test 2\n");
    auto foo = new Foo(5, 1.8, true);    
    Address<Foo> address(foo);
    address.write("foo_data");

    print("{}\n", *address.get_disk_address());
}


int main() {
    test1();
    test2();
}

