#ifndef GENODE_AFL_STDLIB_H
#define GENODE_AFL_STDLIB_H

#include "afl-stddef.h"

extern void *realloc (void *_ptr, size_t _size);

extern void *malloc (size_t _size);

extern void free (void *_ptr);

extern void abort (void);

extern void exit (int _status);

/* Convert a string to an unsigned long integer.  */
extern unsigned long int strtoul (const char *__restrict _nptr, char **__restrict _endptr, int _base);


/* Return the value of envariable NAME, or NULL if it doesn't exist.  */
extern char *getenv (const char *__name);

/* Set NAME to VALUE in the environment. If REPLACE is nonzero, overwrite an existing value.  */
extern int setenv (const char *__name, const char *__value, int __replace);

/* Remove the variable NAME from the environment.  */
extern int unsetenv (const char *__name);

/* Convert a string to an integer.  */
extern int atoi (const char *__nptr);

/* Execute the given line as a shell command. */
extern int system (const char *__command);

/* Convert a string to a quadword integer.  */
extern long long int strtoll (const char *__restrict __nptr, char **__restrict __endptr, int __base);

/* Convert a string to an unsigned quadword integer.  */
extern unsigned long long int strtoull (const char *__restrict __nptr, char **__restrict __endptr, int __base);

/* Allocate NMEMB elements of SIZE bytes each, all initialized to 0.  */
extern void *calloc (size_t __nmemb, size_t __size);

/* These are the functions that actually do things.  The `random', `srandom',
   `initstate' and `setstate' functions are those from BSD Unices.
   The `rand' and `srand' functions are required by the ANSI standard.
   We provide both interfaces to the same random number generator.  */
/* Return a random long integer between 0 and 2^31-1 inclusive.  */
extern long int random (void);

/* Convert a string to a long integer.  */
extern long int strtol (const char *__restrict __nptr, char **__restrict __endptr, int __base);

/* Register a function to be called when `exit' is called.  */
extern int atexit (void (*__func) (void));

typedef int (*__compar_fn_t) (const void *, const void *);
/* Sort NMEMB elements of BASE, of SIZE bytes each, using COMPAR to perform the comparisons.  */
extern void qsort (void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar);

/* Nonzero if STATUS indicates termination by a signal.  */
#define __WIFSIGNALED(status) (((signed char) (((status) & 0x7f) + 1) >> 1) > 0)
# define WIFSIGNALED(status)	__WIFSIGNALED (status)

/* If WIFSIGNALED(STATUS), the terminating signal.  */
#define	__WTERMSIG(status)	((status) & 0x7f)
# define WTERMSIG(status)	__WTERMSIG (status)

/* Nonzero if STATUS indicates the child is stopped.  */
#define	__WIFSTOPPED(status)	(((status) & 0xff) == 0x7f)
# define WIFSTOPPED(status)	__WIFSTOPPED (status)

/* If WIFEXITED(STATUS), the low-order 8 bits of the status.  */
#define	__WEXITSTATUS(status)	(((status) & 0xff00) >> 8)
# define WEXITSTATUS(status)	__WEXITSTATUS (status)

#endif //GENODE_AFL_STDLIB_H