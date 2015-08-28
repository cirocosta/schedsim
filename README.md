# schedsim

> A simplistic process scheduler

## Input

```
Usage:
  ./schedsim <sched> <in> <out> [d]

Parameters:
  sched:    name of the scheduling algorithm
  in:       name of the trace file (input)
  out:      name of the result file (output) to be created
  d:        (optional) output debug info to stderr

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
$ make 
```


## LICENSE

MPLv2. See `LICENSE`.

