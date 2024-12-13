#ifndef GENODE_AFL_STRING_H
#define GENODE_AFL_STRING_H

#include "afl-stddef.h"

/* Return the length of S.  */
extern "C" size_t strlen(const char *s);

/* Find the length of STRING, but scan at most MAXLEN characters.
   If no '\0' terminator is found in that many characters, return MAXLEN.  */
extern "C" size_t strnlen(const char *s, size_t maxlen);

/* Compare N bytes of S1 and S2.  */
extern "C" int memcmp(const void *s1, const void *s2, size_t n);

/* Copy SRC to DEST.  */
extern "C" char *strcpy(char *__restrict dest, const char *__restrict src);

/* Copy no more than N characters of SRC to DEST.  */
extern "C" char *strncpy(char *__restrict dest, const char *__restrict src, size_t n);

/* Return the length of the initial segment of S which consists entirely of characters not in REJECT.  */
extern "C" size_t strcspn(const char *s, const char *reject);

/* Find the first occurrence of NEEDLE in HAYSTACK.  */
extern "C" char *strstr(const char *haystack, const char *needle);

/* Append SRC onto DEST.  */
extern "C" char *strcat(char *__restrict dest, const char *__restrict src);

/* Find the first occurrence of C in S.  */
extern "C" char *strchr(const char *_s, int _c);

/* Return a string describing the meaning of the `errno' code in ERRNUM.  */
extern "C" char *strerror(int errnum);

/* Compare N characters of S1 and S2.  */
extern "C" int strncmp(const char *s1, const char *s2, size_t n);

/* Append no more than N characters from SRC onto DEST.  */
extern "C" char *strncat(char *__restrict dest, const char *__restrict src, size_t n);

/* Copy N bytes of SRC to DEST, guaranteeing correct behavior for overlapping strings.  */
extern "C" void *memmove(void *dest, const void *src, size_t n);

/* Find the last occurrence of C in S.  */
extern "C" char *strrchr(const char *s, int c);

/* Compare S1 and S2.  */
extern "C" int strcmp(const char *s1, const char *s2);

/* Return the next DELIM-delimited token from *STRINGP, terminating it with a '\0', and update *STRINGP to point past it.  */
extern "C" char *strsep(char **__restrict stringp, const char *__restrict delim);

/* Duplicate S, returning an identical malloc'd string.  */
extern "C" char *strdup(const char *s);

/* Search N bytes of S for C.  */
extern "C" void *memchr(const void *s, int c, size_t n);

extern "C" void *memcpy(void *a, const void *b, size_t c);

extern "C" void *memset(void *a, int b, size_t c);

#endif //GENODE_AFL_STRING_H
