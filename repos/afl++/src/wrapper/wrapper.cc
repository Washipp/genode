/* afl++ port includes */
#include "init.h"
#include "sys/shm.h"
//#include "genode-fuzz.h"

/* Genode includes*/
#include <libc/component.h>
#include <base/log.h>
#include <base/attached_rom_dataspace.h>

/* afl++ includes */
#include <types.h>
#include <llvm-alternative-coverage.h>

namespace SUT_wrapper {
    struct Main;
    using namespace Genode;
}

extern int function_to_fuzz(void * input);

static u8 __afl_area_initial[MAP_INITIAL_SIZE];

__thread PREV_LOC_T __afl_prev_loc[NGRAM_SIZE_MAX];
__thread PREV_LOC_T __afl_prev_caller[CTX_MAX_K];
__thread u32        __afl_prev_ctx;

u8 *__afl_area_ptr = __afl_area_initial;
u64 __afl_map_addr;

u8 *__afl_fuzz_ptr;
u32 *__afl_fuzz_len;

struct SUT_wrapper::Main {
    Env &_env;

    Attached_rom_dataspace _config_rom { _env, "config" };
    int _coverage_map_shmid { _config_rom.xml().attribute_value("coverage_map_shmid", 0) };
    int _fuzzing_shmid { _config_rom.xml().attribute_value("fuzzing_shmid", 0) };

    void _afl_map_shm_fuzz()
    {
        u8 *map = NULL;

        map = (u8 *) shmat(_fuzzing_shmid, NULL, 0);

        if (!map || map == (void *) -1) {
            Genode::error("Could not access fuzzing shared memory");
            _env.parent().exit(1);
        } else {
            __afl_fuzz_len = (u32 *) map;
            __afl_fuzz_ptr = map + sizeof(u32);
        }
    }


    void _afl_map_shm(void)
    {
        __afl_area_ptr = (u8 *) shmat(_coverage_map_shmid, (void *) __afl_map_addr, 0);

        if (!__afl_area_ptr || __afl_area_ptr == (void *) -1) {
            Genode::error("shmat for map");
            _env.parent().exit(1);
        }

        /* Write something into the bitmap so that even with low AFL_INST_RATIO,
           our parent doesn't give up on us. */

        __afl_area_ptr[0] = 1;
    }


public:
    Main(Env &env) : _env(env)
    {
        shm_init(env);
        _afl_map_shm();
        _afl_map_shm_fuzz();

        int exit_code = function_to_fuzz(__afl_fuzz_ptr);
        env.parent().exit(exit_code);
    }
};

void Libc::Component::construct(Libc::Env &env)
{
    SUT_wrapper::Main main(env);
    env.parent().exit(0);
}
