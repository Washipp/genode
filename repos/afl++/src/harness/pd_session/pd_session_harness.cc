/* afl++ port includes */
#include "compiler_rt.h"

/* Genode includes*/
#include <base/component.h>
#include <base/stdint.h>
#include <timer_session/connection.h>
#include <pd_session/connection.h>
#include <pd_session/pd_session.h>
#include <pd_session/client.h>
#include <base/attached_ram_dataspace.h>

using namespace Genode;

void Component::construct(Env &env)
{
    compiler_rt_init(env);

    int exit_code = 0;

    /*  First set up the PD session client */
    Pd_session_client psc = Pd_session_client(env.pd_session_cap());

    /* PD signals RPC calls */
    Capability<Signal_source> signal_source;
    psc.signal_source().with_result(
        [&](Capability<Signal_source> ss) {
            signal_source = ss;
            return;
        },
        [&](Pd_session::Signal_source_error error) {
            switch (error) {
                case Pd_session::Signal_source_error::OUT_OF_RAM:
                case Pd_session::Signal_source_error::OUT_OF_CAPS:
                    exit_code = 0; return;
                default:
                    exit_code = 1; return;
            }
        }
    );
    if (!signal_source.valid())
        env.parent().exit(exit_code);

    addr_t result = 0;
    if (*get_fuzz_len() < sizeof(addr_t))
        env.parent().exit(0);
    memcpy(&result, get_fuzz_ptr(), sizeof(addr_t));
    Pd_session::Imprint const imprint { result };
    psc.alloc_context(signal_source, imprint).with_result(
            [&](Capability<Signal_context> sc) {
                Genode::log(sc.local_name());
                return;
            },
            [&](Pd_session::Alloc_context_error error) {
                switch (error) {
                    case Pd_session::Alloc_context_error::OUT_OF_RAM:
                    case Pd_session::Alloc_context_error::OUT_OF_CAPS:
                    case Pd_session::Alloc_context_error::INVALID_SIGNAL_SOURCE:
                        exit_code = 0; return;
                    default:
                        exit_code = 1; return;
                }
            }
    );
    psc.free_signal_source(signal_source);
//    psc.free_signal_source();
//    psc.alloc_context();
//    psc.free_context();
//    psc.submit();

    /* RPC Framework */
//    psc.alloc_rpc_cap();
//    psc.free_rpc_cap();

    /* Virtual address management */
//    psc.address_space();
//    psc.stack_area();
//    psc.linker_area();

    /* Capability allocation */
//    psc.ref_account();
//    psc.transfer_quota();
//    psc.cap_quota();
//    psc.used_caps();
//    psc.avail_caps();

    /* RAM allocation */
//    psc.transfer_quota();
//    psc.ram_quota();
//    psc.used_ram();
//    psc.avail_ram();

    /* kernel specific interface */
//    psc.native_pd();

    /* System management interface */
//    psc.system_control_cap();

    /* user-level device drivers */
//    psc.dma_addr();
//    psc.attach_dma();



    env.parent().exit(exit_code);
}
