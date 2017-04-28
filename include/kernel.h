#pragma once

#include "listx.h"
#include "mikabooq.h"

struct {
	struct list_head ready;
	struct list_head wait;

	unsigned int thread_count;
	unsigned int soft_block_count;
	struct tcb_t *current_thread;
	unsigned int current_thread_tod;
	unsigned int pseudo_tick;
	unsigned int start_pseudo_tick;
} kernel;
