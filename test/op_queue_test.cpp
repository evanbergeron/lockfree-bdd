#include <iostream>
#include <cassert>
#include "op_queue.h"

void test_op_queue() {
  op_queue *q = op_queue_init(10);
  assert(op_queue_dequeue(q) == nullptr);
  op_node node;
  op_queue_enqueue(q, &node);
  assert(op_queue_dequeue(q) == &node);
  assert(op_queue_dequeue(q) == nullptr);

  op_node nodes[128];
  for (int i = 0; i < 2; i++) {
    op_queue_enqueue(q, &(nodes[i]));
  }
  for (int i = 0; i < 2; i++) {
    std::cout << i << std::endl;
    assert(op_queue_dequeue(q) == &(nodes[i]));
  }
  assert(op_queue_dequeue(q) == nullptr);
}
