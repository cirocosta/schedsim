#ifndef SCHEDSIM__QUEUE_H
#define SCHEDSIM__QUEUE_H

// from /proc/sys/kernel/pid_max
#define SM_PID_MAX 32768

#include "schedsim/common.h"
#include <stdlib.h>

typedef struct sm_queue_t {
  void* elems[SM_PID_MAX];
  unsigned f;
  unsigned r;
  size_t n;
} sm_queue_t;

sm_queue_t* sm_queue_create();
void sm_queue_delete(sm_queue_t* q);

void* sm_queue_remove(sm_queue_t* q);
void sm_queue_insert(sm_queue_t* q, void* elem);

static inline void* sm_queue_front(sm_queue_t* q)
{
  return q->elems[q->f];
}

static inline void* sm_queue_back(sm_queue_t* q)
{
  return q->elems[q->r];
}

static inline int sm_queue_empty(sm_queue_t* q)
{
  return q->f == q->r;
}

#endif
