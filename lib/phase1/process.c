#include "mikabooq.h"
#include "const.h"

static struct list_head pcb_free;

struct pcb_t *proc_init(void) {
	static struct pcb_t pcbs[MAXPROC];

	INIT_LIST_HEAD(&pcb_free);

	// aggiungiamo tutti i pcb tranne il primo alla lista libera
	for(int i = 1; i < MAXPROC; i++) {
		list_add(&pcbs[i].p_siblings, &pcb_free);
	}

	pcbs[0].p_parent = NULL;
	// inizializiamo le liste del pcb root
	INIT_LIST_HEAD(&pcbs[0].p_threads);
	INIT_LIST_HEAD(&pcbs[0].p_children);
	INIT_LIST_HEAD(&pcbs[0].p_siblings);

	return &pcbs[0];
}

struct pcb_t *proc_alloc(struct pcb_t *p_parent) {
	if(p_parent == NULL || list_empty(&pcb_free)) {
		return NULL;
	}

	// prendiamo il primo pcb dalla lista libera
	struct pcb_t *pcb = container_of(list_next(&pcb_free), struct pcb_t, p_siblings);
	// e lo rimuoviamo
	list_del(&pcb->p_siblings);
	// settiamo il padre, inizializiamo le liste e lo aggiungiamo alla lista dei figli
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

	// rimuoviamo il processo dalla lista dei figli
	list_del(&oldproc->p_siblings);
	// e lo spostiamo nella lista libera
	list_add_tail(&oldproc->p_siblings, &pcb_free);

	return 0;
}

struct pcb_t *proc_firstchild(struct pcb_t *proc) {
	if(proc == NULL || list_empty(&proc->p_children)) {
		return NULL;
	}

	return container_of(list_next(&proc->p_children), struct pcb_t, p_siblings);
}

struct tcb_t *proc_firstthread(struct pcb_t *proc) {
	if(proc == NULL || list_empty(&proc->p_threads)) {
		return NULL;
	}

	return container_of(list_next(&proc->p_threads), struct tcb_t, t_next);
}
