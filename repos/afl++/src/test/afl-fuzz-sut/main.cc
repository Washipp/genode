#include <libc/component.h>
#include <base/log.h>

void Libc::Component::construct(Libc::Env &env)
{
    (void) env;
    Libc::with_libc([&] () {
        Genode::log("The two errors 'Error: no plugin found for fcntl(199)/write(199)' are expected.");
        Genode::log("They require ./afl-fuzz to run be running.");
        Genode::log("Instrumentation test succeeded.");
    });
}