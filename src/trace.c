#include "schedsim/trace.h"
#include "schedsim/algorithms.h"

static const char SM_SPACE = ' ';

sm_trace_t* sm_parse_trace(const char* trace)
{
  sm_trace_t* in_trace;
  char* curr;
  char* end;

  ASSERT((in_trace = malloc(sizeof(*in_trace))),
         "sm_parse_trace: couldn't allocate mem");
  // t0 : float
  in_trace->t0 = strtof(trace, &curr);
  curr++;

  // pname: string
  end = strchr(curr, SM_SPACE);

  DASSERT(end, "%s", SM_ERR_MALFORMED_TRACE);
  strncpy(in_trace->pname, curr, end - curr);
  curr = end;

  // dt: float
  in_trace->dt = strtof(curr, &end);
  DASSERT(curr != end, "%s", SM_ERR_MALFORMED_TRACE);
  curr = end;

  // deadline: float
  in_trace->deadline = strtof(curr, &end);
  DASSERT(curr != end, "%s", SM_ERR_MALFORMED_TRACE);
  curr = end;

  // p: int
  in_trace->p = strtol(curr, NULL, 10);
  DASSERT((in_trace->p < 20 && in_trace->p >= -20), "%s",
          SM_ERR_MALFORMED_TRACE);

  return in_trace;
}

sm_trace_t** sm_get_traces(const char* fname, size_t* entries)
{
}
