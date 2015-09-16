#ifndef SCHEDSIM__ALGORITHMS_H
#define SCHEDSIM__ALGORITHMS_H

#include "schedsim/schedulers/firstcome_firstserved.h"
#include "schedsim/schedulers/shortest_job_first.h"
#include "schedsim/schedulers/shortest_remaining_time_next.h"
#include "schedsim/schedulers/round_robin.h"
#include "schedsim/schedulers/priority_sched.h"
#include "schedsim/schedulers/rigid_dealines.h"

#include "schedsim/core.h"

typedef struct sm_scheduler_t {
  sm_core_t* core;
  sm_schedulers_e type;
} sm_scheduler_t;

sm_scheduler_t* sm_scheduler_create(sm_schedulers_e type);
void sm_scheduler_destroy(sm_scheduler_t*);
sm_scheduler_t* sm_scheduler_simulate(sm_schedulers_e type, sm_trace_t** traces,
                           unsigned traces_count);

#endif
