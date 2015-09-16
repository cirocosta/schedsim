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

sm_scheduler_t* sm_scheduler_simulate(sm_schedulers_e type, sm_trace_t** traces,
                                      unsigned traces_count)
{
  sm_scheduler_t* sched = sm_scheduler_create(type);
  sm_core_t* core = NULL;

  switch (type) {
    case SM_ALG_FCFS:
      core = sm_core_create(type, &sm_sched_alg_fcfs);
      break;
    case SM_ALG_SJF:
      core = sm_core_create(type, &sm_sched_alg_sjf);
      break;
    case SM_ALG_SRTN:
      core = sm_core_create(type, &sm_sched_alg_srtn);
      break;
    case SM_ALG_RR:
      core = sm_core_create(type, &sm_sched_alg_rr);
      break;
    case SM_ALG_PS:
      core = sm_core_create(type, &sm_sched_alg_ps);
      break;
    case SM_ALG_RD:
      core = sm_core_create(type, &sm_sched_alg_rd);
      break;
  }

  sm_core_run(core, traces, traces_count);

  sched->core = core;

  return sched;
}
