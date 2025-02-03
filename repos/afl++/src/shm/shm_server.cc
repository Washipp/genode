/* afl++ includes */
#include "sys/shm.h"
#include "shm_session.h"

/* Genode includes */
#include <base/log.h>
#include <base/heap.h>
#include <base/component.h>
#include <base/attached_ram_dataspace.h>
#include <util/dictionary.h>

/* libc includes */
#include <sys/ipc.h>

// This value comes from attached_ram_dataspace.h, but somehow it is not linked correctly...
#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

#define NOT_IMPLEMENTED Genode::log("'", __func__, "()' not implemented")

/**
 * RAM data space          --> server
 * Attached RAM data space --> client
 */

using namespace Genode;

int local_name_to_shmid(long local_name);

class SHMAT_Alloc_Failed : public Exception {};

struct Shm_env {


    struct Dict_elem : Dictionary<Dict_elem, int>::Element {

        Ram_dataspace_capability _ds;

        Dict_elem(Dictionary<Dict_elem, int> &dict, int shmid,
                  Ram_dataspace_capability ds) :
                Dictionary<Dict_elem, int>::Element(dict, shmid),
                _ds(ds)
        { }
    };

    /*******************
     ** RPC interface **
     *******************/

    struct Shm_Session_component : Genode::Rpc_object<Shm_Session> {

        Dictionary<Dict_elem, int> &_dict;

        Shm_Session_component(Dictionary<Dict_elem, int> &dict) : _dict(dict)
        { }

        Ram_dataspace_capability shm_get_dataspace(int shmid) override
        {
            return _dict.with_element(shmid,
                                      [&](Dict_elem &elem) -> Ram_dataspace_capability { return elem._ds; },
                                      [&]() -> Ram_dataspace_capability { return {}; });
        }
    };

    Env &env;

    Heap _heap { env.ram(), env.rm() };

    /* Maps the Shared Memory Identifier to the dataspace capability. */
    Dictionary<Dict_elem, int> dict {};

    Shm_Session_component shm_session_component;

    Shm_env(Env &env) : env(env), shm_session_component(dict)
    { }

    ~Shm_env()
    {
        Genode::log("Unloading dict elements");
        // TODO: destroy does not seem to work. We ignore the memory problems for now, just some dangling pointers

//        dict.for_each([&](Dict_elem const &elem)  {
//            Genode::log("Element found");
//            elem.~Dict_elem();
//            Genode::destroy(_heap, &elem._ds);
//            (void) elem;
//        });
    }

    void add_elem(int shmid, Ram_dataspace_capability ds)
    {
        new (_heap) Dict_elem { dict, shmid, ds };
    }

    Ram_dataspace_capability get_elem(int shmid)
    {
        return dict.with_element(shmid,
                                 [&](Dict_elem &elem) -> Ram_dataspace_capability { return elem._ds; },
                                 [&]() -> Ram_dataspace_capability { return {}; });
    }


};

static Constructible<Shm_env> _shm_env;


void shm_init(Env &env)
{
    _shm_env.construct(env);

    // Announce the RPC session component, such that the client (SUT) can request the data space capabilities.
    env.ep().manage(_shm_env->shm_session_component);
}

/**
 * Steps to implement:
 * 1. afl-fuzz uses core's PD session to allocate new RAM data space
 * 2. The local_name() of the data space is then used as the 'shared memory identifier' (shmid)
 * 3. If the shmid is already in use, return the existing one, provided the access is allowed.
 *    (this is possibly already covered within Genode.)
 *
 *
 * Once the new RAM dataspace is allocated,
 * there needs to be a separate mechanism that maps from shmid to the dataspace capability.
 * */

/**
 * We always expect key to be IPC_PRIVATE, AFL++ only uses it in this way.
 * This also means, we can ignore the key and map shmid to the capability directly.
 * */
int shmget(int key, size_t size, int shmflg)
{
    if (!_shm_env.constructed()) {
        Genode::error("Call 'shm_init()' first.");
        return -1;
    }

    if (key != IPC_PRIVATE) {
        Genode::error("shmget called with key '", key, "' Only IPC_PRIVATE is supported.");
        return -1;
    }
    if (_shm_env->dict.exists(key)) {
        // As we only support key == IPC_PRIVATE, this should never be executed.
        // Not sure about the implications yet.
        Genode::warning("Shared memory segment with key '", key, "' already exists.");
    }

    // rounded up to a multiple of PAGE_SIZE
    size_t final_size = size;
    if ((size % PAGE_SIZE) != 0) {
        final_size += PAGE_SIZE - (size % PAGE_SIZE);
    }

    auto ds = _shm_env->env.pd().alloc(final_size);
    auto shmid = local_name_to_shmid(ds.local_name());
    _shm_env->add_elem(shmid, ds);

    // For now the flags are ignored. This way, the compiler is satisfied.
    (void) shmflg;

    return shmid;
}

int local_name_to_shmid(long local_name)
{
    int shmid = static_cast<int>(local_name);
    if (shmid != static_cast<long>(shmid)) {
        Genode::warning("Converting local name to int resulted in precision loss");
    }
    return shmid;
}

/**
 * Steps to implement:
 * 1. Retrieve the capability based on the shmid-dataspace mapping.
 * 2. Increase shm_nattch in shmaddr.
 * 3. Make the dataspace visible in its own address space using an Attached RAM data space.
 * 4. Return client-local-address or server-local-address based on the callees.
 * */

/**
 * shmflg is always considered to be 0 as AFL++ does not use another value.
 * */
void *shmat(int shmid, const void *shmaddr, int shmflg)
{
    if (!_shm_env.constructed()) {
        Genode::error("Call 'shm_init()' first.");
        return (void *) -1;
    }

    auto ds = _shm_env->get_elem(shmid);

    if (!ds.valid()) {
        Genode::error("shmid '", shmid, "' invalid, not capability found.");
        return (void *) -1;
    }

    addr_t server_addr = 0;

    if (shmaddr == NULL) {
        _shm_env->env.rm().attach(ds,  Region_map::Attr {
                .size       = 0,
                .offset     = 0,
                .use_at     = false,
                .at         = {},
                .executable = false,
                .writeable  = true,
        }).with_result(
                [&](Region_map::Range r) {
                    server_addr = r.start;
                },
                [&](Region_map::Attach_error) {
                    throw SHMAT_Alloc_Failed();
                }
        );
    } else {
        Genode::warning("shmaddr is not NULL. This case should not really work.");
        _shm_env->env.rm().attach(ds, Region_map::Attr {
                .size       = 0,
                .offset     = 0,
                .use_at     = true,
                .at         = (unsigned long) (shmaddr),
                .executable = false,
                .writeable  = true,
        }).with_result(
                [&](Region_map::Range r) {
                    server_addr = r.start;
                },
                [&](Region_map::Attach_error) {
                    throw SHMAT_Alloc_Failed();
                }
        );
    }

    // For now the flags are ignored. This way, the compiler is satisfied.
    (void) shmflg;
    return (void *) server_addr;
}

/**
 * Steps to implement:
 * 1. ??? look for an implementation and copy it.
 * */
int shmctl(int shmid, int cmd, struct shmid_ds *buf)
{
    (void) shmid;
    (void) cmd;
    (void) buf;
    NOT_IMPLEMENTED;
    return -1;
}

/**
 * Does not check, if it failed. In this case, return -1.
 * */
int shmdt(const void *shmaddr)
{
    _shm_env->env.rm().detach((addr_t) (shmaddr));
    return 0;
}