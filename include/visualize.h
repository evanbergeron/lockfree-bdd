/**
 * Visualization tools for BBDs
 */

#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <map>
#include <vector>
#include "nodemanager.h"

/** Print graphviz markup */
void bdd_graphviz(bdd_ptr root);

/** Get all satifsying assignments */
std::vector<std::map<int, bool>> *allsat(bdd_ptr fn);

/** Print all satisfying assignments */
void print_allsat(bdd_ptr fn);
void print_sat(std::map<int, bool> &assignments);

#endif /* VISUALIZE_H */
