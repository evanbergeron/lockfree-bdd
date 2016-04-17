#include <stdlib.h>
#include "unique_table.h"
#include "memo_table.h"


#define MIN3(X,Y,Z) (X < Y ? (Z < X ? Z : X) : (Z < Y ? Z : Y))

struct bdd_node {
  int varid;
  bdd_node *lo;
  bdd_node *hi;
};

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
 * Initialize the BDD package
 * nodenum - max number of nodes to use in the package
 * cachesize - max number of elements to cache
 */
int bdd_init(int maxnodes, int cachesize) {
  BDD_TRUE = (bdd_node *)malloc(sizeof(bdd_node)); 
  BDD_FALSE = (bdd_node *)malloc(sizeof(bdd_node)); 
}

