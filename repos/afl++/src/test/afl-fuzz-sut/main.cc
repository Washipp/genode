#include <libc/component.h>
#include <base/log.h>

void Libc::Component::construct(Libc::Env &env)
{
    (void) env;
    Libc::with_libc([&] () {
        Genode::log("Instrumentation test succeeded.");
    });
}