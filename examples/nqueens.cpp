#include <iostream>
#include <cassert>
#include <cmath>
#include "bdd.h"
#include "visualize.h"

/*
 * Given a list of variable indices, returns a bdd node
 * representing the the or of all of them.
 */
bdd_ptr at_least_one(int *idxs, int size) {
  assert(size > 0);
  bdd_ptr result;
  for (int i = 0; i < size; i++) {
    bdd_ptr only_i = ithvar(idxs[i]);
    if (i == 0) { result = only_i; }
    else { result = bdd_or(result, only_i); }
  }
  return result;
}

/*
 * Given a list of variable indices, returns a bdd node
 * representing the boolean function that is true iff
 * at most one of the variables is true.
 */
bdd_ptr at_most_one(int *idxs, int size) {
  assert(size > 0);
  bdd_ptr exactly_one;
  for (int i = 0; i < size; i++) {
    bdd_ptr only_i = ithvar(idxs[i]);
    for (int j = 0; j < size; j++) {
      bdd_ptr not_i = ithvar(idxs[j]);
      if (j != i) {
        only_i = bdd_and(only_i, bdd_not(not_i));
      }
    }
    if (i == 0) { exactly_one = only_i; }
    else { exactly_one = bdd_or(exactly_one, only_i); }
  }
  bdd_ptr none = bdd_not(ithvar(idxs[0]));
  for (int i = 1; i < size; i++) {
    none = bdd_and(none, bdd_not(ithvar(idxs[i])));
  }
  bdd_ptr result = bdd_or(exactly_one, none);
  return result;
}

/*
 * Given a list of variable indices, returns a bdd node
 * representing the boolean function that is true iff
 * exactly one of the variables is true.
 */
bdd_ptr exactly_one(int *idxs, int size) {
  assert(size > 0);
  bdd_ptr result = bdd_and(at_most_one(idxs, size), at_least_one(idxs, size));
  return result;
}

bdd_ptr one_per_row(int n, int row) {
  int idxs[n];
  for (int i = 0; i < n; i ++) {
    idxs[i] = n * row + i;
  }
  bdd_ptr result = exactly_one(idxs, n);
  return result;
}

bdd_ptr one_per_col(int n, int col) {
  int idxs[n];
  for (int i = 0; i < n; i ++) {
    idxs[i] = col + (n * i);
  }
  bdd_ptr result = exactly_one(idxs, n);
  return result;
}

/*
 * pdiags (positive slope diagonals) are indexed from
 * the top left to the lower right. So pdiag 0 has length
 * 1 and the middle pdiag has length n.
 */
bdd_ptr at_most_one_per_pdiag(int n, int pdiag) {
  int size = -std::abs(pdiag - (n - 1)) + n;
  int idxs[size];
  int idx = 0;
  for (int i = 0; i < n * n; i++) {
    if ((i % n) + (i / n) == pdiag) {
      idxs[idx] = i; idx++;
    }
  }
  bdd_ptr result = at_most_one(idxs, size);
  return result;
}

/*
 * ndiag indexing is a bit weird. ndiag is an element
 * of the range [-(n-1), (n-1)]. -(n-1) represents the
 * top right negative slope diagonal of length 1.
 * 0 represents the middle diagonal of length n.
 */
bdd_ptr at_most_one_per_ndiag(int n, int ndiag) {
  int size = -std::abs(ndiag) + n;
  int idxs[size];
  int idx = 0;
  for (int i = 0; i < n * n; i++) {
    if ((i / n) - (i % n) == ndiag) {
      idxs[idx] = i; idx++;
    }
  }
  bdd_ptr result = at_most_one(idxs, size);
  return result;
}

bdd_ptr row_constraints(int n) {
  bdd_ptr result = one_per_row(n, 0);
  for (int row = 1; row < n; row++) {
    result = bdd_and(result, one_per_row(n, row));
  }
  return result;
}

bdd_ptr col_constraints(int n) {
  bdd_ptr result = one_per_col(n, 0);
  for (int col = 1; col < n; col++) {
    result = bdd_and(result, one_per_col(n, col));
  }
  return result;
}

bdd_ptr pdiag_constraints(int n) {
  bdd_ptr result = at_most_one_per_pdiag(n, 0);
  for (int pdiag = 1; pdiag < n + n - 1; pdiag++) {
    result = bdd_and(result, at_most_one_per_pdiag(n, pdiag));
  }
  return result;
}

bdd_ptr ndiag_constraints(int n) {
  bdd_ptr result = at_most_one_per_ndiag(n, -(n-1));
  for (int ndiag = -(n-1) + 1; ndiag <= n - 1; ndiag++) {
    result = bdd_and(result, at_most_one_per_ndiag(n, ndiag));
  }
  return result;
}

void make_board(int n) {

  // Probably big enough?
  uint32_t node_table_size = 1 << 18;
  uint32_t node_cache_size = 1 << 21;
  uint16_t num_vars = n*n+2;
  bdd_init(node_cache_size, node_cache_size, num_vars);

  /*
   * Make variables for each board cell.
   * A cell is true iff a queen is there.
   */
  for (int i = 0; i < n * n; i++)
    ithvar(i);

}

bdd_ptr nqueens(int n) {

  make_board(n);

  bdd_ptr rows = row_constraints(n);
  bdd_ptr cols = col_constraints(n);
  bdd_ptr pdiags = pdiag_constraints(n);
  bdd_ptr ndiags = ndiag_constraints(n);

  bdd_ptr result = bdd_and(rows,
         bdd_and(cols,
         bdd_and(pdiags,
                 ndiags)));
  return result;
}

int main(int argc, char **argv) {
  bdd_ptr result = nqueens(atoi(argv[1]));
  std::cout << countsat(result) << std::endl;
  return 0;
}
