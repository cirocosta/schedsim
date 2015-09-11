#ifndef SCHEDSIM__FIRSTCOME_FIRSTSERVER_H
#define SCHEDSIM__FIRSTCOME_FIRSTSERVER_H

#include "schedsim/common.h"
#include "schedsim/scheduler.h"

#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

void* process(void* arg)
{
  sm_trace_t* trace = (sm_trace_t*)arg;
  struct timeval t_end;
  struct timeval t_start;

  gettimeofday(&t_start, NULL);
  sm_waste_time(trace);
  gettimeofday(&t_end, NULL);

  trace->out.t0 = t_start.tv_sec*1e6 + t_start.tv_usec;
  trace->out.tf = t_end.tv_sec*1e6 + t_end.tv_usec;
  trace->out.tr = trace->out.tf - trace->out.t0;

  sigqueue(getpid(), SIG_PROCESS_END, (const union sigval){.sival_ptr = arg });
  pthread_exit(EXIT_SUCCESS);
}

// main
void sm_sched_firstcome_firstserved(sm_trace_t** traces, size_t traces_size)
{
  unsigned i = 0;
  sigset_t intmask, block_set;
  siginfo_t sig;
  sm_trace_t* trace = NULL;
  sm_trace_t* queue_trace = NULL;
  sm_scheduler_t* sched = sm_scheduler_create(SM_FIRSTCOME_FIRSTSERVED);

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
    sm_create_timer(traces[i]);

  while (traces_size) {
    sigwaitinfo(&intmask, &sig);
    trace = (sm_trace_t*)sig.si_ptr;

    switch (sig.si_signo) {
      case SIG_PROCESS_NEW:
        LOGERR("New process in the system!");
        sm_trace_print(trace);

        pthread_mutex_lock(&sched->proc_mutex);
        if (sched->available_cpus > 0) {
          trace->blocked = 0;
          sched->available_cpus--;
        } else {
          trace->blocked = 1;
          sm_queue_insert(sched->proc_queue, trace);
        }
        pthread_mutex_unlock(&sched->proc_mutex);

        pthread_create(&trace->tid, NULL, &process, sig.si_ptr);
        break;

      case SIG_PROCESS_END:
        LOGERR("Process Terminated!");
        sm_out_trace_print(trace);

        pthread_mutex_lock(&sched->proc_mutex);
        sched->available_cpus++;
        if (!sm_queue_empty(sched->proc_queue)) {
          queue_trace = sm_queue_front(sched->proc_queue);
          sm_queue_remove(sched->proc_queue);
          queue_trace->blocked = 0;
          sem_post(&queue_trace->sem);
        }
        pthread_mutex_unlock(&sched->proc_mutex);

        traces_size--;
        break;
    }
    trace = NULL;
  }

  sm_scheduler_destroy(sched);
}

#endif
