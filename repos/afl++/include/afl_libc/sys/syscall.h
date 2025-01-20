#ifndef GENODE_SYSCALL_H
#define GENODE_SYSCALL_H

/* FIX ME From confname.h The value 8 is chosen arbitrarily */
#ifndef _SC_RTSIG_MAX
# define _SC_RTSIG_MAX 8
#endif

#define	_SC_PAGESIZE		47

#ifndef _SC_PAGE_SIZE
# define _SC_PAGE_SIZE _SC_PAGESIZE
#endif

#define __NR_write 1
#ifdef __NR_write
# define SYS_write __NR_write
#endif

#endif //GENODE_SYSCALL_H
