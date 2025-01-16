#ifndef GENODE_LIMITS_H
#define GENODE_LIMITS_H

#define PATH_MAX        4096	/* # chars in a path name including nul */
#define	NL_TEXTMAX		2048
#define NAME_MAX         255	/* # chars in a file name */

#  if __WORDSIZE == 64
#   define LONG_MAX	9223372036854775807L
#  else
#   define LONG_MAX	2147483647L
#  endif
#  define LONG_MIN	(-LONG_MAX - 1L)
# ifndef LLONG_MAX
#  define LLONG_MAX	__LONG_LONG_MAX__
# endif
# ifndef ULLONG_MAX
#  define ULLONG_MAX	(LLONG_MAX * 2ULL + 1)
# endif
# ifndef LLONG_MIN
#  define LLONG_MIN	(-LLONG_MAX-1)
# endif

/* Maximum value an `unsigned long int' can hold.  (Minimum is 0.)  */
#  if __WORDSIZE == 64
#   define ULONG_MAX	18446744073709551615UL
#  else
#   define ULONG_MAX	4294967295UL
#  endif

#define INT_MAX __INT_MAX__
#define UINT_MAX (INT_MAX * 2U + 1U)

#endif //GENODE_LIMITS_H
