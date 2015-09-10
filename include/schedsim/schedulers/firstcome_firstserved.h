#ifndef SCHEDSIM__FIRSTCOME_FIRSTSERVER_H
#define SCHEDSIM__FIRSTCOME_FIRSTSERVER_H

#include "schedsim/common.h"
#include "schedsim/queue.h"
#include "schedsim/algorithms.h"

#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

typedef struct sm_scheduler_t {
  sm_queue_t* proc_queue;
  pthread_mutex_t proc_mutex;
  unsigned available_cpus;
} sm_scheduler_t;

void* process(void* arg)
{
  LOGERR("A process was spawned! %lu", pthread_self());

  pthread_exit(EXIT_SUCCESS);
}

// main
void sm_sched_firstcome_firstserved(sm_trace_t** traces, size_t traces_size)
{
  unsigned i = 0;
  sigset_t intmask, block_set;
  int sig;
  pthread_t tids[100] = {0};

  if ((sigemptyset(&block_set) == -1) ||
      (sigaddset(&block_set, SIGUSR1) == -1) ||
      (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1)) {
    LOGERR("Error setting thread signal mask: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((sigemptyset(&intmask) == -1) || (sigaddset(&intmask, SIGUSR1) == -1)) {
    perror("sig_setting error:");
    exit(EXIT_FAILURE);
  }

  for (; i < traces_size; i++) {
    sm_create_timer(traces[i]);
  }

  i = 0;
  while (1) {
    sigwait(&intmask, &sig);
    fprintf(stderr, "%s\n", "Signal Received!");
    pthread_create(&tids[i], NULL, &process, NULL);
  }
}

#endif
