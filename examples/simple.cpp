#include "bdd.h"

int main() {
  bdd_init(100, 100);

  bdd_node *a = ithvar(0);
  bdd_node *b = ithvar(1);
  bdd_node *c = ithvar(2);

  bdd_node *result = bdd_or(a, bdd_and(b, c));

  std::vector<std::map<int, bool>> *sats = allsat(result);
  for (auto &sat : (*sats)) {
    print_sat(sat);
  }

  delete sats;

  return 0;
}
