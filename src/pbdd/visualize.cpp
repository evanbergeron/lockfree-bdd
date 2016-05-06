#include <iostream>
#include <stdint.h>
#include "nodemanager.h"
#include "bdd.h"
#include "visualize.h"

// Helpers
void node_graphviz(bdd_ptr root);
std::string node_to_str(bdd_ptr n);

/**
 * Print graphviz markup
 */
void bdd_graphviz(bdd_ptr root) {
  std::cout << "strict digraph G {" << std::endl;
  node_graphviz(root);
  std::cout << "}" << std::endl;
}

/**
 * Recursively print out what a BDD looks like
 */
void node_graphviz(bdd_ptr node) {
  std::cout << " \"" << node_to_str(node) << "\""
            <<  " -> "
            << "\"" << node_to_str(get_lo(node)) << "\""
            << " [label=lo]"
            << std::endl;

  std::cout << " \"" << node_to_str(node) << "\""
            <<  " -> "
            << "\"" << node_to_str(get_hi(node)) << "\""
            << " [label=hi]"
            << std::endl;

  if (get_lo(node) != BDD_TRUE && get_lo(node) != BDD_FALSE) node_graphviz(get_lo(node));
  if (get_hi(node) != BDD_TRUE && get_hi(node) != BDD_FALSE) node_graphviz(get_hi(node));
}

/**
 * Convert a node into a string
 */
std::string node_to_str(bdd_ptr n) {
  if (n == BDD_TRUE) { return "TRUE"; }
  if (n == BDD_FALSE) { return "FALSE"; }
  return std::to_string((uint32_t)n.varid);
}

void __allsat_helper(bdd_ptr fn, std::vector<std::map<int, bool>> &results,
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
  path_vars.push_back(fn.varid);
  path_vals.push_back(false);
  __allsat_helper(get_lo(fn), results, path_vars, path_vals);
  path_vals.pop_back();

  // Follow the hi path
  path_vals.push_back(true);
  __allsat_helper(get_hi(fn), results, path_vars, path_vals);
  path_vals.pop_back();

  path_vars.pop_back();

  return;
}

/**
 * Return a heap-allocated vector of all satisfying assignments
 */
std::vector<std::map<int, bool>> *allsat(bdd_ptr fn) {
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

void print_allsat(bdd_ptr fn) {
  std::vector<std::map<int, bool>> *sats = allsat(fn);
  for (auto &sat : (*sats)) {
    print_sat(sat);
  }
  delete sats;
}
