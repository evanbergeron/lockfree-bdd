#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include <stdint.h>

struct bdd_ptr {
  uint16_t varid;
  uint32_t idx;
}; 

struct bdd_ptr_packed {
  uint16_t varid;
  uint32_t idx;
} __attribute__((packed));

struct bdd {
  bdd_ptr_packed lo;    // 6 bytes
  bdd_ptr_packed hi;    // 6 bytes
  uint16_t varid;       // 2 bytes
  uint16_t refcount;    // 2 bytes
};


#define BDD_EQ(f,g) (f.varid == g.varid && f.idx == g.idx)

/** Initialize the node manager with num_vars levels */
void node_manager_init(unsigned num_vars);
void node_manager_free();

/** Allocate and return a pointer to a new node */
bdd_ptr new_node(unsigned varid);

/** Convert a bdd_ptr to a C pointer */
bdd *bddptr2cptr(bdd_ptr bdd_ref);

/** Lookup or insert a value */
bdd_ptr lookup_or_insert(uint16_t varid, bdd_ptr lo, bdd_ptr hi);


#endif /* NODEMANAGER_H */
