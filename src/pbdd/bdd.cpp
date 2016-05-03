#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "bdd.h"
#include "hash_table.h"
#include "memo_table.h"
#include "r_queue.h"
#include "op_queue.h"
#include "unique_table.h"

#define MIN3(X,Y,Z) (X < Y ? (Z < X ? Z : X) : (Z < Y ? Z : Y))
#define OP_QUEUE_INITIAL_SIZE 16
#define R_QUEUE_INITIAL_SIZE  16

static HashTable *uni;
static int num_varids;
static op_queue **op_queues;
static r_queue **r_queues;

static bdd_node *BDD_TRUE;
static bdd_node *BDD_FALSE;

bdd_node *bfs_op_terminal(bool_op op, bdd_node *f, bdd_node *g) {
  switch (op) {
    case OP_AND:
      if (f == BDD_FALSE || g == BDD_FALSE) {
        return BDD_FALSE;
      }
      if (f == g) { // TODO double check
        return f;
      }
      break;
    case OP_OR:
      if (f == BDD_TRUE || g == BDD_TRUE) {
        return BDD_TRUE;
      }
      if (f == g) { // TODO double check
        return f;
      }
      break;
    case OP_NOT:
      assert(false);
      break;
    default:
      return nullptr;

  }
  return nullptr;
}

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

  bdd_node *T;
  bdd_node *E;
  T = ite(Fv, Gv, Hv);
  E = ite(Fvn, Gvn, Hvn);

  if (T == E) {
    return T;
  }

  bdd_node *R = uni->lookup_or_insert(min_varid, T, E);

  put_result(F, G, H, R);

  return R;
}

void bfs_reduce() {
  for (int varid = num_varids - 1; varid >= 0; varid--) {
    bdd_node *to_reduce;
    while ((to_reduce = r_queue_dequeue(r_queues[varid])) != nullptr) {
      assert(to_reduce->is_forwarding == false);
      if (to_reduce->lo->is_forwarding) {
        to_reduce->lo = to_reduce->lo->lo;
      }
      if (to_reduce->hi->is_forwarding) {
        to_reduce->hi = to_reduce->hi->hi;
      }
      if (to_reduce->hi == to_reduce->lo) {
        to_reduce->is_forwarding = true;
        return;
      }
      bdd_node *already_there;
      if ((already_there = uni->lookup(to_reduce->varid,
                                       to_reduce->hi,
                                       to_reduce->lo)) != nullptr) {
        // already in unique table
        to_reduce->lo = already_there;
        to_reduce->hi = already_there;
        to_reduce->is_forwarding = true;
        return;
      } else {
        // TODO should we be inserting the actual pointer into unique table?
        // lookup_or_insert makes a new allocation
        lookup_or_insert(to_reduce->varid, to_reduce->hi, to_reduce->lo);
      }

    }
  }
}

void bfs_sift(int varid) {
  op_node *next_op;
  while ((next_op = op_queue_dequeue(op_queues[varid])) != nullptr) {

    // Handle lo case
    // TODO this should be a lookup_or_insert

    bool_op lo_op = next_op->op;
    bdd_node *lo_f = (next_op->f->varid == varid) ? next_op->f->lo : next_op->f;
    bdd_node *lo_g = (next_op->g->varid == varid) ? next_op->g->lo : next_op->g;

    bdd_node *lo_terminal = bfs_op_terminal(lo_op, lo_f, lo_g);

    op_node *op_lo;
    if (lo_terminal != nullptr) {
      // TODO this is tricky
      op_lo = (op_node*)lo_terminal;
    } else {
      op_lo = (op_node*)malloc(sizeof(op_node*));
      op_lo->op = lo_op;
      op_lo->f = lo_f;
      op_lo->g = lo_g;
      int lo_min_varid = std::min(op_lo->f->varid, op_lo->g->varid);
      op_queue_enqueue(op_queues[lo_min_varid], op_lo);
    }

    // Handle hi case

    bool_op hi_op = next_op->op;
    bdd_node *hi_f = (next_op->f->varid == varid) ? next_op->f->hi : next_op->f;
    bdd_node *hi_g = (next_op->g->varid == varid) ? next_op->g->hi : next_op->g;

    bdd_node *hi_terminal = bfs_op_terminal(hi_op, hi_f, hi_g);

    // TODO this should be a lookup_or_insert
    op_node *op_hi;
    if (hi_terminal != nullptr) {
      // TODO this is tricky
      op_hi = (op_node*)hi_terminal;
    } else {
      op_hi = (op_node*)malloc(sizeof(op_node*));
      op_hi->op = hi_op;
      op_hi->f = hi_f;
      op_hi->g = hi_g;
      int hi_min_varid = std::min(op_hi->f->varid, op_hi->g->varid);
      op_queue_enqueue(op_queues[hi_min_varid], op_hi);
    }

    // TODO moar cancer
    bdd_node *new_node = (bdd_node *)next_op;
    new_node->lo = (bdd_node *)op_lo;
    new_node->hi = (bdd_node *)op_hi;
    new_node->varid = varid;
    new_node->is_forwarding = false;

    r_queue_enqueue(r_queues[varid], new_node);

    // TODO later
    // lookup_or_insert(partially_done_table, (bdd_node*)next_op);

  }
}

