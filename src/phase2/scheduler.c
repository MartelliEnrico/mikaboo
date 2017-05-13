#include <libuarm.h>

#include "const.h"
#include "listx.h"
#include "kernel.h"
#include "mikabooq.h"
#include "scheduler.h"

static void tick(int slice) {
	// update clock references
	unsigned int clock = getTODLO();
	pseudo_tick += clock - start_pseudo_tick;
	start_pseudo_tick = current_thread_tod = clock;
	// update timer
	if(pseudo_tick >= SCHED_PSEUDO_CLOCK) {
		setTIMER(1);
	} else {
		setTIMER(MIN(
			SCHED_TIME_SLICE - slice,
			SCHED_PSEUDO_CLOCK - pseudo_tick));
	}
	// load state for current thread
	LDST(&current_thread->t_s);
}

void scheduler() {
	if(current_thread == NULL) {
		if(list_empty(&ready_list)) {
			if(thread_count == 1) {
				HALT();
			} else if(soft_block_count == 0) {
				PANIC();
			} else {
				setSTATUS(STATUS_ALL_INT_ENABLE(getSTATUS()));
				WAIT();
			}
		} else {
			current_thread = thread_dequeue(&ready_list);
			current_thread->t_status = 0;
			tick(0);
			PANIC();
		}
	} else {
		tick(current_thread->t_status);
	}
}
