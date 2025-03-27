/* afl++ port includes */
#include "init.h"
#include "forkserver_wrapper.h"
#include "sys/shm.h"

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

    /* Read config */
    Attached_rom_dataspace _config_rom { _env, "config" };
    // TODO: figure out a way to determine the ideal default value of the skipped heartbeats.
    int _max_skipped_heartbeats { _config_rom.xml().attribute_value("max_skipped_heartbeats", 10000) };
    int _max_iterations_before_reset { _config_rom.xml().attribute_value("max_iterations_before_reset", 1000) };
    String<256> _timeout_ms { _config_rom.xml().attribute_value("timeout_ms", String<256>("200")) };
    String<256> _input_dir { _config_rom.xml().attribute_value("input_dir", String<256>("./input")) };
    String<256> _output_dir { _config_rom.xml().attribute_value("output_dir", String<256>("./output")) };
    String<256> _harness { _config_rom.xml().attribute_value("harness", String<256>("example_crash")) };

    /* Reporter starts new SUT */
    Expanding_reporter _init_config_reporter { _env, "config", "config" };
    int _version { 0 };

    Attached_rom_dataspace _state_rom { _env, "state" };

    int _status { -1 };

    /* In order to signal, that the SUT is done, we use a bit of shared memory. */
    char *_sut_status = nullptr;
    int _sut_status_shmid = 0;
    int _runs_before_reset = 0;

    void _setup_execution_status()
    {
        _sut_status_shmid = shmget(0, sizeof(char), 0);
        _sut_status = (char *) shmat(_sut_status_shmid, NULL, 0);
    }

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
            xml.node("heartbeat", [&]() { xml.attribute("rate_ms", 5); });
            xml.node("report", [&]() {
                xml.attribute("ids", "yes");
                xml.attribute("child_ram", "yes");
                xml.attribute("child_caps", "yes");
                xml.attribute("init_ram", "yes");
                xml.attribute("init_caps", "yes");
            });
            xml.node("start", [&]() {
                xml.node("heartbeat", [&]() { });
                xml.attribute("name", _harness);
                xml.attribute("caps", "500");
                xml.attribute("version", _version);
                xml.node("resource", [&]() {
                    xml.attribute("name", "RAM");
                    xml.attribute("quantum", "50M");
                });
                xml.node("config", [&]() {
                    xml.attribute("coverage_map_shmid", coverage_map_shmid);
                    xml.attribute("fuzzing_shmid", fuzzing_shmid);
                    xml.attribute("sut_status_shmid", _sut_status_shmid);
                    xml.attribute("max_iterations_before_reset", _max_iterations_before_reset);
                });
                xml.node("route", [&]() {
                    xml.node("service", [&]() {
                        xml.attribute("name", "Report");
                        xml.attribute("label", "fuzz_config");
                        xml.node("child", [&]() {
                            xml.attribute("name", "SUT_report_rom");
                        }); });
                    xml.node("any-service", [&]() { xml.node("parent", [&]() { }); });
                });
            });
            xml.node("start", [&]() {
                xml.attribute("verbose", "no");
                xml.node("heartbeat", [&]() { });
                xml.attribute("name", "SUT_init");
                xml.attribute("caps", "500");
                xml.attribute("version", _version);
                xml.node("binary", [&]() { xml.attribute("name", "init_instrumented"); });
                xml.node("resource", [&]() {
                    xml.attribute("name", "RAM");
                    xml.attribute("quantum", "500M");
                });
                xml.node("config", [&]() {
                    xml.attribute("coverage_map_shmid", coverage_map_shmid);
                    xml.attribute("fuzzing_shmid", fuzzing_shmid);
                });
                xml.node("route", [&]() {
                    xml.node("service", [&]() {
                        xml.attribute("name", "ROM");
                        xml.attribute("label", "fuzz_config");
                        xml.node("child", [&]() {
                            xml.attribute("name", "SUT_report_rom");
                        });
                    });
                    xml.node("any-service", [&]() { xml.node("parent", [&]() { }); });
                });
            });
            xml.node("start", [&]() {
                xml.attribute("verbose", "no");
                xml.attribute("name", "SUT_report_rom");
                xml.attribute("caps", "500");
                xml.attribute("version", _version);
                xml.node("binary", [&]() { xml.attribute("name", "report_rom"); });
                xml.node("resource", [&]() {
                    xml.attribute("name", "RAM");
                    xml.attribute("quantum", "50M");
                });
                xml.node("provides", [&]() {
                    xml.node("service", [&]() { xml.attribute("name", "Report"); });
                    xml.node("service", [&]() { xml.attribute("name", "ROM"); });
                });
                xml.node("config", [&]() {
                    xml.node("policy", [&]() {
                        xml.attribute("label", "SUT_init -> fuzz_config");

                        /* Concatenate _harness with " -> config" */
                        String<32> suffix = " -> fuzz_config";
                        size_t len1 = Genode::strlen(_harness.string());
                        size_t len2 = Genode::strlen(suffix.string());
                        char* combinedStr = (char *) malloc(((long)(len1 + len2 + 1)) * sizeof(char *));
                        Genode::memcpy(combinedStr, _harness.string(), len1);
                        Genode::memcpy(combinedStr + len1, suffix.string(), len2);
                        xml.attribute("report", combinedStr);
                        free(combinedStr);

//                        xml.attribute("report", "rom_session_harness -> config");
                    });
                });
                xml.node("route", [&]() {
                    xml.node("any-service", [&]() { xml.node("parent", [&]() { }); });
                });
            });
        });

        _version++;
    }


