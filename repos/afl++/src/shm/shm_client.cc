/* afl++ port includes */
#include "shm_session.h"
#include "sys/shm.h"

/* Genode includes */
#include <base/log.h>
#include <base/connection.h>

using namespace Genode;

class SHMAT_Alloc_Failed : public Exception {};

struct Shm_env {

    /*******************
     ** RPC interface **
     *******************/

    struct Shm_session_client : Connection<Shm_session>, Rpc_client<Shm_session> {
        Shm_session_client(Env &env) :

                /* create session */
                Connection<Shm_session>(env, Label(),
                                        Ram_quota { 8*1024 }, Args()),
                /* initialize RPC interface */
                Rpc_client<Shm_session>(cap())
        { }

        int shm_alloc_new_dataspace(int key, size_t size) override
        {
            return call<Rpc_alloc_new_dataspace>(key, size);
        }


        Ram_dataspace_capability shm_get_dataspace(int shmid) override
        {
            return call<Rpc_shm_get_dataspace>(shmid);
        }

        int shm_detach_dataspace(addr_t shmaddr) override
        {
            return call<Rpc_detach_dataspace>(shmaddr);
        }

    };

    Env &env;

    Shm_session_client shm_session_client;

    Shm_env(Env &env) : env(env), shm_session_client(env)
    { }
};

static Constructible<Shm_env> shm_env;


void shm_init(Env &env)
{
    if (!shm_env.constructed()) {
        shm_env.construct(env);
    }
}

/**
 * The client should not need to call shmget(). Can possibly be removed
 * */
int shmget(int key, size_t size, int shmflg)
{
    if (!shm_env.constructed()) {
        Genode::error("Call 'shm_init()' first.");
        return -1;
    }

    if (key != 0) {
        Genode::error("shmget called with key '", key, "' Only IPC_PRIVATE (key = 0) is supported.");
        return -1;
    }
    // This value comes from attached_ram_dataspace.h but is only available as an enum there.
    enum {
        PAGE_SIZE = 4096
    };

    // rounded up to a multiple of PAGE_SIZE
    size_t final_size = size;
    if ((size % PAGE_SIZE) != 0) {
        final_size += PAGE_SIZE - (size % PAGE_SIZE);
    }

    int shmid = shm_env->shm_session_client.shm_alloc_new_dataspace(key, final_size);

    // For now the flags are ignored. This way, the compiler is satisfied.
    (void) shmflg;

    return shmid;
}

/**
 * Steps to implement:
 *  1. RPC call to get the capability
 *  2. Attach capability to the region map.
 * */
void *shmat(int shmid, const void *shmaddr, int shmflg)
{
    if (!shm_env.constructed()) {
        Genode::error("Call 'shm_init()' first.");
        return (void *) -1;
    }

    Ram_dataspace_capability ds = shm_env->shm_session_client.shm_get_dataspace(shmid);

    if (!ds.valid()) {
        Genode::error("shmid '", shmid, "' invalid, no capability found.");
        return (void *) -1;
    }

    addr_t server_addr = 0;

    Region_map::Attr attr {
            .size       = 0,
            .offset     = 0,
            .use_at     = shmaddr != NULL,
            .at         = (unsigned long) (shmaddr),
            .executable = false,
            .writeable  = true,
    };

    shm_env->env.rm().attach(ds, attr).with_result(
        [&](Region_map::Range r) {
            server_addr = r.start;
        },
        [&](Region_map::Attach_error) {
            throw SHMAT_Alloc_Failed();
        }
    );


    // For now the flags are ignored. This way, the compiler is satisfied.
    (void) shmflg;
    return (void *) server_addr;
}

/**
 * The value of cmd is always IPC_RMID as used by AFL++.
 * This means, the memory is marked to be destroyed in order to prevent shm from leaking.
 * Thus, the function does nothing in the context of AFL++.
 * */
int shmctl(int shmid, int cmd, struct shmid_ds *buf)
{
    if (!shm_env.constructed()) {
        Genode::error("Call 'shm_init()' first.");
        return -1;
    }
    (void) shmid;
    (void) cmd;
    (void) buf;
    return 0;
}

/**
 * Does not check, if it failed. In this case, return -1.
 * */
int shmdt(const void *shmaddr)
{
    if (!shm_env.constructed()) {
        Genode::error("Call 'shm_init()' first.");
        return -1;
    }
    shm_env->env.rm().detach((addr_t) shmaddr);
    return 0;
}