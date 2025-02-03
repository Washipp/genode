/* Genode includes */
#include <base/log.h>

/* libc include */
#include <sys/resource.h>

#define NOT_IMPLEMENTED Genode::log("'",__func__, "()' not implemented")


int setrlimit(int resource, const struct rlimit *rlimits)
{
    (void) resource;
    (void) rlimits;
    NOT_IMPLEMENTED;
    // Maybe use the built-in linux syscalls: add syscall-linux to the LIBS in target.mk
//    return (int)lx_syscall(SYS_setrlimit, resource, &rlimits);
    return -1;
}