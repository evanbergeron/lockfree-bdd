#include <mutex>
#include "hash_table.h"
#include "bdd.h"

typedef std::tuple<int, bdd_node*, bdd_node*> local_ut_key;

class OpUniqueTable {
  private:
    HashTable *internal;

  public:
    OpUniqueTable(int maxnodes);
    op_node *lookup(bool_op op, bdd_node *hi, bdd_node *lo);
    op_node *lookup_or_insert(bool_op op, bdd_node *hi, bdd_node *lo);
};
