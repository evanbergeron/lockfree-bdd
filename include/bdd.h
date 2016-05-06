/**
 * bdd.h
 * Interface to the parallel BDD package.
 */

#ifndef BDD_H
#define BDD_H

#include "nodemanager.h"
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

extern bdd_ptr BDD_TRUE;
extern bdd_ptr BDD_FALSE;

/**
 * Initialize the BDD package
 * nodenum - max number of nodes to use in the package
 * cachesize - max number of elements to cache
 */
void bdd_init(int nodenum, int cachesize, int max_vars);

bdd_ptr bdd_apply(bool_op op, bdd_ptr a, bdd_ptr b);

bdd_ptr bdd_and(bdd_ptr a, bdd_ptr b);
bdd_ptr bdd_or (bdd_ptr a, bdd_ptr b);
bdd_ptr bdd_not(bdd_ptr a);
bdd_ptr ithvar(int i);

#endif /* BDD_H */
