/**
 * bdd.h
 * Interface to the parallel BDD package.
 */

#ifndef BDD_H
#define BDD_H

#include <map>
#include <stdint.h>
#include <vector>

typedef uint32_t bool_op;

#define OP_AND  0u
#define OP_OR   1u
#define OP_NOT  2u

struct bdd_node {
  unsigned is_forwarding:1;
  unsigned int varid:31;
  bdd_node *lo;
  bdd_node *hi;
};

struct op_node {
  bool_op op;
  bdd_node *f;
  bdd_node *g;
};

/**
 * Initialize the BDD package
 * nodenum - max number of nodes to use in the package
 * cachesize - max number of elements to cache
 */
int bdd_init(int nodenum, int cachesize);

bdd_node *bdd_apply(bdd_node *a, bdd_node *b, bool_op op);

bdd_node *bdd_and(bdd_node *a, bdd_node *b);
bdd_node *bdd_or (bdd_node *a, bdd_node *b);
bdd_node *bdd_not(bdd_node *a);
bdd_node* ithvar(int i);
void bdd_graphviz(bdd_node *root);

std::vector<std::map<int, bool>> *allsat(bdd_node *fn);
void print_sat(std::map<int, bool> &assignments);
void print_allsat(bdd_node *fn);

void print_stats();

#endif /* BDD_H */
