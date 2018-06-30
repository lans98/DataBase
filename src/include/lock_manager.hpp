#ifndef SCPPDB_LOCK_MANAGER_HPP
#define SCPPDB_LOCK_MANAGER_HPP

#include <deque>
#include <iostream>
#include <unordered_map>

#include <table.hpp>
#include <data_types.hpp>

namespace lock_manager {

  using namespace std;
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
    unordered_map<Record*, PermissionQueue> m_vars;

  public:
    LockManager() = default;
    LockManager(const LockManager&) = delete;

    bool grant_shared(Record* record, int transaction_id) {
      PermissionQueue& deque = m_vars[record];

      if (deque.empty()) {
        deque.push_back(Permission{
          .type = Permission::SHARED,
          .granted = true,
          .transaction_id = transaction_id
        });

        return true;
      }

      Permission permission;

      bool grant_value;

      // Check if the top permission on the deque is exclusive and granted
      permission = deque[0];
      grant_value = !(permission.type == Permission::EXCLUSIVE && permission.granted);

      // Check if the last permission on the deque is shared and granted
      permission = deque.back();
      grant_value = grant_value && (permission.type == Permission::SHARED && permission.granted);

      deque.push_back(Permission{
        .type = Permission::SHARED,
        .granted = grant_value,
        .transaction_id = transaction_id
      });

      return grant_value;
    }

    bool grant_exclusive(Record* record, int transaction_id) {
      PermissionQueue& deque = m_vars[record];

      if (deque.empty()) {
        deque.push_back(Permission{
          .type = Permission::EXCLUSIVE,
          .granted = true,
          .transaction_id = transaction_id
        });

        return true;
      }

      deque.push_back(Permission{
        .type = Permission::EXCLUSIVE,
        .granted = false,
        .transaction_id = transaction_id
      });

      return false;
    }

    bool pop_permission(Record* record, int transaction_id) {
      PermissionQueue& deque = m_vars[record];

      for (auto it = deque.begin(); it != deque.end(); ++it) {
        if (it->transaction_id == transaction_id) {
          deque.erase(it);
          update_permissions(record);
          return true;
        }
      }

      return false;
    }

  private:

    void update_permissions(Record* record) {
      PermissionQueue& deque = m_vars[record];
      if (deque.empty() || deque[0].granted) return;

      deque[0].granted = true;
      for (auto& permission : deque) {
        if (permission.type == Permission::EXCLUSIVE)
          return;
        permission.granted = true;
      }
    }
  };

}

#endif
