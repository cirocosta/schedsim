#include "schedsim/common.h"
#include "schedsim/cli.h"
#include "schedsim/scheduler.h"

#include <stdio.h>

int main(const int argc, const char* argv[])
{
  sm_scheduler_t* sched = NULL;
  unsigned i = 0;
  FILE* fp = NULL;
  size_t traces_count = 0;
  sm_cli_args_t* args = sm_cli_parse(argc, argv);
  sm_trace_t** traces = sm_trace_parse_file(args->in_fname, &traces_count);

#ifndef NDEBUG
  setbuf(stdout, NULL);
#endif

  sched = sm_scheduler_simulate(args->sched_alg, traces, traces_count);

  fp = fopen(args->out_fname, "w");
  ASSERT(fp, "Couldn't open file `%s` for write.", args->out_fname);

  for (; i < traces_count; i++) {
    fprintf(fp, "%s %f %f\n", traces[i]->pname,
            (traces[i]->out.tf - sched->core->start_time) / 1e6,
            traces[i]->out.tr / 1e6);
  }
  fprintf(fp, "%f", sched->core->context_switches);

  fclose(fp);
  sm_scheduler_destroy(sched);
  sm_cli_args_destroy(args);
  while (traces_count-- > 0)
    sm_trace_destroy(traces[i]);

  return 0;
}
