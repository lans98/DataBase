#ifndef SCPPDB_CONCURRENCY_MANAGER_HPP
#define SCPPDB_CONCURRENCY_MANAGER_HPP

#include <string>
#include <unordered_map>

#include <data_types.hpp>
#include <block_manager.hpp>

namespace concurrency_manager {

  using namespace std;
  using namespace data_types;
  using namespace block_manager;

  class ConcurrencyManager {
  private:
    BlockManager m_block_manager;
    unordered_map<string, DataType> m_vars;

  public:
        

  };

}

#endif
