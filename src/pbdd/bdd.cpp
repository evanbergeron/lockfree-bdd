#include <cassert>
#include "bdd.h"
#include "op_queue.h"

#define MIN3(X,Y,Z) (X < Y ? (Z < X ? Z : X) : (Z < Y ? Z : Y))

static bdd_ptr BDD_TRUE;
static bdd_ptr BDD_FALSE;

bdd_ptr get_hi(bdd *F) {
  return unpack_bddptr(F->hi);
}

bdd *get_lo(bdd *F) {
  return bddptr2cptr(unpack_bddptr(F->lo));
}

bdd_ptr ite(bdd_ptr f, bdd_ptr g, bdd_ptr h) {
  if (f == BDD_TRUE) { return g; }
  if (f == BDD_FALSE) { return h; }
  if (g == BDD_TRUE && h == BDD_FALSE) { return f; }

  // TODO check cache for prev result

  int min_varid = MIN3(f.varid, g.varid, h.varid);
  bdd *F = bddptr2cptr(f);
  bdd *G = bddptr2cptr(g);
  bdd *H = bddptr2cptr(h);
  bdd_ptr fv = (f.varid == min_varid) ? unpack_bddptr(F->lo) : f;
  bdd_ptr gv = (g.varid == min_varid) ? unpack_bddptr(G->lo) : g;
  bdd_ptr hv = (h.varid == min_varid) ? unpack_bddptr(H->lo) : h;
  bdd_ptr fvn = (f.varid == min_varid) ? unpack_bddptr(F->hi) : f;
  bdd_ptr gvn = (g.varid == min_varid) ? unpack_bddptr(G->hi) : g;
  bdd_ptr hvn = (h.varid == min_varid) ? unpack_bddptr(H->hi) : h;

  bdd_ptr t;
  bdd_ptr e;
  t = ite(fv, gv, hv);
  e = ite(fvn, gvn, hvn);

  if (t == e) {
    return t;
  }

  // inductively insert ourselves into the unique table
  bdd_ptr result = lookup_or_insert(min_varid, t, e);
  return result;
}

/* Just switches on op and then calls ite of the appropriate form */
bdd_ptr ite_deploy(bool_op op, bdd_ptr a, bdd_ptr b) {
  bdd_ptr result;
  result.varid = 0;
  result.idx = 0;
  switch (op) {
    case OP_AND:
      return ite(a, b, BDD_FALSE);
      break;
    case OP_OR:
      return ite(a, BDD_TRUE, b);
      break;
    default:
      assert(false);
      return result;
  }
}

bdd_ptr bfs_op(bool_op op, bdd_ptr a, bdd_ptr b) {}

bdd_ptr bdd_op(bool_op op, bdd_ptr a, bdd_ptr b) {
#ifdef DFS
  return ite_deploy(op, a, b);
#endif
  return bfs_op(op, a, b);
}

bdd_ptr bdd_apply(bool_op op, bdd_ptr a, bdd_ptr b){
  return bdd_op(op, a, b);
}

bdd_ptr ithvar(int i) {
  return lookup_or_insert(i, BDD_FALSE, BDD_TRUE);
}
