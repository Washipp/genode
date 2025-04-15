#include <base/component.h>
#include <base/log.h>

void Component::construct(Genode::Env &env)
{
    (void) env;
    Genode::log("Instrumentation test succeeded.");
}