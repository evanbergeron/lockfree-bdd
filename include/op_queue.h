/**
 * Work queue for op nodes in bfs
 */

#include "bdd.h"

#ifndef OP_QUEUE_H
#define OP_QUEUE_H

struct op_queue;

/**
 * Initialize and return a pointer to an op queue.
 */
op_queue *op_queue_init(int initial_size);

/**
 * Insert an op node into the op queue
 */
void op_queue_enqueue(op_queue *queue, op_node *node);

/**
 * Dequeue an op node from the op queue. Return NULL if empty.
 */
op_node *op_queue_dequeue(op_queue *queue);

#endif /* OP_QUEUE_H */
