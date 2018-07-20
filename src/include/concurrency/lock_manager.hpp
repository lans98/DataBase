#pragma once

#include <map>
#include <deque>
#include <queue>
#include <chrono>
#include <iostream>
#include <functional>

#include <core/data_types.hpp>
#include <entity/entity.hpp>
#include <entity/field.hpp>
#include <entity/record.hpp>
#include <entity/table.hpp>

namespace lock_manager {

    using namespace std;
    using namespace entity;
    using namespace field;
    using namespace record;
    using namespace table;
    using namespace data_types;

    using Clock = chrono::high_resolution_clock;
    using TimePoint = chrono::time_point<Clock>;

    struct Permission {
        enum Type : int {
            SHARED, 
            EXCLUSIVE,
        };

        Type type;
        bool granted;
        int  transaction_id;
        TimePoint time_point;
    };

    class PermissionDeque : public deque<Permission> {
    private:
        enum LockState {
            NORMAL,
            ISHARED,
            IEXCLUSIVE,
        };

        LockState     state;
        set<EntityID> lock_by;

    public:
        PermissionDeque(): deque(), state(LockState::NORMAL), lock_by() {}
        PermissionDeque(const PermissionDeque&) = default;

        bool lock_shared() { 
            if (state == IEXCLUSIVE)
                return false;

            state = ISHARED;
            return true;
        }
        
        bool lock_exclusive() {
            if (state == ISHARED)
                return false;

            state = IEXCLUSIVE;
            return true;
        }

        bool insert_lock(EntityID id) {
            auto [_, inserted] = lock_by.insert(id);
            return inserted;
        }

        bool remove_lock(EntityID id) {
            auto removed = lock_by.erase(id);
            if (removed == 0UL)
                return false;

            if (lock_by.empty())
                state = LockState::NORMAL;

            return true;
        }

        bool is_lock_by(EntityID id) {
            auto search = lock_by.find(id);
            return (search != lock_by.end());
        }

        bool is_shared_locked() {
            return state == LockState::ISHARED;
        }

        bool is_exclusive_locked() {
            return state == LockState::IEXCLUSIVE;
        }

        bool is_normal() {
            return state == LockState::NORMAL;
        }
    };

    class LockManager {
    private:
        using BasicEntityMap = map<EntityID, PermissionDeque>;
        using TableEntityMap = map<EntityID, tuple<PermissionDeque, BasicEntityMap>>;

        TableEntityMap  table_map;
        PermissionDeque db_pdeque;

    public:
        LockManager() = default;
        LockManager(const LockManager&) = delete;

    private:

        bool grant_shared_basic(EntityID id, EntityType type, int transaction_id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();

            PermissionDeque& deque = get_deque(id, type);
            bool grant_value = true;

            auto check_parent = [&grant_value, this](auto id, auto type) {
                PermissionDeque& deque = this->get_deque(id, type);
                
                grant_value = grant_value && !deque.is_exclusive_locked();
                grant_value = grant_value && !(deque.front().type == Permission::EXCLUSIVE);

                if (deque.is_normal())
                    grant_value = grant_value && (deque.back().type == Permission::SHARED && deque.back().granted);
            };

            auto lock_parent = [this](auto id, auto type, auto lock_id) {
                PermissionDeque& deque = this->get_deque(id, type);

                deque.lock_shared();
                deque.insert_lock(lock_id);
            };

            // Check if our deque is locked 
            grant_value = grant_value && !deque.front().type == Permission::EXCLUSIVE;
            grant_value = grant_value && (deque.back().type == Permission::SHARED && deque.back().granted);

            // Check if our parent is locked
            EntityID parent = id_manager.parent_of(id);
            check_parent(parent, EntityType::TABLE); // check if our direct table parent is locked
            check_parent(EntityID(1U), EntityType::DATABASE); // check if database is locked

            deque.push_back(Permission {
                .type = Permission::SHARED,
                .granted = grant_value,
                .transaction_id = transaction_id,
                .time_point = TimePoint()
            });

            lock_parent(parent, EntityType::TABLE, id);
            lock_parent(EntityID(1U), EntityType::DATABASE, id);
            return grant_value;
        }

