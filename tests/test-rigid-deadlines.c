#include <stdio.h>

#include "schedsim/schedulers/rigid_dealines.h"
#include "schedsim/trace.h"
#include "schedsim/common.h"

// tests supposing a 4 cpu (including virtual cpus) machine

void test1()
{
  unsigned traces_size = 8;
  sm_trace_t* traces[] = { 
    sm_parse_trace("1.0 process0 3 10 10"),
    sm_parse_trace("1.1 process1 3 10 10"),
    sm_parse_trace("1.2 process2 3 10 10"),
    sm_parse_trace("1.3 process3 3 10 10"),
    sm_parse_trace("1.4 process4 3 10 10"),
    sm_parse_trace("1.5 process5 3 10 10"),
    sm_parse_trace("1.6 process6 0.5 5 10"),
    sm_parse_trace("1.7 process7 0.5 5 10"),
  };

  sm_sched_rigid_deadlines(traces, traces_size);

  while (traces_size --> 0)
    sm_trace_destroy(traces[traces_size]);
}

int main(int argc, char* argv[])
{
  TEST(test1);

  return 0;
}
