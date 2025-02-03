#include "init.h"
#include "sys/shm.h"

/* Genode includes*/
#include <libc/component.h>
#include <base/log.h>

/* libc includes*/


/* Used for debugging. Upon call, the execution waits for an ENTER input. */
extern "C" void wait_for_continue(void);

void Libc::Component::construct(Libc::Env &env)
{

    Libc::with_libc([&] () {
        Genode::log("Starting shm-server...");
        shm_init(env);

        int shmid = shmget(0, 4096, 0);

        Genode::log("Got new shmid: ", shmid);

        auto addr = shmat(shmid, NULL, 0);

        Genode::log("Attached segment to: ", addr);

        Genode::memcpy(addr, "Test string from shm_server.", 29);
        Genode::log("Successfully written to", addr);
//        wait_for_continue();
    });


    Genode::log("afl-shm test completed.");
}