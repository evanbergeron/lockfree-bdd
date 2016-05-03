/**
 * Work queue for op nodes in bfs
 */

#include "bdd.h"
#include "op_queue.h"
#include "ring_queue.h"

/**
 * Initialize and return a pointer to an op queue.
 */
op_queue *op_queue_init(int initial_size) {
  return (op_queue *)ring_queue_init(initial_size);
}

/**
 * Insert an op node into the op queue
 */
void op_queue_enqueue(op_queue *queue, op_node *node) {
  ring_queue_enqueue((ring_queue *)queue, (void *)node);
}

/**
 * Dequeue an op node from the op queue. Return NULL if empty.
 */
op_node *op_queue_dequeue(op_queue *queue) {
  return (op_node *)ring_queue_dequeue((ring_queue *)queue);
}
