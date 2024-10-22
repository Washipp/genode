/* Genode includes */
#include <base/attached_ram_dataspace.h>
#include <base/component.h>
#include <base/heap.h>
#include <block/request_stream.h>
#include <os/session_policy.h>
#include <util/string.h>

// TODO: Create run script --> adapt "persistent_bash.run" to include this component.

using namespace Genode;

struct Main : Rpc_object<Typed_root<Block::Session>>
{
    Env &_env;

    // TODO: Define block_session that we will write to

    /*
     * Root interface
     */
    Capability<Session> session(Root::Session_args const &args,
                                Affinity const &) override {}

    void upgrade(Capability<Session>, Root::Upgrade_args const &) override {}

    void close(Capability<Session> cap) override {}

    Main(Env &env) : _env(env) {
        /* Announced "Block::Session" to the parent.
         * This way this component can provide "Block"
         */
        _env.parent().announce(_env.ep().manage(*this));

        // TODO: load key from config file
    }
};


void Component::construct(Genode::Env &env) { static Main main(env); }
