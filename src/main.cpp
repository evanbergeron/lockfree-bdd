#include "bdd.h"

int main() {
  bdd_init(10, 10);
  bdd_node *n0 = ithvar(0);
  bdd_node *n1 = ithvar(1);
  bdd_node *anded = bdd_and(n0, n1);
  return 0;
}
