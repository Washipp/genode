/* afl++ includes */
#include "shm_session.h"
#include "sys/shm.h"

/* Genode includes */
#include <base/log.h>
#include <base/heap.h>
#include <base/component.h>
#include <base/connection.h>
#include <base/attached_ram_dataspace.h>
#include <util/dictionary.h>


#define NOT_IMPLEMENTED Genode::log(__func__, " not implemented")

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

        Ram_dataspace_capability shm_get_dataspace(int shmid) override
        {
            return call<Rpc_shm_get_dataspace>(shmid);
        }

    };

    Env &env;

    Shm_session_client shm_session_client;

    Shm_env(Env &env) : env(env), shm_session_client(env)
    { }
};

static Constructible<Shm_env> _shm_env;


void shm_init(Env &env)
{
    _shm_env.construct(env);
}

/**
 * The client should not need to call shmget(). Can possibly be removed
 * */
int shmget(int key, size_t size, int shmflg)
{
    (void) key;
    (void) size;
    (void) shmflg;
    NOT_IMPLEMENTED;
    return -1;
}

/**
 * Steps to implement:
 *  1. RPC call to get the capability
 *  2. Attach capability to the region map.
 * */
void *shmat(int shmid, const void *shmaddr, int shmflg)
{
    if (!_shm_env.constructed()) {
        Genode::error("Call 'shm_init()' first.");
        return (void *) -1;
    }

    Ram_dataspace_capability ds = _shm_env->shm_session_client.shm_get_dataspace(shmid);

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

    _shm_env->env.rm().attach(ds, attr).with_result(
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
    if (!_shm_env.constructed()) {
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
    if (!_shm_env.constructed()) {
        Genode::error("Call 'shm_init()' first.");
        return -1;
    }
    _shm_env->env.rm().detach((addr_t) (shmaddr));
    return 0;
}