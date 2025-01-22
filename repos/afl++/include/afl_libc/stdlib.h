#ifndef AFL_STDLIB_H
#define AFL_STDLIB_H

#include "stddef.h"
#include <base/allocator.h>

/* Nonzero if STATUS indicates termination by a signal.  */
#ifndef WIFSIGNALED
# define WIFSIGNALED(status)    (((signed char) (((status) & 0x7f) + 1) >> 1) > 0)
#endif

/* If WIFSIGNALED(STATUS), the terminating signal.  */
#ifndef WIFSIGNALED
# define WTERMSIG(status)    ((status) & 0x7f)
#endif

/* Nonzero if STATUS indicates the child is stopped.  */
#ifndef WIFSTOPPED
# define WIFSTOPPED(status)    (((status) & 0xff) == 0x7f)
#endif

/* If WIFEXITED(STATUS), the low-order 8 bits of the status.  */
#ifndef WEXITSTATUS
# define WEXITSTATUS(status)    (((status) & 0xff00) >> 8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize malloc/free
 */
void afl_libc_init(Genode::Allocator &);

/* Re-allocate the previously allocated block
in PTR, making the new block SIZE bytes long. */
void *realloc(void *ptr, size_t size);

/* Allocate SIZE bytes of memory.  */
void *malloc(size_t size);

/* Allocate NMEMB elements of SIZE bytes each, all initialized to 0.  */
void *calloc(size_t nmemb, size_t size);

/* Free a block allocated by `malloc', `realloc' or `calloc'.  */
void free(void *ptr);

/* Abort execution and generate a core-dump.  */
void abort(void);

/* Call all functions registered with `atexit' and `on_exit',
   in the reverse of the order in which they were registered,
   perform stdio cleanup, and terminate program execution with STATUS.  */
void exit(int _status);

/* Register a function to be called when `exit` is called.  */
int atexit(void (*func)(void));

/* Convert a string to a long integer.  */
long int strtol(const char *__restrict nptr, char **__restrict endptr, int base);

/* Convert a string to an unsigned long integer.  */
unsigned long int strtoul(const char *__restrict nptr, char **__restrict endptr, int base);

/* Convert a string to a quadword integer.  */
long long int strtoll(const char *__restrict nptr, char **__restrict endptr, int base);

/* Convert a string to an unsigned quadword integer.  */
unsigned long long int strtoull(const char *__restrict nptr, char **__restrict endptr, int base);

/* Return the value of envariable NAME, or NULL if it doesn't exist.  */
char *getenv(const char *name);

/* Set NAME to VALUE in the environment. If REPLACE is nonzero, overwrite an existing value.  */
int setenv(const char *name, const char *value, int replace);

/* Remove the variable NAME from the environment.  */
int unsetenv(const char *name);

/* Convert a string to an integer.  */
int atoi(const char *nptr);

// Used by afl-fuzz-bitmap.c to execute `afl.infoexec` (-I flag). This has been patched away
/* Execute the given line as a shell command. */
//int system(const char *__command);

/* These are the functions that actually do things.  The `random', `srandom',
   `initstate' and `setstate' functions are those from BSD Unices.
   The `rand' and `srand' functions are required by the ANSI standard.
   We provide both interfaces to the same random number generator.  */
/* Return a random long integer between 0 and 2^31-1 inclusive.  */
long int random(void);

/* The rand() function returns a pseudo-random integer in the range 0 to RAND_MAX inclusive*/
int rand(void);

// Used by afl-fuzz-extras.cc. There is, for now, no support for extras/custom mutators.
/* Sort NMEMB elements of BASE, of SIZE bytes each, using COMPAR to perform the comparisons.  */
//void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar);

#ifdef __cplusplus
}
#endif
    
#endif //AFL_STDLIB_H
