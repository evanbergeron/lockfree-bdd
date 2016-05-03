/**
 * Work queue for bdd nodes to be reduced
 */

#ifndef R_QUEUE_H
#define R_QUEUE_H

#include "bdd.h"

typedef struct ring_queue r_queue;

/**
 * Initialize and return a pointer to an r_queue
 */
r_queue *r_queue_init(int initial_size);

/**
 * Insert a bdd node into the r_queue
 */
void r_queue_enqueue(r_queue *queue, bdd_node *node);

/**
 * Dequeue a bdd node from the r_queue. Return NULL if empty.
 */
bdd_node *r_queue_dequeue(r_queue *queue);

#endif /* R_QUEUE_H */
