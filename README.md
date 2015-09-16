# schedsim

> A simplistic process scheduler

## Input

```
Usage:
  ./schedsim <sched> <in> <out> [d]

Parameters:
  sched:    scheduling algorithm number [s_id] (see below)
  in:       trace file (input) filename
  out:      result file (output) to be created
  d:        (optional) output debug info to stderr

Scheduling Algorithms:
  s_id  name
  1     First-Come First-Served
  2     Shortest Job First
  3     Shortest Remaining Time Next
  4     Round-Robin
  5     Priority Scheduling
  6     Real-Time Scheduling with Rigid Deadlines

Input:
  t0 n dt dl p  (per process)

Input Parameters:
  t0:   timestamp (seconds) in which the process arrives
  n:    name of the process
  dt:   time taken by the process to run
  dl:   deadline, corresponds to the end time of the process
  p:    priority [-20, 20)

Output:
  name tf tr    (per process)
  (...)
  number_of_context_switches

Output Parameters:
  name:   name of the process
  tf:     instant of time when the process finished
  tr:     clock time that the process took to execute
```


## Building

```sh
$ export BUILD=Release
$ make  

# or 

$ export BUILD=Release
$ make  
```


## LICENSE

MPLv2. See `LICENSE`.

