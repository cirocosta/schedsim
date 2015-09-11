#include <stdio.h>

#include "schedsim/schedulers/firstcome_firstserved.h"
#include "schedsim/trace.h"
#include "schedsim/common.h"

// tests supposing a 4 cpu (including virtual cpus) machine

void test1()
{
  sm_trace_t* trace = sm_parse_trace("1 process0 2 3 -20");
  sm_trace_t* trace2 = sm_parse_trace("1.5 process1 5 3 -20");
  sm_trace_t* trace3 = sm_parse_trace("1.2 process2 5 3 -20");
  sm_trace_t* traces[] = { trace, trace2, trace3 };

  sm_sched_firstcome_firstserved(traces, 3);

  LOG("start: %f", trace->t0/1e6);
  LOG("elapsed: %f", trace2->out.tr/1e6);

  ASSERT(trace->out.tf < trace2->out.tf, "");
  ASSERT(trace2->out.tf > trace3->out.tf, "");

  sm_trace_destroy(trace);
  sm_trace_destroy(trace2);
  sm_trace_destroy(trace3);
}

void test2()
{
  unsigned traces_size = 8;
  sm_trace_t* traces[] = { 
    sm_parse_trace("1 process1 5 20"),
    sm_parse_trace("1 process2 5 20"),
    sm_parse_trace("1 process3 5 20"),
    sm_parse_trace("1 process4 5 20"),
    sm_parse_trace("1 process5 5 20"),
    sm_parse_trace("1 process6 5 20"),
    sm_parse_trace("1 process7 5 20"),
    sm_parse_trace("1 process8 5 20"),
  };

  sm_sched_firstcome_firstserved(traces, traces_size);

  ASSERT(traces[0]->out.tf - traces[1]->out.tf < 1e3, "");
  ASSERT(traces[1]->out.tf - traces[2]->out.tf < 1e3, "");
  ASSERT(traces[2]->out.tf - traces[3]->out.tf < 1e3, "");

  ASSERT(traces[0]->out.tf - traces[4]->out.tf > 2e6, "");
  ASSERT(traces[1]->out.tf - traces[5]->out.tf > 2e6, "");
  ASSERT(traces[2]->out.tf - traces[6]->out.tf > 2e6, "");

  ASSERT(traces[3]->out.tf - traces[1]->out.tf < 1e3, "");
  ASSERT(traces[4]->out.tf - traces[2]->out.tf < 1e3, "");
  ASSERT(traces[5]->out.tf - traces[3]->out.tf < 1e3, "");

  while (traces_size --> 0)
    sm_trace_destroy(traces[traces_size]);
}

int main(int argc, char* argv[])
{
  TEST(test1);
  TEST(test2);

  return 0;
}
