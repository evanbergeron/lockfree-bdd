#ifndef BFS_REQS
#define BFS_REQS

#include <cstdint>
#include "nodemanager.h"
#include "bdd.h"

/**
 * Variable requests for all levels.
 * bdd_ptr can point to a specific request.
 */
struct var_reqs {
  req *requests;      // Array of requests
  uint32_t length;    // Length of the array
};

/**
 * A single ITE request. ITE(f,g,h), with the result
 * pre-allocated as result
 */
struct req {
  bdd_ptr_packed f;
  bdd_ptr_packed g;
  bdd_ptr_packed h;
  bdd result;
};

/** Convert a bfs request pointer to a C pointer */
req *reqptr2cptr(bdd_ptr &ptr);

/** Convert a bfs request pointer to a pointer to the result bdd node */
bddptr reqptr2resptr(bdd_ptr &ptr);

/** Initialize the BDD queues */
void bfs_reqs_init(uint16_t numvars);

/** Free the BDD queues */
void bfs_reqs_free();

/** Get a cannonical pointer to a request */
bdd_ptr bfs_reqs_lookup_or_insert(bdd_ptr f, bdd_ptr g, bdd_ptr h);

#endif /* BFS_REQS */
