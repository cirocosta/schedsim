#ifndef SCHEDSIM__CORE_H
#define SCHEDSIM__CORE_H 

#include "schedsim/trace.h"
#include "schedsim/common.h"
#include "schedsim/queue.h"

#include <unistd.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>

typedef struct sm_core_t {
  sm_schedulers_e type;
  sm_queue_t* proc_queue;
  pthread_mutex_t proc_mutex;

  unsigned char available_cpus;
  unsigned char max_cpus;
  sm_trace_t** running_processes; // a proxy to CPUs

  double start_time;
  double context_switches;
} sm_core_t;

sm_core_t* sm_core_create(sm_schedulers_e type);
void sm_core_destroy(sm_core_t* sched);

void* sm_core_process(void* arg);
void sm_core_process_wastetime(sm_trace_t*);
timer_t sm_core_dispatcher_create(sm_trace_t* trace);
timer_t sm_core_quantum_create();

int sm_core_has_available_cpu(sm_core_t* sched);
void sm_core_assign_process_to_cpu(sm_core_t* sched, sm_trace_t* trace);
void sm_core_release_process(sm_core_t* sched, sm_trace_t* trace);

#endif
