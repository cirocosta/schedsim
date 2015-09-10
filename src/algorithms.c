#include "schedsim/algorithms.h"

void sm_waste_time(long long nanosecs)
{
  const struct timespec req = {.tv_sec = nanosecs / BILLION,
                               .tv_nsec = nanosecs % BILLION};
  struct timespec rem;
  int err = 0;

  if ((err = clock_nanosleep(CLOCK_MONOTONIC, 0, &req, &rem))) {
    switch (err) {
      case EFAULT:
        LOGERR("%s", "clock_nanosleep request is invalid");
        exit(EXIT_FAILURE);
      case EINVAL:
        LOGERR("%s", "clock_nanosleep got an invalid tv_nsec or the clock_id "
                     "is invalid");
        exit(EXIT_FAILURE);
      case EINTR:
        LOGERR("Interrupted!");
        break;
    }
  }
}

timer_t sm_create_timer(sm_trace_t* trace, void(func)(union sigval))
{
  // trace->STUFF comes in secs (floating)
  long long nanosecs = BILLION * trace->t0;
  struct itimerspec ts;
  struct sigevent se;
  timer_t tid;

  se.sigev_notify = SIGEV_THREAD;
  se.sigev_value.sival_ptr = trace;
  se.sigev_notify_function = func;
  se.sigev_notify_attributes = NULL;

  ts.it_value.tv_sec = nanosecs / BILLION;
  ts.it_value.tv_nsec = nanosecs % BILLION;
  // do it only once (maybe we'll change this for round robin)
  ts.it_interval.tv_sec = 0;
  ts.it_interval.tv_nsec = 0;

  ASSERT(!timer_create(CLOCK_MONOTONIC, &se, &tid), "Couldn't create timer");
  ASSERT(!timer_settime(tid, 0, &ts, 0), "Coudln't active timer");

  return tid;
}
