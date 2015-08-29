#include "schedsim/trace.h"
#include "schedsim/algorithms.h"

static const char SM_SPACE = ' ';

sm_trace_t* sm_parse_trace(const char* trace)
{
  sm_trace_t* in_trace;
  char* curr = NULL;
  char* end = NULL;

  ASSERT((in_trace = malloc(sizeof(*in_trace))),
         "sm_parse_trace: couldn't allocate mem");
  // t0 : float
  in_trace->t0 = strtof(trace, &curr);
  curr++;

  // pname: string
  end = strchr(curr, SM_SPACE);

  DASSERT(end, "%s", SM_ERR_MALFORMED_TRACE);
  strncpy(in_trace->pname, curr, end - curr);
  in_trace->pname[end - curr] = '\0';
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
  char buf[MAX_INPUT];
  sm_trace_t** traces = NULL;
  FILE* fp = NULL;
  int ch;
  unsigned lines = 0;

  ASSERT((fp = fopen(fname, "r")), "sm_get_traces: couldn't open %s\n", fname);

  // TODO mess around w/ this
  //      verify perf improv w/ kcachegrind
  /* posix_fadvise(fileno(fp), 0, 0, POSIX_FADV_SEQUENTIAL); */

  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '\n')
      lines++;
  }

  fseek(fp, 0, SEEK_SET);
  traces = malloc(sizeof(*traces) * lines);
  ASSERT(traces, "mem allocation error");
  lines = 0;

  while (fgets(buf, MAX_INPUT, fp))
    traces[lines++] = sm_parse_trace(buf);

  ASSERT(~fclose(fp), "Error while closing file %s: %s", fname,
         strerror(errno));
  *entries = lines;

  return traces;
}
