#ifndef SCHEDSIM__ALGORITHMS_H
#define SCHEDSIM__ALGORITHMS_H

#include "schedsim/trace.h"
#include "schedsim/common.h"
#include "schedsim/queue.h"

#include <unistd.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>

typedef enum sm_schedulers_e {
  SM_FIRSTCOME_FIRSTSERVED = 1,
  SM_S_JOB_FIRST,
  SM_S_REMAINING_TIME_NEXT,
  SM_ROUND_ROBIN,
  SM_SCHED_WITH_PRIORITY,
  SM_RT_RIGID_DEADLINES
} sm_schedulers_e;

typedef struct sm_scheduler_t {
  sm_schedulers_e type;
  sm_queue_t* proc_queue;
  pthread_mutex_t proc_mutex;

  unsigned char available_cpus;
  unsigned char max_cpus;
  sm_trace_t** running_processes; // a proxy to CPUs

  double start_time;
  double context_switches;
} sm_scheduler_t;

sm_scheduler_t* sm_scheduler_create(sm_schedulers_e type);
void sm_scheduler_destroy(sm_scheduler_t* sched);

void* sm_user_process(void* arg);
void sm_waste_time(sm_trace_t*);
timer_t sm_create_timer(sm_trace_t* trace);
timer_t sm_create_quantum_timer();

int sm_sched_has_available_cpu(sm_scheduler_t* sched);
void sm_sched_assign_process_to_cpu(sm_scheduler_t* sched, sm_trace_t* trace);
void sm_sched_release_process(sm_scheduler_t* sched, sm_trace_t* trace);

#endif
