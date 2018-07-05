#pragma once

#include <core/data_types.hpp>
#include <entity/entity.hpp>

#include <string>
#include <vector>

namespace record {

    using namespace std;
    using namespace entity;
    using namespace data_types;

    class Record : public Entity {
    private:
        vector<DataType> values;

    public:
        Record(): Entity() {}
        Record(const Record&) = default;
        Record(vector<DataType> vals, optional<EntityID> opt_id = nullopt):
            Entity(EntityType::RECORD, opt_id),
            values(move(vals)) {}
    };
}