        bool grant_shared_table(EntityID id, int transaction_id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();
                
            PermissionDeque& deque = get_deque(id, EntityType::TABLE);
            bool grant_value = true;

            auto check_parent = [&grant_value, this](auto id, auto type) {
                PermissionDeque& deque = this->get_deque(id, type);
                
                grant_value = grant_value && !deque.is_exclusive_locked();
                grant_value = grant_value && !(deque.front().type == Permission::EXCLUSIVE);

                if (deque.is_normal())
                    grant_value = grant_value && (deque.back().type == Permission::SHARED && deque.back().granted);
            };

            auto lock_parent = [this](auto id, auto type, auto lock_id) {
                PermissionDeque& deque = this->get_deque(id, type);

                deque.lock_shared();
                deque.insert_lock(lock_id);
            };

            // Check if our deque is locked 
            grant_value = grant_value && !(deque.is_exclusive_locked() || deque.is_shared_locked());
            grant_value = grant_value && !deque.front().type == Permission::EXCLUSIVE;
            grant_value = grant_value && (deque.back().type == Permission::SHARED && deque.back().granted);

            check_parent(EntityID(1U), EntityType::DATABASE);

            deque.push_back(Permission {
                .type = Permission::SHARED,
                .granted = grant_value,
                .transaction_id = transaction_id,
                .time_point = TimePoint()
            });

            lock_parent(EntityID(1U), EntityType::DATABASE, id);
            return grant_value;
        }

    public:

        bool grant_shared(EntityID id, int transaction_id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();       
            EntityType type = id_manager.type_of(id);

            switch (type) {
                case EntityType::FIELD: {
                    return grant_shared_basic(id, EntityType::FIELD, transaction_id);
                }

                case EntityType::RECORD: {
                    return grant_shared_basic(id, EntityType::RECORD, transaction_id);
                }

                case EntityType::TABLE: {
                    return grant_shared_table(id, transaction_id);
                }

                case EntityType::DATABASE: {
                    bool grant_value = true;

                    PermissionDeque& deque = db_pdeque;

                    grant_value = grant_value && !deque.is_exclusive_locked();
                    grant_value = grant_value && !deque.front().type == Permission::EXCLUSIVE;
                    grant_value = grant_value && (deque.back().type == Permission::SHARED && deque.back().granted);

                    deque.push_back(Permission {
                        .type = Permission::SHARED,
                        .granted = grant_value,
                        .transaction_id = transaction_id,
                        .time_point = TimePoint()
                    });

                    return grant_value;
                }

                case EntityType::UNKNOWN: 
                    throw runtime_error("This Entity needs to have a type");
            }
        }

    private:

        bool grant_exclusive_basic(EntityID id, EntityType type, int transaction_id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();

            PermissionDeque& deque = get_deque(id, type);
            bool grant_value = true;

            auto check_parent = [&grant_value, this](auto id, auto type) {
                PermissionDeque& deque = this->get_deque(id, type);
                
                grant_value = grant_value && !(deque.is_exclusive_locked() || deque.is_shared_locked());
                grant_value = grant_value && !(deque.front().type == Permission::EXCLUSIVE);

                if (deque.is_normal())
                    grant_value = grant_value && !(deque.back().type == Permission::SHARED && deque.back().granted);
            };

            auto lock_parent = [this](auto id, auto type, auto lock_id) {
                PermissionDeque& deque = this->get_deque(id, type);

                deque.lock_exclusive();
                deque.insert_lock(lock_id);
            };

            // Check if our deque is locked 
            grant_value = grant_value && !(deque.front().type == Permission::EXCLUSIVE);
            grant_value = grant_value &&  (deque.back().type == Permission::SHARED && deque.back().granted);

            // Check if our parent is locked
            EntityID parent = id_manager.parent_of(id);
            check_parent(parent, EntityType::TABLE); // check if our direct table parent is locked
            check_parent(EntityID(1U), EntityType::DATABASE); // check if database is locked

            deque.push_back(Permission {
                .type = Permission::EXCLUSIVE,
                .granted = grant_value,
                .transaction_id = transaction_id,
                .time_point = TimePoint()
            });

            lock_parent(parent, EntityType::TABLE, id);
            lock_parent(EntityID(1U), EntityType::DATABASE, id);
            return grant_value;
        }

