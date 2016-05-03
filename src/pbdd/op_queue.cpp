/**
 * Operator queue
 * Dynamically resizing ring-array work queue for operator nodes.
 */

#include "op_queue.h"
#include <stdlib.h>
#include <string.h>

#define MIN_SIZE 8
#define RESIZE_FACTOR 2
#define QUEUE_EMPTY(Q) (Q->head == Q->next)
#define QUEUE_FULL(Q) (Q->head == (Q->next+1) % Q->size)
#define QUEUE_SIZE(Q) ((Q->head <= Q->next) ? \
                       (Q->next-Q->head) : \
                       (Q->size-Q->head+Q->next))

struct op_queue {
  op_node **queue;
  int size;   // Size of the array
  int head;   // Location of the head
  int next;   // Location of the next insertion
};

// Helper functions
void resize(op_queue *queue, int newsize);

/**
 * Initialize and return a pointer to an op queue.
 */
op_queue *op_queue_init(int initial_size) {
  // Initialize queue struct
  op_queue *queue = (op_queue *)malloc(sizeof(op_queue));

  // Initialize array
  queue->queue = (op_node **)malloc(sizeof(op_node *) * initial_size);
      
  // Initialize fields
  queue->size = initial_size;
  queue->head = 0;
  queue->next = 0;

  return queue;
}

/**
 * Insert an op node into the op queue
 */
void op_queue_enqueue(op_queue *queue, op_node *node) {
  if (QUEUE_FULL(queue)) {
    resize(queue, queue->size * RESIZE_FACTOR);
  }

  queue->queue[queue->next] = node;
  queue->next++;

  if (queue->next == queue->size) {
    queue->next = 0;
  }
}

/**
 * Dequeue an op node from the op queue. Return NULL if empty.
 */
op_node *op_queue_dequeue(op_queue *queue) {
  if (QUEUE_EMPTY(queue)) {
    return nullptr;
  }

  queue->next--;

  if (queue->next == -1) {
    queue->next = queue->size - 1;
  }

  if (QUEUE_SIZE(queue) < (queue->size / 4) && queue->size > MIN_SIZE) {
    resize(queue, std::max(queue->size / RESIZE_FACTOR, MIN_SIZE));
  }

  return queue->queue[queue->next];
}

/**
 * Resize the queue to newsize
 */
void resize(op_queue *queue, int newsize) {
  op_node **new_queue = (op_node **)malloc(sizeof(op_node *) * newsize);

  if (queue->head <= queue->next) {
    memcpy(new_queue, queue->queue + queue->head, (queue->next-queue->head)*sizeof(op_node *));
  } else {
    memcpy(new_queue, queue->queue+queue->head, (queue->size-queue->head)*sizeof(op_node *));
    memcpy(new_queue + (queue->size-queue->head), queue->queue, queue->next*sizeof(op_node *));
  }

  free(queue->queue);
  queue->queue = new_queue;
  queue->head = 0;
  queue->next = QUEUE_SIZE(queue);
  queue->size = newsize;
}

