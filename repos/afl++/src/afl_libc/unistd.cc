/* Genode includes */
#include <base/log.h>

/* libc includes */
#include <unistd.h>

#define NOT_IMPLEMENTED Genode::log("'",__func__, "()' not implemented")

/* This function is used exclusively in afl-fuzz when the fuzzing is stopped.
 * The function can be removed, provided that the components are stopped appropriately. */
int getpgid(int pid)
{
    (void) pid;
    NOT_IMPLEMENTED;
    return -1;
}
