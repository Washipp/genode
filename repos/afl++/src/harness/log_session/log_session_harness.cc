/* afl++ port includes */
#include "compiler_rt.h"

/* Genode includes*/
#include <base/component.h>

using namespace Genode;

void Component::construct(Env &env)
{
    compiler_rt_init(env);

    int exit_code = 0;
    /* Test log-session. */
    exit_code = call_function([](void *data, unsigned int *len) {
        Genode::log(Cstring((char const *) data, (size_t) len));
        return 0;
    });


    env.parent().exit(exit_code);
}
