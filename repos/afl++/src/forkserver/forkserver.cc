/* Genode includes */
#include <libc/component.h>
#include <base/log.h>
#include <base/attached_rom_dataspace.h>
#include <os/reporter.h>

/* afl++ includes */
#include "config.h"
#include "types.h"

/* libc includes */
#include <unistd.h>

#define write_error(text) Genode::error("Error: ", text)

namespace Forkserver {
    class Main;

    using namespace Genode;
}

class Forkserver::Main {
    Libc::Env &_env;
    // Through this reporter new SUTs can be started.
    Expanding_reporter _init_config_reporter { _env, "config", "config" };
    int _version { 0 };


    int afl_sharedmem_fuzzing = 1;
    u32 afl_map_size = MAP_SIZE;


    // TODO: this report should be provided by a user.
    // Further, the child needs to be instrumented by adding the afl++ as a LIBS dependency.
    int _report_new_sut()
    {
        _init_config_reporter.generate([&] (Xml_generator & xml) {
            xml.node("parent-provides",[&] () {
                xml.node("service",[&] () { xml.attribute("name", "CPU"); });
                xml.node("service",[&] () { xml.attribute("name", "File_system"); });
                xml.node("service",[&] () { xml.attribute("name", "LOG"); });
                xml.node("service",[&] () { xml.attribute("name", "PD"); });
                xml.node("service",[&] () { xml.attribute("name", "RM"); });
                xml.node("service",[&] () { xml.attribute("name", "ROM"); });
                xml.node("service",[&] () { xml.attribute("name", "Timer"); });
                xml.node("service",[&] () { xml.attribute("name", "Shm_session"); });
            });
            xml.node("start",[&] () {
                xml.attribute("name", "print_component");
                xml.attribute("caps", "50");
                xml.attribute("version", ++_version);
                xml.node("resource",[&] () { xml.attribute("name", "RAM"); xml.attribute("quantum", "64M"); });

                xml.node("route",[&] () {
                    xml.node("service",[&] () { xml.attribute("name", "CPU"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "File_system"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "LOG"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "PD"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "RM"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "ROM"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "Timer"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "Shm_session"); xml.node("parent",[&] () {}); });
                });
            });
        });
        return _version;
    }

    // TODO: Check the status of the child, until the status changes or the heartbeat stops.
    int _wait_for_exit(int child_pid, int *status) {
        Genode::warning("Not implemented. Should wait for the component with version: '", child_pid,"' to finish.");
        (void) status;
        return 0;
    }

public:


/*
     * This commented-out function needs to be executed before the SUT starts.
     * Normally this was done by the forkserver. But It sets up shared memory fuzzing which the SUT needs to do.
     *
     * It is important to set __afl_fuzz_len and __afl_fuzz_ptr
     *
    static void __afl_map_shm_fuzz()
    {
        char *id_str = getenv(SHM_FUZZ_ENV_VAR);

        if (__afl_debug)
            fprintf(stderr, "DEBUG: fuzzcase shmem %s\n", id_str ? id_str : "none");

        if (id_str) {
            u8 *map = NULL;

            u32 shm_id = atoi(id_str);
            map = (u8 *) shmat(shm_id, NULL, 0);

            // Whooooops.

            if (!map || map == (void *) -1) {
                perror("Could not access fuzzing shared memory");
                send_forkserver_error(FS_ERROR_SHM_OPEN);
                exit(1);
            }

            __afl_fuzz_len = (u32 *) map;
            __afl_fuzz_ptr = map + sizeof(u32);

            if (__afl_debug) {
                fprintf(stderr, "DEBUG: successfully got fuzzing shared memory\n");
            }
        } else {
            fprintf(stderr, "Error: variable for fuzzing shared memory is not set\n");
            send_forkserver_error(FS_ERROR_SHM_OPEN);
            exit(1);
        }
    }
*/

