#include <stdio.h>

#include "schedsim/schedulers/priority_sched.h"
#include "schedsim/trace.h"
#include "schedsim/common.h"

// tests supposing a 4 cpu (including virtual cpus) machine
void test2()
{
  unsigned traces_size = 3;
  sm_trace_t* traces[] = { 
    sm_parse_trace("1.0 process1 20 20"),
    sm_parse_trace("1.0 process2 20 20"),
    sm_parse_trace("1.0 process3 20 20"),
  };

  sm_sched_priority_sched(traces, traces_size);

  while (traces_size --> 0)
    sm_trace_destroy(traces[traces_size]);
}

void test1()
{
  unsigned traces_size = 8;
  sm_trace_t* traces[] = { 
    sm_parse_trace("1.0 process1 3 20"),
    sm_parse_trace("1.0 process2 3 20"),
    sm_parse_trace("1.0 process3 3 20"),
    sm_parse_trace("1.0 process4 3 20"),
    sm_parse_trace("1.0 process5 3 20"),
    sm_parse_trace("1.0 process6 3 20"),
    sm_parse_trace("1.0 process7 3 20"),
    sm_parse_trace("1.0 process8 3 20"),
  };

  sm_sched_priority_sched(traces, traces_size);

  while (traces_size --> 0)
    sm_trace_destroy(traces[traces_size]);
}

int main(int argc, char* argv[])
{
  /* TEST(test1); */
  /* TEST(test2); */

  return 0;
}
