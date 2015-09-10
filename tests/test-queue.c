#include "schedsim/queue.h"
#include "schedsim/common.h"
#include "schedsim/trace.h"

#include <stdio.h>

void test1()
{
  sm_queue_t* q = sm_queue_create();
  char* hue = "hue";
  char* br = "br";
  char* front = NULL;

  ASSERT(sm_queue_empty(q) == 1, "Must start empty");

  ASSERT(sm_queue_insert(q, hue) == 0, "Must receive it's 'ticket' correctly");
  front = sm_queue_front(q);
  ASSERT(front == hue, "");

  ASSERT(sm_queue_insert(q, br) == 1, "Must receive it's 'ticket' correctly");
  ASSERT(sm_queue_empty(q) == 0, "Not empty after inserts");

  front = sm_queue_front(q);
  ASSERT(front == hue, "Didn't removed, must remain in front");

  sm_queue_remove(q);
  ASSERT(sm_queue_empty(q) == 0, "");

  front = sm_queue_front(q);
  ASSERT(front == br, "");

  sm_queue_remove(q);
  ASSERT(sm_queue_empty(q) == 1, "Ends empty after removing");

  sm_queue_destroy(q);
}

int sort_func(const void* a, const void* b)
{
  sm_trace_t* at = (sm_trace_t*)(*(void**)a);
  sm_trace_t* bt = (sm_trace_t*)(*(void**)b);

  return at->dt - bt->dt;
}

void test2()
{
  sm_trace_t* front = NULL;
  sm_trace_t* trace1 = sm_trace_create();
  sm_trace_t* trace2 = sm_trace_create();
  sm_trace_t* trace3 = sm_trace_create();
  sm_queue_t* q = sm_queue_create();

  trace1->dt = 30;
  trace2->dt = 5;
  trace3->dt = 10;

  sm_queue_insert(q, trace1);
  sm_queue_insert(q, trace2);
  sm_queue_insert(q, trace3);

  sm_queue_sort(q, sort_func);

  front = sm_queue_front(q);
  ASSERT(front->dt == 5, "front->dt is %f", front->dt);
  sm_queue_remove(q);

  front = sm_queue_front(q);
  ASSERT(front->dt == 10, "front->dt is %f", front->dt);
  sm_queue_remove(q);

  front = sm_queue_front(q);
  ASSERT(front->dt == 30, "front->dt is %f", front->dt);
  sm_queue_remove(q);

  sm_queue_destroy(q);
  sm_trace_destroy(trace1);
  sm_trace_destroy(trace2);
  sm_trace_destroy(trace3);
}

int main(int argc, char* argv[])
{
  TEST(test1);
  TEST(test2);

  return 0;
}