    /**
     * Instead of transferring the child_id we transfer the version of the component, as it uniquely identifies the child.
     * */
    void afl_start_forkserver(void)
    {
        u32 was_killed = 0;
        u32 version = 0x41464c00 + FS_NEW_VERSION_MAX;
        u32 tmp = version ^ 0xffffffff, status2, status = version;
        u8 *msg = (u8 *) &status;
        u8 *reply = (u8 *) &status2;
        u32 child_pid = -1;

        // START forkserver handshake

        // return because possible non-forkserver usage
        if (write(FORKSRV_FD + 1, msg, 4) != 4) { return; }

        if (read(FORKSRV_FD, reply, 4) != 4) { _env.parent().exit(1); }
        if (tmp != status2) {
            write_error("wrong forkserver message from AFL++ tool");
            _env.parent().exit(1);
        }

        // send the set/requested options to forkserver
        status = FS_NEW_OPT_MAPSIZE;  // we always send the map size
        if (afl_sharedmem_fuzzing) { status |= FS_NEW_OPT_SHDMEM_FUZZ; }
/*
        if (__afl_dictionary_len && __afl_dictionary) {
            status |= FS_NEW_OPT_AUTODICT;
        }
*/

        if (write(FORKSRV_FD + 1, msg, 4) != 4) { _env.parent().exit(1); }

        // Now send the parameters for the set options, increasing by option number

        // FS_NEW_OPT_MAPSIZE - we always send the map size
        status = afl_map_size;
        if (write(FORKSRV_FD + 1, msg, 4) != 4) { _env.parent().exit(1); }

        // FS_NEW_OPT_SHDMEM_FUZZ - no data

        // FS_NEW_OPT_AUTODICT - send autodictionary
/*
         * ### For now, we do not support afl-dictionaries. This would need to be defined here.
         * ### But we still need to write it maybe?
         *
          if (__afl_dictionary_len && __afl_dictionary) {
            // pass the dictionary through the forkserver FD
            u32 len = __afl_dictionary_len, offset = 0;

            if (write(FORKSRV_FD + 1, &len, 4) != 4) {
              write(2, "Error: could not send dictionary len\n",
                    strlen("Error: could not send dictionary len\n"));
              _env.parent().exit(1);
            }

            while (len != 0) {
              s32 ret;
              ret = write(FORKSRV_FD + 1, __afl_dictionary + offset, len);

              if (ret < 1) {
                write_error("could not send dictionary");
                _env.parent().exit(1);
              }

              len -= ret;
              offset += ret;
            }
          }
*/
        // send welcome message as final message
        status = version;
        if (write(FORKSRV_FD + 1, msg, 4) != 4) { _env.parent().exit(1); }

        // END forkserver handshake

        if (afl_sharedmem_fuzzing) {
            Genode::log("Using shared memory. The setup should be done by child. See '__afl_map_shm_fuzz()'");
        }

        while (true) {
            int status;

            /* Wait for parent by reading from the pipe. Abort if read fails. */
            if (read(FORKSRV_FD, &was_killed, 4) != 4) {
                write_error("read from AFL++ tool");
                _env.parent().exit(1);
            }


            /* Once woken up, start a new SUT component. */

            child_pid = _report_new_sut();

            /* In parent process: write PID to pipe, then wait for child. */

            if (unlikely(write(FORKSRV_FD + 1, &child_pid, 4) != 4)) {
                write_error("write to afl-fuzz");
                _env.parent().exit(1);
            }

            if (unlikely(_wait_for_exit(child_pid, &status))) {
                write_error("_wait_for_exit");
                _env.parent().exit(1);
            }


            /* Relay wait status to pipe, then loop back. */

            if (unlikely(write(FORKSRV_FD + 1, &status, 4) != 4)) {
                write_error("writing to afl-fuzz");
                _env.parent().exit(1);
            }
        }
    }

    Main(Libc::Env &env) : _env(env)
    {
        afl_start_forkserver();
    }
};

void Libc::Component::construct(Libc::Env &env)
{
    Forkserver::Main main(env);
}
