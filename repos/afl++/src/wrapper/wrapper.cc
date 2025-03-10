/* afl++ port includes */
#include "compiler_rt.h"

void Component::construct(Genode::Env &env)
{
    Compiler_rt::Main main(env);

    int exit_code = function_to_fuzz(__afl_fuzz_ptr);

    env.parent().exit(exit_code);
}
