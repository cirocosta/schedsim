#ifndef SCHEDSIM__ROUND_ROBIN_H
#define SCHEDSIM__ROUND_ROBIN_H

#include "schedsim/common.h"
#include "schedsim/core.h"

#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

inline static sm_core_t* sm_sched_round_robin(sm_trace_t** traces, size_t traces_size)
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
      (sigaddset(&block_set, SIGALRM) == -1) ||
      (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1)) {
    LOGERR("Error setting thread signal mask: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((sigemptyset(&intmask) == -1) ||
      (sigaddset(&intmask, SIG_PROCESS_NEW) == -1) ||
      (sigaddset(&intmask, SIGALRM) == -1) ||
      sigaddset(&intmask, SIG_PROCESS_END)) {
    perror("sig_setting error:");
    exit(EXIT_FAILURE);
  }

  // preares the quantum timer (a timer that dispatches a given signal that we 
  // decide in a regular basis).
  sm_core_quantum_create();
  for (; i < traces_size; i++)
    sm_core_dispatcher_create(traces[i]);
  // PREPARATION

  while (traces_size) {
    sigwaitinfo(&intmask, &sig);

    switch (sig.si_signo) {
      case SIG_PROCESS_NEW:
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
        break;

      // processes the QUANTUM signal
      case SIGALRM:
        i = 0;
        // in case we have more CPUs than elements (also checks for empty q)
        for (; i < sched->max_cpus && sched->proc_queue->length; i++) {
          queue_trace = sm_queue_front(sched->proc_queue);
          sm_queue_remove(sched->proc_queue);
          sm_queue_insert(sched->proc_queue, sched->running_processes[i]);
          sm_core_release_process(sched, sched->running_processes[i]);
          sm_core_assign_process_to_cpu(sched, queue_trace);
          sched->context_switches++;
        }

        break;

      case SIG_PROCESS_END:
        trace = (sm_trace_t*)sig.si_ptr;
        LOGERR("Process `%s` Terminated!", trace->pname);
        sm_out_trace_print(trace);

        traces_size--;
        LOGERR("Traces_size now: %lu", traces_size);
        break;
    }
    trace = NULL;
    queue_trace = NULL;
  }

  return sched;
}

#endif
