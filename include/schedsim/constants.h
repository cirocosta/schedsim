#ifndef SHEDSIM__CONSTANTS_H
#define SHEDSIM__CONSTANTS_H

#define BILLION 1000000000
#define SM_PID_MAX 32768

#define SIG_PROCESS_NEW SIGUSR1
#define SIG_PROCESS_END SIGUSR2
#define SIG_PROCESS_QUANTUM SIGALRM

#define SM_QUANTUM_MS 200

typedef enum sm_schedulers_e {
  SM_ALG_FCFS = 1,
  SM_ALG_SJF,
  SM_ALG_SRTN,
  SM_ALG_RR,
  SM_ALG_PS,
  SM_ALG_RD,
} sm_schedulers_e;

#endif
