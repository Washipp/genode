#ifndef GENODE_AFL_MMAN_H
#define GENODE_AFL_MMAN_H

#include "afl-stddef.h"

#define MAP_FAILED	((void *) -1) /* Return value of `mmap' in case of an error.  */
#define PROT_READ	0x1		/* Page can be read.  */
#define PROT_WRITE	0x2		/* Page can be written.  */

#define MAP_SHARED	0x01		/* Share changes.  */
#define MAP_PRIVATE	0x02		/* Changes are private.  */
#define MAP_ANONYMOUS	0x20		/* Don't use a file.  */
#define MAP_FIXED_NOREPLACE 0x100000	/* MAP_FIXED but do not unmap underlying mapping.  */


/* Deallocate any mapping for the region starting at ADDR and extending LEN
   bytes.  Returns 0 if successful, -1 for errors (and sets errno).  */
extern "C" int munmap (void *addr, size_t len);

extern "C" void *mmap(void *addr, size_t len, int prot, int flags, int fd, __off_t offset);

#endif //GENODE_AFL_MMAN_H
