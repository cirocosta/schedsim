#include "schedsim/queue.h"

sm_queue_t* sm_queue_create()
{
  sm_queue_t* queue = malloc(sizeof(*queue));

  queue->f = 0;
  queue->r = 0;
  queue->n = SM_PID_MAX;

  return queue;
}

void sm_queue_delete(sm_queue_t* q)
{
  FREE(q);
}

void sm_queue_insert(sm_queue_t* q, void* elem)
{
  if (q->r == q->n - 1)
    q->r = 0;
  else
    q->r++;
  q->elems[q->r] = elem;
}


void* sm_queue_remove(sm_queue_t* q)
{
  if (!(q->f))
    q->f = q->n - 1;
  else
    q->f--;
}
