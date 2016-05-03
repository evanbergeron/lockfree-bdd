/**
 * Work queue for bdd nodes to be reduced
 */

#include "bdd.h"
#include "r_queue.h"
#include "ring_queue.h"

/**
 * Initialize and return a pointer to an r_queue
 */
r_queue *r_queue_init(int initial_size) {
  return (r_queue *)ring_queue_init(initial_size);
}

/**
 * Insert a bdd node into the r_queue
 */
void r_queue_enqueue(r_queue *queue, bdd_node *node) {
  ring_queue_enqueue((ring_queue *)queue, (void *)node);
}

/**
 * Dequeue a bdd node from the r_queue. Return NULL if empty.
 */
bdd_node *r_queue_dequeue(r_queue *queue) {
  return (bdd_node *)ring_queue_dequeue((ring_queue *)queue);
}
