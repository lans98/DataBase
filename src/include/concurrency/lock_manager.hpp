#pragma once

#include <map>
#include <deque>
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

    struct Permission {
        enum Type : int {
            SHARED, 
            EXCLUSIVE,
        };

        Type type;
        bool granted;
        int  transaction_id;
    };

    class PermissionDeque : public deque<Permission> {
    public:

        enum LockState {
            NORMAL,
            ISHARED,
            IEXCLUSIVE,
        };

    private:

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

        bool grant_shared(EntityID id, int transaction_id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();       
            EntityType type = id_manager.type_of(id);

            bool grant_value = true;
            switch (type) {
                case EntityType::FIELD:
                case EntityType::RECORD: {
                    bool exclusive_locked = is_locked(id, type, [](auto ptype) -> bool { 
                        return ptype == Permission::EXCLUSIVE;
                    });

                    grant_value = grant_value && !exclusive_locked;

                    bool parent_xlocked = is_locked(id_manager.parent_of(id), EntityType::TABLE, [](auto ptype) -> bool {
                       return ptype == Permission::EXCLUSIVE;     
                    });

                    grant_value = grant_value && !parent_xlocked;

                    PermissionDeque& deque = find_basic(id)->second;
                    deque.push_back(Permission {
                        .type = Permission::SHARED,
                        .granted = grant_value,
                        .transaction_id = transaction_id
                    });
                }

                case EntityType::TABLE: {
                    bool exclusive_locked = is_locked(id, type, [](auto ptype) -> bool {
                        return ptype == Permission::EXCLUSIVE;
                    });

                    grant_value = grant_value && !exclusive_locked;

                    bool parent_xlocked = is_locked(EntityID(1U), EntityType::DATABASE, [](auto ptype) -> bool {
                        return ptype == Permission::EXCLUSIVE;
                    });

                    grant_value = grant_value && !parent_xlocked;
                    BasicEntityMap& childs = get<1>(find_table(id)->second);
                    for (auto& [eid, dq] : childs) {
                        if (!check_deque(dq, [](auto ptype) -> bool { 
                            return ptype == Permission::EXCLUSIVE || ptype == Permission::SHARED; 
                        })) {
                            grant_value = false;
                            break;
                        }
                    }

                    PermissionDeque& deque = get<0>(find_table(id)->second);
                    deque.push_back(Permission {
                        .type = Permission::SHARED,
                        .granted = grant_value,
                        .transaction_id = transaction_id
                    });
                }

                case EntityType::DATABASE: {
                    bool exclusive_locked = is_locked(id, type, [](auto ptype) -> bool {
                        return ptype == Permission::EXCLUSIVE;
                    });

                    grant_value = grant_value && !exclusive_locked;


                    PermissionDeque& deque = db_pdeque;
                    deque.push_back(Permission {
                       .type = Permission::SHARED,
                       .granted = grant_value,
                       .transaction_id = transaction_id
                    });
                }

                default:
                    throw runtime_error("This Entity needs to have a type");
            }

            PermissionDeque& deque = m_vars[id];

            // Check if there is lock on parent of id
            EntityID parent = id_manager.parent_of(id);
            bool id_is_database = parent == 0U && id_manager.type_of(id) == EntityType::DATABASE;
            bool grant_value = true;
            if (!id_is_database) {
                if (parent == 0U) {
                    throw runtime_error("This Entity needs to have a parent!");
                } else {
                    auto search = m_vars.find(parent);
                    if (search != m_vars.end()) {
                        auto& parent_deque = search->second;
                        if (parent_deque[0].type == Permission::EXCLUSIVE)
                            grant_value = false;
                    }
                }
            }  

            // if deque is empty we can safely push a new shared permission
            // and grant it
            if (deque.empty()) {
                deque.push_back(Permission{
                    .type = Permission::SHARED,
                    .granted = grant_value,
                    .transaction_id = transaction_id
                });

                return true;
            }

            Permission permission;

            // Check if the top permission on the deque is exclusive and granted
            // if it's true, we can push a new shared permission but we can't grant 
            // it, so we return false 
            permission = deque[0];
            grant_value = grant_value && !(permission.type == Permission::EXCLUSIVE && permission.granted);

            // Check if the last permission on the deque is shared and granted
            // if it's true depends on the result of the above condition
            permission = deque.back();
            grant_value = grant_value && (permission.type == Permission::SHARED && permission.granted);

            deque.push_back(Permission{
                .type = Permission::SHARED,
                .granted = grant_value,
                .transaction_id = transaction_id
            });

            // finally return the grant value, if it's false it means we added the
            // permission to the deque, but we didn't grant it
            return grant_value;
        }

        bool grant_exclusive(EntityID id, int transaction_id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();       

            PermissionDeque& deque = m_vars[id];

            // Check if there is lock on parent of id
            EntityID parent = id_manager.parent_of(id);
            bool id_is_database = parent == 0U && id_manager.type_of(id) == EntityType::DATABASE;
            bool grant_value = true;
            if (!id_is_database) {
                if (parent == 0U) {
                    throw runtime_error("This Entity needs to have a parent!");
                } else {
                    auto search = m_vars.find(parent);
                    if (search != m_vars.end()) {
                        auto& parent_deque = search->second;
                        if (parent_deque[0].type == Permission::EXCLUSIVE || parent_deque[0].type == Permission::SHARED)
                            grant_value = false;
                    }
                }
            } 


            // if deque is empty, we can safely push a new exclusive permission
            if (deque.empty()) {
                deque.push_back(Permission{
                    .type = Permission::EXCLUSIVE,
                    .granted = grant_value,
                    .transaction_id = transaction_id
                });

                return grant_value;
            }

            // in the other case, the deque isn't empty then there is 
            // a chain of shared permissions, or an exclusive permission 
            // on the top, in all cases, we can't grant the new permission 
            // so we just push it and return false, because it's waiting to 
            // be granted
            deque.push_back(Permission{
                .type = Permission::EXCLUSIVE,
                .granted = false,
                .transaction_id = transaction_id
            });

            return false;
        }

        // return value shows success or failure
        bool pop_permission(EntityID id, int transaction_id) {
            PermissionDeque& deque = m_vars[id];

            // pop the first permission found in the deque of the 
            // record in use and same transaction id
            for (auto it = deque.begin(); it != deque.end(); ++it) {
                if (it->transaction_id == transaction_id) {
                    deque.erase(it);
                    update_permissions(id);
                    return true;
                }
            }

            return false;
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
                    PermissionDeque& deque = find_basic(id)->second;
                    return deque;
                }

                case EntityType::TABLE: {
                    PermissionDeque& deque = get<0>(find_table(id)->second);
                    return deque;
                }

                case EntityType::DATABASE: {
                    return db_pdeque;
                }

                default:
                    throw runtime_error("This Entity needs to have a type");
            }
        }

        void update_permissions(EntityID id) {
            PermissionDeque& deque = m_vars[id];
            // if the deque is empty, early return
            // if the top permission in the deque is granted
            // then it should be either a chain of contiguous shared 
            // permissions or an unique exclusive permission, in that
            // case early return
            if (deque.empty() || deque[0].granted) return;

            // now grant the top permission in the deque 
            deque[0].granted = true;
            // iterate over the deque, searching if there is an
            // exclusive permission, there are two cases:
            // the top permission is shared and we already granted it, 
            // if the contiguous permissions are also shared we can 
            // grant them until we find an exclusive permission that breaks
            // the chain, otherwise if the top permission we granted is 
            // exclusive, when we start iterating we would enter the 'if' and
            // break the 'for'
            for (auto& permission : deque) {
                if (permission.type == Permission::EXCLUSIVE)
                    return;
                permission.granted = true;
            }
        }
    };
}
