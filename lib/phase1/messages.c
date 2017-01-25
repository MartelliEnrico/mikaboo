#include "mikabooq.h"
#include "const.h"

static struct list_head msg_free;

void msgq_init(void) {
	static struct msg_t msgs[MAXMSG];

	INIT_LIST_HEAD(&msg_free);

	for(int i = 0; i < MAXMSG; i++) {
		list_add(&msgs[i].m_next, &msg_free);
	}
}

int msgq_add(struct tcb_t *sender, struct tcb_t *destination, uintptr_t value) {
	if(sender == NULL || destination == NULL || list_empty(&msg_free)) {
		return -1;
	}

	struct msg_t *msg = next_msg(&msg_free);
	list_del(&msg->m_next);
	msg->m_sender = sender;
	msg->m_value = value;
	list_add_tail(&msg->m_next, &destination->t_msgq);

	return 0;
}

int msgq_get(struct tcb_t **sender, struct tcb_t *destination, uintptr_t *value) {
	if(destination == NULL || list_empty(&destination->t_msgq)) {
		return -1;
	}

	struct msg_t *msg = NULL;

	if(sender == NULL || *sender == NULL) {
		msg = next_msg(&destination->t_msgq);
		if(sender != NULL) {
			*sender = msg->m_sender;
		}
	} else {
		struct msg_t *item;
		list_for_each_entry(item, &destination->t_msgq, m_next) {
			if(item->m_sender == *sender) {
				msg = item;
			}
		}
	}

	if(msg == NULL) {
		return -1;
	}

	*value = msg->m_value;
	list_del(&msg->m_next);
	list_add_tail(&msg->m_next, &msg_free);

	return 0;
}
