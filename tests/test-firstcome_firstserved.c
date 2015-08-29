#include <stdio.h>

#include "schedsim/algorithms.h"
#include "schedsim/trace.h"
#include "schedsim/common.h"

void test1()
{
  sm_trace_t* trace = sm_parse_trace("1 process0 2 3 -20");
  sm_trace_t* traces[] = { trace };

  sm_sched_firstcome_firstserved(traces, 1); 

  sm_trace_delete(trace);
}

int main(int argc, char* argv[])
{
  TEST(test1, "hellloooo");

  return 0;
}
