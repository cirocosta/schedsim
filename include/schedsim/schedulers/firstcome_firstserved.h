#ifndef SCHEDSIM__FIRSTCOME_FIRSTSERVER_H
#define SCHEDSIM__FIRSTCOME_FIRSTSERVER_H

#include "schedsim/common.h"
#include "schedsim/core.h"

static inline void _fcfs_on_process_new(sm_core_t* sched, siginfo_t sig)
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

static inline void _fcfs_on_process_end(sm_core_t* sched, siginfo_t sig)
{
  sm_trace_t* trace = NULL;
  sm_trace_t* queue_trace = NULL;

  trace = (sm_trace_t*)sig.si_ptr;
  LOGERR("Process Terminated!");
  sm_out_trace_print(trace);

  sm_core_release_process(sched, trace);

  if (!sm_queue_empty(sched->proc_queue)) {
    queue_trace = sm_queue_front(sched->proc_queue);
    sm_queue_remove(sched->proc_queue);
    sm_core_assign_process_to_cpu(sched, queue_trace);
  }
}

static sm_core_sched_algorithm sm_sched_alg_fcfs = {
  .on_process_new = _fcfs_on_process_new,
  .on_process_end = _fcfs_on_process_end,
  .on_process_quantum = NULL,
};

#endif
