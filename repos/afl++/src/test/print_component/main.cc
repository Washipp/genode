/* afl++ includes */
#include "init.h"

/* Genode includes*/
#include <base/log.h>
#include <libc/component.h>


void Libc::Component::construct(Libc::Env &env)
{
    Libc::with_libc([&] () {
        Genode::log("Hello from the print component. Trying to setup shared memory:");
        shm_init(env);
        env.parent().exit(0);
    });
}