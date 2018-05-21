#ifndef SCPPDB_LOCK_MANAGER_HPP
#define SCPPDB_LOCK_MANAGER_HPP

#include <deque>
#include <iostream>
#include <unordered_map>

#include <data_types.hpp>

namespace lock_manager {

  using namespace std;
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
    unordered_map<string, PermissionQueue> m_vars;

  public:
    LockManager() = default;
    LockManager(const LockManager&) = delete;

    bool grant_shared(const string& var_name, int transaction_id) {
      PermissionQueue& deque = m_vars[var_name];

      if (deque.empty()) {
        deque.push_back(Permission{
          .type = Permission::SHARED,
          .granted = true,
          .transaction_id = transaction_id
        });

        return true;
      }

      Permission permission;

      permission = deque[0];
      if (permission.type == Permission::EXCLUSIVE && permission.granted)
        return false;

      permission = deque.back();
      bool grant_value = permission.type == Permission::SHARED && permission.granted;
      deque.push_back(Permission{
        .type = Permission::SHARED,
        .granted = grant_value,
        .transaction_id = transaction_id
      });

      return true;
    }

    bool grant_exclusive(const string& var_name, int transaction_id) {
      PermissionQueue& deque = m_vars[var_name];

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

      return true;
    }

    bool pop_permission(const string& var_name, int transaction_id) {
      PermissionQueue& deque = m_vars[var_name];

      for (auto it = deque.begin(); it != deque.end(); ++it) {
        if (it->transaction_id == transaction_id) {
          deque.erase(it);
          update_permissions(var_name);
          return true;
        }
      }

      return false;
    }

  private:

    void update_permissions(const string& var_name) {
      PermissionQueue& deque = m_vars[var_name];
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
