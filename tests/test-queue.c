#include "schedsim/queue.h"
#include "schedsim/common.h"

#include <stdio.h>

void test1()
{
  sm_queue_t* q = sm_queue_create();

  sm_queue_delete(q);
}

int main(int argc, char* argv[])
{
  TEST(test1);

  return 0;
}
