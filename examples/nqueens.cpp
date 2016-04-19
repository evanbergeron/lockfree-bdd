#include <iostream>
#include <cmath>
#include "../bdd.h"

/*
 * Given a list of variable indices, returns a bdd node
 * representing the the or of all of them.
 */
bdd_node *at_least_one(int *idxs) {
  bdd_node *result;
  for (int i = 0; i < sizeof(idxs) / sizeof(*idxs); i++) {
    bdd_node *only_i = ithvar(idxs[i]);
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
bdd_node *at_most_one(int *idxs) {
  bdd_node *result;
  for (int i = 0; i < sizeof(idxs) / sizeof(*idxs); i++) {
    bdd_node *only_i = ithvar(idxs[i]);
    for (int j = 0; j < sizeof(idxs) / sizeof(*idxs); j++) {
      bdd_node *not_i = ithvar(idxs[j]);
      if (j != i) {
        only_i = bdd_and(only_i, bdd_not(not_i));
      }
    }
    if (i == 0) { result = only_i; }
    else { result = bdd_or(result, only_i); }
  }
  return result;
}

/*
 * Given a list of variable indices, returns a bdd node
 * representing the boolean function that is true iff
 * exactly one of the variables is true.
 */
bdd_node *exactly_one(int *idxs) {
  return bdd_and(at_most_one(idxs), at_least_one(idxs));
}

bdd_node *one_per_row(int n, int row) {
  int idxs[n];
  for (int i = 0; i < n; i ++) {
    idxs[i] = n * row + i;
  }
  return exactly_one(idxs);
}

bdd_node *one_per_col(int n, int col) {
  int idxs[n];
  for (int i = 0; i < n; i ++) {
    idxs[i] = col + (n * i);
  }
  return exactly_one(idxs);
}

/*
 * pdiags (positive slope diagonals) are indexed from
 * the top left to the lower right. So pdiag 0 has length
 * 1 and the middle pdiag has length n.
 */
bdd_node *at_most_one_per_pdiag(int n, int pdiag) {
  int idxs[-std::abs(pdiag - n) + n];
  int idx = 0;
  for (int i = 0; i < n * n; i++) {
    if ((i % n) + (i / n) == pdiag) {
      idxs[idx] = i; idx++;
    }
  }
  return at_most_one(idxs);
}

/*
 * ndiag indexing is a bit weird. ndiag is an element
 * of the range [-(n-1), (n-1)]. -(n-1) represents the
 * top right negative slope diagonal of length 1.
 * 0 represents the middle diagonal of length n.
 */
bdd_node *at_most_one_per_ndiag(int n, int ndiag) {
  int idxs[-std::abs(ndiag) + n];
  int idx = 0;
  for (int i = 0; i < n * n; i++) {
    if ((i / n) - (i % n) == ndiag) {
      idxs[idx] = i; idx++;
    }
  }
  return at_most_one(idxs);
}

bdd_node *row_constraints(int n) {
  bdd_node *result = one_per_row(n, 0);
  for (int row = 1; row < n; row++) {
    result = bdd_and(result, one_per_row(n, row));
  }
  return result;
}

bdd_node *col_constraints(int n) {
  bdd_node *result = one_per_col(n, 0);
  for (int col = 1; col < n; col++) {
    result = bdd_and(result, one_per_col(n, col));
  }
  return result;
}

bdd_node *pdiag_constraints(int n) {}
bdd_node *ndiag_constraints(int n) {}

void make_board(int n) {

  bdd_init(n * n * n * n, n * n * n * n);

  /*
   * Make variables for each board cell.
   * A cell is true iff a queen is there.
   */
  for (int i = 0; i < n * n; i++)
    ithvar(i);

}

void nqueens(int n) {
  make_board(n);
}

int main() {
  nqueens(8);
  return 0;
}
