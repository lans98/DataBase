#pragma once

#include <string>
#include <ostream>

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
        Field(string name, optional<EntityID> opt_id = nullopt): 
            Entity(EntityType::FIELD, opt_id), 
            name(name),
            type(Type::UNKNOWN) {}

        Field(string name, Type type, bool visible = true, optional<EntityID> opt_id = nullopt):
            Entity(EntityType::FIELD, opt_id),
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

        friend ostream& debug(ostream& out, const Field& field) {
            string type;
            switch (field.type) {
                define_case_strify(type, Type::SHORT   )
                define_case_strify(type, Type::UINT    )
                define_case_strify(type, Type::ULONG   )
                define_case_strify(type, Type::DOUBLE  )
                define_case_strify(type, Type::INT     )
                define_case_strify(type, Type::LONG    )
                define_case_strify(type, Type::STRING  )
                define_case_strify(type, Type::BOOL    )
                define_case_strify(type, Type::CHAR    )
                define_case_strify(type, Type::UNKNOWN )
            }

            out << "Field { name: " << field.name << ", type: " << type << ", visible: " << boolalpha << field.visible << " }";
            return out;
        }

        friend ostream& operator<<(ostream& out, const Field& field) {
            out << field.name;
            return out;
        }
    }; 
}
