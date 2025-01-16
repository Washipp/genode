#include "errno.h"
#include <base/log.h>

int *__errno_location(void) {
    Genode::log("errno_location called");
    return nullptr;
}