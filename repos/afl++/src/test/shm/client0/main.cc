#include "init.h"
#include "sys/shm.h"

/* Genode includes */
#include <base/component.h>
#include <base/log.h>

void Component::construct(Genode::Env &env)
{
    env.exec_static_constructors();
    Genode::log("Client00 started. Setting up connection.");
    shm_init(env);
    Genode::log("Client00 init setup.");


    Genode::log("Trying to attach segment");

    int shmid = shmget(0, 4096, 0);

    Genode::log("Got new shmid: ", shmid);

    auto addr = shmat(shmid, NULL, 0);

    Genode::log("Attached segment to: ", addr);

    Genode::memcpy(addr, "Hello from client 00.", 21);

    Genode::log("Successfully written to ", addr);
}