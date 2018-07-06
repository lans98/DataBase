#include <cstdio>

#include <core/address.hpp>
#include <address/foo.hpp>

using namespace foo;
using namespace address;

void test1() {
    printf("Test 1:\n");
    
    auto foo = new Foo(1, 2.3);    
    Address<Foo> address(foo);
    address.write("foo_data");

    printf("%lu\n", *address.get_disk_address());
}

void test2() {
    printf("Test 2:\n");
    auto foo = new Foo(5, 1.8);    
    Address<Foo> address(foo);
    address.write("foo_data");

    printf("%lu\n", *address.get_disk_address());
}


int main() {
    test1();
    test2();
}

