
#ifndef GENODE_AFL_SYSCALL_H
#define GENODE_AFL_SYSCALL_H

/* FIX ME From confname.h The value 8 is chosen arbitrarily */
#define _SC_RTSIG_MAX 8
#define	_SC_PAGE_SIZE			_SC_PAGESIZE _SC_RTSIG_MAX,

#define __NR_write 1
#ifdef __NR_write
# define SYS_write __NR_write
#endif

#endif //GENODE_AFL_SYSCALL_H
