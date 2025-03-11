/* afl++ port includes */
#include "compiler_rt.h"

/* Genode includes*/
#include <base/component.h>

//extern void sanitizer_init(Genode::Env &);
//extern void sanitizer_exit();

extern int function_to_fuzz(void *input);

void Component::construct(Genode::Env &env)
{
    compiler_rt_init(env);

    int exit_code = call_function(function_to_fuzz);

    env.parent().exit(exit_code);
}
