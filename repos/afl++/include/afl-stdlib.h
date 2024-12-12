#ifndef GENODE_AFL_STDLIB_H
#define GENODE_AFL_STDLIB_H

#include "afl-stddef.h"

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

extern "C" void *realloc(void *ptr, size_t size);

extern "C" void *malloc(size_t size);

/* Allocate NMEMB elements of SIZE bytes each, all initialized to 0.  */
extern "C" void *calloc(size_t nmemb, size_t size);

extern "C" void free(void *ptr);

extern "C" void abort(void);

extern "C" void exit(int _status);

/* Register a function to be called when `exit` is called.  */
extern "C" int atexit(void (*func)(void));

/* Convert a string to a long integer.  */
extern "C" long int strtol(const char *__restrict nptr, char **__restrict endptr, int base);

/* Convert a string to an unsigned long integer.  */
extern "C" unsigned long int strtoul(const char *__restrict nptr, char **__restrict endptr, int base);

/* Convert a string to a quadword integer.  */
extern "C" long long int strtoll(const char *__restrict nptr, char **__restrict endptr, int base);

/* Convert a string to an unsigned quadword integer.  */
extern "C" unsigned long long int strtoull(const char *__restrict nptr, char **__restrict endptr, int base);

/* Return the value of envariable NAME, or NULL if it doesn't exist.  */
extern "C" char *getenv(const char *name);

/* Set NAME to VALUE in the environment. If REPLACE is nonzero, overwrite an existing value.  */
extern "C" int setenv(const char *name, const char *value, int replace);

/* Remove the variable NAME from the environment.  */
extern "C" int unsetenv(const char *name);

/* Convert a string to an integer.  */
extern "C" int atoi(const char *nptr);

// Used by afl-fuzz-bitmap.c to execute `afl.infoexec` (-I flag). This has been patched away
/* Execute the given line as a shell command. */
//extern "C" int system(const char *__command);

/* These are the functions that actually do things.  The `random', `srandom',
   `initstate' and `setstate' functions are those from BSD Unices.
   The `rand' and `srand' functions are required by the ANSI standard.
   We provide both interfaces to the same random number generator.  */
/* Return a random long integer between 0 and 2^31-1 inclusive.  */
extern "C" long int random(void);

/* The rand() function returns a pseudo-random integer in the range 0 to RAND_MAX inclusive*/
extern "C" int rand(void);

// Used by afl-fuzz-extras.cc. There is, for now, no support for extras/custom mutators.
/* Sort NMEMB elements of BASE, of SIZE bytes each, using COMPAR to perform the comparisons.  */
//extern "C" void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar);


#endif //GENODE_AFL_STDLIB_H