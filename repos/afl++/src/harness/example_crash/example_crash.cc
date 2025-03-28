/* afl++ port includes */
#include "compiler_rt.h"
#include "sys/shm.h"

/* Genode includes*/
#include <base/component.h>
#include <util/string.h>
#include <base/attached_rom_dataspace.h>

using namespace Genode;


void crash(char * input, int val) {
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
        if (inp[1] == 'u') {
            Genode::log("b");
            alloc[1] = inp[1];
            if (inp[1] == 'h') {
                Genode::log("d");
                while (1);
            }
        }
        if (inp[1] == 'g') {
            Genode::log("c");
            alloc[2] = inp[2];
            crash(alloc, -50000);
        }
        if (inp[1] == 'z') {
            Genode::log("e");
            int * ptr = nullptr;
            ptr[0] = 5;
        }
    }
    return 0;
}

void Component::construct(Env &env)
{
    compiler_rt_init(env);

    Attached_rom_dataspace _config_rom { env, "config" };
    int _sut_status_shmid { _config_rom.xml().attribute_value("sut_status_shmid", 0) };
    int _max_iterations_before_reset { _config_rom.xml().attribute_value("max_iterations_before_reset", 1) };
    volatile char *_sut_status = (char *) shmat(_sut_status_shmid, NULL, 0);


    int exit_code = 0;

    for (int i = 0; i < _max_iterations_before_reset; i++) {

        exit_code = call_function(function_to_fuzz);

        if (exit_code == 0) {
            _sut_status[0] = 1;

            /* Poll for a status update. */
            while (_sut_status[0] == 1);

        } else {
            break;
        }
    }

    env.parent().exit(exit_code);
}
