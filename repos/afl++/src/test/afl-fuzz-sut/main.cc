#include <libc/component.h>
#include <base/log.h>


/* Used for debugging. Upon call, the execution waits for an ENTER input. */
extern "C" void wait_for_continue(void);

#include <base/component.h>


void Component::construct(Genode::Env &env)
{
    (void) env;
    Genode::log("hello test completed");
}
