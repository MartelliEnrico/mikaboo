/* subset of the Linux Kernel source file: "include/linux/syscalls.h"
CPLv2 */
#ifndef _SSICALLS_H
#define _SSICALLS_H

#define __MAP0(m,...)
#define __MAP1(m,t,a) m(t,a)
#define __MAP2(m,t,a,...) m(t,a), __MAP1(m,__VA_ARGS__)
#define __MAP3(m,t,a,...) m(t,a), __MAP2(m,__VA_ARGS__)
#define __MAP4(m,t,a,...) m(t,a), __MAP3(m,__VA_ARGS__)
#define __MAP(n,...) __MAP##n(__VA_ARGS__)

#define __MAPS0(m,...)
#define __MAPS1(m,t,a) m(t,a);
#define __MAPS2(m,t,a,...) m(t,a); __MAPS1(m,__VA_ARGS__)
#define __MAPS3(m,t,a,...) m(t,a); __MAPS2(m,__VA_ARGS__)
#define __MAPS4(m,t,a,...) m(t,a); __MAPS3(m,__VA_ARGS__)
#define __MAPS(n,...) __MAPS##n(__VA_ARGS__)

#define __SC_DECL(t, a)	t a
#define __SC_ARGS(t, a)	i->a

#define SSICALL_DEFINE0(sname)	void *ssi_##sname(void)

#define SSICALL_DEFINE1(name, ...) SSICALL_DEFINEx(1, _##name, __VA_ARGS__)
#define SSICALL_DEFINE2(name, ...) SSICALL_DEFINEx(2, _##name, __VA_ARGS__)
#define SSICALL_DEFINE3(name, ...) SSICALL_DEFINEx(3, _##name, __VA_ARGS__)
#define SSICALL_DEFINE4(name, ...) SSICALL_DEFINEx(4, _##name, __VA_ARGS__)

#define SSICALL_DEFINEx(x, name, ...)					\
	static inline void *SSIC##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
	void *ssi##name(void *params) {					\
		struct i_##name {					\
			void *rt; __MAPS(x, __SC_DECL, __VA_ARGS__)	\
		};							\
		struct i_##name *i = (struct i_##name *) params;	\
		return SSIC##name(__MAP(x,__SC_ARGS,__VA_ARGS__));	\
	}								\
	static inline void *SSIC##name(__MAP(x,__SC_DECL,__VA_ARGS__))

void ssi_handler();

void *ssi_get_errno(); // GET_ERRNO
void *ssi_create_process(void *); // CREATE_PROCESS
void *ssi_create_thread(void *); // CREATE_THREAD
void *ssi_terminate_process(); // TERMINATE_PROCESS
void *ssi_terminate_thread(); // TERMINATE_THREAD
void *ssi_setpgmmgr(void *); // SETPGMMGR
void *ssi_settlbmgr(void *); // SETTLBMGR
void *ssi_setsysmgr(void *); // SETSYSMGR
void *ssi_get_cputime(); // GET_CPUTIME
void *ssi_wait_for_clock(); // WAIT_FOR_CLOCK
void *ssi_do_io(void *); // DO_IO
void *ssi_get_processid(void *); // GET_PROCESSID
void *ssi_get_parentprocid(void *); // GET_PARENTPROCID
void *ssi_get_mythreadid(); // GET_MYTHREADID

#endif
