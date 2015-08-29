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

void sm_queue_remove(sm_queue_t* q);
void sm_queue_insert(sm_queue_t* q, void* elem);

static inline int sm_queue_empty(sm_queue_t* q)
{
  return q->f == q->r;
}

static inline void* sm_queue_back(sm_queue_t* q)
{
  if (!sm_queue_empty(q))
    return q->elems[q->r];
  return NULL;
}

static inline void* sm_queue_front(sm_queue_t* q)
{
  if (!sm_queue_empty(q))
    return q->elems[q->f];
  return NULL;
}

static inline void sm_queue_display(sm_queue_t* q)
{
  LOG("n: %lu, f: %u, r: %u", q->n, q->f, q->r);
}

#endif