        bool grant_exclusive_table(EntityID id, int transaction_id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();

            PermissionDeque& deque = get_deque(id, EntityType::TABLE);
            bool grant_value = true;

            auto check_parent = [&grant_value, this](auto id, auto type) {
                PermissionDeque& deque = this->get_deque(id, type);
                
                grant_value = grant_value && !(deque.is_exclusive_locked() || deque.is_shared_locked());
                grant_value = grant_value && !(deque.front().type == Permission::EXCLUSIVE);

                if (deque.is_normal())
                    grant_value = grant_value && !(deque.back().type == Permission::SHARED && deque.back().granted);
            };

            auto lock_parent = [this](auto id, auto type, auto lock_id) {
                PermissionDeque& deque = this->get_deque(id, type);

                deque.lock_exclusive();
                deque.insert_lock(lock_id);
            };

            grant_value = grant_value && !(deque.is_exclusive_locked() || deque.is_shared_locked());
            grant_value = grant_value && !(deque.front().type == Permission::EXCLUSIVE);
            grant_value = grant_value && !(deque.back().type == Permission::SHARED && deque.back().granted);
            grant_value = grant_value && !deque.empty();

            check_parent(EntityID(1U), EntityType::DATABASE); // check if database is locked

            deque.push_back(Permission {
                .type = Permission::EXCLUSIVE,
                .granted = grant_value,
                .transaction_id = transaction_id,
                .time_point = TimePoint()
            });

            lock_parent(EntityID(1U), EntityType::DATABASE, id);
            return grant_value;
        }

   public:

        bool grant_exclusive(EntityID id, int transaction_id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();       
            EntityType type = id_manager.type_of(id);

            switch (type) {
                case EntityType::FIELD: {
                    return grant_exclusive_basic(id, EntityType::FIELD, transaction_id);
                }

                case EntityType::RECORD: {
                    return grant_exclusive_basic(id, EntityType::RECORD, transaction_id);
                }

                case EntityType::TABLE: {
                    return grant_exclusive_table(id, transaction_id);
                }

                case EntityType::DATABASE: {
                    bool grant_value = true;

                    PermissionDeque& deque = db_pdeque;

                    grant_value = grant_value && !(deque.is_exclusive_locked() || deque.is_shared_locked());
                    grant_value = grant_value && !(deque.front().type == Permission::EXCLUSIVE);
                    grant_value = grant_value && !(deque.back().type == Permission::SHARED && deque.back().granted);
                    grant_value = grant_value && !deque.empty();

                    deque.push_back(Permission{
                        .type = Permission::EXCLUSIVE,
                        .granted = grant_value,
                        .transaction_id = transaction_id,
                        .time_point = TimePoint()
                    });

                    return grant_value;
                }

                case EntityType::UNKNOWN:
                    throw runtime_error("This Entity needs to have a type");
            }
        }

   private:

        struct TimePriority {
            EntityID  id;
            TimePoint time_point;

            bool operator<(const TimePriority& other) const {
                return time_point < other.time_point;
            }
        };

