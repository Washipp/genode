/*
 * \brief  Compiler-rt, adapted from afl++ `compiler-rt.o.c`
 * \author silas
 * \date   2025-03-11
 */

/* afl++ port includes */
#include "init.h"
#include "sys/shm.h"
#include "compiler_rt.h"

/* Genode includes*/
#include <base/component.h>
#include <base/log.h>
#include <base/attached_rom_dataspace.h>

using namespace Genode;

/* afl++ type definitions
 * They come from types.h and llvm-alternative-coverage.h
 * This way, wrapper does not need libc.
 * */
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long int u64;

/* Do not change this unless you really know what you are doing. */
#define MAP_SIZE (1U << MAP_SIZE_POW2)
#if MAP_SIZE <= 2097152
#define MAP_INITIAL_SIZE (2 << 20)  // = 2097152
#else
#define MAP_INITIAL_SIZE MAP_SIZE
#endif

/* Maximum ngram size */
#define NGRAM_SIZE_MAX 16U

/* Maximum K for top-K context sensitivity */
#define CTX_MAX_K 32U

/* the __afl_* variables are required by the instrumented binary and need to be setup before
 * the fuzzing starts. */

static u8 __afl_area_initial[MAP_INITIAL_SIZE];

__thread u16 __afl_prev_loc[NGRAM_SIZE_MAX];
__thread u16 __afl_prev_caller[CTX_MAX_K];
__thread u32 __afl_prev_ctx;

u8 *__afl_area_ptr = __afl_area_initial;
u64 __afl_map_addr;

u8 *__afl_fuzz_ptr;
u32 *__afl_fuzz_len;


struct Compiler_rt {
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
            Genode::error("Could not access coverage map shared memory");
            _env.parent().exit(1);
        }

        /* Write something into the bitmap so that even with low AFL_INST_RATIO,
           our parent doesn't give up on us. */

        __afl_area_ptr[0] = 1;
    }


public:

    Compiler_rt(Env &env) : _env(env)
    {
        shm_init(env);
        _afl_map_shm();
        _afl_map_shm_fuzz();
    }
};


Constructible<Compiler_rt> c_rt;

int call_function(Function_to_fuzz function_to_fuzz) {
    if (!c_rt.constructed()) {
        Genode::warning("Compiler runtime not constructed yet. This could lead to issues."
                        "Call 'compiler_rt_init()' first. ");
    }
    return function_to_fuzz(__afl_fuzz_ptr);
}

void compiler_rt_init(Genode::Env &env) {
    if (!c_rt.constructed()) {
        c_rt.construct(env);
    }
}