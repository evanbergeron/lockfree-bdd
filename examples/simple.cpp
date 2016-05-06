#include "bdd.h"
#include "visualize.h"

int main() {
  bdd_init(100, 100, 3);

  bdd_ptr a = ithvar(0);
  bdd_ptr b = ithvar(1);
  /* bdd_ptr c = ithvar(2); */

  bdd_ptr result = bdd_or(a, b);
  /* bdd_ptr result = bdd_or(a, bdd_and(b, c)); */
  /* bdd_node *result = bdd_or(a,b); */

  bdd_graphviz(result);

  std::vector<std::map<int, bool>> *sats = allsat(result);
  for (auto &sat : (*sats)) {
    print_sat(sat);
  }

  delete sats;

  return 0;
}
