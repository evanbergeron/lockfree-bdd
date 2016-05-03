/**
 * Dynamically-resizing ring queue interface
 */

#ifndef RING_QUEUE_H
#define RING_QUEUE_H

typedef struct ring_queue ring_queue;

/**
 * Initialize and return a pointer to an op queue.
 */
ring_queue *ring_queue_init(int initial_size);

/**
 * Insert an op node into the op queue
 */
void ring_queue_enqueue(ring_queue *queue, void *node);

/**
 * Dequeue an op node from the op queue. Return NULL if empty.
 */
void *ring_queue_dequeue(ring_queue *queue);

#endif /* RING_QUEUE_H */
