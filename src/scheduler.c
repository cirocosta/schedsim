#include "schedsim/scheduler.h"

sm_scheduler_t* sm_scheduler_create(sm_schedulers_e type)
{
  sm_scheduler_t* sched = malloc(sizeof(*sched));
  PASSERT(sched, "Couldn't properly allocate memory");

  sched->type = type;
  sched->available_cpus = sysconf(_SC_NPROCESSORS_ONLN);
  pthread_mutex_init(&sched->proc_mutex, NULL);
  sched->proc_queue = sm_queue_create();

  return sched;
}

void sm_scheduler_destroy(sm_scheduler_t* sched)
{
  pthread_mutex_destroy(&sched->proc_mutex);
  sm_queue_destroy(sched->proc_queue);
}

void sm_waste_time(sm_trace_t* trace)
{
  struct timespec start, stop;
  double end_time = trace->dt * 1e6;
  double tot_time = 0;
  unsigned i;

  while (tot_time < end_time) {
    if (trace->blocked)
      sem_wait(&trace->sem);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    i = 1e6;
    while (i-- > 0)
      ;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    tot_time += (stop.tv_sec - start.tv_sec) * 1e6 +
                (stop.tv_nsec - start.tv_nsec) / 1e3;
  }
}

timer_t sm_create_timer(sm_trace_t* trace)
{
  // trace->STUFF comes in secs (floating)
  long long nanosecs = BILLION * trace->t0;
  struct itimerspec ts;
  struct sigevent se;
  timer_t tid;

  se.sigev_notify = SIGEV_SIGNAL;
  se.sigev_signo = SIGUSR1;
  se.sigev_value.sival_ptr = trace;

  ts.it_value.tv_sec = nanosecs / BILLION;
  ts.it_value.tv_nsec = nanosecs % BILLION;
  // do it only once (maybe we'll change this for round robin)
  ts.it_interval.tv_sec = 0;
  ts.it_interval.tv_nsec = 0;

  ASSERT(!timer_create(CLOCK_MONOTONIC, &se, &tid), "Couldn't create timer");
  ASSERT(!timer_settime(tid, 0, &ts, 0), "Coudln't active timer");

  return tid;
}
