#include <iostream>
#include <block_manager.hpp>

using namespace std;
using namespace block_manager;

int main() {
  BlockManager bm;
  bm.grant_shared("x", 1);
  bm.grant_shared("x", 2);
  bm.grant_exclusive("x", 1);
  bm.pop_permission("x", 1);
  bm.pop_permission("x", 2);
}
