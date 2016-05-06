#include <stdlib.h>
#include <stdint.h>

#include "nodemanager.h"

#define INITIAL_CHAIN_SIZE 1024u

/** Node manager structures */
union ht_bdd {
  bdd bdd_node;
  __int128 raw;
};

struct bdd_vars {
  uint16_t varid;
  uint32_t length;
  ht_bdd *bdds;
};

/** Globals */
static bdd_vars *bdds = NULL;      // The chained arrays
static uint16_t num_vars = 0u;     // Number of variables (max varid == num_vars-1)


/** Helpers */
bdd *resize(bdd *bdd_array, size_t new_size);
uint32_t hash(ht_bdd *node);


/** Initialize the node manager */
void node_manager_init(uint16_t init_num_vars) {
  num_vars = init_num_vars;

  bdds = (bdd_vars *)malloc(sizeof(bdd_vars *) * num_vars);
  if (bdds == NULL) {
    exit(EXIT_FAILURE);
  }

  for (uint16_t i = 0; i < num_vars; i++) {
    bdds[i].varid = i;
    bdds[i].length = INITIAL_CHAIN_SIZE;
    bdds[i].bdds = (ht_bdd *)malloc(sizeof(ht_bdd) * INITIAL_CHAIN_SIZE);
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


/** Allocate and return a pointer to a new node */
bdd_ptr new_node(unsigned varid) {

}


/** Covert a bdd_ptr to a C pointer */
bdd *bddptr2cptr(bdd_ptr bdd_ref) {
  return (bdd *)(bdds[bdd_ref.varid].bdds + bdd_ref.idx);
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

  // Search through the array
  ht_bdd *bdd_array = bdds[varid].bdds;
  uint32_t length = bdds[varid].length;
  for (uint32_t i = hash(&to_find); ; i++) {
    i %= length;
    ht_bdd expected;
    expected.raw = 0;
    bool success = __atomic_compare_exchange_n(bdd_array+i, &expected, to_find.raw, false, __, __);
    if (success) {
      // Write went through
      // 
    } else if () {  // keys match
      // Write didn't go through, current value is in *expected

    }
  }
}

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

/** Resize a bdd array chain */
bdd *resize(bdd *bdd_array, size_t new_size) {
  bdd *result = (bdd *)realloc(bdd_array, new_size);
  if (result == NULL) {
    exit(EXIT_FAILURE);
  }
  return result;
}
