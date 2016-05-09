#include <iostream>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "nodemanager.h"
#include "bfs_reqs_ht.h"

#define INITIAL_HT_SIZE (1 << 15)
#define ATOMICITY __ATOMIC_SEQ_CST

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

/** A request value */
struct req_value {
  uint32_t version;
  uint32_t req_idx;
};

union ht_value {
  req_value value;
  uint64_t raw;
};

/** A bucket in the hashtable */
struct bfs_ht_bucket {
  ht_key key;
  ht_value value;
  uint8_t lock;       // per-bucket lock
};

/** Hashtable metadata */
struct bfs_ht {
  bfs_ht_bucket *ht;
  uint8_t is_resizing;        // A global lock on the hashtable
  uint32_t cur_num_threads;   // The current number of threads operating on the table
  uint32_t size;              // The length of ht
  uint32_t elems;             // The number of elements in the set
  uint32_t next_idx;          // The next value to use
  uint32_t version;           // The current version number
  uint32_t varid;             // The varid this hashtable is associated with
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

/** Test a lock */
inline bool blocking_wait(uint8_t *lock) {
  bool blocked = false;
  while (__atomic_load_n(lock, ATOMICITY) != 0) {
    blocked = true;
  }
  return blocked;
}

/** Lock something */
inline void blocking_lock(uint8_t *lock) {
  while (__atomic_test_and_set(lock, ATOMICITY));
}

/** Unlock something */
inline void unlock(uint8_t *lock) {
  __atomic_clear(lock, ATOMICITY);
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
  ht->is_resizing = 0;
  ht->cur_num_threads = 0;
  ht->size = INITIAL_HT_SIZE;
  ht->elems = 0;
  ht->next_idx = 0;
  ht->version = 1;  // initially, all versions are 0
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
  ht->version++;
  // We currently only support about UINT32_MAX versions
  assert(ht->version != 0);
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

  // Wait for the resize to complete
  blocking_wait(&ht->is_resizing);

  uint32_t iters = 0;

  // Do a probe
  for (uint32_t i = hash(key.key); ; i++) {
    i %= ht->size;
    iters++;

    // First, read the contents of the bucket
    {

      // Wait for this bucket to be done processing
      bool was_locked = blocking_wait(&ht->ht[i].lock);

      // Is this bucket on the current version?
      // is_current - version of the bucket is the current version
      bool is_current;
      if (was_locked) {
        is_current = true;
      } else {
        uint32_t cur_bucket_version = __atomic_load_n(
                            &ht->ht[i].value.value.version, ATOMICITY);
        is_current = cur_bucket_version == ht->version;
      }

      // The version is current. Read the key and see if it's a match
      if (is_current) {

        // Load the key
        ht_key cur_key;
        cur_key.raw = __atomic_load_n(&ht->ht[i].key.raw, ATOMICITY);

        // Keys match - return the value here
        if (cur_key.raw == key.raw) {
          uint32_t idx = __atomic_load_n(&ht->ht[i].value.value.req_idx, ATOMICITY);
      assert(idx != 0); 
          return -((int32_t)idx);
        }

        // Keys do not match - move on to the next bucket
        else {
          continue;
        }

      }

    }

    // If we make it here, we know that the bucket's version is old
    // Try to get a lock to write here.

    // Lock the bucket
    blocking_lock(&ht->ht[i].lock);

    uint32_t cur_bucket_version = __atomic_load_n(
                            &ht->ht[i].value.value.version, ATOMICITY);

    // The version is current - inspect the key
    if (cur_bucket_version == ht->version) {

      unlock(&ht->ht[i].lock);
      
      ht_key cur_key;
      cur_key.raw = __atomic_load_n(&ht->ht[i].key.raw, ATOMICITY);

      // This is the one we want
      if (cur_key.raw == key.raw) {
        uint32_t idx = __atomic_load_n(&ht->ht[i].value.value.req_idx, ATOMICITY);
      assert(idx != 0); 
        return -((int32_t)idx);
      }

      else {
        continue;
      }

    }

    // The version is old - write our contents
    // This is the critical section
    else {
      ht->ht[i].key = key;
      ht->ht[i].value.value.version = ht->version;
      uint32_t idx = __atomic_add_fetch(&ht->next_idx, 1, ATOMICITY);
      ht->ht[i].value.value.req_idx = idx;
      uint32_t elems = __atomic_add_fetch(&ht->elems, 1, ATOMICITY);
      unlock(&ht->ht[i].lock);
      assert(idx != 0); 
      return (int32_t)idx;

    }


  }
}
