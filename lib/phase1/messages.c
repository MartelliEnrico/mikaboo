#include "mikabooq.h"
#include "const.h"

static struct list_head msg_free;

void msgq_init(void) {
	static struct msg_t msgs[MAXMSG];

	INIT_LIST_HEAD(&msg_free);

	// aggiungiamo tutti i msg alla lista libera
	for(int i = 0; i < MAXMSG; i++) {
		list_add(&msgs[i].m_next, &msg_free);
	}
}

int msgq_add(struct tcb_t *sender, struct tcb_t *destination, uintptr_t value) {
	if(sender == NULL || destination == NULL || list_empty(&msg_free)) {
		return -1;
	}

	// prendiamo il primo msg dalla lista libera
	struct msg_t *msg = next_msg(&msg_free);
	// e lo rimuoviamo
	list_del(&msg->m_next);
	// settiamo sender, value e lo aggiungiamo all lista dei messaggi
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

	// se non conosciamo il mittente
	if(sender == NULL || *sender == NULL) {
		// prendiamo il primo messaggio
		msg = next_msg(&destination->t_msgq);
		if(sender != NULL) {
			*sender = msg->m_sender;
		}
	// se conosiamo il mittente
	} else {
		// cerchiamo il primo messaggio con il mittente selezionato
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

	// salviamo il valore
	*value = msg->m_value;
	// rimuoviamo il messaggio dalla lista
	list_del(&msg->m_next);
	// e lo aggiungiamo alla lista libera
	list_add_tail(&msg->m_next, &msg_free);

	return 0;
}
