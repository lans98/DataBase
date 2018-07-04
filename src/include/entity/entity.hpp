#ifndef SCPPDB_ENTITY_HPP
#define SCPPDB_ENTITY_HPP

#include <string>

namespace entity {

    using namespace std;

    /**
     * Entity class used to identify different
     * kinds of objects in the data base like
     * records, tables or the data base itself
     * because they inherit this type
     */
    class Entity {
    private:
        string type;

    public:
        Entity(): type("unknown") {}
        Entity(string type): type(type) {}

        virtual string get_type() { return type; }
    };
}

#endif
