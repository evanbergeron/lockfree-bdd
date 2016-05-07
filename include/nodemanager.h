/**
 * Unique-node manager with lock-free O(1) lookup_or_insert operator.
 *
 * bdd_ptr structs refer to a unique BDD node. They are a replacement
 * for regular C pointers, and should be passed around by value.
 *
 * bdd structs refer to BDD nodes themselves. Unique instances of bdd
 * structs represent unique BDDs. They should only be passed around
 * by reference.
 */

#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include <stdint.h>
#include "bdd.h"

/** A reference to a specific BDD node */
struct bdd_ptr {
  uint16_t varid;
  uint32_t idx;
  inline bool operator == (const bdd_ptr &other) {
    return this->varid == other.varid && this->idx == other.idx;
  }
  inline bool operator != (const bdd_ptr &other) {
    return this->varid != other.varid || this->idx != other.idx;
  }
};

struct bdd_ptr_packed {
  uint16_t varid;
  uint32_t idx;
} __attribute__((packed));

/**
 * A BDD node. Unique allocations of this struct represent
 * distinct BDD nodes.
 */
struct bdd {
  bdd_ptr_packed lo;    // 6 bytes
  bdd_ptr_packed hi;    // 6 bytes
  uint16_t varid;       // 2 bytes
  uint16_t refcount;    // 2 bytes
};

#define BDD_EQ(f,g) (f.varid == g.varid && f.idx == g.idx)

/** Initialize the node manager with num_vars levels */
void node_manager_init(uint16_t num_vars, uint32_t chain_size);

/** Free the node manager and all of its associated nodes */
void node_manager_free();

/** Allocate and return a pointer to a new node */
bdd_ptr new_node(unsigned varid);

/** Lookup or insert a value */
bdd_ptr lookup_or_insert(uint16_t varid, bdd_ptr lo, bdd_ptr hi);

/** Covert a bdd_ptr to a C pointer */
bdd *bddptr2cptr(bdd_ptr bdd_ref);
bdd *packedbddptr2cptr(bdd_ptr_packed bdd_ref);

/** Pack/unpack bdd pointers */
bdd_ptr unpack_bddptr(bdd_ptr_packed b);
bdd_ptr_packed pack_bddptr(bdd_ptr b);

/** Get the hi/lo branches of a node */
bdd_ptr get_hi(bdd_ptr f);
bdd_ptr get_lo(bdd_ptr f);


/** Implementations of the utility functions */
inline bdd_ptr unpack_bddptr(bdd_ptr_packed b) {
  bdd_ptr result;
  result.varid = b.varid;
  result.idx = b.idx;
  return result;
}

inline bdd_ptr_packed pack_bddptr(bdd_ptr b) {
  bdd_ptr_packed result;
  result.varid = b.varid;
  result.idx = b.idx;
  return result;
}

inline bdd_ptr get_hi(bdd_ptr f) {
  return unpack_bddptr(bddptr2cptr(f)->hi);
}

inline bdd_ptr get_lo(bdd_ptr f) {
  return unpack_bddptr(bddptr2cptr(f)->lo);
}

#endif /* NODEMANAGER_H */
