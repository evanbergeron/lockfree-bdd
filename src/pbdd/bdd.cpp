#include <stdlib.h>
#include <cassert>
#include <limits>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <map>
#include "unique_table.h"
#include "memo_table.h"
#include "hash_table.h"

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
  bdd_node *cached_result = get_result(F, G, H);
  if (cached_result != nullptr) {
    return cached_result;
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
    default:
      return NULL;
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

static HashTable *uni;

/**
 * Initialize the BDD package
 * nodenum - max number of nodes to use in the package
 * cachesize - max number of elements to cache
 */
int bdd_init(int maxnodes, int cachesize) {
  BDD_TRUE = (bdd_node *)malloc(sizeof(bdd_node));
  BDD_FALSE = (bdd_node *)malloc(sizeof(bdd_node));
  if (BDD_TRUE == NULL || BDD_FALSE == NULL) {
    return 1;
  }

  // TODO maybe reconsider this?
  BDD_TRUE->varid = std::numeric_limits<int>::max();
  BDD_FALSE->varid = std::numeric_limits<int>::max() - 1;

  memo_table_init(cachesize);
  unique_table_init(maxnodes);
  /* uni = new HashTable(maxnodes); */
  return 0;
}


void __allsat_helper(bdd_node *fn, std::vector<std::map<int, bool>> &results,
                                   std::vector<int> &path_vars,
                                   std::vector<bool> &path_vals) {
  // Base case - insert the path taken
  if (fn == BDD_TRUE) {
    std::map<int, bool> result;
    for (std::size_t i = 0; i < path_vars.size(); i++) {
      int varid = path_vars.at(i);
      bool value = path_vals.at(i);
      result.insert(std::pair<int, bool>(varid, value));
    }
    results.push_back(result);
    return;
  }

  // Base case - don't do anything
  if (fn == BDD_FALSE) {
    return;
  }

  // Follow the lo path
  path_vars.push_back(fn->varid);
  path_vals.push_back(false);
  __allsat_helper(fn->lo, results, path_vars, path_vals);
  path_vals.pop_back();

  // Follow the hi path
  path_vals.push_back(true);
  __allsat_helper(fn->hi, results, path_vars, path_vals);
  path_vals.pop_back();

  path_vars.pop_back();

  return;
}

/**
 * Return a heap-allocated vector of all satisfying assignments
 */
std::vector<std::map<int, bool>> *allsat(bdd_node *fn) {
  std::vector<std::map<int, bool>> *results = new std::vector<std::map<int, bool>>;
  std::vector<int> path_vars;
  std::vector<bool> path_vals;
  __allsat_helper(fn, *results, path_vars, path_vals);
  return results;
}

void print_sat(std::map<int, bool> &assignments) {
  if (assignments.empty()) {
    std::cout << "Empty assignments" << std::endl;
  }

  int max_var = assignments.rbegin()->first;

  for (int i = 0; i < max_var; i++) {
    if (assignments.count(i) > 0) {
      std::cout << (assignments[i] ? "1" : "0");
    } else {
      std::cout << "X";
    }
  }

  std::cout << std::endl;

  return;
}

void print_allsat(bdd_node *fn) {
  std::vector<std::map<int, bool>> *sats = allsat(fn);
  for (auto &sat : (*sats)) {
    print_sat(sat);
  }
  delete sats;
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

  std::cout << " \"" << node_to_str(node)  << "_" << (void*)(node) << "\""
            <<  " -> "
            << "\"" << node_to_str(node->lo) << "_" << (void*)(node->lo) << "\""
            << " [label=lo]"
            << std::endl;

  std::cout << " \"" << node_to_str(node)  << "_" << (void*)(node) << "\""
            <<  " -> "
            << "\"" << node_to_str(node->hi) << "_" << (void*)(node->hi) << "\""
            << " [label=hi]"
            << std::endl;

  if (node->lo != BDD_TRUE && node->lo != BDD_FALSE) node_graphviz(node->lo);
  if (node->hi != BDD_TRUE && node->hi != BDD_FALSE) node_graphviz(node->hi);
}

void bdd_graphviz(bdd_node *root) {
  std::cout << "strict digraph G {" << std::endl;
  node_graphviz(root);
  std::cout << "}" << std::endl;
}

void print_stats() {
  print_mt_stats();
}
