#ifndef GENODE_AFL_STRING_H
#define GENODE_AFL_STRING_H

#include "afl-stddef.h"

void lx_emul_trace(const char *s);

/* Return the length of S.  */
extern size_t strlen (const char *_s);

/* Compare N bytes of S1 and S2.  */
extern int memcmp (const void *_s1, const void *_s2, size_t _n);

/* Copy SRC to DEST.  */
extern char *strcpy (char *__restrict _dest, const char *__restrict _src);

/* Copy no more than N characters of SRC to DEST.  */
extern char *strncpy (char *__restrict _dest, const char *__restrict _src, size_t _n);

/* Return the length of the initial segment of S which consists entirely of characters not in REJECT.  */
extern size_t strcspn (const char *_s, const char *_reject);

/* Find the first occurrence of NEEDLE in HAYSTACK.  */
extern char *strstr (const char *_haystack, const char *_needle);

/* Append SRC onto DEST.  */
extern char *strcat (char *__restrict __dest, const char *__restrict __src);

/* Find the first occurrence of C in S.  */
extern char *strchr (const char *_s, int _c);

/* Return a string describing the meaning of the `errno' code in ERRNUM.  */
extern char *strerror (int __errnum);

/* Compare N characters of S1 and S2.  */
extern int strncmp (const char *s1, const char *s2, size_t n);

/* Append no more than N characters from SRC onto DEST.  */
extern char *strncat (char *__restrict __dest, const char *__restrict __src, size_t __n);

/* Copy N bytes of SRC to DEST, guaranteeing correct behavior for overlapping strings.  */
extern void *memmove (void *dest, const void *src, size_t n);

/* Find the last occurrence of C in S.  */
extern char *strrchr (const char *__s, int __c);

/* Compare S1 and S2.  */
extern int strcmp (const char *__s1, const char *__s2);

/* Return the next DELIM-delimited token from *STRINGP, terminating it with a '\0', and update *STRINGP to point past it.  */
extern char *strsep (char **__restrict __stringp, const char *__restrict __delim);

/* Duplicate S, returning an identical malloc'd string.  */
extern char *strdup (const char *__s);

/* Search N bytes of S for C.  */
extern void *memchr (const void *__s, int __c, size_t __n);

extern void *memcpy(void *__a, const void *__b, size_t __c);
extern void *memset(void *__a, int __b, size_t __c);

#endif //GENODE_AFL_STRING_H
