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
    // FIXME: maybe a different env.pd_session_cap() is needed?
    Pd_session_client psc = Pd_session_client(env.pd_session_cap());

    /* PD signals RPC calls */
    Capability<Signal_source> signal_source;
    // FIXME: maybe define a different signal_source
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
    Capability<Signal_context> context;
    psc.alloc_context(signal_source, imprint).with_result(
            [&](Capability<Signal_context> sc) {
                Genode::log(sc.local_name());
                context = sc;
            },
            [&](Pd_session::Alloc_context_error error) {
                switch (error) {
                    case Pd_session::Alloc_context_error::OUT_OF_RAM:
                    case Pd_session::Alloc_context_error::OUT_OF_CAPS:
                    case Pd_session::Alloc_context_error::INVALID_SIGNAL_SOURCE:
                        env.parent().exit(0);
                        break;
                    default:
                        env.parent().exit(1);
                }
            }
    );
    if (!context.valid())
        env.parent().exit(exit_code);

    psc.submit(context, *get_fuzz_len());
    psc.free_signal_source(signal_source);
    psc.free_context(context);

    /* RPC Framework */
    // FIXME: define psc.native_pd()
    Native_capability native_capability;
    psc.alloc_rpc_cap(psc.native_pd()).with_result(
            [&](Native_capability nc) {
                native_capability = nc;
            },
            [&](Pd_session::Alloc_rpc_cap_error error) {
                switch (error) {
                    case Pd_session::Alloc_rpc_cap_error::OUT_OF_RAM:
                    case Pd_session::Alloc_rpc_cap_error::OUT_OF_CAPS:
                        env.parent().exit(0);
                        break;
                    default:
                        env.parent().exit(1);
                }
            }
    );
    if (!native_capability.valid())
        env.parent().exit(exit_code);

    psc.free_rpc_cap(native_capability);

    /* Virtual address management */
    // Nothing to test here, this simply returns capabilities to RM
//    psc.address_space();
//    psc.stack_area();
//    psc.linker_area();

    /* Capability allocation */
    Pd_connection pd_reference { env };

    psc.ref_account(pd_reference.cap());
    /* This interprets the first 8 bytes of the pointer as a number. */
    size_t interpret_as_number = 1024 * 8;
    ascii_to(((char*) get_fuzz_ptr()), interpret_as_number);
    Pd_session::Transfer_cap_quota_result cap_transfer_result = psc.transfer_quota(pd_reference.cap(), Cap_quota{interpret_as_number});
    switch (cap_transfer_result) {
        case Pd_session::Transfer_cap_quota_result::OK:
        case Pd_session::Transfer_cap_quota_result::OUT_OF_CAPS:
        case Pd_session::Transfer_cap_quota_result::INVALID_SESSION:
        case Pd_session::Transfer_cap_quota_result::NO_REF_ACCOUNT:
            break;
        default:
            env.parent().exit(1);
    }

    /* RAM allocation */
    Pd_session::Transfer_ram_quota_result ram_transfer_result = psc.transfer_quota(pd_reference.cap(), Ram_quota{interpret_as_number});
    switch (ram_transfer_result) {
        case Pd_session::Transfer_ram_quota_result::OK:
            break;
        case Pd_session::Transfer_ram_quota_result::OUT_OF_RAM:
            break;
        case Pd_session::Transfer_ram_quota_result::INVALID_SESSION:
        case Pd_session::Transfer_ram_quota_result::NO_REF_ACCOUNT:
            exit_code |= 1;
            break;
        default:
            break;
    }

    /* kernel specific interface */
    // Nothing to be done here

    /* System management interface */
    // FIXME: Not sure on how to use this...
//    psc.system_control_cap(Affinity::Location());

    /* user-level device drivers */
//    psc.dma_addr();
//    psc.attach_dma();



    env.parent().exit(exit_code);
}
