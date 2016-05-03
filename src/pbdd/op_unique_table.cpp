#include "op_unique_table.h"

OpUniqueTable::OpUniqueTable(int maxnodes) {
  internal = new HashTable(maxnodes);
}

op_node *OpUniqueTable::lookup(bool_op op, bdd_node *hi, bdd_node *lo) {
  return (op_node*)internal->lookup(op, hi, lo);
}

op_node *OpUniqueTable::lookup_or_insert(bool_op op, bdd_node *hi, bdd_node *lo) {
  return (op_node*)internal->lookup_or_insert(op, hi, lo);
}
