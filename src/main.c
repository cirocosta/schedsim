#include "schedsim/common.h"
#include "schedsim/cli.h"
#include "schedsim/scheduler.h"

#include "schedsim/schedulers/firstcome_firstserved.h"
#include "schedsim/schedulers/shortest_job_first.h"
#include "schedsim/schedulers/shortest_remaining_time_next.h"
#include "schedsim/schedulers/round_robin.h"
#include "schedsim/schedulers/priority_sched.h"

#include <time.h>
#include <stdio.h>

int main(const int argc, const char* argv[])
{
  unsigned i = 0;
  FILE* fp;

#ifndef NDEBUG
  setbuf(stdout, NULL);
#endif
  sm_cli_args_t* args = sm_cli_parse(argc, argv);
  size_t traces_count;
  sm_trace_t** traces = sm_get_traces(args->in_fname, &traces_count);

  switch (args->sched_alg) {
    case SM_FIRSTCOME_FIRSTSERVED:
      sm_sched_firstcome_firstserved(traces, traces_count);
    case SM_S_JOB_FIRST:
      sm_sched_shortest_job_first(traces, traces_count);
    case SM_S_REMAINING_TIME_NEXT:
      sm_sched_shortest_remaining_time_next(traces, traces_count);
    case SM_ROUND_ROBIN:
      sm_sched_round_robin(traces, traces_count);
    case SM_SCHED_WITH_PRIORITY:
      LOGERR("Unsupported SCHED WITH PRIORITY");
    /* sm_sched_priority_sched(traces, traces_count); */
    case SM_RT_RIGID_DEADLINES:
      LOGERR("Unsupported RIGIT DEADLINES");
      break;
  }

  fp = fopen(args->out_fname, "w");
  ASSERT(fp, "Couldn't open file `%s` for write.", args->out_fname);

  for (; i < traces_count; i++) {
    fprintf(fp, "%s %f %f\n", traces[i]->pname, traces[i]->out.tf / 1e6,
            traces[i]->out.tr / 1e6);
  }
  fprintf(fp, "%u", 10);

  fclose(fp);
  FREE(args);
  FREE_ARR(traces, traces_count);

  return 0;
}
