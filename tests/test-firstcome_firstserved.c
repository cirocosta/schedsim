#include <stdio.h>

#include "schedsim/schedulers/firstcome_firstserved.h"
#include "schedsim/trace.h"
#include "schedsim/common.h"

void test1()
{
  sm_trace_t* trace = sm_parse_trace("1 process0 1 3 -20");
  sm_trace_t* trace2 = sm_parse_trace("1.1 process1 1 3 -20");
  sm_trace_t* trace3 = sm_parse_trace("1.2 process2 1 3 -20");
  sm_trace_t* traces[] = { trace, trace2, trace3 };

  sm_sched_firstcome_firstserved(traces, 3);

  LOG("start: %f", trace->t0);
  LOG("final: %f", trace->out.tf);
  LOG("elapsed: %f", trace->out.tr);

  sm_trace_destroy(trace);
  sm_trace_destroy(trace2);
  sm_trace_destroy(trace3);
}

int main(int argc, char* argv[])
{
  TEST(test1, "hellloooo");

  return 0;
}
