#include <cstddef>
#include <cstdint>
#include <tuple>
#include <unordered_map>
#include "bdd.h"

typedef std::tuple<int, bdd_node*, bdd_node*> ut_key;

/*
 * Build a hash function for ut_keys to use in unordered_map.
 * Xor's the three elements of the tuple together, treating
 * the pointers as integers.
 */
struct ut_key_hash : public std::unary_function<ut_key, std::size_t> {
  std::size_t operator()(const ut_key& k) const {
    return std::get<0>(k) ^ reinterpret_cast<std::uintptr_t>(std::get<1>(k))
            ^ reinterpret_cast<std::uintptr_t>(std::get<2>(k));
  }
};

typedef std::unordered_map<ut_key, bdd_node*, ut_key_hash> ut;

// TODO - is this on the stack or the heap?
static ut unique_table;

/*
 * Lookup or insert a triple (v,lo,hi) into the unique table
 */
bdd_node *lookup_or_insert(int varid, bdd_node *lo, bdd_node *hi) {
  const ut_key key {varid, lo, hi};
  if (unique_table.count(key) > 0) {
    return unique_table[key];
  } else {
    /* bdd_node *data = (bdd_node*)malloc(sizeof(data)); */
    bdd_node *data = (bdd_node*)malloc(sizeof(data));
    data->varid = varid;
    data->lo = lo;
    data->hi = hi;
    unique_table[key] = data;
    return data;
  }
}

/**
 * Clear the unique table
 */
void clear_table() {
  unique_table.clear();
}

/*
 * TODO - nothing to do for this current implementation
 */
void unique_table_init(int size) {
  NULL;
}
