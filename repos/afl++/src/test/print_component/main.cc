/* afl++ includes */
#include "init.h"

/* Genode includes*/
#include <base/log.h>
#include <base/component.h>

void Component::construct(Genode::Env &env)
{
    shm_init(env);
    Genode::log("Hello from the print component");
}