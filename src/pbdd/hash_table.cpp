#include <cstdint>
#include <iostream>
#include <tuple>
#include <unordered_map>

#include "hash_table.h"

void add_to_front(LinkedList *l, Node *n) {
  if (!l->head) {
    l->head = n;
    return;
  }
  n->next = l->head;
  l->head = n;
}

bdd_node *find(LinkedList *l, int varid, bdd_node *hi, bdd_node *lo) {
  for (Node *cur = l->head; cur != NULL; cur = cur->next) {
    if (cur->data->varid == varid &&
        cur->data->hi == hi &&
        cur->data->lo == lo)
      return cur->data;
  }
  return NULL;
}

LinkedList *newLinkedList() {
  return (LinkedList*)std::malloc(sizeof(LinkedList));
}

int HashTable::hash(local_ut_key k) {
  return std::get<0>(k)
          ^ reinterpret_cast<std::uintptr_t>(std::get<1>(k))
          ^ reinterpret_cast<std::uintptr_t>(std::get<2>(k));
}

HashTable::HashTable(int maxnodes) {
  array = (LinkedList**)std::malloc(sizeof(LinkedList*) * maxnodes);
  bucket_locks = (std::mutex*)std::malloc(sizeof(std::mutex) * maxnodes);
  for (int i = 0; i < maxnodes; i++)
    array[i] = newLinkedList();
}

bdd_node *HashTable::lookup_or_insert(int varid, bdd_node *hi, bdd_node *lo) {

  local_ut_key k (varid, hi, lo);
  int idx = hash(k) % 100;

  bucket_locks[idx].lock();

  LinkedList *chain = array[idx];
  bdd_node *result;

  if ((result = find(chain, varid, hi, lo)) != NULL) {
    bucket_locks[idx].unlock();
    return result;
  }

  Node *n = (Node*)malloc(sizeof(Node));

  result = (bdd_node*)std::malloc(sizeof(bdd_node));
  result->varid = varid;
  result->hi = hi;
  result->lo = lo;

  n->data = result;
  add_to_front(array[idx], n);
  bucket_locks[idx].unlock();

  return result;
}
