#include "mikabooq.h"
#include "const.h"

static struct list_head tcb_free;

void thread_init(void) {
	static struct tcb_t tcbs[MAXTHREAD];

	INIT_LIST_HEAD(&tcb_free);

	for(int i = 0; i < MAXTHREAD; i++) {
		list_add(&tcbs[i].t_next, &tcb_free);
	}
}

struct tcb_t *thread_alloc(struct pcb_t *process) {
	if(process == NULL || list_empty(&tcb_free)) {
		return NULL;
	}

	struct tcb_t *tcb = container_of(list_next(&tcb_free), struct tcb_t, t_next);
	list_del(&tcb->t_next);
	tcb->t_pcb = process;
	tcb->t_status = T_STATUS_NONE;
	tcb->t_wait4sender = NULL;
	list_add_tail(&tcb->t_next, &process->p_threads);
	INIT_LIST_HEAD(&tcb->t_sched);
	INIT_LIST_HEAD(&tcb->t_msgq);

	return tcb;
}

int thread_free(struct tcb_t *oldthread) {
	if(! list_empty(&oldthread->t_msgq)) {
		return -1;
	}

	list_del(&oldthread->t_next);
	list_add_tail(&oldthread->t_next, &tcb_free);

	return 0;
}

void thread_enqueue(struct tcb_t *new, struct list_head *queue) {
	list_add(&new->t_next, queue);
}

struct tcb_t *thread_qhead(struct list_head *queue) {
	if(list_empty(queue)) {
		return NULL;
	}

	return container_of(list_next(queue), struct tcb_t, t_next);
}

struct tcb_t *thread_dequeue(struct list_head *queue) {
	if(list_empty(queue)) {
		return NULL;
	}

	struct tcb_t *tcb = container_of(list_prev(queue), struct tcb_t, t_next);
	list_del(&tcb->t_next);

	return tcb;
}
