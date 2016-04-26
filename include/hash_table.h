class HashTable {
  public:
    HashTable(int maxnodes);
    bdd_node *lookup_or_insert(int varid, bdd_node *hi, bdd_node *lo);
};
