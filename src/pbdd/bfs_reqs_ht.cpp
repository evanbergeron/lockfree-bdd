#include <assert.h>
#include <iostream>
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

struct req_versioned_key {
  uint64_t hi;    // The highest-order bit is the version bit
  uint64_t lo;
};

union ht_key {
  req_key key;
  req_versioned_key v_raw;
  __int128 raw;
};

/** A request value (idx into the queue) */
struct req_value {
  uint32_t version;
  uint32_t req_idx;
};

union ht_value {
  req_value value;
  uint64_t raw;     // higher 32 bits are version
};

/** A bucket in the hashtable */
struct bfs_ht_bucket {
  ht_key key;
  ht_value value;
};

/** Hashtable metadata */
struct bfs_ht {
  bfs_ht_bucket *ht;
  uint32_t size;
  uint32_t elems;
  uint32_t next_idx;  // The next value to use
  uint32_t varid;     // The varid this hashtable is associated with
  uint8_t version;    // 0 or 1
};

/** Determine if cur_key has the current version */
inline bool is_old_version(ht_key key, uint8_t cur_version) {
  uint64_t mask = 0x80000000;
  uint8_t version = (uint8_t)((key.v_raw.hi & mask) >> (64-1));
  return version != cur_version;
}

/** Determine if value has the current version */
inline bool is_old_version(ht_value value, uint8_t cur_version) {
  return value.value.version != cur_version;
}

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
inline ht_key make_req_key(uint32_t varid,
                         uint8_t version,
                         const bdd_ptr_packed &f,
                         const bdd_ptr_packed &g,
                         const bdd_ptr_packed &h) {
  ht_key result;

  if (f.varid == varid) {
    result.key.a_varid = g.varid;
    result.key.a_idx   = g.idx;
    result.key.b_varid = h.varid;
    result.key.b_idx   = h.idx;
    result.key.c_idx   = f.idx;
  }

  else if (g.varid == varid) {
    result.key.a_varid = f.varid;
    result.key.a_idx   = f.idx;
    result.key.b_varid = h.varid;
    result.key.b_idx   = h.idx;
    result.key.c_idx   = g.idx;
  }

  else if (h.varid == varid) {
    result.key.a_varid = f.varid;
    result.key.a_idx   = f.idx;
    result.key.b_varid = g.varid;
    result.key.b_idx   = g.idx;
    result.key.c_idx   = h.idx;
  }

  uint64_t version_mask = ((uint64_t) version) << (64-1);
  result.v_raw.hi = ((result.v_raw.hi << 1) >> 1) | version_mask;

  return result;

}

/** Return a pointer to a newly-allocated bfs_ht */
bfs_ht *bfs_ht_init(uint16_t varid) {
  bfs_ht *ht = (bfs_ht *)malloc(sizeof(bfs_ht));

  ht->ht = (bfs_ht_bucket *)calloc(sizeof(bfs_ht_bucket), INITIAL_HT_SIZE);
  ht->size = INITIAL_HT_SIZE;
  ht->elems = 0;
  ht->next_idx = 0;
  ht->varid = varid;
  ht->version = 0;  // since all versions are 0 initially

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
  ht->version = !ht->version;
}

/**
 * Lookup or insert an element into the bfs_ht.
 * Returns -idx-1 if no value was inserted (key already exists)
 * Returns idx+1 if a value was inserted (key was added)
 */
int32_t bfs_ht_lookup_or_insert(bfs_ht *ht,
                          const bdd_ptr_packed &f,
                          const bdd_ptr_packed &g,
                          const bdd_ptr_packed &h) {
  // get the key 
  ht_key key = make_req_key(ht->varid, ht->version, f, g, h);
 
  // Do a probe
  for (uint32_t i = hash(key.key); ; i++) {
    i %= ht->size;

    ht_key cur_key;
    cur_key.raw = __atomic_load_n(&ht->ht[i].key.raw, ATOMICITY);

    // This is our key, with the current version
    if (cur_key.raw == key.raw) {
      ht_value cur_value;
      do {
        cur_value.raw = __atomic_load_n(&ht->ht[i].value.raw, ATOMICITY);
      } while (is_old_version(cur_value, ht->version));
      assert(cur_value.value.req_idx < INT32_MAX);
      return -((int32_t)cur_value.value.req_idx)-1;
    }

    // We can try to write here if this slot is empty (all 0s from the calloc),
    // or if it's an old version.
    else if (cur_key.raw == 0 || is_old_version(cur_key, ht->version)) {
      ht_key expected = cur_key;    

      // CMPXCHG16B the struct
      bool success = __atomic_compare_exchange_n(&ht->ht[i].key.raw,
                &expected.raw, key.raw, false, ATOMICITY, ATOMICITY);
      
      // Did the write go through?
      if (success) {
        // Grab the next index
        uint32_t idx = __atomic_fetch_add(&ht->next_idx, 1, ATOMICITY);
        // Increment the count
        __atomic_fetch_add(&ht->elems, 1, ATOMICITY);
        // Construct the value to write (idx + version)
        ht_value value; 
        value.value.req_idx = idx;
        value.value.version = (uint32_t) ht->version;
        // Store the value
        __atomic_store_n(&ht->ht[i].value.raw, value.raw, ATOMICITY);
        assert(idx < INT32_MAX);
        return (int32_t) (idx + 1);
      }
      
      // Did we find the key?
      else if (expected.raw == key.raw) {
        ht_value cur_value;
        do {
          cur_value.raw = __atomic_load_n(&ht->ht[i].value.raw, ATOMICITY);
        } while (is_old_version(cur_value, ht->version));
        assert(cur_value.value.req_idx < INT32_MAX);
        return -((int32_t)cur_value.value.req_idx)-1;
      }

    }

  }
}
