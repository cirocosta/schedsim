#include "schedsim/queue.h"
#include "schedsim/common.h"

#include <stdio.h>

void test1()
{
  sm_queue_t* q = sm_queue_create();
  char* hue = "hue";
  char* br = "br";
  char* front = NULL;

  ASSERT(sm_queue_empty(q) == 1, "Must start empty");

  sm_queue_insert(q, hue);
  front = sm_queue_front(q);
  ASSERT(front == hue, "");

  sm_queue_insert(q, br);
  ASSERT(sm_queue_empty(q) == 0, "Not empty after inserts");

  front = sm_queue_front(q);
  ASSERT(front == hue, "Didn't removed, must remain in front");

  sm_queue_remove(q);
  ASSERT(sm_queue_empty(q) == 0, "");

  front = sm_queue_front(q);
  ASSERT(front == br, "");

  sm_queue_remove(q);
  ASSERT(sm_queue_empty(q) == 1, "Ends empty after removing");

  sm_queue_delete(q);
}

int main(int argc, char* argv[])
{
  TEST(test1);

  return 0;
}
