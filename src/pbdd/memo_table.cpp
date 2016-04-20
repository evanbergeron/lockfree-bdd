#include <iostream>
#include <cstdlib>
#include <cstdint>
#include "bdd.h"

#define MT_KEY_EQUAL(X,Y) (X.F == Y.F && X.G == Y.G && X.H == Y.H)

struct mt_key {
  bdd_node *F; 
  bdd_node *G; 
  bdd_node *H; 
};

struct mt_table_entry {
  bdd_node *F;
  bdd_node *G;
  bdd_node *H;
  bdd_node *value;
};

static struct MemoTable {
  mt_table_entry *table;
  int size;
  uint64_t misses;
  uint64_t hits;
} mt;

uint32_t hash(bdd_node *F, bdd_node *G, bdd_node *H) {
  uint64_t c = 0x27d4eb2du;
  uint64_t f = (uint64_t)((uintptr_t)F);
  uint64_t g = (uint64_t)((uintptr_t)G);
  uint64_t h = (uint64_t)((uintptr_t)H);
  return (uint32_t) ((((f*c)+g)*c+h)*c);
}

/**
 * Initialize the memoization table with initial capacity size.
 */
void memo_table_init(int capacity) {
  mt.size = capacity;
  mt.table = (mt_table_entry *)std::calloc(sizeof(mt_table_entry), capacity);
  mt.misses = 0;
  mt.hits = 0;
}

/**
 * Search the table for pre-computed results (F,G,H).
 * Returns a pointer to the bdd node if it was in the table, otherwise nullptr.
 */
bdd_node *get_result(bdd_node *F, bdd_node *G, bdd_node *H) {
  int idx = hash(F, G, H) % (uint32_t)mt.size;

  if (F == mt.table[idx].F &&
      G == mt.table[idx].G &&
      H == mt.table[idx].H) {
    mt.hits++;
    return mt.table[idx].value;
  }
  mt.misses++;

  return nullptr;
}

/**
 * Place a computed result in the memoization table (F,G,H) -> result.
 */
void put_result(bdd_node *F, bdd_node *G, bdd_node *H, bdd_node *result) {
  int idx = hash(F, G, H) % (uint32_t)mt.size;
  
  mt.table[idx].F = F;
  mt.table[idx].G = G;
  mt.table[idx].H = H;
  mt.table[idx].value = result;
}

/**
 * Returns true of the results for (F,G,H) are in the table, else fase.
 */
bool contains_key(bdd_node *F, bdd_node *G, bdd_node *H) {
  int idx = hash(F, G, H) % (uint32_t)mt.size;
  return F == mt.table[idx].F &&
         G == mt.table[idx].G &&
         H == mt.table[idx].H;
}

void print_mt_stats() {
  std::cout << "hits   " << mt.hits << std::endl;
  std::cout << "misses " << mt.misses << std::endl;
}
