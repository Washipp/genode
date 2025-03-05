#ifndef AFL_SHM_SESSION_H
#define AFL_SHM_SESSION_H

#include <session/session.h>
#include <base/rpc.h>
#include <base/attached_ram_dataspace.h>

using namespace Genode;

struct Shm_session : Genode::Session
{
    static const char *service_name() { return "Shm_session"; }
    enum { CAP_QUOTA = 4 };

    virtual int shm_alloc_new_dataspace(int key, size_t size) = 0;
    virtual Ram_dataspace_capability shm_get_dataspace(int shmid) = 0;
    virtual int shm_detach_dataspace(addr_t shmaddr) = 0;

    /*******************
     ** RPC interface **
     *******************/

    GENODE_RPC(Rpc_alloc_new_dataspace, int, shm_alloc_new_dataspace, int, size_t);
    GENODE_RPC(Rpc_shm_get_dataspace, Ram_dataspace_capability, shm_get_dataspace, int);
    GENODE_RPC(Rpc_detach_dataspace, int, shm_detach_dataspace, addr_t);

    GENODE_RPC_INTERFACE(Rpc_alloc_new_dataspace, Rpc_shm_get_dataspace, Rpc_detach_dataspace);
};

#endif //AFL_SHM_SESSION_H
