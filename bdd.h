/**
 * bdd.h
 * Interface to the parallel BDD package.
 */

#ifndef BDD_H
#define BDD_H

enum bool_op {
  AND,
  OR,
  NOT,
};

struct bdd_node {
  int varid;
  bdd_node *lo;
  bdd_node *hi;
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
bdd_node *bdd_not(bdd_node *a, bdd_node *b);
bdd_node* ithvar(int i);

#endif /* BDD_H */
