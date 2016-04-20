#include "bdd.h"
#include <tuple>

#ifndef MEMO_TABLE_H
#define MEMO_TABLE_H

struct key_value {
  std::tuple<bdd_node*, bdd_node*, bdd_node*> key;
  bdd_node *value;
};

struct memo_table {
  int *hash_array;
  key_value *data_array;
};

void memo_table_init(int size);

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
bool contains_key(bdd_node *F, bdd_node *G, bdd_node *H);

#endif /* MEMO_TABLE_H */
