#include <stdlib.h>
#include <stdint.h>
#include "bfs_reqs.h"
#include "nodemanager.h"

#define INITIAL_CHAINSIZE 1024u
#define RESIZE_FACTOR     2u
#define MIN3(X,Y,Z) (X < Y ? (Z < X ? Z : X) : (Z < Y ? Z : Y))


/** Helpers */
static bool reqs_equal(const bdd_ptr &f,
                const bdd_ptr &g,
                const bdd_ptr &h,
                const req &request);

/** All the requests */
all_reqs requests;

/** The termainal request signal */
bdd_ptr_packed terminal_req_signal;

/** Get a cannonical pointer to a request */
req_ptr bfs_reqs_lookup_or_insert(bdd_ptr f, bdd_ptr g, bdd_ptr h) {
  uint16_t min_varid = MIN3(f.varid, g.varid, h.varid);

  var_reqs *reqs = &requests.reqs[min_varid];

  // Check for membership
  for (uint32_t i = 0; i < reqs->numnodes; i++) {
    if (reqs_equal(f, g, h, reqs->requests[i])) {
      req_ptr result;
      result.varid = min_varid;
      result.idx = i;
      return result;
    }
  }

  uint32_t idx = __atomic_fetch_add(&reqs->numnodes, 1, __ATOMIC_CONSUME);
  uint32_t cap = __atomic_load_n(&reqs->capacity, __ATOMIC_CONSUME);

  // Not in array, insert at tail
  // First, check if we need to resize
  if (cap == idx) {
    // This is the resizing thread
    reqs->requests = (req *)realloc(reqs->requests, sizeof(req)*cap*RESIZE_FACTOR); 
    __atomic_store_n(&reqs->capacity, cap*2, __ATOMIC_CONSUME);
  } else if (cap < idx) {
    // These threads will wait until the array is resized
    while (__atomic_load_n(&reqs->capacity, __ATOMIC_CONSUME) < idx);
  }

  // Then, insert at tail
  reqs->requests[idx].f = pack_bddptr(f);
  reqs->requests[idx].g = pack_bddptr(g);
  reqs->requests[idx].h = pack_bddptr(h);
  reqs->requests[idx].result.varid = min_varid;
  // refcount is zeroed already

  // To be safe, let's fill in the lo/hi nodes with junk
  uint16_t magic_num_16 = 0xffff;
  uint32_t magic_num_32 = 0xffffffff;
  reqs->requests[idx].result.lo.varid = magic_num_16;
  reqs->requests[idx].result.hi.varid = magic_num_16;
  reqs->requests[idx].result.lo.idx = magic_num_32;
  reqs->requests[idx].result.hi.idx = magic_num_32;

  req_ptr result;
  result.varid = min_varid;
  result.idx = idx;
  return result;
}

/** Zero out the request queues */
void bfs_reqs_reset() {
  for (uint16_t i = 0; i < requests.numvars; i++) {
    requests.reqs[i].numnodes = 0u;
  }
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
  terminal_req_signal.varid = UINT16_MAX - 0xf;
  terminal_req_signal.idx = UINT32_MAX;
}

/** Free the BDD queues */
void bfs_reqs_free() {
  for (uint16_t i = 0; i < requests.numvars; i++) {
    free(requests.reqs[i].requests);
  }
  free(requests.reqs);
}

static inline bool reqs_equal(const bdd_ptr &f,
                       const bdd_ptr &g,
                       const bdd_ptr &h,
                       const req &request) {
  return f.varid == request.f.varid &&
         g.varid == request.g.varid &&
         h.varid == request.h.varid &&
         f.idx == request.f.idx &&
         g.idx == request.g.idx &&
         h.idx == request.h.idx;
}
