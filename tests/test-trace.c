#include "schedsim/trace.h"
#include "schedsim/common.h"

#include <stdio.h>

void test1()
{
  const char* trace = "1 processo0 10 11 -20";
  sm_trace_t expected = {
    .t0 = 1.0, .pname = "processo0", .dt = 10.0, .deadline = 11.0, .p = -20
  };

  sm_trace_t* actual = sm_parse_trace(trace);

  ASSERT(actual->t0 == expected.t0, "");
  ASSERT(!strncmp(actual->pname, expected.pname, NAME_MAX), "");
  ASSERT(actual->dt == expected.dt, "");
  ASSERT(actual->deadline == expected.deadline, "");
  ASSERT(actual->p == expected.p, "");

  FREE(actual);
}
void test2()
{
  size_t traces_size = 0;
  sm_trace_t** traces = NULL;

  traces = sm_trace_parse_file("./tests/assets/deadline-poucos.txt",
                         &traces_size);

  ASSERT(traces_size == 13, "");
  ASSERT(!strncmp(traces[0]->pname, "processo0", NAME_MAX), "");
  ASSERT(!strncmp(traces[1]->pname, "processo1", NAME_MAX), "");
  ASSERT(!strncmp(traces[2]->pname, "processo2", NAME_MAX), "");
  ASSERT(!strncmp(traces[8]->pname, "processo8", NAME_MAX), "");

  FREE_ARR(traces, traces_size);
}

int main(int argc, char* argv[])
{
  TEST(test1);
  TEST(test2);

  return 0;
}
