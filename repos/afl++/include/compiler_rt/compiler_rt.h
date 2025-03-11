#ifndef GENODE_COMPILER_RT_H
#define GENODE_COMPILER_RT_H

/* Genode includes*/
#include <base/component.h>

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

// TODO: Once things are figured out, the unused attribute should be removed.
static u8 __afl_area_initial[MAP_INITIAL_SIZE]  __attribute__((unused));;

extern __thread u16 __afl_prev_loc[NGRAM_SIZE_MAX];
extern __thread u16 __afl_prev_caller[CTX_MAX_K];
extern __thread u32 __afl_prev_ctx;

extern u8 *__afl_area_ptr;
extern u64 __afl_map_addr;

extern u8 *__afl_fuzz_ptr;
extern u32 *__afl_fuzz_len;


extern void compiler_rt_init(Genode::Env &env);

#endif //GENODE_COMPILER_RT_H
