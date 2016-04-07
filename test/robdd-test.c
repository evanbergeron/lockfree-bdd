#include <stdio.h>
#include "../include/assert.h"
#include "../include/xalloc.h"
#include "../include/robdd.h"

int and(int b1, int b2) {
  return b1 * b2;
}
int or(int b1, int b2) {
  return (b1 >= b2) ? b1 : b2;
}
int xor(int b1, int b2) {
  return (b1 + b2) % 2;
}

int main () {
  int i;
  int* u = xcalloc(37,sizeof(int));
  bdd B = bdd_new(2);
  u[1] = make(B, 2, 0, 1);	/* u[1] = x2 */
  u[2] = make(B, 1, 0, u[1]);	/* u[2] = x1 /\ x2 */
  onesat(B, u[2]);
  assert(satcount(B, u[2]) == 1);
  u[3] = apply(B, &xor, u[2], 1); /* u[3] = ~(x1 /\ x2) */
  u[4] = apply(B, &and, u[2], u[3]);	/* u[4] = u[2] /\ u[3] = 0 */
  assert(satcount(B, u[4]) == 0);
  assert(u[4] == 0);
  u[5] = make(B, 2, 0, 1);	/* u[3] = x2 */
  u[6] = make(B, 1, u[5], 1);	/* u[4] = x1 \/ x2 */
  onesat(B, u[6]);
  assert(satcount(B, u[6]) == 3);
  u[7] = make(B, 1, 0, 1);	/* u[7] = x1 */
  u[8] = make(B, 2, 0, 1);	/* u[8] = x2 */
  u[9] = apply(B, &and, u[7], u[8]); 	/* u[9] = x1 /\ x2 */
  for (i = 1; i < 10; i++) {
    printf("u[%d] = %d\n", i, u[i]);
  }
  assert(u[9] == u[2]);
  // print("ROBDD of "); printint(B->num_vars);
  // print(" vars has "); printint(B->size); print(" nodes\n");
  printf("passed all tests!\n");
  return 0;
}

