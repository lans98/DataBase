#pragma once

#include <string>

#include <core/data_types.hpp>
#include <entity/entity.hpp>

namespace field {

    using namespace std;
    using namespace entity;
    using namespace data_types;

    /**
     * The field class is used to identify an entire 
     * column by its name, type and if this entire 
     * column is visible or not.
     */
    class Field : public Entity {
    private:
        string name;
        Type   type;
        bool   visible;

    public:
        Field(string name): name(name) {}
        Field(string name, Type type, bool visible = true):
            name(name),
            type(type),
            visible(visible) {}

        bool operator<(const Field& r) const { return name < r.name; }
        bool operator>(const Field& r) const { return name > r.name; }
        bool operator==(const Field& r) const { return name == r.name; }
        bool operator!=(const Field& r) const { return name == r.name; }

        string get_name() { return name; }
        void set_name(string name) { this->name = move(name); }

        Type get_type() { return type; }
        void set_type(Type type) { this->type = type; }

        bool is_visible() { return visible; }
        void set_visible(bool visible) { this->visible = visible; }
    }; 
}
