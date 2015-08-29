#ifndef SCHEDSIM__ALGORITHMS_H
#define SCHEDSIM__ALGORITHMS_H

#include "schedsim/trace.h"
#include "schedsim/common.h"
#include "schedsim/queue.h"

#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define BILLION 1000000000

typedef enum sm_algorithms_e {
  SM_FIRSTCOME_FIRSTSERVED = 1,
  SM_S_JOB_FIRST,
  SM_S_REMAINING_TIME_NEXT,
  SM_ROUND_ROBIN,
  SM_SCHED_WITH_PRIORITY,
  SM_RT_RIGID_DEADLINES
} sm_algorithms_e;

void sm_waste_time(float time_to_spend);

/**
 * First Come, First Served.
 *
 * Uses a FIFO implemented as a circular queue as it's data structure.
 * The decision for using a circular queue is the fact that even though the OS
 * will be limited to a maximum number of processes running simultaneously
 * it won't have to constantly deal with dynamic memory allocation and
 * deallocation all the time (compared to a linked-list-based FIFO
 * implementation - which could grow indefinetly but would have to pay for that)
 * .
 */
void sm_sched_firstcome_firstserved(sm_trace_t** traces, size_t traces_size);

#endif
