#include <stdio.h>

#include "schedsim/schedulers/round_robin.h"
#include "schedsim/trace.h"
#include "schedsim/common.h"

// tests supposing a 4 cpu (including virtual cpus) machine
void test2()
{
  unsigned traces_size = 2;
  sm_trace_t* traces[] = { 
    sm_parse_trace("1.0 process1 3 20 -20"),
    sm_parse_trace("1.0 process2 3 20 -20"),
  };

  sm_sched_round_robin(traces, traces_size);

  while (traces_size --> 0)
    sm_trace_destroy(traces[traces_size]);
}

void test1()
{
  unsigned traces_size = 8;
  sm_trace_t* traces[] = { 
    sm_parse_trace("1.0 process1 2 20"),
    sm_parse_trace("1.0 process2 2 20"),
    sm_parse_trace("1.0 process3 2 20"),
    sm_parse_trace("1.0 process4 2 20"),
    sm_parse_trace("1.0 process5 2 20"),
    sm_parse_trace("1.0 process6 2 20"),
    sm_parse_trace("1.0 process7 2 20"),
    sm_parse_trace("1.0 process8 2 20"),
  };

  sm_sched_round_robin(traces, traces_size);

  while (traces_size --> 0)
    sm_trace_destroy(traces[traces_size]);
}

int main(int argc, char* argv[])
{
  TEST(test1);
  TEST(test2);

  return 0;
}
