#include <cassert>
#include <cstdint>
#include <cmath>
#include <iostream>
#include "memo_table.h"
#include "bdd.h"
#include "bfs_reqs.h"
#include "nodemanager.h"
#include "op_queue.h"

#define MIN3(X,Y,Z) (X < Y ? (Z < X ? Z : X) : (Z < Y ? Z : Y))

#define ITE_OP bf_ite
//#define ITE_OP df_ite

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

bool is_terminal(bdd_ptr f,bdd_ptr g, bdd_ptr h) {
  if (f == BDD_TRUE) { return true; }
  else if (f == BDD_FALSE) { return true; }
  else if (g == BDD_TRUE && h == BDD_FALSE) { return true; }
  return false;
}

bdd_ptr terminal_case(bdd_ptr f,bdd_ptr g, bdd_ptr h) {
  assert(is_terminal(f, g, h));
  if (f == BDD_TRUE) { return g; }
  if (f == BDD_FALSE) { return h; }
  if (g == BDD_TRUE && h == BDD_FALSE) { return f; }
}

/**
 * Expand the request nodes from top to bottom.
 * If the lo node expansion resulted in a terminal case, its g node will
 * be set to the magic bdd_ptr_packed specified in bfs_reqs.h.
 * If the hi node expansion resulted in a terminal case, its h node will
 * be set to the magic bdd_ptr_packed specified in bfs_reqs.h.
 */
void bf_ite_expand() {
  for (int varid = 0; varid < requests.numvars; varid++) {
    // TODO data parallelism
    //#pragma omp parallel for
    for (int i = 0; i < requests.reqs[varid].numnodes; i++) {

      req *cur_req = varididx2cptr(varid, i);

      bdd_ptr f = unpack_bddptr(cur_req->f);
      bdd_ptr g = unpack_bddptr(cur_req->g);
      bdd_ptr h = unpack_bddptr(cur_req->h);

      int min_varid = MIN3(f.varid, g.varid, h.varid);
      bdd_ptr fv = (f.varid == min_varid) ? get_lo(f) : f;
      bdd_ptr gv = (g.varid == min_varid) ? get_lo(g) : g;
      bdd_ptr hv = (h.varid == min_varid) ? get_lo(h) : h;
      bdd_ptr fvn = (f.varid == min_varid) ? get_hi(f) : f;
      bdd_ptr gvn = (g.varid == min_varid) ? get_hi(g) : g;
      bdd_ptr hvn = (h.varid == min_varid) ? get_hi(h) : h;

      // Handle lo case
      if (is_terminal(fv, gv, hv)) {
        cur_req->result.lo = pack_bddptr(terminal_case(fv, gv, hv));
        cur_req->g.varid = terminal_req_signal.varid;
        cur_req->g.idx = terminal_req_signal.idx;
      } else {
        cur_req->result.lo = pack_bddptr(bfs_reqs_lookup_or_insert(fv, gv, hv));
      }

      // Handle hi case
      if (is_terminal(fvn, gvn, hvn)) {
        cur_req->result.hi = pack_bddptr(terminal_case(fvn, gvn, hvn));
        cur_req->h.varid = terminal_req_signal.varid;
        cur_req->h.idx = terminal_req_signal.idx;
      } else {
        cur_req->result.hi = pack_bddptr(bfs_reqs_lookup_or_insert(fvn, gvn, hvn));
      }
    }
  }
}

bool is_forwarding(bdd *b) {
  /*
   * BDD nodes in the request table don't need refcounting.
   * We reuse this field for forwarding.
   *
   * This is a bit of a hack :P
   *
   * */
  return b->refcount == 1;
}

req *get_req_lo(req *r) {
  uint16_t varid_lo = unpack_bddptr(r->result.lo).varid;
  uint32_t idx_lo = unpack_bddptr(r->result.lo).idx;
  req *r_lo = varididx2cptr(varid_lo, idx_lo);
  return r_lo;
}

/**
 * Perform the reduction step on all bdd nodes, from bottom to top.
 * If a node's child was terminal (as specified by the g/h nodes in
 * bf_ite_expand), then leave that node alone. Otherwise, perform
 * the typical reduction step.
 */
