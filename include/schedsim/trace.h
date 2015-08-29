#ifndef SCHEDSIM__TRACE_H
#define SCHEDSIM__TRACE_H 

#include "schedsim/common.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

typedef struct sm_trace_t {
  float t0;             // time at which the process comes
  char pname[NAME_MAX]; // process' name
  float dt;             // cpu real time
  float deadline;       // time at which the process must stop
  int p;                // priority
} sm_trace_t;

typedef struct sm_out_trace {
  sm_trace_t* trace; // input trace that generated it
  float tf;          // timestamp of process termination
  float tr;          // clock time that it took to execute (tf-t0)
} sm_out_trace;

sm_trace_t* sm_parse_trace(const char* trace);
sm_trace_t** sm_get_traces(const char* fname, size_t* entries);

#endif
