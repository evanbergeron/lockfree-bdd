#include <cassert>
#include <stdint.h>
#include <thread>
#include "nodemanager.h"

int main() {
  uint16_t num_vars = 10;
  node_manager_init(num_vars);
  uint16_t varid = 0;

  bdd_ptr left;
  bdd_ptr right;
  bdd_ptr s = lookup_or_insert(varid, left, right);
  bdd_ptr t = lookup_or_insert(varid, left, right);

  assert(s.varid == t.varid);
  assert(s.idx == t.idx);

  bdd_ptr t1;
  bdd_ptr t2;
  bdd_ptr t3 = lookup_or_insert(varid, t1, t2);
  assert(t3.idx == lookup_or_insert(varid, t1, t2).idx);
  return 0;
}
