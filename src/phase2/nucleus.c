#include <arch.h>
#include <libuarm.h>
#include <uARMtypes.h>

#include "nucleus.h"
#include "mikabooq.h"
#include "ssicalls.h"
#include "scheduler.h"
#include "interrupts.h"
#include "exceptions.h"

#define GUARD(cond) if(!(cond)) PANIC();

struct list_head ready_list;
struct list_head wait_list;
unsigned int thread_count;
unsigned int soft_block_count;
struct tcb_t *current_thread;
unsigned int current_thread_tod;
unsigned int pseudo_tick;
unsigned int start_pseudo_tick;

void *SSI;

struct pcb_t *root_pcb;
struct pcb_t *test_pcb;

static void init_handler(memaddr addr, void handler()) {
	state_t *mem = (state_t *)addr;
	STST(mem);
	mem->pc = (memaddr) handler;
	mem->sp = RAM_TOP;
	mem->cpsr = STATUS_ALL_INT_DISABLE(mem->cpsr | STATUS_SYS_MODE);
}

static void launch_thread(struct pcb_t *process, void handler()) {
	struct tcb_t *thread;
	GUARD(process = proc_alloc(root_pcb));
	GUARD(thread = thread_alloc(process));
	thread->t_s.pc = (memaddr) handler;
	thread->t_s.sp = RAM_TOP - FRAME_SIZE;
	thread->t_s.cpsr = STATUS_ALL_INT_ENABLE(thread->t_s.cpsr) | STATUS_SYS_MODE;
	thread->t_s.CP15_Control = CP15_DISABLE_VM(thread->t_s.CP15_Control);
	thread_enqueue(thread, &ready_list);
	thread_count++;
}

extern void test();

int main() {
	init_handler(INT_NEWAREA, int_handler);
	init_handler(TLB_NEWAREA, tlb_handler);
	init_handler(PGMTRAP_NEWAREA, pgmtrap_handler);
	init_handler(SYSBK_NEWAREA, sysbk_handler);

	GUARD(root_pcb = proc_init());
	thread_init();
	msgq_init();

	INIT_LIST_HEAD(&ready_list);
	INIT_LIST_HEAD(&wait_list);

	thread_count = 0;
	soft_block_count = 0;
	current_thread = NULL;
	pseudo_tick = 0;
	current_thread_tod = 0;

	launch_thread(SSI, ssi_handler);
	launch_thread(test_pcb, test);

	start_pseudo_tick = getTODLO();

	scheduler();

	return 0;
}