void bf_ite_reduce() {
  for (int varid = requests.numvars - 1; varid >= 0; varid--) {
    // TODO data parallelis
    #pragma omp parallel for
    for (int i = 0; i < requests.reqs[varid].numnodes; i ++) {

      // This is the request node we're reducing right now
      req *cur_req = varididx2cptr(varid, i);

      // This is the bdd node we're reducing right now
      bdd *to_reduce = &cur_req->result;

      // Check if to_reduce's lo is terminal. If it was, then our lo
      // pointer is already filled in for us.
      if (!(cur_req->g.varid == terminal_req_signal.varid &&
            cur_req->g.idx == terminal_req_signal.idx)) {

        // Check if to_reduce's lo is forwarding
        req_ptr rp_lo = (req_ptr)unpack_bddptr(to_reduce->lo);
        bdd *to_reduce_lo = &reqptr2cptr(rp_lo)->result;
        if (is_forwarding(to_reduce_lo)) {
          to_reduce->lo.varid = to_reduce_lo->lo.varid;
          to_reduce->lo.idx = to_reduce_lo->lo.idx;
        }

      }

      // Do the same with to_reduce's hi node.
      if (!(cur_req->h.varid == terminal_req_signal.varid &&
            cur_req->h.idx == terminal_req_signal.idx)) {

        // Check if to_reduce's hi is forwarding
        req_ptr rp_hi = (req_ptr)unpack_bddptr(to_reduce->hi);
        bdd *to_reduce_hi = &reqptr2cptr(rp_hi)->result;
        if (is_forwarding(to_reduce_hi)) {
          to_reduce->hi.varid = to_reduce_hi->lo.varid;
          to_reduce->hi.idx = to_reduce_hi->lo.idx;
        }

      }

      // Are the lo/hi branches equivalent?
      if (to_reduce->lo.varid == to_reduce->hi.varid &&
          to_reduce->lo.idx == to_reduce->hi.idx) {
        // Forwarding hack
        to_reduce->refcount = 1;
        continue;
      }

      // Forward into the unique table
      bdd_ptr final_result = lookup_or_insert(to_reduce->varid,
         unpack_bddptr(to_reduce->lo),
         unpack_bddptr(to_reduce->hi));

      to_reduce->lo = pack_bddptr(final_result);
      to_reduce->hi = pack_bddptr(final_result);
      to_reduce->refcount = 1;
    }
  }
}

bdd_ptr bf_ite(bdd_ptr f, bdd_ptr g, bdd_ptr h) {
  if (is_terminal(f, g, h)) { return terminal_case(f, g, h); }
  bfs_reqs_reset();
  req_ptr initial_request = bfs_reqs_lookup_or_insert(f, g, h);
  bf_ite_expand();
  bf_ite_reduce();
  return unpack_bddptr(reqptr2cptr(initial_request)->result.lo);
}

bdd_ptr df_ite(bdd_ptr f, bdd_ptr g, bdd_ptr h) {
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
  t = df_ite(fv, gv, hv);
  e = df_ite(fvn, gvn, hvn);

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
      result = ITE_OP(a, b, BDD_FALSE);
      break;
    case OP_OR:
      result = ITE_OP(a, BDD_TRUE, b);
      break;
    case OP_NOT:
      result = ITE_OP(a, BDD_FALSE, BDD_TRUE);
      break;
    case OP_XOR:
      result = ITE_OP(a, bdd_not(b), b);
      break;
    case OP_NAND:
      result = ITE_OP(a, bdd_not(b), BDD_TRUE);
      break;
    case OP_NOR:
      result = ITE_OP(a, BDD_FALSE, bdd_not(b));
      break;
    default:
      assert(false);
      return result;
  }
  std::cout << "---" << op << "---" << std::endl;
  node_manager_print_stats();
  print_mt_stats();
  return result;
}

bdd_ptr bdd_op(bool_op op, bdd_ptr a, bdd_ptr b) {
  return ite_deploy(op, a, b);
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
  return ITE_OP(a, BDD_FALSE, BDD_TRUE);
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

void bdd_init(uint32_t chainsize, uint32_t cachesize, uint16_t num_vars) {
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

  node_manager_init(num_vars, chainsize);
  memo_table_init(cachesize);
  bfs_reqs_init(num_vars);
}

/* ========== Analysis Functions ========= */

/**
 * Return the highest varid (excluding BDD_TRUE and BDD_FALSE) seen from b.
 */
uint16_t max_varid(bdd_ptr b) {
  if (b == BDD_TRUE) { return 0; }
  if (b == BDD_FALSE) { return 1; }
  uint16_t child_max = std::max(max_varid(get_lo(b)), max_varid(get_hi(b)));
  return std::max(child_max, b.varid);
}

double __countsat_helper(bdd_ptr b, uint16_t max_depth) {
  if (b == BDD_TRUE) { return 1; }
  if (b == BDD_FALSE) { return 0; }
  bdd_ptr lo = get_lo(b);
  bdd_ptr hi = get_hi(b);
  uint16_t lo_varid = (lo == BDD_TRUE || lo == BDD_FALSE) ? max_depth+1 : lo.varid;
  uint16_t hi_varid = (hi == BDD_TRUE || hi == BDD_FALSE) ? max_depth+1 : hi.varid;
  uint16_t lo_diff = lo_varid - b.varid;
  uint16_t hi_diff = hi_varid - b.varid;
  double lo_count = __countsat_helper(get_lo(b), max_depth) * std::pow(2, lo_diff-1);
  double hi_count = __countsat_helper(get_hi(b), max_depth) * std::pow(2, hi_diff-1);
  return lo_count + hi_count;
}

double countsat(bdd_ptr b) {
  return __countsat_helper(b, max_varid(b));
}
