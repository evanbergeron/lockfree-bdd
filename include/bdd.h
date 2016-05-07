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
#define OP_XOR  3u
#define OP_NAND 4u
#define OP_NOR  5u

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
extern bdd *BDD_TRUE_ADDR;
extern bdd *BDD_FALSE_ADDR;

/**
 * Initialize the BDD package
 * nodenum - max number of nodes to use in the package
 * cachesize - max number of elements to cache
 */
void bdd_init(uint32_t nodenum, uint32_t cachesize, uint16_t max_vars);

bdd_ptr bdd_apply(bool_op op, bdd_ptr a, bdd_ptr b);

bdd_ptr bdd_and(bdd_ptr a, bdd_ptr b);
bdd_ptr bdd_or (bdd_ptr a, bdd_ptr b);
bdd_ptr bdd_not(bdd_ptr a);
bdd_ptr bdd_xor (bdd_ptr a, bdd_ptr b);
bdd_ptr bdd_nand (bdd_ptr a, bdd_ptr b);
bdd_ptr bdd_nor (bdd_ptr a, bdd_ptr b);
bdd_ptr ithvar(int i);

/* =========== Analysis Functions ========== */
int countsat(bdd_ptr b);

#endif /* BDD_H */
