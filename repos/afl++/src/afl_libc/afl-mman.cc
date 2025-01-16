#include "sys/afl-mman.h"

int munmap(void *addr, size_t len) noexcept {
    return lx_munmap(addr, len);
}

void *mmap(void *addr, size_t len, int prot, int flags, int fd, __off_t offset) noexcept {
    return lx_mmap(addr, len, prot, flags, fd, offset);
}
