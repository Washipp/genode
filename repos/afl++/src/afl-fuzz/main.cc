/* afl++ port includes */
#include "init.h"
#include "forkserver_wrapper.h"

/* Genode includes */
#include <libc/component.h>
#include <base/log.h>
#include <os/reporter.h>

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
    // Through this reporter new SUTs can be started.
    Expanding_reporter _init_config_reporter { _env, "config", "config" };
    int _version { 0 };

public:

    int report_new_forkserver(int st_pipe_0, int ctl_pipe_1, int out_fd, int coverage_map_shmid, int fuzzing_shmid) {
        _init_config_reporter.generate([&] (Xml_generator& xml) {
            xml.node("parent-provides",[&] () {
                xml.node("service",[&] () { xml.attribute("name", "File_system"); });
                xml.node("service",[&] () { xml.attribute("name", "Shm_session"); });
                xml.node("service",[&] () { xml.attribute("name", "Report"); });
                xml.node("service",[&] () { xml.attribute("name", "Timer"); });
                xml.node("service",[&] () { xml.attribute("name", "CPU"); });
                xml.node("service",[&] () { xml.attribute("name", "LOG"); });
                xml.node("service",[&] () { xml.attribute("name", "PD"); });
                xml.node("service",[&] () { xml.attribute("name", "RM"); });
                xml.node("service",[&] () { xml.attribute("name", "ROM"); });
            });
            xml.node("start",[&] () {
                xml.attribute("name", "startup_init");
                xml.attribute("caps", "100");
                xml.attribute("version", ++_version);
                xml.node("binary",[&] () { xml.attribute("name", "init"); });
                xml.node("resource",[&] () { xml.attribute("name", "RAM"); xml.attribute("quantum", "64M"); });
                xml.node("config",[&] () {
                    // Report ROM
                    xml.node("start",[&] () {
                        xml.attribute("name", "runtime_report_rom");
                        xml.attribute("verbose", "yes");
                        xml.node("binary",[&] () { xml.attribute("name", "report_rom"); });
                        xml.node("resource",[&] () { xml.attribute("name", "RAM"); xml.attribute("quantum", "4M"); });
                        xml.node("provides",[&] () {
                            xml.node("service",[&] () { xml.attribute("name", "Report"); });
                            xml.node("service",[&] () { xml.attribute("name", "ROM"); });
                        });
                        xml.node("config",[&] () {
                            xml.node("policy",[&] () { xml.attribute("label", "runtime_init -> config"); xml.attribute("report", "forkserver -> config"); });
                            xml.node("policy",[&] () { xml.attribute("label", "forkserver -> state"); xml.attribute("report", "runtime_init -> state"); });
                        });
                    });

                    // Forkserver
                    xml.node("start",[&] () {
                        xml.attribute("name", "forkserver");
                        xml.node("resource",[&] () { xml.attribute("name", "RAM"); xml.attribute("quantum", "300M"); });
                        xml.node("config",[&] () {
                            xml.attribute("st_pipe_0", st_pipe_0);
                            xml.attribute("ctl_pipe_1", ctl_pipe_1);
                            xml.attribute("out_fd", out_fd);
                            xml.attribute("coverage_map_shmid", coverage_map_shmid);
                            xml.attribute("fuzzing_shmid", fuzzing_shmid);
                            xml.node("vfs",[&] () { xml.node("fs",[&] () { }); });
                            xml.node("libc",[&] () {
                                xml.attribute("stdin", "/dev/log");
                                xml.attribute("stdout", "/dev/log");
                                xml.attribute("stderr", "/dev/log");
                                xml.attribute("rtc", "/dev/rtc");
                                xml.attribute("rng", "/dev/urandom");
                                xml.attribute("pipe", "/dev/pipe");
                            });
                        });
                        xml.node("route",[&] () {
                            xml.node("service",[&] () {
                                xml.attribute("name", "Report");
                                xml.attribute("label", "config");
                                xml.node("child",[&] () {xml.attribute("name", "runtime_report_rom");});
                            });
                            xml.node("service",[&] () {
                                xml.attribute("name", "ROM");
                                xml.attribute("label", "state");
                            });
                            xml.node("service",[&] () { xml.attribute("name", "File_system"); xml.node("parent",[&] () {}); });
                            xml.node("service",[&] () { xml.attribute("name", "Shm_session"); xml.node("parent",[&] () {}); });
                            xml.node("service",[&] () { xml.attribute("name", "Timer"); xml.node("parent",[&] () {}); });
                            xml.node("any-service",[&] () { xml.node("parent",[&] () {}); });
                        });
                    });

                    // runtime_init
                    xml.node("start",[&] () {
                        xml.attribute("name", "runtime_init");
                        xml.node("binary",[&] () { xml.attribute("name", "init"); });
                        xml.node("resource",[&] () { xml.attribute("name", "RAM"); xml.attribute("quantum", "500M"); });
                        xml.node("config",[&] () { xml.node("report",[&] () {
                            xml.attribute("ids", "yes");
                            xml.attribute("child_ram", "yes");
                            xml.attribute("child_caps", "yes");
                            xml.attribute("init_ram", "yes");
                            xml.attribute("init_caps", "yes");
                        }); });
                        xml.node("heartbeat",[&] () { xml.attribute("rate_ms", "2000"); });
                        xml.node("route",[&] () {
                             xml.node("service",[&] () {
                                xml.attribute("name", "ROM");
                                xml.attribute("label", "config");
                                xml.node("child",[&] () { xml.attribute("name", "runtime_report_rom"); });
                            });
                            xml.node("service",[&] () {
                                xml.attribute("name", "Report");
                                xml.node("child",[&] () { xml.attribute("name", "runtime_report_rom"); xml.attribute("label", "state"); });
                            });
                            xml.node("service",[&] () { xml.attribute("name", "File_system"); xml.node("parent",[&] () {}); });
                            xml.node("service",[&] () { xml.attribute("name", "Shm_session"); xml.node("parent",[&] () {}); });
                            xml.node("service",[&] () { xml.attribute("name", "Timer"); xml.node("parent",[&] () {}); });
                            xml.node("service",[&] () { xml.attribute("name", "CPU"); xml.node("parent",[&] () {}); });
                            xml.node("service",[&] () { xml.attribute("name", "LOG"); xml.node("parent",[&] () {}); });
                            xml.node("service",[&] () { xml.attribute("name", "PD"); xml.node("parent",[&] () {}); });
                            xml.node("service",[&] () { xml.attribute("name", "RM"); xml.node("parent",[&] () {}); });
                            xml.node("service",[&] () { xml.attribute("name", "ROM"); xml.node("parent",[&] () {}); });
                        });
                    });
                });
                xml.node("route",[&] () {
                    xml.node("service",[&] () {
                        xml.attribute("name", "ROM");
                        xml.attribute("label", "config");
                        xml.node("child",[&] () { xml.attribute("name", "startup_report_rom"); });
                    });
                    xml.node("service",[&] () {
                        xml.attribute("name", "File_system");
                        xml.node("child",[&] () { xml.attribute("name", "vfs"); });
                    });
                    xml.node("service",[&] () {
                        xml.attribute("name", "Shm_session");
                        xml.node("child",[&] () { xml.attribute("name", "afl-fuzz"); });
                    });
                    xml.node("service",[&] () {
                        xml.attribute("name", "Timer");
                        xml.node("child",[&] () { xml.attribute("name", "timer"); });
                    });
                    xml.node("service",[&] () { xml.attribute("name", "CPU"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "LOG"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "PD"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "RM"); xml.node("parent",[&] () {}); });
                    xml.node("service",[&] () { xml.attribute("name", "ROM"); xml.node("parent",[&] () {}); });
                });
            });
        });
        return _version;
    }

    Main(Libc::Env &env) : _env(env)
    {

    }
};

