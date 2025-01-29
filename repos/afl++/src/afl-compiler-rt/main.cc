#include <libc/component.h>
#include <base/log.h>


/* Used for debugging. Upon call, the execution waits for an ENTER input. */
extern "C" void wait_for_continue(void);

void Libc::Component::construct(Libc::Env &env)
{
    Libc::with_libc([&] () {
        Genode::log("Compiler-rt libc entry point called."); (void)env;
    });
}