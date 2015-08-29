#include "schedsim/algorithms.h"

static pthread_mutex_t g_mutex_queue = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond_counter = PTHREAD_COND_INITIALIZER;
static pthread_cond_t g_cond_queue = PTHREAD_COND_INITIALIZER;
static sm_queue_t* g_queue;

void sm_waste_time(float time_to_spend)
{
  const struct timespec req = {.tv_nsec = time_to_spend };
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

  LOG("Finished!");
}

void ticket_acquire(unsigned my, sm_queue_t* q)
{
}

void notify_function(union sigval val)
{
  sm_trace_t* trace = (sm_trace_t*)val.sival_ptr;
  unsigned my_ticket;

  ASSERT(!pthread_mutex_lock(&g_mutex_queue), "Couldn't acquire mutex");
  my_ticket = sm_queue_insert(g_queue, trace);

  while (my_ticket != g_queue->f) {
    ASSERT(!pthread_cond_wait(&g_cond_queue, &g_mutex_queue), "");
  }

  ASSERT(!pthread_mutex_unlock(&g_mutex_queue), "Couldn't unlock mutex");

  sm_waste_time(trace->dt);

  ASSERT(!pthread_mutex_lock(&g_mutex_queue), "Couldn't acquire mutex");
  sm_queue_remove(g_queue);
  pthread_cond_broadcast(&g_cond_queue);

  if (sm_queue_empty(g_queue))
    pthread_cond_signal(&g_cond_counter);

  ASSERT(!pthread_mutex_unlock(&g_mutex_queue), "Couldn't unlock mutex");
}

timer_t sm_create_timer(sm_trace_t* trace, void(func)(union sigval))
{
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
  ts.it_interval.tv_sec = 0;
  ts.it_interval.tv_nsec = 0;

  ASSERT(!timer_create(CLOCK_REALTIME, &se, &tid), "Couldn't create timer");
  ASSERT(!timer_settime(tid, 0, &ts, 0), "Coudln't active timer");

  return tid;
}

void sm_sched_firstcome_firstserved(sm_trace_t** traces, size_t traces_size)
{
  unsigned i = 0;
  sm_trace_t* trace = NULL;

  g_queue = sm_queue_create();

  for (; i < traces_size; i++) {
    trace = traces[i];
    sm_create_timer(trace, notify_function);
  }

  ASSERT(!pthread_mutex_lock(&g_mutex_queue), "");
  ASSERT(!pthread_cond_wait(&g_cond_counter, &g_mutex_queue), "");
  while (!sm_queue_empty(g_queue))
    ASSERT(!pthread_cond_wait(&g_cond_counter, &g_mutex_queue), "");
  ASSERT(!pthread_mutex_unlock(&g_mutex_queue), "");

  pthread_mutex_destroy(&g_mutex_queue);
  pthread_cond_destroy(&g_cond_queue);
  pthread_cond_destroy(&g_cond_counter);
  sm_queue_delete(g_queue);
}