public:

    /* This function reports and starts a new child. Then it waits until there is an exit code
     * in the reported state, the timeout is reached or max tries is reached. */
    int __attribute__((hot)) report_new_target(int coverage_map_shmid,
                                               int fuzzing_shmid,
                                               struct Exec_data *exec_data,
                                               uint64_t timeout)
    {
        uint64_t start = _timer.curr_time().trunc_to_plain_ms().value;

        /* status != 0 means that we need to restart the component. */
        if (_runs_before_reset >= _max_iterations_before_reset || _status != 0) {
            _generate_new_report(coverage_map_shmid, fuzzing_shmid);
        } else {
            _runs_before_reset++;
            /* Reset the status bit. */
            _sut_status[0] = 0;
        }

        for (;;) {

            _state_rom.update();
            const Xml_node cfg = _state_rom.xml();
            if (!cfg.has_type("empty") && cfg.has_sub_node("child")) {
                const Xml_node child = cfg.sub_node("child");

                if (child.has_attribute("exited")) {
                    _status = child.attribute_value("exited", 0);
                    break;
                } else if (child.has_attribute("skipped_heartbeats")) {
                    if (child.attribute_value("skipped_heartbeats", 0) > _max_skipped_heartbeats) {
                        _status = EXIT_FAILURE;
                        break;
                    }
                }

            }

            if (_timer.curr_time().trunc_to_plain_ms().value - start > timeout) {
                /* special value to signal timeout. */
                _status = -11;
                break;
            }

            /* The SUT indicated, that it is ready to read a new test case. */
            if (_sut_status[0] != 0) {
                _status = 0;
                break;
            }
        }

        /* Version in Exec_data is used as the process id of the child. */
        exec_data->version = _version;
        exec_data->status = _status;

        /* everything good. */
        return 1;
    }

    void start_afl_fuzz()
    {
        Libc::with_libc([&]() {
            int argc = 9;
            char **argv_orig = (char **) malloc(argc * sizeof(char *));
            char **envp = (char **) malloc(1 * sizeof(char *));

            const char *args[] = { "./program",
                                   "-i", _input_dir.string(),
                                   "-o", _output_dir.string(),
                                   "-t", _timeout_ms.string(),
                                   "--", "/Genode" };

            for (int i = 0; i < argc; ++i) {
                argv_orig[i] = strdup(args[i]);
            }

            main(argc, argv_orig, envp);
        });
    }

    Main(Libc::Env &env) : _env(env)
    {
        _setup_execution_status();
    }

    // define these constructors for the _sut_status pointer.
    Main(const Main &) = delete;              // copy ctor
    Main(Main &&) = delete;                   // move ctor
    Main &operator=(const Main &) = delete;   // copy assignment
    Main &operator=(Main &&) = delete;        // move assignment
    ~Main() { }                               // dtor
};


/* This pointer allows afl++ to use the function defined in Main. */
Afl_fuzz::Main *main_reporter;

/* This function call replaces the fork() and execv() call in afl-fuzz. */
extern "C" int __attribute__((hot)) call_report_new_forkserver(int coverage_map_shmid,
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

    init_main.start_afl_fuzz();
}