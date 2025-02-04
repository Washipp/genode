#include "init.h"
#include "sys/shm.h"

/* Genode includes */
#include <libc/component.h>
#include <base/log.h>


/* Used for debugging. Upon call, the execution waits for an ENTER input. */
extern "C" void wait_for_continue(void);

void Libc::Component::construct(Libc::Env &env)
{
    Libc::with_libc([&] () {
        Genode::log("Shm_client started. Setting up connection.");
        shm_init(env);
        Genode::log("Shm_client init started.");

        Genode::log("Trying to attach segment");
        auto addr = shmat(0, NULL, 0);

        Genode::log("Attached segment to: ", addr);
        Genode::memcpy(addr, "Test string from shm_server.", 29);
        Genode::log("Successfully written to ", addr);
    });
    Genode::log("afl-shm test completed.");
}