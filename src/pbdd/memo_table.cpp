#include <cstddef>
#include <cstdlib>
#include <tuple>
#include "bdd.h"

#define TABLE_SIZE 100

struct key_value {
  std::tuple<bdd_node*, bdd_node*, bdd_node*> key;
  bdd_node *value;
};

struct memo_table {
  int *hash_array;
  key_value *data_array;
};

static memo_table *table;// = (memo_table*)std::malloc(sizeof(memo_table));

int calculate_hash(bdd_node *F, bdd_node *G, bdd_node* H) {
  return 0;
}

void memo_table_init(int size){
  table = (memo_table*)std::malloc(sizeof(memo_table));
  table->hash_array = (int*)std::malloc(sizeof(int) * size);
  table->data_array = (key_value*)std::malloc(sizeof(key_value) * size);
}

/**
 * docs yay
 */
bdd_node *get_result(bdd_node *F, bdd_node *G, bdd_node *H) {
  int hash = calculate_hash(F, G, H);
  int index = hash % TABLE_SIZE;
  int cur_hash = table->hash_array[index];
  if (cur_hash != hash) {
    return NULL; // TODO this is a bad design decision
  }
  std::tuple<bdd_node*, bdd_node*, bdd_node*> key {F, G, H};
  if (table->data_array[index].key == key) {
    bdd_node *result = table->data_array[index].value;
    table->hash_array[index] = hash;
    return result;
  } else {
    table->hash_array[index] = hash;
    return NULL;
  }
}

/**
 * docs yay
 */
bdd_node *put_result(bdd_node *F, bdd_node *G, bdd_node *H, bdd_node *result) {
  int hash = calculate_hash(F, G, H);
  int index = hash % TABLE_SIZE;
  int cur_hash = table->hash_array[index];
  std::tuple<bdd_node*, bdd_node*, bdd_node*> key {F, G, H};
  if (cur_hash == hash && table->data_array[index].key == key) {
    return result;
  }
  table->data_array[index].key = key;
  table->data_array[index].value = result;
  table->hash_array[index] = hash;
  return result;
}

/**
 * docs yay
 */
bool contains_key(bdd_node *F, bdd_node *G, bdd_node *H) {
  int hash = calculate_hash(F, G, H);
  int index = hash % TABLE_SIZE;
  int cur_hash = table->hash_array[index];
  std::tuple<bdd_node*, bdd_node*, bdd_node*> key {F, G, H};
  return (cur_hash == hash && table->data_array[index].key == key);
}
