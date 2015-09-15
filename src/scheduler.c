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
      core = sm_core_create(type, sm_sched_alg_fcfs);
      break;
    case SM_S_JOB_FIRST:
      core = sm_core_create(type, sm_sched_alg_sjf);
      break;
    case SM_S_REMAINING_TIME_NEXT:
      core = sm_core_create(type, sm_sched_alg_srtn);
      break;
    case SM_ROUND_ROBIN:
      core = sm_core_create(type, sm_sched_alg_rr);
      break;
    case SM_SCHED_WITH_PRIORITY:
      core = sm_core_create(type, sm_sched_alg_ps);
      break;
    case SM_RT_RIGID_DEADLINES:
      core = sm_core_create(type, sm_sched_alg_rd);
      break;
  }

  sm_core_run(core, traces, traces_count);

  sched->core = core;

  return sched;
}
