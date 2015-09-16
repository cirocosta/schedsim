#include <stdio.h>

#include "schedsim/scheduler.h"

// tests supposing a 4 cpu (including virtual cpus) machine
void test2()
{
  sm_scheduler_t* scheduler = NULL;
  unsigned traces_size = 2;
  sm_trace_t* traces[] = {
    sm_parse_trace("1.0 process1 3 20 19"),
    sm_parse_trace("1.0 process2 3 20 19"),
  };

  scheduler = sm_scheduler_simulate(SM_ALG_RR, traces, traces_size);

  while (traces_size-- > 0)
    sm_trace_destroy(traces[traces_size]);

  sm_scheduler_destroy(scheduler);
}

void test1()
{
  sm_scheduler_t* scheduler = NULL;
  unsigned traces_size = 8;
  sm_trace_t* traces[] = {
    sm_parse_trace("1.0 process1 2 3 19"),
    sm_parse_trace("1.0 process2 2 3 19"),
    sm_parse_trace("1.0 process3 2 3 19"),
    sm_parse_trace("1.0 process4 2 3 19"),
    sm_parse_trace("1.0 process5 2 3 19"),
    sm_parse_trace("1.0 process6 2 3 19"),
    sm_parse_trace("1.0 process7 2 3 19"),
    sm_parse_trace("1.0 process8 2 3 19"),
  };

  scheduler = sm_scheduler_simulate(SM_ALG_RR, traces, traces_size);

  while (traces_size-- > 0)
    sm_trace_destroy(traces[traces_size]);

  sm_scheduler_destroy(scheduler);
}

int main(int argc, char* argv[])
{
  TEST(test1);
  TEST(test2);

  return 0;
}
