#include "schedsim/cli.h"
#include "schedsim/common.h"

#include <stdio.h>

int main(const int argc, const char* argv[])
{
  sm_cli_args_t* args = sm_cli_parse(argc, argv);
  size_t entries_count;
  sm_trace_t** entries = sm_get_traces(args->in_fname, &entries_count);

  switch (args->sched_alg) {
    case SM_FIRSTCOME_FIRSTSERVED:
      break;
    case SM_S_JOB_FIRST:
      break;
    case SM_S_REMAINING_TIME_NEXT:
      break;
    case SM_ROUND_ROBIN:
      break;
    case SM_SCHED_WITH_PRIORITY:
      break;
    case SM_RT_RIGID_DEADLINES:
      break;
  }

  FREE(args);
  FREE_ARR(entries, entries_count);

  return 0;
}
