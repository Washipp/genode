#include "fcntl.h"

#include <base/log.h>
#include <base/sleep.h>

#define NOT_IMPLEMENTED Genode::log(__func__, " not implemented")

int open (const char *file, int oflag, ...) {
    (void )file;
    (void )oflag;
    NOT_IMPLEMENTED;
    return 0;
}

/* Do the file control operation described by CMD on FD. The remaining arguments are interpreted depending on CMD.*/
int fcntl (int fd, int cmd, ...) {
    (void ) fd;
    (void ) cmd;
    NOT_IMPLEMENTED;
    return 0;
}