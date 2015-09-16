#ifndef SCHEDSIM__ROUND_ROBIN_H
#define SCHEDSIM__ROUND_ROBIN_H

#include "schedsim/common.h"
#include "schedsim/core.h"

static inline void _rr_on_process_new(sm_core_t* sched, siginfo_t sig)
{
  sm_trace_t* trace = NULL;

  trace = (sm_trace_t*)sig.si_ptr;
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
static inline void _rr_on_process_quantum(sm_core_t* sched, siginfo_t sig)
{
  sm_trace_t* queue_trace = NULL;
  unsigned i = 0;

  // in case we have more CPUs than elements (also checks for empty q)
  for (; i < sched->max_cpus && sched->proc_queue->length; i++) {
    queue_trace = sm_queue_front(sched->proc_queue);
    sm_queue_remove(sched->proc_queue);
    sm_queue_insert(sched->proc_queue, sched->running_processes[i]);
    sm_core_release_process(sched, sched->running_processes[i]);
    sm_core_assign_process_to_cpu(sched, queue_trace);
    sched->context_switches++;
  }
}
static inline void _rr_on_process_end(sm_core_t* sched, siginfo_t sig)
{
  sm_trace_t* trace = NULL;

  trace = (sm_trace_t*)sig.si_ptr;
  LOGERR("Process `%s` Terminated!", trace->pname);
  sm_out_trace_print(trace);
}

static sm_core_sched_algorithm sm_sched_alg_rr = {
  .on_process_new = _rr_on_process_new,
  .on_process_end = _rr_on_process_end,
  .on_process_quantum = _rr_on_process_quantum,
};

#endif
