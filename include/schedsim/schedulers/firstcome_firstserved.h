#ifndef SCHEDSIM__FIRSTCOME_FIRSTSERVER_H
#define SCHEDSIM__FIRSTCOME_FIRSTSERVER_H 

#include "schedsim/common.h"
#include "schedsim/queue.h"
#include "schedsim/algorithms.h"

#include <pthread.h>
#include <sys/time.h>

static unsigned available_cpus = 0;
static pthread_mutex_t g_mutex_queue = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond_counter = PTHREAD_COND_INITIALIZER;
static pthread_cond_t g_cond_queue = PTHREAD_COND_INITIALIZER;
static sm_queue_t* g_queue;


void process_func(union sigval val)
{
  sm_trace_t* trace = (sm_trace_t*)val.sival_ptr;
  unsigned my_ticket;
  struct timeval t_start;
  struct timeval t_end;

  ASSERT(!pthread_mutex_lock(&g_mutex_queue), "Couldn't acquire mutex");
  my_ticket = sm_queue_insert(g_queue, trace);

  while (!((my_ticket == g_queue->f) && (available_cpus > 0))) {
    ASSERT(!pthread_cond_wait(&trace->cond, &g_mutex_queue), "");
  }
  sm_queue_remove(g_queue);
  ASSERT(!pthread_mutex_unlock(&g_mutex_queue), "Couldn't unlock mutex");

  gettimeofday(&t_start, NULL);
  sm_waste_time(BILLION * trace->dt);
  gettimeofday(&t_end, NULL);

  ASSERT(!pthread_mutex_lock(&g_mutex_queue), "Couldn't acquire mutex");
  pthread_cond_broadcast(&g_cond_queue);

  if (sm_queue_empty(g_queue))
    pthread_cond_signal(&g_cond_counter);

  ASSERT(!pthread_mutex_unlock(&g_mutex_queue), "Couldn't unlock mutex");

  trace->out.tr = (t_end.tv_usec - t_start.tv_usec);
  trace->out.tf = trace->t0 + trace->out.tr;
}



void sm_sched_firstcome_firstserved(sm_trace_t** traces, size_t traces_size)
{
  unsigned i = 0;
  sm_trace_t* trace = NULL;

  g_queue = sm_queue_create();

  for (; i < traces_size; i++) {
    trace = traces[i];
    sm_create_timer(trace, process_func);
  }

  ASSERT(!pthread_mutex_lock(&g_mutex_queue), "");
  ASSERT(!pthread_cond_wait(&g_cond_counter, &g_mutex_queue), "");
  while (!sm_queue_empty(g_queue))
    ASSERT(!pthread_cond_wait(&g_cond_counter, &g_mutex_queue), "");
  ASSERT(!pthread_mutex_unlock(&g_mutex_queue), "");

  pthread_mutex_destroy(&g_mutex_queue);
  pthread_cond_destroy(&g_cond_queue);
  pthread_cond_destroy(&g_cond_counter);
  sm_queue_destroy(g_queue);
}


#endif
