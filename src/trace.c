#include "schedsim/trace.h"

static const char SM_SPACE = ' ';

sm_trace_t* sm_trace_create()
{
  sm_trace_t* trace = malloc(sizeof(*trace));
  PASSERT(trace, "couldn't allocate mem");

  *trace = (sm_trace_t){ 0 };
  sem_init(&trace->sem, 0, 0);

  return trace;
}

void sm_trace_destroy(sm_trace_t* trace)
{
  sem_destroy(&trace->sem);
  FREE(trace);
}

void sm_out_trace_print(sm_trace_t* trace)
{
  LOGERR("%s %f %f", trace->pname, trace->out.tf/1e6, trace->out.tr/1e6);
}

void sm_trace_print(sm_trace_t* trace)
{
  LOGERR("%f %s %f %f %d", trace->t0, trace->pname, trace->dt, trace->deadline,
         trace->p);
}

sm_trace_t* sm_parse_trace(const char* trace)
{
  sm_trace_t* in_trace;
  char* curr = NULL;
  char* end = NULL;

  in_trace = sm_trace_create();
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
  in_trace->p = strtol(curr, &end, 10);
  DASSERT(curr != end, "%s", SM_ERR_MALFORMED_TRACE);
  DASSERT((in_trace->p < 20 && in_trace->p >= -20), "%s",
          SM_ERR_MALFORMED_TRACE);

  return in_trace;
}

sm_trace_t** sm_trace_parse_file(const char* fname, size_t* entries)
{
  char buf[MAX_INPUT];
  sm_trace_t** traces = NULL;
  FILE* fp = NULL;
  int ch;
  unsigned lines = 0;

  ASSERT((fp = fopen(fname, "r")), "sm_trace_parse_file: couldn't open %s\n", fname);

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
