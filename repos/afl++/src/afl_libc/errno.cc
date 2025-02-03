/* Genode include */
#include <base/log.h>

/* libc include */
#include <errno.h>

int *__errno_location(void)
{
    Genode::log("errno_location called");
    return nullptr;
}