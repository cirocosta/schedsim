#include "schedsim/queue.h"

sm_queue_t* sm_queue_create()
{
  sm_queue_t* queue = malloc(sizeof(*queue));

  queue->f = 0;
  queue->r = 0;
  queue->length = 0;
  queue->n = SM_PID_MAX;
  memset(queue->elems, '\0', SM_PID_MAX);

  return queue;
}

void sm_queue_destroy(sm_queue_t* q)
{
  FREE(q);
}

void sm_queue_sort(sm_queue_t* q, sm_queue_sort_func func)
{
  qsort(q->elems, q->length, sizeof(void*), func);
}

int sm_queue_empty(sm_queue_t* q)
{
  return q->f == q->r;
}

void* sm_queue_back(sm_queue_t* q)
{
  if (!sm_queue_empty(q))
    return q->elems[q->r];
  return NULL;
}

void* sm_queue_front(sm_queue_t* q)
{
  if (!sm_queue_empty(q))
    return q->elems[q->f];
  return NULL;
}

void sm_queue_display(sm_queue_t* q)
{
  LOG("n: %lu, f: %u, r: %u", q->n, q->f, q->r);
}

unsigned sm_queue_insert(sm_queue_t* q, void* elem)
{
  unsigned pos = q->r;

  q->elems[q->r] = elem;

  if (q->r == q->n - 1)
    q->r = 0;
  else
    q->r++;

  q->length++;

  return pos;
}

void sm_queue_remove(sm_queue_t* q)
{
  if (q->f == q->n - 1)
    q->f = 0;
  else
    q->f++;

  q->length--;
}
