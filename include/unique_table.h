/**
 * Unique table for the bdd
 */

#include <vector>
#include "bdd.h"

#ifndef UNIQUE_TABLE_H
#define UNIQUE_TABLE_H

/**
 * Lookup or insert a triple (v,lo,hi) into the unique table
 */
bdd_node *lookup_or_insert(int varid, bdd_node *lo, bdd_node *hi);

/**
 * Clear the unique table
 */
void clear_table();

/**
 * Get all the nodes in the graph
 */
std::vector<bdd_node *> all_nodes();

/**
 * Initialize the unique table
 */
void unique_table_init(int size);

#endif /* UNIQUE_TABLE_H */
