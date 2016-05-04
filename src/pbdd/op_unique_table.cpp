#include "op_unique_table.h"

OpUniqueTable::OpUniqueTable(int maxnodes) {
  internal = new HashTable(maxnodes);
}

op_node *OpUniqueTable::lookup(bool_op op, bdd_node *f, bdd_node *g) {
  return (op_node*)internal->lookup(op, g, f);
}

op_node *OpUniqueTable::lookup_or_insert(bool_op op, bdd_node *f, bdd_node *g) {
  return (op_node*)internal->lookup_or_insert(op, g, f);
}
