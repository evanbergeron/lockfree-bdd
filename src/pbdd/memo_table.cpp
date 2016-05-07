#include <iostream>
#include <cstdlib>
#include <cstdint>
#include "bdd.h"
#include "nodemanager.h"

#define MT_KEY_EQUAL(X,Y) (X.F == Y.F && X.G == Y.G && X.H == Y.H)

struct mt_key {
  bdd_ptr F;
  bdd_ptr G;
  bdd_ptr H;
};

struct mt_table_entry {
  bdd_ptr F;
  bdd_ptr G;
  bdd_ptr H;
  bdd_ptr value;
};

static struct MemoTable {
  mt_table_entry *table;
  int size;
  uint64_t misses;
  uint64_t hits;
} mt;

uint32_t hash(bdd_ptr &F, bdd_ptr &G, bdd_ptr &H) {

  uint32_t prime = 0x3a8f05c5;

  uint32_t varids = F.varid;
  varids << 16u;
  varids |= G.varid;

  uint32_t result = varids;
  result ^= H.varid * prime;
  result ^= F.idx * prime;
  result ^= H.idx * prime;
  result ^= G.idx * prime;

  return result;
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

bool is_empty(const mt_table_entry &entry) {
  return entry.F.varid == 0 && entry.F.idx == 0 &&
         entry.G.varid == 0 && entry.G.idx == 0 &&
         entry.H.varid == 0 && entry.H.idx == 0;
}

/**
 * Search the table for pre-computed results (F,G,H).
 * Returns a pointer to the bdd node if it was in the table, otherwise nullptr.
 */
bdd_ptr get_result(bdd_ptr F, bdd_ptr G, bdd_ptr H) {
  int idx = hash(F, G, H) % (uint32_t)mt.size;

  while (!is_empty(mt.table[idx])) {
    if (F == mt.table[idx].F &&
        G == mt.table[idx].G &&
        H == mt.table[idx].H) {
      mt.hits++;
      return mt.table[idx].value;
    }
    idx++;
    mt.misses++;
  }

  /* return nullptr; */
}

/**
 * Place a computed result in the memoization table (F,G,H) -> result.
 */
void put_result(bdd_ptr F, bdd_ptr G, bdd_ptr H, bdd_ptr result) {
  int idx = hash(F, G, H) % (uint32_t)mt.size;

  while (!is_empty(mt.table[idx])) {
    idx++;
  }

  mt.table[idx].F = F;
  mt.table[idx].G = G;
  mt.table[idx].H = H;
  mt.table[idx].value = result;
}

/**
 * Returns true of the results for (F,G,H) are in the table, else fase.
 */
bool contains_key(bdd_ptr F, bdd_ptr G, bdd_ptr H) {
  int idx = hash(F, G, H) % (uint32_t)mt.size;

  while (!is_empty(mt.table[idx])) {
    if (mt.table[idx].F == F &&
        mt.table[idx].G == G &&
        mt.table[idx].H == H) {
      return true;
    }
    idx++;
  }

  return false;
}

void print_mt_stats() {
  std::cout << "hits   " << mt.hits << std::endl;
  std::cout << "misses " << mt.misses << std::endl;
}
