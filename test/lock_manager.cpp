#include <iostream>
#include <lock_manager.hpp>

using namespace std;
using namespace lock_manager;

int main() {
  LockManager lm;
  lm.grant_shared("x", 1);
  lm.grant_shared("x", 2);
  lm.grant_exclusive("x", 1);
  lm.pop_permission("x", 1);
  lm.pop_permission("x", 2);
}
