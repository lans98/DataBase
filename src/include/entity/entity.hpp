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
        struct EntityInfo { 
            EntityID   parent;
            EntityType idtype;
        };

        struct DiskCell {
            EntityID id;
            EntityID parent;
            EntityType idtype;
        };

        map<EntityID, EntityInfo> used_ids;

        static EntityIDManager* instance;
        EntityIDManager(): used_ids() {}

    public:
        static EntityIDManager& get_instance() {
            if (!instance)
                instance = new EntityIDManager();

            return *instance;
        }

        EntityID generate(EntityID parent_id, EntityType type) {
            random_device rand;
            mt19937_64 gen(rand());
            uniform_int_distribution<EntityID> dist(2, numeric_limits<EntityID>::max());

            EntityID id;
            do {
                id = dist(gen); 
            } while (used_ids.find(id) != used_ids.end());

            used_ids.emplace(id, EntityInfo { .parent = parent_id, .idtype = type });
            return id;
        }

        bool save(EntityID id, EntityID parent_id, EntityType type) {
            auto search = used_ids.find(id);
            if (search != used_ids.end())
                return false;

            used_ids.emplace(id, EntityInfo { .parent = parent_id, .idtype = type });
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

        bool save_ids(const string& file_name = "") {
            if (file_name == "")        
                return false;

            ofstream file(file_name, ios::binary);
            if (!file)
                return false;

            DiskCell tmp;
            for (auto id: used_ids) {
                tmp.id = id.first;
                tmp.parent = id.second.parent;
                tmp.idtype = id.second.idtype;
                file.write(reinterpret_cast<char*>(&tmp), sizeof(DiskCell));
            }

            return true;
        }

        bool load_ids(const string& file_name = "") {
            if (file_name == "")
                return false;

            ifstream file(file_name, ios::binary);
            if (!file)
                return false;

            auto tmp = new DiskCell();
            while (file) {
                file.read(reinterpret_cast<char*>(tmp), sizeof(DiskCell));
                used_ids.emplace(tmp->id, EntityInfo { .parent = tmp->parent, .idtype = tmp->idtype });
            }

            return true;
        }

        EntityType type_of(EntityID id) {
            if (id == 0U) 
                return EntityType::UNKNOWN;

            auto search = used_ids.find(id);
            if (search == used_ids.end())
                throw runtime_error("Not found ID");
        
            return search->second;
        }
    };

    EntityIDManager* EntityIDManager::singleton = nullptr;
    using EntityIDManagerInstance = EntityIDManager&;

    /**
     * Entity class used to identify different
     * kinds of objects in the data base like
     * records, tables or the data base itself
     * because they inherit this type
     */
    class Entity {
    protected:
        EntityID  id;
        EntityID  parent;

    public:
        Entity(): id(0U), parent(0U) {}
        Entity(EntityType etype, optional<EntityID> opt_id, optional<EntityID> opt_parent): 
            id(opt_id.value_or(EntityIDManager::get_instance().generate(etype))),
            parent(opt_parent.value_or(0U))
        { 
            if (opt_id.has_value() && !EntityIDManager::get_instance().save(*opt_id, etype))
                throw runtime_error("Trying to create an entity with an already used id");
        }

        virtual ~Entity() { EntityIDManager::get_instance().free(id); }
        EntityID get_id() { return id; }        
        bool has_parent() { return parent; }
    };
}
