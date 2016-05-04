/**
 * Unique table for bdd nodes and op nodes
 */

#ifndef UNIQUE_TABLE_H
#define UNIQUE_TABLE_H

#include <mutex>
#include "bdd.h"

typedef std::tuple<int, bdd_node*, bdd_node*> local_ut_key;

struct Node {
  bdd_node *data;
  Node *next;
};

struct LinkedList {
  Node *head;
};

class HashTable {
  private:
    int *arr;
    LinkedList **array;
    std::mutex *bucket_locks;
    int num_buckets;

    unsigned hash(local_ut_key k);

  public:
    HashTable(int maxnodes);
    bdd_node *lookup(int varid, bdd_node *hi, bdd_node *lo);
    void insert(bdd_node *ptr);
    bdd_node *lookup_or_insert(int varid, bdd_node *hi, bdd_node *lo);
};

#endif /* UNIQUE_TABLE_H */
