#include "mikabooq.h"
#include "const.h"

static struct list_head pcb_free;

struct pcb_t *proc_init(void) {
	static struct pcb_t pcbs[MAXPROC];

	INIT_LIST_HEAD(&pcb_free);

	for(int i = 1; i < MAXPROC; i++) {
		list_add(&pcbs[i].p_siblings, &pcb_free);
	}

	INIT_LIST_HEAD(&pcbs[0].p_threads);
	INIT_LIST_HEAD(&pcbs[0].p_children);
	INIT_LIST_HEAD(&pcbs[0].p_siblings);

	return &pcbs[0];
}

struct pcb_t *proc_alloc(struct pcb_t *p_parent) {
	if(p_parent == NULL || list_empty(&pcb_free)) {
		return NULL;
	}

	struct pcb_t *pcb = next_pcb(&pcb_free);
	list_del(&pcb->p_siblings);
	pcb->p_parent = p_parent;
	INIT_LIST_HEAD(&pcb->p_threads);
	INIT_LIST_HEAD(&pcb->p_children);
	list_add_tail(&pcb->p_siblings, &p_parent->p_children);

	return pcb;
}

int proc_delete(struct pcb_t *oldproc) {
	if(oldproc == NULL || ! list_empty(&oldproc->p_threads) || ! list_empty(&oldproc->p_children)) {
		return -1;
	}

	list_del(&oldproc->p_siblings);
	list_add_tail(&oldproc->p_siblings, &pcb_free);

	return 0;
}

struct pcb_t *proc_firstchild(struct pcb_t *proc) {
	if(proc == NULL || list_empty(&proc->p_children)) {
		return NULL;
	}

	return next_pcb(&proc->p_children);
}

struct tcb_t *proc_firstthread(struct pcb_t *proc) {
	if(proc == NULL || list_empty(&proc->p_threads)) {
		return NULL;
	}

	return next_tcb(&proc->p_threads);
}
