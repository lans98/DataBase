#pragma once

#include <set>
#include <string>
#include <random>
#include <limits>

namespace entity {

    using namespace std;

    using EntityID = size_t;

    /**
     * Generate random unique ID's for each entity
     * database is also a global entity, but it always
     * have the ID 1
     */
    class EntityIDGenerator {
    private:
        set<EntityID> used_ids;

        static EntityIDGenerator* singleton;
        EntityIDGenerator(): used_ids() {}

    public:
        static EntityIDGenerator& get() {
            if (!singleton)
                singleton = new EntityIDGenerator();
            return *singleton;
        }

        EntityID generate() {
            random_device rand;
            mt19937_64 gen(rand());
            uniform_int_distribution<EntityID> dist(2, numeric_limits<EntityID>::max());

            EntityID id;
            do {
                id = dist(gen);
            } while (used_ids.find(id) != used_ids.end());

            return id;
        }
    
    };

    /**
     * Entity class used to identify different
     * kinds of objects in the data base like
     * records, tables or the data base itself
     * because they inherit this type
     */
    class Entity {
    private:
        EntityID id;

    public:
        Entity(): id(EntityIDGenerator::get().generate()) {}
        EntityID get_id() { return id; }        
    };
}
