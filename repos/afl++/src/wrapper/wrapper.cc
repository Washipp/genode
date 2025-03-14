/* afl++ port includes */
#include "compiler_rt.h"

/* Genode includes*/
#include <base/component.h>
#include <base/stdint.h>

using namespace Genode;

int log(void *input, unsigned int * len) {
    Genode::log(Cstring((char const *)input, (size_t)len));
    return 0;
}

void Component::construct(Env &env)
{
    compiler_rt_init(env);

    int exit_code = call_function(log);

    env.parent().exit(exit_code);
}
