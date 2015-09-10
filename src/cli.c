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
  args->sched_alg = (sm_schedulers_e)strtoul(argv[1], NULL, 10);

  if (args->sched_alg > SM_RT_RIGID_DEADLINES ||
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
