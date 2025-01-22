#include <base/component.h>
#include <base/log.h>
#include <base/heap.h>

#include "afl_libc/stdlib.h"
#include "afl_libc/string.h"
#include "afl_libc/sys/null.h"

void Component::construct(Genode::Env &env)
{
    env.exec_static_constructors();

    Genode::Heap _heap { env.ram(), env.rm() };

    afl_libc_init(_heap);
    Genode::log("Allocator has been initialized.");
    int success = setenv("test-var", "test-value", 0);
    if (!success && strcmp(getenv("test-var"), "test-value") == 0) {
        Genode::log("Get-Setenv test successful.");
    } else {
        Genode::log("Get-Setenv test failed.");
    }

    unsetenv("test-var");

    if(getenv("test-var") == NULL) {
        Genode::log("Unsetenv-Getenv test successful.");
    }

    Genode::log("afl_libc test completed.");
}
