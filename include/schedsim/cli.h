#ifndef SCHEDSIM__CLI_H
#define SCHEDSIM__CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

static const char* SM_CLI_HELP_TXT =
    "Usage:\n"
    "\t./schedsim <sched> <in> <out> [d]\n"
    "\n"
    "Parameters:\n"
    "\tsched:    scheduling algorithm number [s_id] (see below)\n"
    "\tin:       trace file (input) filename\n"
    "\tout:      result file (output) to be created\n"
    "\td:        (optional) output debug info to stderr\n"
    "\n"
    "Scheduling Algorithms:\n"
    "\ts_id\tname\n"
    "\t1\tFirst-Come First-Served\n"
    "\t2\tShortest Job First\n"
    "\t3\tShortest Remaining Time Next\n"
    "\t4\tRound-Robin\n"
    "\t5\tScheduling With Priority\n"
    "\t6\tReal-Time scheduling with rigit Deadlines\n"
    "\n"
    "Input:\n"
    "\tt0 n dt dl p  (per process)\n"
    "\n"
    "Input Parameters:\n"
    "\tt0:   timestamp (seconds) in which the process arrives\n"
    "\tn:    name of the process\n"
    "\tdt:   time taken by the process to run\n"
    "\tdl:   deadline, corresponds to the end time of the process\n"
    "\tp:    priority [-20, 20)\n"
    "\n"
    "Output:\n"
    "\tname tf tr    (per process)\n"
    "\t(...)\n"
    "\tnumber_of_context_switches\n"
    "\n"
    "Output Parameters:\n"
    "\tname:   name of the process\n"
    "\ttf:     instant of time when the process finished\n"
    "\ttr:     clock time that the process took to execute\n"
    "\n";

enum sm_sched_algorithms {
  SM_FIRSTCOME_FIRSTSERVED = 1,
  SM_S_JOB_FIRST,
  SM_S_REMAINING_TIME_NEXT,
  SM_ROUND_ROBIN,
  SM_SCHED_WITH_PRIORITY,
  SM_RT_RIGID_READLINES
};

typedef struct sm_cli_args_t {
  enum sm_sched_algorithms sched_alg;
  char in_fname[PATH_MAX];
  char out_fname[PATH_MAX];
  int debug;
} sm_cli_args_t;

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

  args = malloc(sizeof(*args));
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

void sm_cli_parse_input(const char* filename)
{
}

#endif
