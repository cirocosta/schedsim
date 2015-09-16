#include "schedsim/core.h"

sm_core_t* sm_core_create(sm_schedulers_e type, sm_core_sched_algorithm* alg)
{
  struct timeval t_start;
  sm_core_t* sched = malloc(sizeof(*sched));

  PASSERT(sched, "Couldn't properly allocate memory");

  sched->algorithm = alg;
  sched->type = type;
  sched->max_cpus = sysconf(_SC_NPROCESSORS_ONLN);
  sched->available_cpus = sched->max_cpus;
  sched->proc_queue = sm_queue_create();

  sched->running_processes =
      calloc(sched->available_cpus, sizeof(*sched->running_processes));
  PASSERT(sched->available_cpus, "Couldn't properly allocate memory");

  gettimeofday(&t_start, NULL);

  sched->start_time = t_start.tv_sec * 1e6 + t_start.tv_usec;

  return sched;
}

void sm_core_destroy(sm_core_t* sched)
{
  sm_queue_destroy(sched->proc_queue);
  FREE(sched->running_processes);
}

sm_core_t* sm_core_run(sm_core_t* sched, sm_trace_t** traces,
                       size_t traces_size)
{
  unsigned i = 0;
  sigset_t intmask, block_set;
  siginfo_t sig;

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

  if (sched->type == SM_ALG_RR || sched->type == SM_ALG_PS)
    sm_core_quantum_create();

  for (; i < traces_size; i++)
    sm_core_dispatcher_create(traces[i]);

  while (traces_size) {
    sigwaitinfo(&intmask, &sig);

    switch (sig.si_signo) {
      case SIG_PROCESS_NEW:
        sched->algorithm->on_process_new(sched, sig);
        break;
      case SIGALRM:
        if (sched->algorithm->on_process_quantum)
          sched->algorithm->on_process_quantum(sched, sig);
        break;
      case SIG_PROCESS_END:
        sched->algorithm->on_process_end(sched, sig);
        traces_size--;
        break;
    }
  }

  return sched;
}

void* sm_core_process(void* arg)
{
  sm_trace_t* trace = (sm_trace_t*)arg;
  struct timeval t_end;
  struct timeval t_start;

  gettimeofday(&t_start, NULL);
  sm_core_process_wastetime(trace);
  gettimeofday(&t_end, NULL);

  trace->out.t0 = t_start.tv_sec * 1e6 + t_start.tv_usec;
  trace->out.tf = t_end.tv_sec * 1e6 + t_end.tv_usec;
  trace->out.tr = trace->out.tf - trace->out.t0;

  sigqueue(getpid(), SIG_PROCESS_END, (const union sigval){.sival_ptr = arg });
  pthread_exit(EXIT_SUCCESS);
}

int sm_core_has_available_cpu(sm_core_t* sched)
{
  return sched->available_cpus > 0;
}

void sm_core_assign_process_to_cpu(sm_core_t* sched, sm_trace_t* trace)
{
  unsigned i = 0;

  for (; i < sched->max_cpus; i++)
    if (!sched->running_processes[i])
      break;

  trace->blocked = 0;
  trace->current_cpu = i;
  sched->available_cpus--;
  sem_post(&trace->sem);
  sched->running_processes[i] = trace;

  LOGERR("process `%s` now using CPU `%d`", trace->pname, trace->current_cpu);
}

void sm_core_release_process(sm_core_t* sched, sm_trace_t* trace)
{
  sched->running_processes[trace->current_cpu] = NULL;
  sched->available_cpus++;
  trace->blocked = 1;

  LOGERR("process `%s` released CPU `%d`", trace->pname, trace->current_cpu);

  trace->current_cpu = -1;
}

void sm_core_process_wastetime(sm_trace_t* trace)
{
  struct timespec start, stop;
  unsigned i;

  trace->remaining_time = trace->dt * 1e9; // nanosecs

  while (trace->remaining_time > 0) {
    // not 'if' because we may have trace->sem's value > 1
    while (trace->blocked)
      sem_wait(&trace->sem);

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);

    i = 1e3;
    while (i-- > 0)
      ;

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop);
    trace->remaining_time -=
        (stop.tv_sec - start.tv_sec) * 1e9 + (stop.tv_nsec - start.tv_nsec);
  }
}

timer_t sm_core_quantum_create()
{
  long long nanosecs = SM_QUANTUM_MS * 1e6;
  struct itimerspec ts;
  struct sigevent se;
  timer_t tid;

  se.sigev_notify = SIGEV_SIGNAL;
  se.sigev_signo = SIGALRM;
  se.sigev_value.sival_ptr = NULL;

  ts.it_value.tv_sec = nanosecs / BILLION;
  ts.it_value.tv_nsec = nanosecs % BILLION;
  ts.it_interval.tv_sec = ts.it_value.tv_sec;
  ts.it_interval.tv_nsec = ts.it_value.tv_nsec;

  PASSERT(!timer_create(CLOCK_MONOTONIC, &se, &tid),
          "timer_create: Couldn't create timer\n");
  PASSERT(!timer_settime(tid, 0, &ts, 0),
          "timer_settime: Couldn't activate timer\n");

  return tid;
}

timer_t sm_core_dispatcher_create(sm_trace_t* trace)
{
  // trace->STUFF comes in secs (floating)
  long long nanosecs = BILLION * trace->t0;
  struct itimerspec ts;
  struct sigevent se;
  timer_t tid;

  se.sigev_notify = SIGEV_SIGNAL;
  se.sigev_signo = SIG_PROCESS_NEW;
  se.sigev_value.sival_ptr = trace;

  ts.it_value.tv_sec = nanosecs / BILLION;
  ts.it_value.tv_nsec = nanosecs % BILLION;
  ts.it_interval.tv_sec = 0;
  ts.it_interval.tv_nsec = 0;

  PASSERT(!timer_create(CLOCK_MONOTONIC, &se, &tid),
          "timer_create: Couldn't create timer\n");
  PASSERT(!timer_settime(tid, 0, &ts, 0),
          "timer_settime: Couldn't activate timer\n");

  return tid;
}
