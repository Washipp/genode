/*
 * \brief  Fuzz harness for XML parser
 * \author Silas Meier
 * \date   2025-03-25
 */

/* afl++ port includes */
#include "compiler_rt.h"
#include "sys/shm.h"

/* Genode includes*/
#include <base/component.h>
#include <base/attached_rom_dataspace.h>

using namespace Genode;

void Component::construct(Env &env)
{
    compiler_rt_init(env);

    Attached_rom_dataspace _config_rom { env, "config" };
    int _sut_status_shmid { _config_rom.xml().attribute_value("sut_status_shmid", 0) };
    int _max_iterations_before_reset { _config_rom.xml().attribute_value("max_iterations_before_reset", 1) };
    volatile char *_sut_status = (char *) shmat(_sut_status_shmid, NULL, 0);

    int exit_code = 0;

    for (int i = 0; i < _max_iterations_before_reset; i++) {

        /* Test log-session. */
        exit_code = call_function([](void *data, unsigned int *len) {
            try {
                Genode::Xml_node((char const *) data, (size_t) len);
                return 0;
            } catch (Xml_node::Invalid_syntax) {
                return 0;
            }
        });
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