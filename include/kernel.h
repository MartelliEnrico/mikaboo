#pragma once

#include "listx.h"
#include "mikabooq.h"

extern struct list_head ready_list;
extern struct list_head wait_list;
extern unsigned int thread_count;
extern unsigned int soft_block_count;
extern struct tcb_t *current_thread;
extern unsigned int current_thread_tod;
extern unsigned int pseudo_tick;
extern unsigned int start_pseudo_tick;
