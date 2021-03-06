#include <uARMtypes.h>

#include "mikabooq.h"
#include "ssicalls.h"

void ssi_handler() {
	
}

SSICALL_DEFINE0(get_errno) {
	return 0;
}

SSICALL_DEFINE1(create_process, state_t *, s) {
	(void)s;
	return 0;
}

SSICALL_DEFINE1(create_thread, state_t *, s) {
	(void)s;
	return 0;
}

SSICALL_DEFINE0(terminate_process) {
	return 0;
}

SSICALL_DEFINE0(terminate_thread) {
	return 0;
}

SSICALL_DEFINE1(setpgmmgr, struct tcb_t *, s) {
	(void)s;
	return 0;
}

SSICALL_DEFINE1(settlbmgr, struct tcb_t *, s) {
	(void)s;
	return 0;
}

SSICALL_DEFINE1(setsysmgr, struct tcb_t *, s) {
	(void)s;
	return 0;
}

SSICALL_DEFINE0(get_cputime) {
	return 0;
}

SSICALL_DEFINE0(wait_for_clock) {
	return 0;
}

SSICALL_DEFINE4(do_io, uintptr_t, device, uintptr_t, command, uintptr_t, data1, uintptr_t, data2) {
	(void)device, (void)command, (void)data1, (void)data2;
	return 0;
}

SSICALL_DEFINE1(get_processid, struct tcb_t *, s) {
	(void)s;
	return 0;
}

SSICALL_DEFINE1(get_parentprocid, struct pcb_t *, s) {
	(void)s;
	return 0;
}

SSICALL_DEFINE0(get_mythreadid) {
	return 0;
}
