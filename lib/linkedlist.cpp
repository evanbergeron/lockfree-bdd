#include <iostream>
#include <mutex>
#include <cassert>
#include <thread>

struct Node {
  int value;
  Node *next;
  std::mutex lock;
};

struct List {
  Node *head;
  std::mutex lock;
};

void insert(List *list, int value) {

  Node *n = new Node;
  n->value = value;

  list->lock.lock();
  if (!list->head) {
    list->head = n;
    list->lock.unlock();
    return;
  }
  list->lock.unlock();

  // Insert before head
  list->lock.lock();
  list->head->lock.lock();
  if (list->head->value > value) {

    Node *old_head = list->head;
    n->next = list->head;
    list->head = n;

    list->lock.unlock();
    old_head->lock.unlock();

    return;
  }
  list->lock.unlock();
  list->head->lock.unlock();

  list->lock.lock();
  Node *prev = list->head;
  Node *cur = list->head->next;

  // Hand-over-hand move to list->head and list->head->next
  prev->lock.lock();
  list->lock.unlock();
  if (cur) cur->lock.lock();

  while (cur) {
    if (cur->value > value)
      break;

    Node *old_prev = prev;
    prev = prev->next;
    cur = cur->next;

    // Release old prev, grab new cur
    old_prev->lock.unlock();
    if (cur) cur->lock.lock();
  }

  n->next = cur;
  prev->next = n;

  prev->lock.unlock();
  if (cur) cur->lock.unlock();

}

void remove(List *list, int value) {

  Node *prev = list->head;
  Node *cur = list->head->next;

  while (cur) {
    if (cur->value == value) {
      prev->next = cur->next;
      delete cur;
      return;
    }
    prev = prev->next;
    cur = cur->next;
  }
}

void print_list(List *list) {

  for (Node *n = list->head; n != NULL; n = n->next) {
    std::cout << n->value << ", ";
  }
  std::cout << std::endl;
}

void thread_one(List *l) {
  // insert odds backwards
  int size = 6;
  for (int i = 2 * (size - 1) + 1; i >= 0; i -= 2) {
    std::cout << "Thread 0 inserting " << i << std::endl;
    insert(l, i);
  }
}

void thread_two(List *l) {
  // insert evens forwards
  int size = 6;
  for (int i = 0; i <2 * size; i += 2) {
    std::cout << "Thread 1 inserting " << i << std::endl;
    insert(l, i);
  }
}

void assert_full(List *l) {
  int i = 0;
  int size = 6;
  for (Node *n = l->head; n != NULL; n = n->next) {
    assert(n->value == i);
    i++;
  }
  if (i < 2 * size) assert(0);
}

int main() {
  List *l = new List;
  std::thread first(thread_one, l);
  std::thread second(thread_two, l);
  first.join();
  second.join();
  print_list(l);
  assert_full(l);
  return 0;
}
