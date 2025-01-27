/* afl_libc includes */
#include "stdlib.h"
#include "string.h"
#include "init.h"

/* Genode includes */
#include <base/allocator.h>
#include <util/string.h>
#include <base/exception.h>

struct Allocator_not_Initialized : Genode::Exception { };
static Genode::Allocator *alloc_ptr;
static Genode::Allocator &alloc()
{
    if (alloc_ptr)
        return *alloc_ptr;

    Genode::error("The allocator has not been initialized. Call `afl_libc_init()`");
    throw Allocator_not_Initialized();
}


void afl_libc_init(Genode::Allocator &alloc) { alloc_ptr = &alloc; }


void *malloc(size_t size)
{
    /*
     * We store the size of the allocation at the very
     * beginning of the allocated block and return
     * the subsequent address. This way, we can retrieve
     * the size information when freeing the block.
     */
    unsigned long const real_size = size + sizeof(unsigned long);

    return alloc().try_alloc(real_size).convert<void *>(

            [&] (void *ptr) {
                *(unsigned long *)ptr = real_size;
                return (unsigned long *)ptr + 1; },

            [&] (Genode::Allocator::Alloc_error) {
                return nullptr; });
}


void *calloc(size_t nmemb, size_t size)
{
    void *addr = malloc(nmemb*size);
    memset(addr, 0, nmemb*size);
    return addr;
}


void free(void *ptr)
{
    unsigned long *addr = ((unsigned long *)ptr) - 1;
    alloc().free(addr, *addr);
}


void *realloc(void *ptr, size_t size) {
    void * new_block = malloc(size);
    return memcpy(new_block, ptr, size);
}