#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "nodemanager.h"
#include "bfs_reqs_ht.h"

#define INITIAL_HT_SIZE (1 << 15)
#define NULL_IDX  UINT32_MAX
#define ATOMICITY __ATOMIC_RELAXED

/** A request key */
struct req_key {
  uint16_t a_varid;
  uint16_t b_varid;
  uint32_t a_idx;
  uint32_t b_idx;
  uint32_t c_idx;
};

union ht_key {
  req_key key;
  __int128 raw;
};

/** A bucket in the hashtable */
struct bfs_ht_bucket {
  ht_key key;
  uint32_t req_idx;
};

/** Hashtable metadata */
struct bfs_ht {
  bfs_ht_bucket *ht;
  uint32_t size;
  uint32_t elems;
  uint32_t next_idx;  // The next value to use
  uint32_t varid;     // The varid this hashtable is associated with
};

/** Hash a key */
inline uint32_t hash(const req_key &key) {
  uint32_t prime = 0x3a8f05c5;
  uint32_t result = key.a_varid;
  result ^= key.a_idx * prime;
  result ^= key.b_varid * prime;
  result ^= key.b_idx * prime;
  result ^= key.c_idx * prime;
  return result;
}

/** Given a unique (f,g,h) triple, generate a unique req_key */
inline void make_req_key(uint32_t varid,
                         const bdd_ptr_packed &f,
                         const bdd_ptr_packed &g,
                         const bdd_ptr_packed &h,
                         req_key *result) {

  if (f.varid == varid) {
    result->a_varid = g.varid;
    result->a_idx   = g.idx;
    result->b_varid = h.varid;
    result->b_idx   = h.idx;
    result->c_idx   = f.idx;
    return;
  }

  if (g.varid == varid) {
    result->a_varid = f.varid;
    result->a_idx   = f.idx;
    result->b_varid = h.varid;
    result->b_idx   = h.idx;
    result->c_idx   = g.idx;
    return;
  }

  if (h.varid == varid) {
    result->a_varid = f.varid;
    result->a_idx   = f.idx;
    result->b_varid = g.varid;
    result->b_idx   = g.idx;
    result->c_idx   = h.idx;
    return;
  }

}

/** Return a pointer to a newly-allocated bfs_ht */
bfs_ht *bfs_ht_init(uint16_t varid) {
  bfs_ht *ht = (bfs_ht *)malloc(sizeof(bfs_ht));
  ht->ht = (bfs_ht_bucket *)calloc(sizeof(bfs_ht_bucket), INITIAL_HT_SIZE);

  #pragma omp parallel for
  for (uint32_t i = 0; i < INITIAL_HT_SIZE; i++) {
    ht->ht[i].req_idx = NULL_IDX;
  }

  ht->size = INITIAL_HT_SIZE;
  ht->elems = 0;
  ht->next_idx = 0;
  ht->varid = varid;
  return ht;
}

/** Free a bfs_ht created with bfs_ht_init */
void bfs_ht_free(bfs_ht *ht) {
  free(ht->ht);
  free(ht);
}

/** Clear all elements from the hashtable */
void bfs_ht_clear(bfs_ht *ht) {
  ht->elems = 0;
  ht->next_idx = 0;
  
  #pragma omp parallel for
  for (uint32_t i = 0; i < ht->size; i++) {
    ht->ht[i].key.raw = 0;
    ht->ht[i].req_idx = NULL_IDX;
  }
}

/** Lookup or insert an element into the bfs_ht. Return the value of the counter */
int32_t bfs_ht_lookup_or_insert(bfs_ht *ht,
                                const bdd_ptr_packed &f,
                                const bdd_ptr_packed &g,
                                const bdd_ptr_packed &h) {
  // get the key 
  ht_key key;
  key.raw = 0;
  make_req_key(ht->varid, f, g, h, &key.key);
 
  // Do a probe
  for (uint32_t i = hash(key.key); ; i++) {
    i %= ht->size;
    
    // Create dummy node
    ht_key expected;
    expected.raw = 0;

    // CMPXCHG16B the struct
    bool success = __atomic_compare_exchange_n(&ht->ht[i].key.raw,
              &expected.raw, key.raw, false, ATOMICITY, ATOMICITY);
    
    // Did the write go through?
    if (success) {
      uint32_t idx = __atomic_fetch_add(&ht->next_idx, 1, ATOMICITY);
      __atomic_fetch_add(&ht->elems, 1, ATOMICITY);
      __atomic_store_n(&ht->ht[i].req_idx, idx, ATOMICITY);
      assert(idx < INT32_MAX);
      return (int32_t) idx + 1;
    }
    
    // Did we find the key?
    else if (expected.raw == key.raw) {
      uint32_t idx;
      while ((idx = __atomic_load_n(&ht->ht[i].req_idx, ATOMICITY)) == NULL_IDX); 
      assert(idx < INT32_MAX);
      return -((int32_t)idx) - 1;
    }
  }
}