bdd_node *bfs_op(bool_op op, bdd_node *f, bdd_node *g) {

  bdd_node *terminal_result = bfs_op_terminal(op, f, g);
  if (terminal_result != nullptr) {
    return terminal_result;
  }

  int min_varid = std::min(f->varid, g->varid);

  // TODO change to lookup_or_insert in op node unique table
  op_node *new_op = (op_node*)malloc(sizeof(op_node));
  new_op->op = op;
  new_op->f = f;
  new_op->g = g;

  op_queue_enqueue(op_queues[min_varid], new_op);

  // Expansion phase
  for (int varid = min_varid; varid < num_varids; varid++) {
    bfs_sift(varid);
  }

  bfs_reduce();

  return (bdd_node*)new_op;
}

/**
 * Apply a boolean operation on a and b
 */
bdd_node *bdd_apply(bdd_node *a, bdd_node *b, bool_op op) {
  return bfs_op(op, a, b);
}

bdd_node *bdd_and(bdd_node *a, bdd_node *b) {
  return bdd_apply(a, b, OP_AND);
}

bdd_node *bdd_or(bdd_node *a, bdd_node *b) {
  return bdd_apply(a, b, OP_OR);
}

bdd_node *bdd_not(bdd_node *a) {
  return ite(a, BDD_FALSE, BDD_TRUE);
}

// TODO maybe change to next var?
bdd_node* ithvar(int i) {
  assert(i < num_varids);
  return uni->lookup_or_insert(i, BDD_TRUE, BDD_FALSE);
}


/**
 * Initialize the BDD package
 * nodenum - max number of nodes to use in the package
 * cachesize - max number of elements to cache
 */
int bdd_init(int maxnodes, int cachesize, int num_vars) {
  // Initialize constants true and false
  BDD_TRUE = (bdd_node *)malloc(sizeof(bdd_node));
  BDD_FALSE = (bdd_node *)malloc(sizeof(bdd_node));
  if (BDD_TRUE == NULL || BDD_FALSE == NULL) {
    return 1;
  }
  num_varids = num_vars;
  BDD_TRUE->varid = std::numeric_limits<int>::max();
  BDD_FALSE->varid = std::numeric_limits<int>::max() - 1;
  memo_table_init(cachesize);
  /* unique_table_init(maxnodes); */
  uni = new HashTable(maxnodes);
  op_queues = (op_queue**)malloc(sizeof(op_queue*) * num_vars); // TODO reconsider size
  for (int i = 0; i < num_vars; i++) {
    op_queues[i] = op_queue_init(OP_QUEUE_INITIAL_SIZE);
  }
  r_queues = (r_queue**)malloc(sizeof(r_queue*) * num_vars); // TODO reconsider size
  for (int i = 0; i < num_vars; i++) {
    r_queues[i] = r_queue_init(R_QUEUE_INITIAL_SIZE);
  }
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
