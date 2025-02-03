#ifndef AFL_SHM_SESSION_H
#define AFL_SHM_SESSION_H

#include <session/session.h>
#include <base/rpc.h>
#include <base/attached_ram_dataspace.h>

using namespace Genode;

struct Shm_Session : Genode::Session
{
    static const char *service_name() { return "Shm_Session"; }
    enum { CAP_QUOTA = 4 };

    virtual Ram_dataspace_capability shm_get_dataspace(int shmid) = 0;

    /*******************
     ** RPC interface **
     *******************/

    GENODE_RPC(Rpc_shm_get_dataspace, Ram_dataspace_capability, shm_get_dataspace, int);

    GENODE_RPC_INTERFACE(Rpc_shm_get_dataspace);
};

#endif //AFL_SHM_SESSION_H
