#pragma once

#include <map>
#include <string>
#include <random>
#include <limits>
#include <fstream>
#include <stdexcept>

namespace entity {

    using namespace std;

    using EntityID = unsigned int;
    
    enum EntityType {
        DATABASE,
        FIELD,
        RECORD,
        TABLE,
        UNKNOWN
    };

    /**
     * Generate random unique ID's for each entity
     * database is also a global entity, but it always
     * have the ID 1, and the ID 0 is generic for entities
     * that are waiting for it's ID
     */
    class EntityIDManager {
    private:
        map<EntityID, EntityType> used_ids;

        static EntityIDManager* singleton;
        EntityIDManager(): used_ids() {}

        struct EntityIDType {
            EntityID   id;
            EntityType type;
        };

    public:
        static EntityIDManager& get() {
            if (!singleton)
                singleton = new EntityIDManager();

            return *singleton;
        }

        EntityID generate(EntityType etype) {
            random_device rand;
            mt19937_64 gen(rand());
            uniform_int_distribution<EntityID> dist(2, numeric_limits<EntityID>::max());

            EntityID id;
            do {
                id = dist(gen); 
            } while (used_ids.find(id) != used_ids.end());

            used_ids.emplace(id, etype);
            return id;
        }

        bool save(EntityID id, EntityType etype) {
            auto search = used_ids.find(id);
            if (search != used_ids.end())
                return false;

            used_ids.emplace(id, etype);
            return true;
        }

        void free(EntityID id)  {
            auto search = used_ids.find(id);
            if (search == used_ids.end())
                return;        

            used_ids.erase(search);
        }

        bool is_valid(EntityID id) {
            auto search = used_ids.find(id);
            return search != used_ids.end();
        }

        bool write_to(const string& file_name = "") {
            if (file_name == "")        
                return false;

            ofstream file(file_name, ios::binary);
            if (!file)
                return false;

            EntityIDType tmp;
            for (auto id: used_ids) {
                tmp.id = id.first;
                tmp.type = id.second;
                file.write(reinterpret_cast<char*>(&tmp), sizeof(EntityIDType));
            }

            return true;
        }

        bool read_from(const string& file_name = "") {
            if (file_name == "")
                return false;

            ifstream file(file_name, ios::binary);
            if (!file)
                return false;

            auto tmp = new EntityIDType();
            while (file) {
                file.read(reinterpret_cast<char*>(tmp), sizeof(EntityIDType));
                used_ids.emplace(tmp->id, tmp->type);
            }

            return true;
        }

        EntityType type_of(EntityID id) {
            auto search = used_ids.find(id);
            if (search == used_ids.end())
                throw runtime_error("Not found ID");
        
            return search->second;
        }
    };

    EntityIDManager* EntityIDManager::singleton = nullptr;

    /**
     * Entity class used to identify different
     * kinds of objects in the data base like
     * records, tables or the data base itself
     * because they inherit this type
     */
    class Entity {
    protected:
        EntityID  id;

    public:
        Entity(): id(0UL) {}
        Entity(EntityType etype, optional<EntityID> opt_id): 
            id(opt_id.has_value()? *opt_id : EntityIDManager::get().generate(etype))
        { 
            if (opt_id.has_value() && !EntityIDManager::get().save(*opt_id, etype))
                throw runtime_error("Trying to create an entity with an already used id");
        }

        virtual ~Entity() { EntityIDManager::get().free(id); }
        EntityID get_id() { return id; }        
    };
}
