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

/* Link is used once, and is optional. So we default to returning -1, error.
 * AFL++ should be able to handle it accordingly. */
int link(const char *from, const char *to)
{
    (void) from;
    (void) to;
    return -1;
}

/* Create a new session with the calling process as its leader.
   The process group IDs of the session and the calling process
   are set to the process ID of the calling process, which is returned.  */
pid_t setsid(void)
{
    /* Note, even though it is not implemented, it does not seem to be a problem.
     * The usage within AFL++ was patched out and the functionality was then tested.
     * But this is no guarantee, that it is safe to remove. */
    Genode::log("'",__func__, "()' not implemented. Which is okay for now. Expect more clutter on the terminal.");
    return -1;
}