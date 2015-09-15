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

struct sm_core_t;

typedef void (*sm_sched_event_handler)(struct sm_core_t* core, siginfo_t sig);

typedef struct sm_core_sched_algorithm {
  sm_sched_event_handler on_process_new;
  sm_sched_event_handler on_process_end;
  sm_sched_event_handler on_process_quantum;
} sm_core_sched_algorithm;

typedef struct sm_core_t {
  sm_schedulers_e type;
  sm_queue_t* proc_queue;

  unsigned char available_cpus;
  unsigned char max_cpus;
  // a proxy to Processing Units
  sm_trace_t** running_processes;

  double start_time;
  double context_switches;

  sm_core_sched_algorithm* algorithm;
} sm_core_t;

sm_core_t* sm_core_create(sm_schedulers_e type, sm_core_sched_algorithm* alg);
void sm_core_destroy(sm_core_t* sched);

sm_core_t* sm_core_run(sm_core_t* sched, sm_trace_t** traces, size_t traces_size);
void* sm_core_process(void* arg);
void sm_core_process_wastetime(sm_trace_t*);
timer_t sm_core_dispatcher_create(sm_trace_t* trace);
timer_t sm_core_quantum_create();

int sm_core_has_available_cpu(sm_core_t* sched);
void sm_core_assign_process_to_cpu(sm_core_t* sched, sm_trace_t* trace);
void sm_core_release_process(sm_core_t* sched, sm_trace_t* trace);

#endif
