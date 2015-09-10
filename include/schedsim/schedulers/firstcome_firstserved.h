#ifndef SCHEDSIM__FIRSTCOME_FIRSTSERVER_H
#define SCHEDSIM__FIRSTCOME_FIRSTSERVER_H

#include "schedsim/common.h"
#include "schedsim/queue.h"
#include "schedsim/algorithms.h"

#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

#define SIG_PROCESS_NEW SIGUSR1
#define SIG_PROCESS_END SIGUSR2

/**
 * SIGUSR1 ---> new process
 * SIGUSR2 ---> process terminated
 */

typedef struct sm_scheduler_t {
  sm_queue_t* proc_queue;
  pthread_mutex_t proc_mutex;
  unsigned available_cpus;
} sm_scheduler_t;

void* process(void* arg)
{
  sm_trace_t* trace = (sm_trace_t*)arg;

  LOGERR("A process was spawned! %lu", pthread_self());
  LOGERR("trace->dt %f", trace->dt);

  sigqueue(getpid(), SIG_PROCESS_END, (const union sigval){.sival_ptr = arg });

  pthread_exit(EXIT_SUCCESS);
}

// main
void sm_sched_firstcome_firstserved(sm_trace_t** traces, size_t traces_size)
{
  unsigned i = 0;
  sigset_t intmask, block_set;
  siginfo_t sig;

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

  for (; i < traces_size; i++) {
    sm_create_timer(traces[i]);
  }

  i = 0;
  while (1) {
    sigwaitinfo(&intmask, &sig);
    switch (sig.si_signo) {
      case SIG_PROCESS_NEW:
        LOGERR("New Process!");
        pthread_create(&((sm_trace_t*)sig.si_ptr)->tid, NULL, &process,
                       sig.si_ptr);
        break;
      case SIG_PROCESS_END:
        LOGERR("Process Terminated! tid = %lu", ((sm_trace_t*)sig.si_ptr)->tid);
        break;
    }
  }
}

#endif
