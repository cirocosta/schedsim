#ifndef SCHEDSIM__ALGORITHMS_H
#define SCHEDSIM__ALGORITHMS_H

#include "schedsim/trace.h"
#include "schedsim/common.h"

#include <sys/signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>

typedef enum sm_algorithms_e {
  SM_FIRSTCOME_FIRSTSERVED = 1,
  SM_S_JOB_FIRST,
  SM_S_REMAINING_TIME_NEXT,
  SM_ROUND_ROBIN,
  SM_SCHED_WITH_PRIORITY,
  SM_RT_RIGID_DEADLINES
} sm_algorithms_e;

void sm_waste_time(long long time_to_spend);
timer_t sm_create_timer(sm_trace_t* trace);

#endif
