#include "schedsim/cli.h"

void sm_cli_help()
{
  fprintf(stderr, "%s\n", SM_CLI_HELP_TXT);
}

sm_cli_args_t* sm_cli_parse(const int argc, const char** argv)
{
  sm_cli_args_t* args = NULL;

  if (argc < 4) {
    sm_cli_help();
    fprintf(stderr, "%s\n", "Error: Not enough arguments provided.");
    exit(EXIT_FAILURE);
  }

  ASSERT((args = malloc(sizeof(*args))), "Couldn't allocate memory");
  args->sched_alg = (enum sm_sched_algorithms)strtoul(argv[1], NULL, 10);

  if (args->sched_alg > SM_RT_RIGID_READLINES ||
      args->sched_alg < SM_FIRSTCOME_FIRSTSERVED) {
    sm_cli_help();
    fprintf(stderr, "%s\n", "Error: invalid scheduling algorithm number.");
    exit(EXIT_FAILURE);
  }

  strncpy(args->in_fname, argv[2], PATH_MAX);
  strncpy(args->out_fname, argv[3], PATH_MAX);
  args->debug = argc == 5;

  return args;
}


sm_in_entry_t* sm_parse_trace(const char* trace)
{
  sm_in_entry_t* in_trace;
  char* curr;
  char* end;

  ASSERT((in_trace = malloc(sizeof(*in_trace))),
         "sm_parse_trace: couldn't allocate mem");
  // t0 : float
  in_trace->t0 = strtof(trace, &curr);
  curr++;

  // pname: string
  end = strchr(curr, ' ');

  ASSERT(end, "%s", SM_ERR_MALFORMED_TRACE);
  strncpy(in_trace->pname, curr, end - curr);
  curr = end;

  // dt: float
  in_trace->dt = strtof(curr, &end);
  ASSERT(curr != end, "%s", SM_ERR_MALFORMED_TRACE);
  curr = end;

  // deadline: float
  in_trace->deadline = strtof(curr, &end);
  ASSERT(curr != end, "%s", SM_ERR_MALFORMED_TRACE);
  curr = end;

  // p: int
  in_trace->p = strtol(curr, NULL, 10);

  return in_trace;
}
