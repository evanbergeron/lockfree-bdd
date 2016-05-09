#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "bfs_reqs.h"
#include "bfs_reqs_ht.h"
#include "nodemanager.h"

#define INITIAL_CHAINSIZE 1024u
#define RESIZE_FACTOR     2u
#define MIN3(X,Y,Z) (X < Y ? (Z < X ? Z : X) : (Z < Y ? Z : Y))
#define ATOMICITY __ATOMIC_SEQ_CST


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
  int32_t ht_idx = bfs_ht_lookup_or_insert(requests.reqs[min_varid].requests_ht,
                                         pack_bddptr(f),
                                         pack_bddptr(g),
                                         pack_bddptr(h));

  // negative return means that the value was found
  if (ht_idx < 0) {
    req_ptr result;
    result.varid = min_varid;
    result.idx = (uint32_t)((-ht_idx) - 1);
    return result;
  }

  std::cout << "idx: " << reqs->capacity << " " << ht_idx << std::endl;
  assert(ht_idx != 0);

  ht_idx--;

  __atomic_fetch_add(&reqs->numnodes, 1, ATOMICITY);
  uint32_t idx = (uint32_t)ht_idx;
  uint32_t cap = __atomic_load_n(&reqs->capacity, ATOMICITY);
 
  // we're over capacity 
  if (cap <= idx) {

    // got the resize lock
    if (!__atomic_test_and_set(&reqs->requests_resize_lock, ATOMICITY)) {
      __atomic_thread_fence(ATOMICITY);

      reqs->requests = (req *)realloc(reqs->requests, sizeof(req)*cap*RESIZE_FACTOR); 
      __atomic_store_n(&reqs->capacity, cap*RESIZE_FACTOR, ATOMICITY);
      __atomic_clear(&reqs->requests_resize_lock, ATOMICITY);

      __atomic_thread_fence(ATOMICITY);
    }
    
    // did not get the resize lock
    else {
      __atomic_thread_fence(ATOMICITY);
      __atomic_thread_fence(ATOMICITY);
    }
  }
  
  // there's a resize going on now, so wait
  else if (__atomic_load_n(&reqs->requests_resize_lock, ATOMICITY) != 0) {
      __atomic_thread_fence(ATOMICITY);
      __atomic_thread_fence(ATOMICITY);
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
    bfs_ht_clear(requests.reqs[i].requests_ht);
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
    requests.reqs[i].requests_ht = bfs_ht_init(i);
    requests.reqs[i].requests_resize_lock = 0;
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
