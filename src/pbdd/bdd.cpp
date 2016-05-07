#include <cassert>
#include <cstdint>
#include <cilk/cilk.h>
#include "memo_table.h"
#include "bdd.h"
#include "nodemanager.h"
#include "op_queue.h"

#define DFS

#define MIN3(X,Y,Z) (X < Y ? (Z < X ? Z : X) : (Z < Y ? Z : Y))

bdd_ptr BDD_TRUE;
bdd_ptr BDD_FALSE;
bdd *BDD_TRUE_ADDR;
bdd *BDD_FALSE_ADDR;

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
  if (contains_key(f, g, h)) { return get_result(f, g, h); }

  int min_varid = MIN3(f.varid, g.varid, h.varid);
  bdd_ptr fv = (f.varid == min_varid) ? get_lo(f) : f;
  bdd_ptr gv = (g.varid == min_varid) ? get_lo(g) : g;
  bdd_ptr hv = (h.varid == min_varid) ? get_lo(h) : h;
  bdd_ptr fvn = (f.varid == min_varid) ? get_hi(f) : f;
  bdd_ptr gvn = (g.varid == min_varid) ? get_hi(g) : g;
  bdd_ptr hvn = (h.varid == min_varid) ? get_hi(h) : h;

  bdd_ptr t;
  bdd_ptr e;
  t = cilk_spawn ite(fv, gv, hv);
  e = ite(fvn, gvn, hvn);
  cilk_sync;

  if (t == e) {
    return t;
  }

  // inductively insert ourselves into the unique table
  bdd_ptr result = lookup_or_insert(min_varid, t, e);

  put_result(f, g, h, result);

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
    case OP_NOT:
      return ite(a, BDD_FALSE, BDD_TRUE);
      break;
    case OP_XOR:
      return ite(a, bdd_not(b), b);
    case OP_NAND:
      return ite(a, bdd_not(b), BDD_TRUE);
    case OP_NOR:
      return ite(a, BDD_FALSE, bdd_not(b));
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

bdd_ptr bdd_or(bdd_ptr a, bdd_ptr b) {
  return bdd_op(OP_OR, a, b);
}

bdd_ptr bdd_and(bdd_ptr a, bdd_ptr b) {
  return bdd_op(OP_AND, a, b);
}

bdd_ptr bdd_not(bdd_ptr a) {
  return ite(a, BDD_FALSE, BDD_TRUE);
}

bdd_ptr bdd_xor(bdd_ptr a, bdd_ptr b) {
  return bdd_op(OP_XOR, a, b);
}

bdd_ptr bdd_nand(bdd_ptr a, bdd_ptr b) {
  return bdd_op(OP_NAND, a, b);
}

bdd_ptr bdd_nor(bdd_ptr a, bdd_ptr b) {
  return bdd_op(OP_NOR, a, b);
}

bdd_ptr ithvar(int i) {
  return lookup_or_insert(i, BDD_FALSE, BDD_TRUE);
}

void bdd_init(int maxnodes, int cachesize, int num_vars) {
  BDD_TRUE.varid = UINT16_MAX;
  BDD_TRUE.idx = UINT32_MAX;
  BDD_FALSE.varid = UINT16_MAX - 1;
  BDD_FALSE.idx = UINT32_MAX;

  uint32_t magic_num = 777777;

  BDD_FALSE_ADDR = (bdd *)malloc(sizeof(bdd));
  BDD_FALSE_ADDR->lo.varid = magic_num;
  BDD_FALSE_ADDR->lo.idx = magic_num;
  BDD_FALSE_ADDR->hi.varid = magic_num;
  BDD_FALSE_ADDR->hi.idx = magic_num;
  BDD_FALSE_ADDR->varid = BDD_FALSE.varid;
  BDD_FALSE_ADDR->refcount = UINT16_MAX;

  BDD_TRUE_ADDR  = (bdd *)malloc(sizeof(bdd));
  BDD_TRUE_ADDR->lo.varid = magic_num;
  BDD_TRUE_ADDR->lo.idx = magic_num;
  BDD_TRUE_ADDR->hi.varid = magic_num;
  BDD_TRUE_ADDR->hi.idx = magic_num;
  BDD_TRUE_ADDR->varid = BDD_TRUE.varid;
  BDD_TRUE_ADDR->refcount = UINT16_MAX;

  uint32_t chain_size = 1 << 18;
  node_manager_init(num_vars, chain_size);
  memo_table_init(cachesize);
}

/* ========== Analysis Functions ========= */

int countsat(bdd_ptr b) {
  if (b == BDD_TRUE) { return 1; }
  if (b == BDD_FALSE) { return 0; }
  return countsat(get_lo(b)) + countsat(get_hi(b));
}