// This function call replaces the fork() and execv() call in afl-fuzz.
extern "C" int call_report_new_forkserver(void* forkserver, int st_pipe_0, int ctl_pipe_1, int out_fd, int coverage_map_shmid, int fuzzing_shmid) {
    return static_cast<Afl_fuzz::Main*>(forkserver)->report_new_forkserver(st_pipe_0, ctl_pipe_1, out_fd, coverage_map_shmid, fuzzing_shmid);
}

void Libc::Component::construct(Libc::Env &env)
{
    shm_init(env);

    Afl_fuzz::Main init_main(env);

    // This configuration should be done in the config of afl-fuzz
    setenv("AFL_NO_UI", "1", 1);
    setenv("AFL_PERSISTENT", "1", 1);

    Libc::with_libc([&] () {
        int argc = 7;
        char **argv_orig = (char **) malloc(argc * sizeof(char *));
        char **envp = (char **) malloc(1 * sizeof(char *));

        argv_orig[0] = strdup("./program");
        argv_orig[1] = strdup("-i");
        argv_orig[2] = strdup("./input");
        argv_orig[3] = strdup("-o");
        argv_orig[4] = strdup("./output");
        argv_orig[5] = strdup("--");
        argv_orig[6] = strdup("/binary/posix_bin");

        start_afl_fuzz(argc, argv_orig, envp, (void*) &init_main);
        Genode::log("afl-fuzz test completed.");
    });
}