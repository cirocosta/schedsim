#ifndef SCHEDSIM__ROUND_ROBIN_H
#define SCHEDSIM__ROUND_ROBIN_H

#include "schedsim/common.h"
#include "schedsim/scheduler.h"

#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

// main
sm_scheduler_t* sm_sched_round_robin(sm_trace_t** traces, size_t traces_size)
{
  unsigned i = 0;
  sigset_t intmask, block_set;
  siginfo_t sig;
  sm_trace_t* trace = NULL;
  sm_trace_t* queue_trace = NULL;
  sm_scheduler_t* sched = sm_scheduler_create(SM_S_JOB_FIRST);

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

  sm_create_quantum_timer();
  for (; i < traces_size; i++)
    sm_create_timer(traces[i]);
  // PREPARATION

  while (traces_size) {
    sigwaitinfo(&intmask, &sig);

    switch (sig.si_signo) {
      case SIG_PROCESS_NEW:
        trace = (sm_trace_t*)sig.si_ptr;
        LOGERR("New process in the system!");
        sm_trace_print(trace);

        if (sm_sched_has_available_cpu(sched)) {
          sm_sched_assign_process_to_cpu(sched, trace);
        } else {
          trace->blocked = 1;
          sm_queue_insert(sched->proc_queue, trace);
        }

        pthread_create(&trace->tid, NULL, &sm_user_process, sig.si_ptr);
        break;

      case SIGALRM:
        i = 0;
        // in case we have more CPUs than elements (also checks for empty q)
        for (; i < sched->max_cpus && sched->proc_queue->length; i++) {
          queue_trace = sm_queue_front(sched->proc_queue);
          sm_queue_remove(sched->proc_queue);
          sm_queue_insert(sched->proc_queue, sched->running_processes[i]);
          sm_sched_release_process(sched, sched->running_processes[i]);
          sm_sched_assign_process_to_cpu(sched, queue_trace);
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
