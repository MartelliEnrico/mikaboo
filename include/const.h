#ifndef CONST_H
#define CONST_H

#define MAXPROC 20
#define MAXTHREAD 30
#define MAXMSG 40

/* Scheduling constants */
#define SCHED_TIME_SLICE 5000U     /* in microseconds, aka 5 milliseconds */
#define SCHED_PSEUDO_CLOCK 100000U /* pseudo-clock tick "slice" length */
#define SCHED_BOGUS_SLICE 500000U  /* just to make sure */

#endif
