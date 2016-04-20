#ifndef MEMO_TABLE_H
#define MEMO_TABLE_H

#include "bdd.h"

/**
 * Initialize the memoization table with initial capacity size.
 */
void memo_table_init(int capacity);

/**
 * Search the table for pre-computed results (F,G,H).
 * Returns a pointer to the bdd node if it was in the table, otherwise nullptr.
 */
bdd_node *get_result(bdd_node *F, bdd_node *G, bdd_node *H);

/**
 * Place a computed result in the memoization table (F,G,H) -> result.
 */
void put_result(bdd_node *F, bdd_node *G, bdd_node *H, bdd_node *result);

/**
 * Returns true of the results for (F,G,H) are in the table, else fase.
 */
bool contains_key(bdd_node *F, bdd_node *G, bdd_node *H);

#endif /* MEMO_TABLE_H */
