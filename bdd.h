/**
 * bdd.h
 * Interface to the parallel BDD package.
 */

#ifndef BDD_H
#define BDD_H

struct bdd_node;

/**
 * Initialize the BDD package
 * nodenum - max number of nodes to use in the package
 * cachesize - max number of elements to cache
 */
int bdd_init(int nodenum, int cachesize);


#endif /* BDD_H */
