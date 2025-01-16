#include "sys/mman.h"

int munmap(void *addr, size_t len) noexcept {
    (void)addr;
    (void)len;
    return 0;
//return lx_munmap(addr, len);
}

void *mmap(void *addr, size_t len, int prot, int flags, int fd, __off_t offset) noexcept {
    (void) addr;
    (void)len;
    (void)prot;
    (void)flags;
    (void)fd;
    (void)offset;
    return nullptr;
    //return lx_mmap(addr, len, prot, flags, fd, offset);
}
