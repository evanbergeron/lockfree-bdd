#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>

#include "nodemanager.h"
#include "bdd.h"

#define INITIAL_CHAIN_SIZE 1024u
#define ATOMICITY __ATOMIC_SEQ_CST

/** Node manager structures */
union ht_bdd {
  bdd bdd_node;
  __int128 raw;
};

struct bdd_vars {
  uint16_t varid;
  uint32_t length;
  uint32_t numnodes;
  ht_bdd *bdds;
};

/** Globals */
static bdd_vars *bdds = NULL;      // The chained arrays
static uint16_t num_vars = 0u;     // Number of variables (max varid == num_vars-1)
bool keys_equal(bdd *f, bdd *g);   // Test if two bdds are the same

/** Helpers */
bdd *resize(bdd *bdd_array, size_t new_size);
uint32_t hash(ht_bdd *node);


void node_manager_print_stats() {
  for (uint16_t i = 0; i < num_vars; i++) {
    std::cout << "varid " << i << ": " << bdds[i].numnodes << std::endl;
  }
}

/** Initialize the node manager */
void node_manager_init(uint16_t init_num_vars, uint32_t chain_size) {
  num_vars = init_num_vars;

  bdds = (bdd_vars *)malloc(sizeof(bdd_vars) * num_vars);
  if (bdds == NULL) {
    exit(EXIT_FAILURE);
  }

  for (uint16_t i = 0; i < num_vars; i++) {
    bdds[i].varid = i;
    bdds[i].length = chain_size;
    bdds[i].numnodes = 0;
    bdds[i].bdds = (ht_bdd *)calloc(sizeof(ht_bdd), chain_size);
    if (bdds[i].bdds == NULL) {
      exit(EXIT_FAILURE);
    }
  }
}


/** Free the node manager */
void node_manager_free() {
  for (uint16_t i = 0; i < num_vars; i++) {
    free(bdds[i].bdds);
  }
  free(bdds);
  bdds = NULL;
  num_vars = 0u;
}


/** Lookup or insert a value */
bdd_ptr lookup_or_insert(uint16_t varid, bdd_ptr lo, bdd_ptr hi) {
  // Create a temp node to search for
  ht_bdd to_find;
  to_find.bdd_node.varid = varid;
  to_find.bdd_node.lo.varid = lo.varid;
  to_find.bdd_node.lo.idx = lo.idx;
  to_find.bdd_node.hi.varid = hi.varid;
  to_find.bdd_node.hi.idx = hi.idx;
  to_find.bdd_node.refcount = 1u;

  ht_bdd *bdd_array = bdds[varid].bdds;
  uint32_t length = bdds[varid].length;

  // Search through the array
  for (uint32_t i = hash(&to_find); ; i++) {
    i %= length;

    // Create a dummy node
    ht_bdd expected;
    expected.raw = 0;

    // CMPXCHG16B the struct
    bool success = __atomic_compare_exchange_n(&bdd_array[i].raw,
        &expected.raw, to_find.raw, false, ATOMICITY, ATOMICITY);

    if (success) {
      // Write went through
      bdd_ptr result;
      result.varid = varid;
      result.idx = i;
      bdds[varid].numnodes++;
      return result;
    } else if (keys_equal(&expected.bdd_node, &to_find.bdd_node)) {
      // Write didn't go through, someone else already put the current value here
      // Increment ref count and return
      // TODO
      /* __atomic_fetch_add(&bdd_array[i].bdd_node.refcount, 1, __ATOMIC_CONSUME); */
      bdd_ptr result;
      result.varid = varid;
      result.idx = i;
      return result;
    } else {
      // Write didn't go through, the value here is not equal
    }
  }
}

/** Hash a bdd node */
uint32_t hash(ht_bdd *node) {
  bdd *bdd_node = (bdd *)node;
  uint32_t prime = 0x3a8f05c5;
  uint32_t result = bdd_node->varid * prime;
  result ^= bdd_node->lo.varid * prime;
  result ^= bdd_node->lo.idx * prime;
  result ^= bdd_node->hi.varid * prime;
  result ^= bdd_node->hi.idx * prime;
  return result;
}

/** Return true iff f and g have values which represent the same bdd */
inline bool keys_equal(bdd *f, bdd *g) {
  return f->lo.varid == g->lo.varid &&
         f->hi.varid == g->hi.varid &&
         f->lo.idx == g->lo.idx &&
         f->hi.idx == g->hi.idx &&
         f->varid == g->varid;
}

/** Resize a bdd array chain */
bdd *resize(bdd *bdd_array, size_t new_size) {
  bdd *result = (bdd *)realloc(bdd_array, new_size);
  if (result == NULL) {
    exit(EXIT_FAILURE);
  }
  return result;
}


bdd *bddptr2cptr(bdd_ptr bdd_ref) {
  if (bdd_ref == BDD_TRUE) { return BDD_TRUE_ADDR; }
  if (bdd_ref == BDD_FALSE) { return BDD_FALSE_ADDR; }
  return (bdd *)(bdds[bdd_ref.varid].bdds + bdd_ref.idx);
}

bdd *packedbddptr2cptr(bdd_ptr_packed bdd_ref) {
  return bddptr2cptr(unpack_bddptr(bdd_ref));
}

