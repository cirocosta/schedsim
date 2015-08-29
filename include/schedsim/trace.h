#ifndef SCHEDSIM__TRACE_H
#define SCHEDSIM__TRACE_H

#include "schedsim/common.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

// TODO consider double precision
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

  sm_out_trace_t out; // result
} sm_trace_t;

void sm_print_out_trace(sm_out_trace_t);

sm_trace_t* sm_parse_trace(const char* trace);
sm_trace_t** sm_get_traces(const char* fname, size_t* entries);
static inline void sm_trace_delete(sm_trace_t* trace)
{
  FREE(trace);
}

#endif