        void update_permissions() {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();
            priority_queue<TimePriority> pqueue;

            TimePriority tmp_time_priority;
            Permission   tmp_permission;

            // First check that there isn't an implicit exclusive lock on 
            // the database
            if (db_pdeque.is_exclusive_locked() || (db_pdeque.front().granted && db_pdeque.front().type == Permission::EXCLUSIVE))
                return;

            // Now lets push to a priority queue all permissions that aren't granted 
            // yet, but under certain conditions
            for (auto& [table_id, table_val]: table_map) {
                auto& tmp_table_deque = get<0>(table_val);
                auto& tmp_basic_map   = get<1>(table_val);

                // If our table has an implicit exclusive lock 
                // skip it because we can't do anything on that table
                if (tmp_table_deque.is_exclusive_locked() || (tmp_table_deque.front().granted && tmp_table_deque.front().type == Permission::EXCLUSIVE))
                    continue;

                // But if our table isn't locked we can push 
                if (tmp_table_deque.is_normal() || (tmp_table_deque.front().type == Permission::SHARED)) {
                    pqueue.push(TimePriority {
                        .id = table_id,
                        .time_point = tmp_table_deque.front().time_point
                    });
                }

                // If it isn't implicitly exclusive locked,
                // then it isn't locked or has an implicit shared lock
                for (auto& [basic_id, tmp_basic_deque]: tmp_basic_map) {

                    // If some record/field has a granted permission
                    // then we skip it
                    if (tmp_basic_deque.front().granted && tmp_basic_deque.front().type == Permission::EXCLUSIVE)
                        continue;

                    if (tmp_table_deque.is_normal() || (tmp_basic_deque.front().type == Permission::SHARED)) {
                        pqueue.push(TimePriority {
                            .id = basic_id,
                            .time_point = tmp_basic_deque.front().time_point
                        });
                    }
                }
            }
        }

        bool revoke_permission_basic(EntityID id, EntityType type) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();
            PermissionDeque& deque = get_deque(id, type);
            EntityID parent = id_manager.parent_of(id);

            if (deque.empty()) return false;

            auto& [_, val] = *get_table(parent);
            auto& table_deque = get<1>(val);

            table_deque.remove_lock(id);
            db_pdeque.remove_lock(parent);
            
            deque.pop_front();
            return true;
        }

        bool revoke_permission_table(EntityID id) {
            PermissionDeque& deque = get_deque(id, EntityType::TABLE);

            if (deque.empty()) return false;

            db_pdeque.remove_lock(id);

            deque.pop_front();

            return true;
        }

   public:

        bool revoke_permission(EntityID id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();
            EntityType type = id_manager.type_of(id);

            switch (type) {
                case EntityType::FIELD: {
                    return revoke_permission_basic(id, EntityType::FIELD);
                }

                case EntityType::RECORD: {
                    return revoke_permission_basic(id, EntityType::FIELD);
                }

                case EntityType::TABLE: {
                    return revoke_permission_table(id);
                }

                case EntityType::DATABASE: {
                }

                case EntityType::UNKNOWN:
                    throw runtime_error("This Entity needs to have a type");
            }
        }

    private:

        TableEntityMap::iterator get_table(EntityID id) {
            table_map[id];
            return table_map.find(id);
        }

        BasicEntityMap::iterator get_basic(EntityID id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();

            auto basic_map = get<1>(table_map[id_manager.parent_of(id)]);
            basic_map[id];
            return basic_map.find(id);
        }

        PermissionDeque& get_deque(EntityID id, EntityType type) {
            switch (type) {
                case EntityType::FIELD:
                case EntityType::RECORD: {
                    PermissionDeque& deque = get_basic(id)->second;
                    return deque;
                }

                case EntityType::TABLE: {
                    PermissionDeque& deque = get<0>(get_table(id)->second);
                    return deque;
                }

                case EntityType::DATABASE: {
                    return db_pdeque;
                }

                default:
                    throw runtime_error("This Entity needs to have a type");
            }
        }
    };
}
