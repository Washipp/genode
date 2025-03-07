/* afl++ port includes */
#include "init.h"
#include "forkserver_wrapper.h"

/* Genode includes */
#include <libc/component.h>
#include <base/log.h>
#include <os/reporter.h>
#include <base/attached_rom_dataspace.h>
#include <timer_session/connection.h>

/* libc includes */
#include <stdlib.h>
#include <string.h>

/* Used for debugging. Upon call, the execution waits for an ENTER input. */
extern "C" void wait_for_continue(void);

namespace Afl_fuzz {
    class Main;

    using namespace Genode;
}

class Afl_fuzz::Main {

    Libc::Env &_env;
    Timer::Connection _timer { _env };

    /* Reporter starts new SUT */
    Expanding_reporter _init_config_reporter { _env, "config", "config" };
    int _version { 0 };

    Attached_rom_dataspace _state_rom { _env, "state" };
    Signal_receiver _signal_receiver { };
    Signal_context sc { };
    Signal_context_capability _signal_handler = _signal_receiver.manage(sc);

    int _status { 0 };

    void _generate_new_report(int coverage_map_shmid, int fuzzing_shmid)
    {
        _init_config_reporter.generate([&](Xml_generator &xml) {
            xml.attribute("verbose", "no");
            xml.node("parent-provides", [&]() {
                xml.node("service", [&]() { xml.attribute("name", "Shm_session"); });
                xml.node("service", [&]() { xml.attribute("name", "Timer"); });
                xml.node("service", [&]() { xml.attribute("name", "CPU"); });
                xml.node("service", [&]() { xml.attribute("name", "LOG"); });
                xml.node("service", [&]() { xml.attribute("name", "PD"); });
                xml.node("service", [&]() { xml.attribute("name", "RM"); });
                xml.node("service", [&]() { xml.attribute("name", "ROM"); });
            });
            xml.node("report", [&]() {
                xml.attribute("delay_ms", 50);
                xml.attribute("ids", "yes");
                xml.attribute("child_ram", "yes");
                xml.attribute("child_caps", "yes");
                xml.attribute("init_ram", "yes");
                xml.attribute("init_caps", "yes");
            });
            xml.node("heartbeat", [&]() { xml.attribute("rate_ms", 5); });
            xml.node("start", [&]() {
                xml.attribute("name", "wrapper");
                xml.attribute("caps", "500");
                xml.attribute("version", ++_version);
                xml.node("resource", [&]() {
                    xml.attribute("name", "RAM");
                    xml.attribute("quantum", "500M");
                });
                xml.node("config", [&]() {
                    xml.attribute("coverage_map_shmid", coverage_map_shmid);
                    xml.attribute("fuzzing_shmid", fuzzing_shmid);
                    xml.node("vfs", [&]() {
                        xml.node("dir", [&]() {
                            xml.attribute("name", "dev");
                            xml.node("log", [&]() { });
                        });
                    });
                    xml.node("libc", [&]() {
                        xml.attribute("stdin", "/dev/log");
                        xml.attribute("stdout", "/dev/log");
                        xml.attribute("stderr", "/dev/log");
                    });
                    xml.node("default-policy", [&]() {
                        xml.attribute("root", "/");
                        xml.attribute("writeable", "yes");
                    });
                });

                xml.node("route", [&]() {
                    xml.node("any-service", [&]() { xml.node("parent", [&]() { }); });
                });
            });
        });

    }


public:

    int report_new_target(int coverage_map_shmid, int fuzzing_shmid, struct Exec_data *exec_data, uint64_t timeout)
    {
        uint64_t start = _timer.curr_time().trunc_to_plain_ms().value;

        _generate_new_report(coverage_map_shmid, fuzzing_shmid);

        // TODO: Make max-tries configurable?
        int max_tries = 100;
        bool finished_execution = false;

        for(int i = 0; i < max_tries && !finished_execution; i++) {
            auto sig = _signal_receiver.wait_for_signal();

            _state_rom.update();
            const Xml_node cfg = _state_rom.xml();
            if (!cfg.has_type("empty")) {
                if (cfg.has_sub_node("child") && cfg.sub_node("child").has_attribute("exited")) {
                    finished_execution = true;
                    _status = cfg.sub_node("child").attribute_value("exited", 0);
                }
            }

            if (_timer.curr_time().trunc_to_plain_ms().value - start > timeout) {
                /* special value to signal timeout. */
                _status = -11;
                break;
            }
        }

        exec_data->version = _version;
        exec_data->status = _status;

        /* everything good. */
        return 1;
    }

    Main(Libc::Env &env) : _env(env)
    {
        _state_rom.sigh(_signal_handler);
    }
};


/* This pointer allows afl++ to use the function defined in Main. */
Afl_fuzz::Main *main_reporter;

/* This function call replaces the fork() and execv() call in afl-fuzz. */
extern "C" int call_report_new_forkserver(int coverage_map_shmid,
                                          int fuzzing_shmid,
                                          struct Exec_data *exec_data,
                                          int timeout)
{
    return main_reporter->report_new_target(coverage_map_shmid, fuzzing_shmid, exec_data, (Genode::uint64_t) timeout);
}

void Libc::Component::construct(Libc::Env &env)
{
    shm_init(env);

    Afl_fuzz::Main init_main(env);
    main_reporter = &init_main;

    // TODO: Patch the option directly into afl++.
    setenv("AFL_NO_UI", "1", 1);
    setenv("AFL_NO_FORKSRV", "1", 1);

    Libc::with_libc([&]() {
        int argc = 7;
        char **argv_orig = (char **) malloc(argc * sizeof(char *));
        char **envp = (char **) malloc(1 * sizeof(char *));

        argv_orig[0] = strdup("./program");
        argv_orig[1] = strdup("-i");
        argv_orig[2] = strdup("./input");
        argv_orig[3] = strdup("-o");
        argv_orig[4] = strdup("./output");
        argv_orig[5] = strdup("--");
        argv_orig[6] = strdup("Genode");

        main(argc, argv_orig, envp);
        Genode::log("afl-fuzz test completed.");
    });
}