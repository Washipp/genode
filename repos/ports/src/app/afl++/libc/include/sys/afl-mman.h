#ifndef GENODE_AFL_MMAN_H
#define GENODE_AFL_MMAN_H

#include "afl-stddef.h"

/* Deallocate any mapping for the region starting at ADDR and extending LEN
   bytes.  Returns 0 if successful, -1 for errors (and sets errno).  */
extern int munmap (void *__addr, size_t __len);
extern void *mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, __off_t __offset);

#define MAP_FAILED	((void *) -1) /* Return value of `mmap' in case of an error.  */
#define PROT_READ	0x1		/* Page can be read.  */

#define MAP_PRIVATE	0x02		/* Changes are private.  */

#endif //GENODE_AFL_MMAN_H
