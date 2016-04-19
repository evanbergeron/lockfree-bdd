#include <iostream>
#include "../bdd.h"

bdd_node *only_nth(int n) {
  return nullptr;
}

/*
 * Every row must have at least one queen.
 */
bdd_node *each_row_at_least_one_queen(int n) {

  bdd_node *at_least_one_per_row[n];
  for (int row = 0; row < n; row++) {
    int i = row * n;
    bdd_node *this_row = ithvar(i);
    for (int col = 1; col < n; col++) {
      int j = row * n + col;
      this_row = bdd_and(this_row, ithvar(j));
    }
    at_least_one_per_row[row] = this_row;
  }

  bdd_node *result = at_least_one_per_row[0];
  for (int i = 1; i < n; i++) {
    result = bdd_and(result, at_least_one_per_row[i]);
  }

  return result;
}

void make_board(int n) {

  bdd_init(n * n * n * n, n * n * n * n);

  /*
   * Make variables for each board cell.
   * A cell is true iff a queen is there.
   */
  for (int i = 0; i < n * n; i++)
    ithvar(i);

  bdd_node *all_rows = each_row_at_least_one_queen(n);
}

void nqueens(int n) {
  make_board(n);
}

int main() {
  nqueens(8);
  return 0;
}
