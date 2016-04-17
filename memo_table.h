#include "bdd.h"

#ifndef MEMO_TABLE_H
#define MEMO_TABLE_H

/**
 * docs yay
 */
bdd_node *get_result(bdd_node *F, bdd_node *G, bdd_node *H);

/**
 * docs yay
 */
bdd_node *put_result(bdd_node *F, bdd_node *G, bdd_node *H, bdd_node *result);

/**
 * docs yay
 */
bdd_node *contains_key(bdd_node *F, bdd_node *G, bdd_node *H);

#endif /* MEMO_TABLE_H */
