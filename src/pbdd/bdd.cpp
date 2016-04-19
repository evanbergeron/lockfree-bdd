#include <stdlib.h>
#include <cassert>
#include <limits>
#include <iostream>
#include <unordered_map>
#include <string>
#include "unique_table.h"
#include "memo_table.h"

#define MIN3(X,Y,Z) (X < Y ? (Z < X ? Z : X) : (Z < Y ? Z : Y))

struct bdd {
  bdd_node *root;
};

static bdd_node *BDD_TRUE;
static bdd_node *BDD_FALSE;


bdd_node *ite(bdd_node *F, bdd_node *G, bdd_node *H) {
  // base cases
  if (F == BDD_TRUE) {
    return G;
  }
  if (F == BDD_FALSE) {
    return H;
  }
  if (G == BDD_TRUE && H == BDD_FALSE) {
    return F;
  }
  // check memo table
  if (contains_key(F, G, H)) {
    return get_result(F, G, H);
  }

  int min_varid = MIN3(F->varid, G->varid, H->varid);
  bdd_node *Fv = F->varid == min_varid ? F->hi : F;
  bdd_node *Gv = G->varid == min_varid ? G->hi : G;
  bdd_node *Hv = H->varid == min_varid ? H->hi : H;
  bdd_node *Fvn = F->varid == min_varid ? F->lo : F;
  bdd_node *Gvn = G->varid == min_varid ? G->lo : G;
  bdd_node *Hvn = H->varid == min_varid ? H->lo : H;

  bdd_node *T = ite(Fv, Gv, Hv);
  bdd_node *E = ite(Fvn, Gvn, Hvn);

  if (T == E) {
    return T;
  }

  bdd_node *R = lookup_or_insert(min_varid, T, E);

  put_result(F, G, H, R);

  return R;
}

/**
 * Apply a boolean operation on a and b
 */
bdd_node *bdd_apply(bdd_node *a, bdd_node *b, bool_op op) {
  bdd_node *result;
  switch (op) {
    case AND:
      result = ite(a, b, BDD_FALSE);
      break;
    case OR:
      result = ite(a, BDD_TRUE, b);
      break;
    case NOT:
      result = ite(a, BDD_FALSE, BDD_TRUE);
      break;
  }
  return result;
}

bdd_node *bdd_and(bdd_node *a, bdd_node *b) {
  return bdd_apply(a, b, bool_op::AND);
}

bdd_node *bdd_or (bdd_node *a, bdd_node *b) {
  return bdd_apply(a, b, bool_op::OR);
}

bdd_node *bdd_not(bdd_node *a) {
  return bdd_apply(a, a, bool_op::NOT);
}

bdd_node* ithvar(int i) {
  return lookup_or_insert(i, BDD_TRUE, BDD_FALSE);
}

/**
 * Initialize the BDD package
 * nodenum - max number of nodes to use in the package
 * cachesize - max number of elements to cache
 */
int bdd_init(int maxnodes, int cachesize) {
  BDD_TRUE = (bdd_node *)malloc(sizeof(bdd_node));
  BDD_FALSE = (bdd_node *)malloc(sizeof(bdd_node));

  // TODO maybe reconsider this?
  BDD_TRUE->varid = std::numeric_limits<int>::max();
  BDD_FALSE->varid = std::numeric_limits<int>::max() - 1;

  memo_table_init(cachesize);
  unique_table_init(maxnodes);
}

std::string node_to_str(bdd_node *n) {
  if (n == BDD_TRUE) { return "TRUE"; }
  if (n == BDD_FALSE) { return "FALSE"; }
  if (n == nullptr) { return ""; }
  return std::to_string(n->varid);
}

void node_graphviz(bdd_node *node) {
  if (node == nullptr) {
    return;
  }

  std::cout << " " << node_to_str(node) <<  " -> "
            << node_to_str(node->lo)
            << " [label=lo]"
            << std::endl;

  std::cout << " " << node_to_str(node) <<  " -> "
            << node_to_str(node->hi)
            << " [label=hi]"
            << std::endl;

  if (node->lo != BDD_TRUE && node->lo != BDD_FALSE) node_graphviz(node->lo);
  if (node->hi != BDD_TRUE && node->hi != BDD_FALSE) node_graphviz(node->hi);
}

void bdd_graphviz(bdd_node *root) {
  std::cout << "digraph G {" << std::endl;
  node_graphviz(root);
  std::cout << "}" << std::endl;
}
