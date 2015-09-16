#include <stdio.h>

#include "schedsim/scheduler.h"

// tests supposing a 4 cpu (including virtual cpus) machine

void test1()
{
  sm_scheduler_t* scheduler = NULL;
  unsigned traces_size = 8;
  sm_trace_t* traces[] = {
    sm_parse_trace("1.0 process1 9 10 10"),
    sm_parse_trace("1.1 process2 8 10 10"),
    sm_parse_trace("1.2 process3 7 10 10"),
    sm_parse_trace("1.3 process4 6 10 10"),
    sm_parse_trace("1.4 process5 5 10 10"),
    sm_parse_trace("1.5 process6 4 10 10"),
    sm_parse_trace("1.6 process7 3 10 10"),
    sm_parse_trace("1.7 process8 2 10 10"),
  };

  scheduler = sm_scheduler_simulate(SM_ALG_SJF, traces, traces_size);

  while (traces_size-- > 0)
    sm_trace_destroy(traces[traces_size]);
  sm_scheduler_destroy(scheduler);
}

int main(int argc, char* argv[])
{
  TEST(test1);

  return 0;
}
