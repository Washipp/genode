/* afl++ includes */
#include "init.h"

/* Genode includes*/
#include <base/log.h>
#include <libc/component.h>

void Component::construct(Genode::Env &env)
{
    shm_init(env);
    Genode::log("Hello from the print component");
}

void Libc::Component::construct(Libc::Env &env)
{
    Libc::with_libc([&] () {
        shm_init(env);
        Genode::log("Hello from the print component");
    });
}