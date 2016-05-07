#include <stdlib.h>
#include <stdint.h>
#include "bfs_reqs.h"

#define INITIAL_CHAINSIZE 1024u


/** All the requests */
all_reqs requests;

/** Convert a bfs request pointer to a C pointer */
inline req *reqptr2cptr(req_ptr &ptr) {
  return &requests.reqs[ptr.varid].requests[ptr.idx];
}

/** Convert a varid and idx to a C pointer */
inline req *varididx2cptr(uint16_t varid, uint32_t idx) {
  return &requests.reqs[varid].requests[idx];
}

/** Initialize the BDD queues */
void bfs_reqs_init(uint16_t numvars) {
  requests.numvars = numvars;
  requests.reqs = (var_reqs *)calloc(sizeof(var_reqs), numvars);
  for (uint16_t i = 0; i < numvars; i++) {
    requests.reqs[i].capacity = INITIAL_CHAINSIZE;
    requests.reqs[i].numnodes = 0u;
    requests.reqs[i].requests = (req *)calloc(sizeof(req), INITIAL_CHAINSIZE);
  }
}

/** Free the BDD queues */
void bfs_reqs_free() {
  for (uint16_t i = 0; i < requests.numvars; i++) {
    free(requests.reqs[i].requests);
  }
  free(requests.reqs);
}

/** Get a cannonical pointer to a request */
req_ptr bfs_reqs_lookup_or_insert(bdd_ptr f, bdd_ptr g, bdd_ptr h);
