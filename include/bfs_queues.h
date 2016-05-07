#ifndef BFS_QUEUES
#define BFS_QUEUES
#include <cstdint>
#include "nodemanager.h"
#include "bdd.h"

struct var_reqs {
  uint32_t length;
  req *requests;
};

// 18 + 16 bytes
struct req {
  bdd_ptr_packed f;
  bdd_ptr_packed g;
  bdd_ptr_packed h;
  bdd result;
};

void bfs_queues_init();
void bfs_queues_free();

void new_req(bdd_ptr f, bdd_ptr g, bdd_ptr h, bdd_ptr g);

#endif /* BFS_QUEUES */
