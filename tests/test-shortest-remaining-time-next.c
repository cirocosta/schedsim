#include <stdio.h>

#include "schedsim/scheduler.h"

// tests supposing a 4 cpu (including virtual cpus) machine

void test1()
{
  sm_scheduler_t* scheduler = NULL;
  unsigned traces_size = 8;
  sm_trace_t* traces[] = {
    sm_parse_trace("1.0 process0 5 10 10"),
    sm_parse_trace("1.1 process1 5 10 10"),
    sm_parse_trace("1.2 process2 5 10 10"),
    sm_parse_trace("1.3 process3 5 10 10"),
    sm_parse_trace("1.4 process4 1 10 10"),
    sm_parse_trace("1.5 process5 1 10 10"),
    sm_parse_trace("1.6 process6 1 10 10"),
    sm_parse_trace("1.7 process7 1 10 10"),
  };

  scheduler = sm_scheduler_simulate(SM_ALG_SRTN, traces, traces_size);

  while (traces_size-- > 0)
    sm_trace_destroy(traces[traces_size]);
  sm_scheduler_destroy(scheduler);
}

int main(int argc, char* argv[])
{
  TEST(test1);

  return 0;
}
