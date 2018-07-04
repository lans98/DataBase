#ifndef SCPPDB_CONCURRENCY_MANAGER_HPP
#define SCPPDB_CONCURRENCY_MANAGER_HPP

#include <string>
#include <unordered_map>

#include <data_types.hpp>
#include <lock_manager.hpp>

namespace concurrency_manager {

    using namespace std;
    using namespace data_types;
    using namespace lock_manager;

    class ConcurrencyManager {
    private:
        LockManager                      m_lock_manager;
        unordered_map<Record*, DataType> m_vars;

    public:



    };

}

#endif
