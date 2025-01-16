#include "sys/wait.h"

#include <base/log.h>

#define NOT_IMPLEMENTED Genode::log(__func__, " not implemented")

__pid_t waitpid (__pid_t pid, int *stat_loc, int options) {
    (void) pid;
    (void)stat_loc;
    (void)options;
    NOT_IMPLEMENTED;
    return 0;
}
