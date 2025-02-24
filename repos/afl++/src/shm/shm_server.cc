/* afl++ includes */
#include "sys/shm.h"
#include "shm_session.h"

/* Genode includes */
#include <base/log.h>
#include <base/heap.h>
#include <base/component.h>
#include <base/attached_ram_dataspace.h>
#include <util/dictionary.h>
#include <root/component.h>

/* libc includes */
#include <sys/ipc.h>

#define NOT_IMPLEMENTED Genode::log("'", __func__, "()' not implemented")

using namespace Genode;

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

    struct Shm_session_component : Genode::Rpc_object<Shm_session> {

        Dictionary<Dict_elem, int> &_dict;

        Shm_session_component(Dictionary<Dict_elem, int> &dict) : _dict(dict)
        { }

        Ram_dataspace_capability shm_get_dataspace(int shmid) override
        {
            // TODO: Should be removed at some point...
            // This case is used to test the functionality, if the shmid is not available.
            if (shmid == -1) {
                Ram_dataspace_capability a;
                _dict.with_any_element([&](Dict_elem &elem) {
                    a = elem._ds;
                });
                return a;
            }

            return _dict.with_element(shmid,
                                      [&](Dict_elem &elem) -> Ram_dataspace_capability { return elem._ds; },
                                      [&]() -> Ram_dataspace_capability { return {}; });
        }
    };

    class Shm_session_root_component : public Genode::Root_component<Shm_session_component> {

        Dictionary<Dict_elem, int> &_dict;
    protected:
        Shm_session_component *_create_session(const char *) override
        {
            return new(md_alloc()) Shm_session_component(_dict);
        }

    public:

        Shm_session_root_component(Genode::Entrypoint &ep,
                                   Genode::Allocator &alloc,
                                   Dictionary<Dict_elem, int> &dict) :
                Genode::Root_component<Shm_session_component>(ep, alloc), _dict(dict)
        { }
    };

    Env &env;

    Heap _heap { env.ram(), env.rm() };

    /* Maps the Shared Memory Identifier to the dataspace capability. */
    Dictionary<Dict_elem, int> dict {};

    Shm_session_root_component root { env.ep(), _heap, dict};

    Shm_env(Env &env) : env(env)
    {
        env.parent().announce(env.ep().manage(root));
    }

    ~Shm_env()
    {
        while (dict.with_any_element([&](Dict_elem &elem)  {
            destroy(_heap, &elem);
        }));
    }

    void add_elem(int shmid, Ram_dataspace_capability ds)
    {
        new(_heap) Dict_elem { dict, shmid, ds };
    }

    Ram_dataspace_capability get_elem(int shmid)
    {
        return dict.with_element(shmid,
                                 [&](Dict_elem &elem) -> Ram_dataspace_capability { return elem._ds; },
                                 [&]() -> Ram_dataspace_capability { return {}; });
    }

    int local_name_to_shmid(long local_name)
    {
        int shmid = static_cast<int>(local_name);
        if (shmid != static_cast<long>(shmid)) {
            Genode::warning("Converting local name to int resulted in precision loss");
        }
        return shmid;
    }
};

static Constructible<Shm_env> _shm_env;


void shm_init(Env &env)
{
    _shm_env.construct(env);
}

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
    // This value comes from attached_ram_dataspace.h but is only available as an enum there.
    enum { PAGE_SIZE = 4096 };

    // rounded up to a multiple of PAGE_SIZE
    size_t final_size = size;
    if ((size % PAGE_SIZE) != 0) {
        final_size += PAGE_SIZE - (size % PAGE_SIZE);
    }

    auto ds = _shm_env->env.pd().alloc(final_size);
    auto shmid = _shm_env->local_name_to_shmid(ds.local_name());
    _shm_env->add_elem(shmid, ds);

    // For now the flags are ignored. This way, the compiler is satisfied.
    (void) shmflg;

    return shmid;
}

/**
 * shmflg is always considered to be 0 as AFL++ does not use another value.
 * */
void *shmat(int shmid, const void *shmaddr, int shmflg)
{
    if (!_shm_env.constructed()) {
        Genode::error("Call 'shm_init()' first.");
        return (void *) -1;
    }

    Ram_dataspace_capability ds = _shm_env->get_elem(shmid);

    if (!ds.valid()) {
        Genode::error("shmid '", shmid, "' invalid, not capability found.");
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