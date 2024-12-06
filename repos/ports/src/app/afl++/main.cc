#include <base/component.h>
#include <base/log.h>
#include <hello_session/connection.h>


void Component::construct(Genode::Env &env)
{
    env.parent().heartbeat_response();
    Genode::log("Test completed.");
}
