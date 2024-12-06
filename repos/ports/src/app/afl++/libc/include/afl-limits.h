#ifndef GENODE_AFL_LIMITS_H
#define GENODE_AFL_LIMITS_H

#define PATH_MAX        4096	/* # chars in a path name including nul */
#define NAME_MAX         255	/* # chars in a file name */


# ifndef LLONG_MIN
#  define LLONG_MIN	(-LLONG_MAX-1)
# endif
# ifndef LLONG_MAX
#  define LLONG_MAX	__LONG_LONG_MAX__
# endif
# ifndef ULLONG_MAX
#  define ULLONG_MAX	(LLONG_MAX * 2ULL + 1)
# endif

#define INT_MAX __INT_MAX__
#define UINT_MAX (INT_MAX * 2U + 1U)

#endif //GENODE_AFL_LIMITS_H
