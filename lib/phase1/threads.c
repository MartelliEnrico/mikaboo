#include <mikabooq.h>

void thread_init(void) {

}

struct tcb_t *thread_alloc(struct pcb_t *process) {

}

int thread_free(struct tcb_t *oldthread) {

}

void thread_enqueue(struct tcb_t *new, struct list_head *queue) {

}

struct tcb_t *thread_qhead(struct list_head *queue) {

}

struct tcb_t *thread_dequeue(struct list_head *queue) {

}
