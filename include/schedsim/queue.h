#ifndef SCHEDSIM__QUEUE_H
#define SCHEDSIM__QUEUE_H

// from /proc/sys/kernel/pid_max

#include "schedsim/common.h"

#include <pthread.h>
#include <stdlib.h>

typedef struct sm_queue_t {
  void* elems[SM_PID_MAX];
  unsigned f;
  unsigned r;
  size_t n;
  size_t length;
} sm_queue_t;

sm_queue_t* sm_queue_create();
void sm_queue_destroy(sm_queue_t* q);
void sm_queue_remove(sm_queue_t* q);
unsigned sm_queue_insert(sm_queue_t* q, void* elem);

typedef int (*sm_queue_sort_func)(const void* a, const void* b);

// TODO when r < f we'll have a problem.
//      - maybe memmove() + r&f fix in such case
void sm_queue_sort(sm_queue_t* q, sm_queue_sort_func func);
int sm_queue_empty(sm_queue_t* q);
void* sm_queue_back(sm_queue_t* q);
void* sm_queue_front(sm_queue_t* q);
void sm_queue_display(sm_queue_t* q);

#endif
