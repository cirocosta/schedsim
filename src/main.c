#include "schedsim/common.h"

#include <time.h>
#include <stdio.h>

int main(const int argc, const char* argv[])
{
#ifndef NDEBUG
  setbuf(stdout, NULL);
#endif
  /* sm_sched_firstcome_firstserved(NULL, 0); */

#if 0
  sm_cli_args_t* args = sm_cli_parse(argc, argv);
  size_t traces_count;
  sm_trace_t** traces = sm_get_traces(args->in_fname, &traces_count);

  switch (args->sched_alg) {
    case SM_FIRSTCOME_FIRSTSERVED:
      sm_sched_firstcome_firstserved(traces);
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
  FREE_ARR(traces, traces_count);
#endif
  return 0;
}
