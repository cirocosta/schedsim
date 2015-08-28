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

int main(int argc, char* argv[])
{
  test1();

  fprintf(stderr, "%s\n", "test-cli: OK!\n");

  return EXIT_SUCCESS;
}
