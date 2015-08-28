#include "schedsim/common.h"
#include "schedsim/cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void test1()
{
  const char* argv[] = { "./schedsim", "1", "hue.txt", "brbr.txt", NULL };
  const int argc = 4;
  sm_cli_args_t expected = {.sched_alg = SM_FIRSTCOME_FIRSTSERVED,
                            .in_fname = "hue.txt",
                            .out_fname = "brbr.txt",
                            .debug = 0 };

  sm_cli_args_t* actual = sm_cli_parse(argc, argv);

  ASSERT(actual->debug == expected.debug, "Debug Flag");
  ASSERT(actual->sched_alg == expected.sched_alg, "Scheduling Algorithm");
  ASSERT(!strncmp(actual->in_fname, expected.in_fname, PATH_MAX),
         "Input Filename");
  ASSERT(!strncmp(actual->out_fname, expected.out_fname, PATH_MAX),
         "Output Filename");

  FREE(actual);
}

void test2()
{
  const char* trace = "1 processo0 10 11 -20";
  sm_in_entry_t expected = {
    .t0 = 1.0, .pname = "processo0", .dt = 10.0, .deadline = 11.0, .p = -20
  };
  sm_in_entry_t* actual = sm_parse_trace(trace);

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
  test2();

  fprintf(stderr, "%s\n", "test-cli: OK!\n");

  return EXIT_SUCCESS;
}
