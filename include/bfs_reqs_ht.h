/**
 * Hashtable to maintain unique entries in the BFS queue
 */

#ifndef BFS_REQS_HT
#define BFS_REQS_HT

#include "nodemanager.h"

struct bfs_ht;

/** Return a pointer to a newly-allocated bfs_ht */
bfs_ht *bfs_ht_init(uint16_t varid);

/** Free a bfs_ht created with bfs_ht_init */
void bfs_ht_free(bfs_ht *ht);

/** Lookup or insert an element into the bfs_ht. Return the value of the counter */
int32_t bfs_ht_lookup_or_insert(bfs_ht *ht,
                                 const bdd_ptr_packed &f,
                                 const bdd_ptr_packed &g,
                                 const bdd_ptr_packed &h);

/** Remove all elements from the hashtable */
void bfs_ht_clear(bfs_ht *ht);

#endif /* BFS_REQS_HT */
