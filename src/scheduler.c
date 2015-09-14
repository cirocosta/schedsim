#include "schedsim/scheduler.h"

sm_scheduler_t* sm_scheduler_create(sm_schedulers_e type)
{
  sm_scheduler_t* sched = malloc(sizeof(*sched));
  PASSERT(sched, "Couldn't allocate memory");

  sched->core = NULL;
  sched->type = type;

  return sched;
}

void sm_scheduler_destroy(sm_scheduler_t* sched)
{
  sm_core_destroy(sched->core);
  FREE(sched);
}

sm_scheduler_t* sm_scheduler_simulate(sm_schedulers_e type,
                        sm_trace_t** traces, unsigned traces_count)
{
  sm_scheduler_t* sched = sm_scheduler_create(type);
  sm_core_t* core = NULL;

  switch (type) {
    case SM_FIRSTCOME_FIRSTSERVED:
      core = sm_sched_firstcome_firstserved(traces, traces_count);
      break;
    case SM_S_JOB_FIRST:
      core = sm_sched_shortest_job_first(traces, traces_count);
      break;
    case SM_S_REMAINING_TIME_NEXT:
      core = sm_sched_shortest_remaining_time_next(traces, traces_count);
      break;
    case SM_ROUND_ROBIN:
      core = sm_sched_round_robin(traces, traces_count);
      break;
    case SM_SCHED_WITH_PRIORITY:
      LOGERR("Unsupported SCHED WITH PRIORITY");
      break;
    /* context_switches = sm_sched_priority_sched(traces, traces_count); */
    case SM_RT_RIGID_DEADLINES:
      LOGERR("Unsupported RIGIT DEADLINES");
      break;
  }

  sched->core = core;

  return sched;
}
