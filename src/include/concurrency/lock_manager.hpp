#pragma once

#include <map>
#include <deque>
#include <iostream>

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
            SHARED, EXCLUSIVE
        };

        Type type;
        bool granted;
        int  transaction_id;
    };

    using PermissionQueue = deque<Permission>;

    class LockManager {
    private:
        map<EntityID, PermissionQueue> m_vars;

    public:
        LockManager() = default;
        LockManager(const LockManager&) = delete;

        bool find(EntityID id) {
            auto search = m_vars.find(id);
            return search == m_vars.end();
        }

        bool grant_shared(EntityID id, int transaction_id) {
            EntityIDManagerInstance id_manager = EntityIDManager::get_instance();       

            PermissionQueue& deque = m_vars[id];

            // Check if there is lock on parent of id
            EntityID parent = id_manager.parent_of(id);
            bool is_database = parent == 0U && id_manager.type_of(id) == EntityType::DATABASE;
            bool grant_value = true;
            if (!is_database) {
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
            PermissionQueue& deque = m_vars[id];

            // if deque is empty, we can safely push a new exclusive permission
            if (deque.empty()) {
                deque.push_back(Permission{
                    .type = Permission::EXCLUSIVE,
                    .granted = true,
                    .transaction_id = transaction_id
                });

                return true;
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
            PermissionQueue& deque = m_vars[id];

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

        void update_permissions(EntityID id) {
            PermissionQueue& deque = m_vars[id];
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
