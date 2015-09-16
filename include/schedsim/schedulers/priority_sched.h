#ifndef SCHEDSIM__PRIORITY_SCHED_H
#define SCHEDSIM__PRIORITY_SCHED_H

#include "schedsim/common.h"
#include "schedsim/core.h"

#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

static inline void _ps_on_process_new(sm_core_t* sched, siginfo_t sig)
{
  sm_trace_t* trace = NULL;

  trace = (sm_trace_t*)sig.si_ptr;
  trace->remaining_quantums = 20 - trace->p;
  LOGERR("New process in the system!");
  sm_trace_print(trace);

  if (sm_core_has_available_cpu(sched)) {
    sm_core_assign_process_to_cpu(sched, trace);
  } else {
    trace->blocked = 1;
    sm_queue_insert(sched->proc_queue, trace);
  }

  pthread_create(&trace->tid, NULL, &sm_core_process, sig.si_ptr);
}

static inline void _ps_on_process_quantum(sm_core_t* sched, siginfo_t sig)
{
  sm_trace_t* queue_trace = NULL;
  unsigned i = 0;
  sm_trace_t* trace = NULL;

  // in case we have more CPUs than elements (also checks for empty q)
  for (; i < sched->max_cpus && sched->proc_queue->length; i++) {
    trace = sched->running_processes[i];

    if (trace && trace->remaining_quantums) {
      trace->remaining_quantums--;
      continue;
    }

    queue_trace = sm_queue_front(sched->proc_queue);
    sm_queue_remove(sched->proc_queue);

    if (trace) {
      trace->remaining_quantums = 20 - trace->p;
      sm_queue_insert(sched->proc_queue, trace);
      sm_core_release_process(sched, trace);
    }

    sm_core_assign_process_to_cpu(sched, queue_trace);
    sched->context_switches++;
  }
}

static inline void _ps_on_process_end(sm_core_t* sched, siginfo_t sig)
{
  sm_trace_t* trace = NULL;

  trace = (sm_trace_t*)sig.si_ptr;
  LOGERR("Process `%s` Terminated!", trace->pname);
  sm_out_trace_print(trace);
  sm_core_release_process(sched, trace);
}

static sm_core_sched_algorithm sm_sched_alg_ps = {
  .on_process_new = _ps_on_process_new,
  .on_process_end = _ps_on_process_end,
  .on_process_quantum = _ps_on_process_quantum,
};

#endif
