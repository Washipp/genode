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

typedef int (*Function_to_fuzz)(void*, unsigned int *);

extern int call_function(Function_to_fuzz function_to_fuzz);

u8 *get_fuzz_ptr();
u32 *get_fuzz_len();
extern void compiler_rt_init(Genode::Env &env);

#endif //GENODE_COMPILER_RT_H
