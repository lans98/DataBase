#include <ostream>
#include <entity/entity.hpp>

namespace foo {

    using namespace std;
    using namespace entity;

    class Foo : public Entity {
    private:
        int a;
        double b;

    public:
        Foo() = default;
        Foo(int a, double b): Entity(UNKNOWN, nullopt, nullopt), a(a), b(b) {}

        friend ostream& operator<<(ostream& out, const Foo& foo) {
            out << foo.a << ' ' << foo.b;
            return out;
        }
    };
}

