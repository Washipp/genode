/*
 * \brief  Fuzz harness for the ROM session
 * \author Silas Meier
 * \date   2025-03-25
 */

/* afl++ port includes */
#include "compiler_rt.h"
#include "sys/shm.h"

/* Genode includes*/
#include <base/component.h>
#include <base/attached_rom_dataspace.h>
#include <os/reporter.h>

using namespace Genode;

void Component::construct(Env &env)
{
    compiler_rt_init(env);

    Attached_rom_dataspace _config_rom { env, "config" };
    int _sut_status_shmid { _config_rom.xml().attribute_value("sut_status_shmid", 0) };
    int _max_iterations_before_reset { _config_rom.xml().attribute_value("max_iterations_before_reset", 1) };
    volatile char *_sut_status = (char *) shmat(_sut_status_shmid, NULL, 0);

    Expanding_reporter _init_config_reporter { env, "config", "fuzz_config" };

    int exit_code = 0;

    for (int i = 0; i < _max_iterations_before_reset; i++) {

        try {
            auto node = Genode::Xml_node((char const *) get_fuzz_ptr(), (size_t) get_fuzz_len());
            _init_config_reporter.generate(node);
        } catch (Xml_node::Invalid_syntax) {
            exit_code = 0;
        }

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