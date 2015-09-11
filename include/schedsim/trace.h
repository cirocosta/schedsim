#ifndef SCHEDSIM__TRACE_H
#define SCHEDSIM__TRACE_H

#include "schedsim/common.h"

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

typedef struct sm_out_trace_t {
  double tf; // timestamp of process termination
  double tr; // clock time that it took to execute (tf-t0)
  double t0; // actual start
} sm_out_trace_t;

typedef struct sm_trace_t {
  float t0;             // time at which the process comes
  char pname[NAME_MAX]; // process' name
  float dt;             // cpu real time
  float deadline;       // time at which the process must stop
  int p;                // priority

  pthread_t tid;
  sem_t sem;
  int blocked;
  int current_cpu;
  double remaining_time;

  sm_out_trace_t out; // result
} sm_trace_t;

// ctor & dtor
sm_trace_t* sm_trace_create();
void sm_trace_destroy(sm_trace_t* trace);

// general
void sm_out_trace_print(sm_trace_t* trace);
void sm_trace_print(sm_trace_t* trace);
sm_trace_t* sm_parse_trace(const char* trace);
sm_trace_t** sm_get_traces(const char* fname, size_t* entries);

#endif
