#include <libc/component.h>
#include <base/log.h>


/* Used for debugging. Upon call, the execution waits for an ENTER input. */
extern "C" void wait_for_continue(void);

void Libc::Component::construct(Libc::Env &env)
{
    env.exec_static_constructors();
    Genode::log("Pls work");
    Libc::with_libc([&] () {
        Genode::log("Shm_client.");
    });
}