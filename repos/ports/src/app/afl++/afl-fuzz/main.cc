#include "base/component.h"
#include "base/log.h"


void Component::construct(Genode::Env &env)
{
    env.exec_static_constructors();
    Genode::log("Test completed.");
}
