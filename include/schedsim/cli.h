#ifndef SCHEDSIM__CLI_H
#define SCHEDSIM__CLI_H

#include "schedsim/common.h"
#include "schedsim/scheduler.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
    "\t6\tReal-Time scheduling with rigid Deadlines\n"
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

const static char* SM_ERR_MALFORMED_TRACE =
    "ERROR: Malformed trace.\n"
    "\tShould be: <t0 : f> <pname : str> <dt : f> <deadline : f> <p : int>";

typedef struct sm_cli_args_t {
  sm_schedulers_e sched_alg;
  char in_fname[PATH_MAX];
  char out_fname[PATH_MAX];
  int debug;
} sm_cli_args_t;

void sm_cli_help();
sm_cli_args_t* sm_cli_parse(const int argc, const char** argv);
void sm_cli_args_destroy(sm_cli_args_t* args);

#endif
