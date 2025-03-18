/* afl++ port includes */
#include "compiler_rt.h"

/* Genode includes*/
#include <base/component.h>
#include <util/string.h>

using namespace Genode;


void crash(char * input, int val) {
    Genode::memcpy(&input[val], input, 5);
    if ((val/ (val + 50000)) > 0 )
        input[val] = 'c';
}

int function_to_fuzz(void *input, unsigned int *len)
{
    (void) len;
    char *inp = (char *) input;
    char alloc[5];

    if (inp[0] == 'b') {
        alloc[0] = inp[0];
        Genode::log(Cstring((char const *)input, 5));
        if (inp[1] == 'u' || inp[1] == 'b') {
            alloc[1] = inp[1];
            Genode::memcpy(alloc, input, 5);
            crash(alloc, -50000);
        }
    }
    return 0;
}
void Component::construct(Env &env)
{
    (void) env;
    compiler_rt_init(env);

    /* Should crash after a while. */
    int exit_code = call_function(function_to_fuzz);

    env.parent().exit(exit_code);
}
