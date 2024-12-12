#ifndef GENODE_AFL_STRING_H
#define GENODE_AFL_STRING_H

#include "afl-stddef.h"

void lx_emul_trace(const char *s);

/* Return the length of S.  */
extern size_t strlen (const char *s);

/* Find the length of STRING, but scan at most MAXLEN characters.
   If no '\0' terminator is found in that many characters, return MAXLEN.  */
extern size_t strnlen (const char *string, size_t maxlen);

/* Compare N bytes of S1 and S2.  */
extern int memcmp (const void *s1, const void *s2, size_t n);

/* Copy SRC to DEST.  */
extern char *strcpy (char *__restrict dest, const char *__restrict src);

/* Copy no more than N characters of SRC to DEST.  */
extern char *strncpy (char *__restrict dest, const char *__restrict src, size_t n);

/* Return the length of the initial segment of S which consists entirely of characters not in REJECT.  */
extern size_t strcspn (const char *s, const char *reject);

/* Find the first occurrence of NEEDLE in HAYSTACK.  */
extern char *strstr (const char *haystack, const char *needle);

/* Append SRC onto DEST.  */
extern char *strcat (char *__restrict dest, const char *__restrict src);

/* Find the first occurrence of C in S.  */
extern char *strchr (const char *_s, int _c);

/* Return a string describing the meaning of the `errno' code in ERRNUM.  */
extern char *strerror (int errnum);

/* Compare N characters of S1 and S2.  */
extern int strncmp (const char *s1, const char *s2, size_t n);

/* Append no more than N characters from SRC onto DEST.  */
extern char *strncat (char *__restrict dest, const char *__restrict src, size_t n);

/* Copy N bytes of SRC to DEST, guaranteeing correct behavior for overlapping strings.  */
extern void *memmove (void *dest, const void *src, size_t n);

/* Find the last occurrence of C in S.  */
extern char *strrchr (const char *s, int c);

/* Compare S1 and S2.  */
extern int strcmp (const char *s1, const char *s2);

/* Return the next DELIM-delimited token from *STRINGP, terminating it with a '\0', and update *STRINGP to point past it.  */
extern char *strsep (char **__restrict stringp, const char *__restrict delim);

/* Duplicate S, returning an identical malloc'd string.  */
extern char *strdup (const char *s);

/* Search N bytes of S for C.  */
extern void *memchr (const void *s, int c, size_t n);

extern void *memcpy(void *a, const void *b, size_t c);
extern void *memset(void *a, int b, size_t c);

#endif //GENODE_AFL_STRING_H
