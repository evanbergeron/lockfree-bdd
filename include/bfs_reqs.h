#ifndef BFS_REQS
#define BFS_REQS

#include <cstdint>
#include "nodemanager.h"
#include "bdd.h"

typedef bdd_ptr req_ptr;

/**
 * A single ITE request. ITE(f,g,h), with the result
 * pre-allocated as result
 */
struct req {
  bdd_ptr_packed f;   // pointer to actual bdd node
  bdd_ptr_packed g;   // pointer to actual bdd node
  bdd_ptr_packed h;   // pointer to actual bdd node
  bdd result;         // temp bdd node
};

/**
 * Variable requests for an individual level.
 */
struct var_reqs {
  req *requests;      // Array of requests
  uint32_t capacity;  // Length of the array
  uint32_t numnodes;  // Number of requests in the array
};

struct all_reqs {
  var_reqs *reqs;
  uint32_t numvars;
};

extern all_reqs requests;

/** Convert a bfs request pointer to a C pointer */
req *reqptr2cptr(req_ptr &ptr);

/** Convert a varid and idx to a C pointer */
req *varididx2cptr(uint16_t varid, uint32_t idx);

/** Initialize the BDD queues */
void bfs_reqs_init(uint16_t numvars);

/** Free the BDD queues */
void bfs_reqs_free();

/** Get a cannonical pointer to a request */
req_ptr bfs_reqs_lookup_or_insert(bdd_ptr f, bdd_ptr g, bdd_ptr h);

#endif /* BFS_REQS */