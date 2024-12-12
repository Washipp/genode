#include "afl-string.h"
#include "afl-stddef.h"
#include "afl-limits.h"
#include "afl-stdio.h"
#include "sys/afl-null.h"
#include <util/string.h>
#include <base/log.h>

#define NOT_IMPLEMENTED Genode::log(__func__, " not implemented")

/* Return the length of S.  */
size_t strlen(const char *s) {
    return Genode::strlen(s);
}

/* Compare N bytes of S1 and S2.  */
int memcmp(const void *s1, const void *s2, size_t n) {
    return Genode::memcmp(s1, s2, n);
}

/* Copy SRC to DEST.  */
char *strcpy(char *__restrict dest, const char *__restrict src) {
    Genode::copy_cstring(dest, src, strlen(src) + 1);

    return dest;
}

/* Copy no more than N characters of SRC to DEST.  */
char *strncpy(char *__restrict dest, const char *__restrict src, size_t _n) {
    if (_n == 0) return nullptr;
    while ((_n > 1UL) && *src)
        *dest++ = *src++;
    return dest;
}

/* Return the length of the initial segment of S which consists entirely of characters not in charset.  */
size_t strcspn(const char *s, const char *charset) {(void)s;(void)charset;
    NOT_IMPLEMENTED;
    return 0;
}

/* Find the first occurrence of NEEDLE in HAYSTACK.  */
char *strstr(const char *haystack, const char *needle) {(void)haystack;(void)needle;
    NOT_IMPLEMENTED;
    return 0;
}

/* Append SRC onto DEST. (libc implementation)  */
char *strcat(char *__restrict dest, const char *__restrict src) {
    char *save = dest;

    for (; *dest; ++dest);
    while ((*dest++ = *src++));
    return (save);
}

/* Return a string describing the meaning of the `errno' code in ERRNUM.  */
char *strerror(int errnum) {
    static char ebuf[NL_TEXTMAX];
    snprintf(ebuf, NL_TEXTMAX, "Unknown error number: %d", errnum);
    return ebuf;
}

/* Compare N characters of S1 and S2.  */
int strncmp(const char *s1, const char *s2, size_t n) {
    return Genode::strcmp(s1, s2, n);
}

/* Compare S1 and S2.  */
int strcmp(const char *s1, const char *s2) {
    return Genode::strcmp(s1, s2);
}

/* Append no more than N characters from SRC onto DEST. (libc implementation)  */
char *strncat(char *__restrict dst, const char *__restrict src, size_t n) {
    if (n != 0) {
        char *d = dst;
        const char *s = src;

        while (*d != 0)
            d++;
        do {
            if ((*d = *s++) == 0)
                break;
            d++;
        } while (--n != 0);
        *d = 0;
    }
    return (dst);
}

/* Copy N bytes of SRC to DEST, guaranteeing correct behavior for overlapping strings.  */
void *memmove(void *dest, const void *src, size_t n) {
    return Genode::memmove(dest, src, n);
}

/* Find the first occurrence of CH in P. (libc implementation)
 * Else, see libc.cc form the gcov implementation. */
char *strchr(const char *p, int ch){
    char c;

    c = (char)ch;
    for (;; ++p) {
        if (*p == c)
            return ((char *)p);
        if (*p == '\0')
            return (NULL);
    }
}

/* Find the last occurrence of CH in P. (libc implementation) */
char *strrchr(const char *p, int ch) {
    char *save;
    char c;

    c = (char)ch;
    for (save = NULL;; ++p) {
        if (*p == c)
            save = (char *)p;
        if (*p == '\0')
            return (save);
    }
}


/* Return the next DELIM-delimited token from *STRINGP, terminating it with a '\0',
 * and update *STRINGP to point past it. (libc implementation) */
char *strsep(char **__restrict stringp, const char *__restrict delim) {
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;

    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;) {
        c = (unsigned char)*s++;
        spanp = delim;
        do {
            if ((sc = (unsigned char)*spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
}

/* Duplicate S, returning an identical malloc'd string.  */
char *strdup(const char *str) {(void)str;
    // TODO Implement malloc/port it somehow from malloc_free.cc
//    size_t len;
//    char *copy;
//
//    len = strlen(str) + 1;
//    if ((copy = malloc(len)) == NULL)
//        return (NULL);
//    Genode::memcpy(copy, str, len);
//    return (copy);
    NOT_IMPLEMENTED;
    return nullptr;
}

/* Search N bytes of S for C. (libc implementation) */
void *memchr(const void *s, int c, size_t n) {
    if (n != 0) {
        const auto *p = static_cast<const unsigned char *>(s);

        do {
            if (*p++ == (unsigned char)c)
                return ((void *)(p - 1));
        } while (--n != 0);
    }
    return (NULL);
}

void *memcpy(void *a, const void *b, size_t c) {
    return Genode::memcpy(a, b, c);
}

void *memset(void *a, int b, size_t c) {
    return Genode::memset(a, (Genode::uint8_t) b, c);
}
