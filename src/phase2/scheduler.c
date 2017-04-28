#include <libuarm.h>

#include "const.h"
#include "listx.h"
#include "kernel.h"
#include "mikabooq.h"
#include "scheduler.h"

static void tick() {
	kernel.pseudo_tick += getTODLO() - kernel.start_pseudo_tick;
	kernel.start_pseudo_tick = getTODLO();
	kernel.current_thread_tod = getTODLO();
}

void scheduler() {
	if(kernel.current_thread == NULL) {
		if(list_empty(&kernel.ready)) {
			if(kernel.thread_count == 1) {
				HALT();
			} else if(kernel.soft_block_count == 0) {
				PANIC();
			} else {
				setSTATUS(STATUS_ALL_INT_ENABLE(getSTATUS()));
				WAIT();
			}
		} else {
			kernel.current_thread = thread_dequeue(&kernel.ready);
			kernel.current_thread->t_status = 0;
			tick();
			if(kernel.pseudo_tick >= SCHED_PSEUDO_CLOCK) {
				setTIMER(1);
			} else {
				setTIMER(MIN(
					SCHED_TIME_SLICE,
					SCHED_PSEUDO_CLOCK - kernel.pseudo_tick));
			}
			LDST(&kernel.current_thread->t_s);
			PANIC();
		}
	} else {
		tick();
		if(kernel.pseudo_tick >= SCHED_PSEUDO_CLOCK) {
			setTIMER(1);
		} else {
			setTIMER(MIN(
				SCHED_TIME_SLICE - kernel.current_thread->t_status,
				SCHED_PSEUDO_CLOCK - kernel.pseudo_tick));
		}
		LDST(&kernel.current_thread->t_s);
	}
}
