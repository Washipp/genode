/*
 * \brief  Init to setup Genode requirements for afl-libc
 * \author Silas Meier
 * \date   2025-01-23
 */

#ifndef AFL_INIT_H
#define AFL_INIT_H

#include <base/allocator.h>
#include <base/env.h>

/**
 * Initialize the allocator for malloc/free
 */
void afl_libc_init(Genode::Allocator &);

/**
 * Initialize the file system.
 */
void afl_fs_env_init(Genode::Env &env);

#endif //AFL_INIT_H
