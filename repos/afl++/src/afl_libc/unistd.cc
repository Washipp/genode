/* Afl-libc includes */
#include "unistd.h"
#include "stddef.h"

/* Genode includes */
#include <base/log.h>
#include <base/sleep.h>

#define NOT_IMPLEMENTED Genode::log("'",__func__, "()' not implemented")

int getpgid(int pid)
{
    (void) pid;
    NOT_IMPLEMENTED;
    return 0;
}

