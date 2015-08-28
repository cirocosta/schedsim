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

int main(int argc, char* argv[])
{
  test1();

  fprintf(stderr, "%s\n", "test-trace: OK!\n");

  return 0;
}
