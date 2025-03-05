#include "init.h"
#include "sys/shm.h"

/* Genode includes */
#include <libc/component.h>
#include <base/log.h>
#include <timer_session/connection.h>


void Libc::Component::construct(Libc::Env &env)
{
    Timer::Connection _timer { env };
    // Give the other client time to setup and write.
    _timer.msleep(500);
    Libc::with_libc([&] () {
        Genode::log("Client01 started. Setting up connection.");
        shm_init(env);
        Genode::log("Client01 init started.");

        Genode::log("Trying to attach segment");

        auto addr = shmat(-1, NULL, 0);

        if (addr == (void *) -1) {
            Genode::error("Shmat failed");
            env.parent().exit(1);
        }

        char result[21];

        Genode::log("Attached segment to: ", addr);
        Genode::memcpy(result, addr, 21);
        Genode::log("Read the value: '", Genode::String<32>(result), "'");

        shmdt(addr);
    });
    Genode::log("afl-shm test completed.");
}