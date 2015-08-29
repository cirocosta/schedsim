#include "schedsim/queue.h"

sm_queue_t* sm_queue_create()
{
  sm_queue_t* queue = malloc(sizeof(*queue));

  queue->f = 0;
  queue->r = 0;
  queue->n = SM_PID_MAX;
  memset(queue->elems, '\0', SM_PID_MAX);

  return queue;
}

void sm_queue_delete(sm_queue_t* q)
{
  FREE(q);
}

unsigned sm_queue_insert(sm_queue_t* q, void* elem)
{
  unsigned pos = q->r;

  q->elems[q->r] = elem;

  if (q->r == q->n - 1)
    q->r = 0;
  else
    q->r++;

  return pos;
}

void sm_queue_remove(sm_queue_t* q)
{
  if (q->f == q->n-1)
    q->f = 0;
  else
    q->f++;
}
