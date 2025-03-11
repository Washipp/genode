#ifndef GENODE_COMPILER_RT_H
#define GENODE_COMPILER_RT_H

/* Genode includes*/
#include <base/component.h>

typedef int (*Function_to_fuzz)(void*);

extern int call_function(Function_to_fuzz function_to_fuzz);

extern void compiler_rt_init(Genode::Env &env);

#endif //GENODE_COMPILER_RT_H
