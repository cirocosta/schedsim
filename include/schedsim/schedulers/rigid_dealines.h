#ifndef SCHEDSIM__RIGID_DEADLINES_H
#define SCHEDSIM__RIGID_DEADLINES_H

#include "schedsim/common.h"
#include "schedsim/core.h"

inline static int sm_sched_sort_rd(const void* a, const void* b)
{
  sm_trace_t* at = (sm_trace_t*)(*(void**)a);
  sm_trace_t* bt = (sm_trace_t*)(*(void**)b);

  return at->deadline - bt->deadline;
}

// main
inline static sm_core_t* sm_sched_rigid_deadlines(sm_trace_t** traces,
                                                  size_t traces_size)
{
  unsigned i = 0;
  sigset_t intmask, block_set;
  siginfo_t sig;
  sm_trace_t* trace = NULL;
  sm_trace_t* queue_trace = NULL;
  sm_core_t* sched = sm_core_create(SM_S_JOB_FIRST);

  // PREPARATION
  if ((sigemptyset(&block_set) == -1) ||
      (sigaddset(&block_set, SIG_PROCESS_NEW) == -1) ||
      (sigaddset(&block_set, SIG_PROCESS_END) == -1) ||
      (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1)) {
    LOGERR("Error setting thread signal mask: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((sigemptyset(&intmask) == -1) ||
      (sigaddset(&intmask, SIG_PROCESS_NEW) == -1) ||
      sigaddset(&intmask, SIG_PROCESS_END)) {
    perror("sig_setting error:");
    exit(EXIT_FAILURE);
  }

  for (; i < traces_size; i++)
    sm_core_dispatcher_create(traces[i]);
  // PREPARATION

  i = 0;
  while (traces_size) {
    sigwaitinfo(&intmask, &sig);
    trace = (sm_trace_t*)sig.si_ptr;

    switch (sig.si_signo) {
      case SIG_PROCESS_NEW:
        LOGERR("New process in the system!");
        sm_trace_print(trace);

        if (sm_core_has_available_cpu(sched)) {
          sm_core_assign_process_to_cpu(sched, trace);
        } else {
          trace->blocked = 1;
          sm_queue_insert(sched->proc_queue, trace);
          sm_queue_sort(sched->proc_queue, sm_sched_sort_rd);

          queue_trace = sm_queue_front(sched->proc_queue);

          i = 0;
          for (; i < sched->max_cpus; i++) {
            if (queue_trace->deadline < sched->running_processes[i]->deadline) {

              sm_queue_remove(sched->proc_queue);

              // the process won't block instantly.
              // there should be a "sm_cpu_t" which would then
              // hold a semaphore and we'd block on that now.

              sm_queue_insert(sched->proc_queue, sched->running_processes[i]);
              sm_core_release_process(sched, sched->running_processes[i]);
              sm_core_assign_process_to_cpu(sched, queue_trace);
              sched->context_switches++;
              break;
            }
          }
        }

        pthread_create(&trace->tid, NULL, &sm_core_process, sig.si_ptr);
        break;

      case SIG_PROCESS_END:
        LOGERR("Process `%s` Terminated!", trace->pname);
        sm_out_trace_print(trace);

        sm_core_release_process(sched, trace);
        if (!sm_queue_empty(sched->proc_queue)) {
          queue_trace = sm_queue_front(sched->proc_queue);
          sm_queue_remove(sched->proc_queue);
          sm_core_assign_process_to_cpu(sched, queue_trace);
        }

        traces_size--;
        LOGERR("Traces_size now: %lu", traces_size);
        break;
    }
    trace = NULL;
  }

  return sched;
}

#endif
