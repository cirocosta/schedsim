#include <stdio.h>

#include "schedsim/scheduler.h"

// tests supposing a 4 cpu (including virtual cpus) machine

void test1()
{
  sm_trace_t* trace = sm_parse_trace("1 process0 1 3 -20");
  sm_trace_t* trace2 = sm_parse_trace("1.5 process1 1 3 -20");
  sm_trace_t* trace3 = sm_parse_trace("1.2 process2 1 3 -20");
  sm_trace_t* traces[] = { trace, trace2, trace3 };

  sm_scheduler_simulate(SM_ALG_FCFS, traces, 3);

  sm_trace_destroy(trace);
  sm_trace_destroy(trace2);
  sm_trace_destroy(trace3);
}

void test2()
{
  unsigned traces_size = 8;
  sm_trace_t* traces[] = { 
    sm_parse_trace("1 process1 1.5 3 -20"),
    sm_parse_trace("1 process2 1.5 3 -20"),
    sm_parse_trace("1 process3 1.5 3 -20"),
    sm_parse_trace("1 process4 1.5 3 -20"),
    sm_parse_trace("1 process5 1.5 3 -20"),
    sm_parse_trace("1 process6 1.5 3 -20"),
    sm_parse_trace("1 process7 1.5 3 -20"),
    sm_parse_trace("1 process8 1.5 3 -20"),
  };

  sm_scheduler_simulate(SM_ALG_FCFS, traces, 3);

  while (traces_size --> 0)
    sm_trace_destroy(traces[traces_size]);
}

int main(int argc, char* argv[])
{
  TEST(test1);
  TEST(test2);

  return 0;
}
