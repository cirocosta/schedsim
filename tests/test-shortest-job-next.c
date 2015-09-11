#include <stdio.h>

#include "schedsim/schedulers/shortest_job_first.h"
#include "schedsim/trace.h"
#include "schedsim/common.h"

// tests supposing a 4 cpu (including virtual cpus) machine

void test1()
{
  unsigned traces_size = 8;
  sm_trace_t* traces[] = { 
    sm_parse_trace("1.0 process1 9 20"),
    sm_parse_trace("1.1 process2 8 20"),
    sm_parse_trace("1.2 process3 7 20"),
    sm_parse_trace("1.3 process4 6 20"),
    sm_parse_trace("1.4 process5 5 20"),
    sm_parse_trace("1.5 process6 4 20"),
    sm_parse_trace("1.6 process7 3 20"),
    sm_parse_trace("1.7 process8 2 20"),
  };

  sm_sched_shortest_job_first(traces, traces_size);

  while (traces_size --> 0)
    sm_trace_destroy(traces[traces_size]);
}

int main(int argc, char* argv[])
{
  TEST(test1);

  return 0;
}
