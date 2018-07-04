#ifndef SCPPDB_RECORD_HPP
#define SCPPDB_RECORD_HPP

#include <entity.hpp>
#include <data_types.hpp>

#include <string>
#include <vector>

namespace record {

    using namespace std;
    using namespace entity;
    using namespace data_types;

    /**
     * Prototypes of friend classes
     */
    class Table;

    /**
     * The field class is used to identify an entire 
     * column by its name, type and if this entire 
     * column is visible or not.
     */
    class Field : public Entity {
    private:
        friend class Table;

        string name;
        Type   type;
        bool   visible;

    public:
        Field(string name): name(name) {}
        Field(string name, Type type, bool visible):
            name(name),
            type(type),
            visible(visible) {}

        bool operator<(const Field& r) const { return name < r.name; }
        bool operator>(const Field& r) const { return name > r.name; }
        bool operator==(const Field& r) const { return name == r.name; }
        bool operator!=(const Field& r) const { return name == r.name; }
    }; 

    class Record : public Entity {
    private:
        vector<DataType> values;

    public:
        Record() = default;
        Record(const Record&) = default;
        Record(vector<DataType> vals): values(move(vals)) {}
    };
}

#endif
