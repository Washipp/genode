#include <libc/component.h>
#include <base/log.h>


/* Used for debugging. Upon call, the execution waits for an ENTER input. */
extern "C" void wait_for_continue(void);

void Libc::Component::construct(Libc::Env &env)
{
    (void) env;
    Libc::with_libc([&] () {
        Genode::log("The two errors 'Error: no plugin found for fcntl(199)/write(199)' are expected.");
        Genode::log("They require ./afl-fuzz to run.");
        Genode::log("Instrumentation test succeeded.");
    });
}