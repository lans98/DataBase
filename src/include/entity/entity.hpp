#pragma once

#include <set>
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
        set<EntityID> used_ids;

        static EntityIDManager* singleton;
        EntityIDManager(): used_ids() {}

    public:
        static EntityIDManager& get() {
            if (!singleton)
                singleton = new EntityIDManager();

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

            used_ids.insert(id);
            return id;
        }

        bool save(EntityID id) {
            auto search = used_ids.find(id);
            if (search != used_ids.end())
                return false;

            used_ids.insert(id);
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

            for (auto id: used_ids)
                file.write(reinterpret_cast<char*>(&id), sizeof(EntityID));

            return true;
        }

        bool read_from(const string& file_name = "") {
            if (file_name == "")
                return false;

            ifstream file(file_name, ios::binary);
            if (!file)
                return false;

            EntityID* tmp = new EntityID();
            while (file) {
                file.read(reinterpret_cast<char*>(tmp), sizeof(EntityID));
                used_ids.insert(*tmp);
            }

            return true;
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
        EntityID id;

    public:
        Entity(bool gen = true): id(gen? EntityIDManager::get().generate() : 0UL) {}
        Entity(EntityID id): id(id) { 
            if (!EntityIDManager::get().save(id))
                throw runtime_error("Trying to save an already used id");
        }

        virtual ~Entity() { EntityIDManager::get().free(id); }

        EntityID get_id() { return id; }        
        void set_id(EntityID id) { this->id = id; }
    };
}
