/* afl++ port includes */
#include "shm_session.h"

#include <base/component.h>
#include <base/log.h>
#include <base/heap.h>
#include <base/attached_ram_dataspace.h>
#include <util/dictionary.h>
#include <root/component.h>

namespace Shm {
    class Main;
    class Shm_session_root_component;

    struct Dict_elem;
    struct Shm_session_component;
}


using namespace Genode;

struct Shm::Dict_elem : Dictionary<Dict_elem, int>::Element {

    Ram_dataspace_capability _ds;

    Dict_elem(Dictionary<Dict_elem, int> &dict, int shmid,
              Ram_dataspace_capability ds) :
            Dictionary<Dict_elem, int>::Element(dict, shmid),
            _ds(ds)
    { }
};

struct Shm::Shm_session_component : Rpc_object<Shm_session> {

    Env &_env;
    Heap &_heap;
    Dictionary<Dict_elem, int> &_dict;

    static int local_name_to_shmid(long local_name)
    {
        int shmid = static_cast<int>(local_name);
        if (shmid != static_cast<long>(shmid)) {
            Genode::warning("Converting local name to int resulted in precision loss");
        }
        return shmid;
    }

    Shm_session_component(Env &env,
                          Heap &alloc,
                          Dictionary<Dict_elem, int> &dict) : _env(env), _heap(alloc), _dict(dict)
    { }

    /*******************
     ** RPC interface **
     *******************/


    int shm_alloc_new_dataspace(int key, size_t size) override
    {
        try {
            if (_dict.exists(key)) {
                // As we only support key == IPC_PRIVATE, this should never be executed.
                // Not sure about the implications yet.
                Genode::warning("Shared memory segment with key '", key, "' already exists.");
            }
            auto ds = _env.pd().alloc(size);
            auto shmid = local_name_to_shmid(ds.local_name());
            new (_heap) Dict_elem { _dict, shmid, ds };
            return shmid;
        }  catch (Ram_allocator::Alloc_error error) {
            switch (error) {
                case Ram_allocator::Alloc_error::OUT_OF_RAM:
                    Genode::error("Out of RAM while trying to allocate new dataspace.");
                    break;
                case Ram_allocator::Alloc_error::OUT_OF_CAPS:
                    Genode::error("Out of CAPS while trying to allocate new dataspace.");
                    break;
                case Ram_allocator::Alloc_error::DENIED:
                    Genode::error("Allocation of a new dataspace was denied.");
                    break;
            }
            return -1;
        }

    }

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


    int shm_detach_dataspace(addr_t shmaddr) override
    {
        _env.rm().detach((addr_t) shmaddr);
        return 0;
    }
};

class Shm::Shm_session_root_component : public Root_component<Shm_session_component> {
    Env &_env;
    Heap &_heap;
    Dictionary<Dict_elem, int> &_dict;
protected:
    Shm_session_component *_create_session(const char *) override
    {
        return new(md_alloc()) Shm_session_component(_env, _heap, _dict);
    }

public:

    Shm_session_root_component(Env &env,
                               Heap &heap,
                               Dictionary<Dict_elem, int> &dict) :
            Genode::Root_component<Shm_session_component>(env.ep(), heap),
            _env(env), _heap(heap), _dict(dict)
    { }
};

class Shm::Main {


    Genode::Env &_env;

    Heap _heap { _env.ram(), _env.rm() };

    /* Maps the Shared Memory Identifier to the dataspace capability. */
    Dictionary<Dict_elem, int> _dict {};

    Shm_session_root_component root { _env, _heap, _dict };

public:
    Main(Genode::Env &env) : _env(env)
    {
        env.parent().announce(env.ep().manage(root));
    }
};

void Component::construct(Genode::Env &env)
{
    static Shm::Main main(env);
}
