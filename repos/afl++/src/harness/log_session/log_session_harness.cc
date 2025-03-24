/* afl++ port includes */
#include "compiler_rt.h"
#include "sys/shm.h"

/* Genode includes*/
#include <base/component.h>
#include <base/attached_rom_dataspace.h>

using namespace Genode;

void Component::construct(Env &env)
{
    env.exec_static_constructors();
    compiler_rt_init(env);

    Attached_rom_dataspace _config_rom { env, "config" };
    int _sut_status_shmid { _config_rom.xml().attribute_value("sut_status_shmid", 0) };
    char *_sut_status = (char *) shmat(_sut_status_shmid, NULL, 0);

    int exit_code = 0;

    while (exit_code == 0) {

        /* Test log-session. */
        exit_code = call_function([](void *data, unsigned int *len) {
            Genode::log(Cstring((char const *) data, (size_t) len));
            return 0;
        });

        if (exit_code == 0) {
            _sut_status[0] = 1;

            // Poll for a status update.
            while (_sut_status[0] == 1);

        } else {
            env.parent().exit(exit_code);
        }
    }

}
