#include <arch.h>
#include <libuarm.h>
#include <uARMtypes.h>

#include "kernel.h"
#include "nucleus.h"
#include "mikabooq.h"
#include "ssicalls.h"
#include "scheduler.h"
#include "interrupts.h"
#include "exceptions.h"

#define GUARD(cond) if(!(cond)) PANIC();

struct pcb_t *ROOT;
struct tcb_t *SSI;
struct tcb_t *TEST;

static void init_handler(memaddr addr, void handler()) {
	state_t* mem = (state_t*) addr;
	STST(mem);

	mem->pc = (memaddr) handler;
	mem->sp = RAM_TOP;
	mem->cpsr = STATUS_ALL_INT_DISABLE(mem->cpsr | STATUS_SYS_MODE);
}

static void launch_thread(struct tcb_t *thread, void handler()) {
	struct pcb_t *process = proc_alloc(ROOT);
	GUARD(process != NULL);
	thread = thread_alloc(process);
	GUARD(thread != NULL);
	thread->t_s.pc = (memaddr) handler;
	thread->t_s.sp = RAM_TOP - FRAME_SIZE;
	thread->t_s.cpsr = STATUS_ALL_INT_ENABLE(thread->t_s.cpsr) | STATUS_SYS_MODE;
	thread->t_s.CP15_Control = CP15_DISABLE_VM(thread->t_s.CP15_Control);
	thread_enqueue(thread, &kernel.ready);
	kernel.thread_count++;
}

extern void test();

int main() {
	init_handler(INT_NEWAREA, int_handler);
	init_handler(TLB_NEWAREA, tlb_handler);
	init_handler(PGMTRAP_NEWAREA, pgmtrap_handler);
	init_handler(SYSBK_NEWAREA, sysbk_handler);

	ROOT = proc_init();
	GUARD(ROOT != NULL);
	thread_init();
	msgq_init();

	INIT_LIST_HEAD(&kernel.ready);
	INIT_LIST_HEAD(&kernel.wait);

	kernel.thread_count = 0;
	kernel.soft_block_count = 0;
	kernel.current_thread = NULL;
	kernel.pseudo_tick = 0;
	kernel.start_pseudo_tick = 0;
	kernel.current_thread_tod = 0;

	launch_thread(SSI, ssi_handler);
	launch_thread(TEST, test);

	kernel.start_pseudo_tick = getTODLO();

	scheduler();

	return 0;
}
