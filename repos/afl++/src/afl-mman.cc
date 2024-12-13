#include "sys/afl-mman.h"
#include <base/log.h>

#define NOT_IMPLEMENTED Genode::log(__func__, " not implemented")

int munmap (void *addr, size_t len) {
    (void) addr;
    (void) len;
    NOT_IMPLEMENTED;
    return 0;
}

void *mmap(void *addr, size_t len, int prot, int flags, int fd, __off_t offset) {
    (void)addr;
    (void)len;
    (void)prot;
    (void)flags;
    (void)fd;
    (void)offset;
    NOT_IMPLEMENTED;
    return nullptr;
}