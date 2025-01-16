#include "stdlib.h"
#include "ctype.h"
#include "sys/null.h"
#include "limits.h"
#include "errno.h"

#include <base/log.h>
#include <base/sleep.h>

#define NOT_IMPLEMENTED Genode::log(__func__, " not implemented")

#define RAND_MAD 32767
static unsigned long next = 1;

void *realloc(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    NOT_IMPLEMENTED;
    return nullptr;
}

void *malloc(size_t size) {
    (void)size;
    NOT_IMPLEMENTED;
    return nullptr;
}

void *calloc(size_t nmemb, size_t size) {
    (void)nmemb;
    (void)size;
    NOT_IMPLEMENTED;
    return nullptr;
}

void free(void *ptr) {
    (void)ptr;
    NOT_IMPLEMENTED;
}

void abort(void) {
    NOT_IMPLEMENTED;
    Genode::sleep_forever();
}

void exit(int status) {
    (void)status;
    NOT_IMPLEMENTED;
    Genode::sleep_forever();
}

int atexit(void (*func)(void)) {
    (void)func;
    NOT_IMPLEMENTED;
    return 0;
}

char *getenv(const char *name) {
    Genode::log("getenv called: ", name);
    return nullptr;
}

int setenv(const char *name, const char *value, int replace) {
    (void)value;
    (void)replace;
    Genode::log("setenv called: name:", name);
    return 0;
}

int unsetenv(const char *name){
    Genode::log("unsetenv called: name:", name);
    return 0;
}

int atoi(const char *str) {
    return (int)strtol(str, NULL, 10);
}

/* The errno are not set. (libc implementation) */
long int strtol(const char *__restrict nptr, char **__restrict endptr, int base) {
    const char *s;
    unsigned long acc;
    char c;
    unsigned long cutoff;
    int neg, any, cutlim;

    /*
     * Skip white space and pick up leading +/- sign if any.
     * If base is 0, allow 0x for hex and 0 for octal, else
     * assume decimal; if base is already 16, allow 0x.
     */
    s = nptr;
    do {
        c = *s++;
    } while (isspace((unsigned char)c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else {
        neg = 0;
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X') &&
        ((s[1] >= '0' && s[1] <= '9') ||
         (s[1] >= 'A' && s[1] <= 'F') ||
         (s[1] >= 'a' && s[1] <= 'f'))) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    acc = any = 0;
    if (base < 2 || base > 36)
        goto noconv;

    /*
     * Compute the cutoff value between legal numbers and illegal
     * numbers.  That is the largest legal value, divided by the
     * base.  An input number that is greater than this value, if
     * followed by a legal input character, is too big.  One that
     * is equal to this value may be valid or not; the limit
     * between valid and invalid numbers is then based on the last
     * digit.  For instance, if the range for longs is
     * [-2147483648..2147483647] and the input base is 10,
     * cutoff will be set to 214748364 and cutlim to either
     * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
     * a value > 214748364, or equal but the next digit is > 7 (or 8),
     * the number is too big, and we will return a range error.
     *
     * Set 'any' if any `digits' consumed; make it negative to indicate
     * overflow.
     */
    cutoff = neg ? (unsigned long)-(LONG_MIN + LONG_MAX) + LONG_MAX : LONG_MAX;
    cutlim = (int)cutoff % base;
    cutoff /= base;
    for ( ; ; c = *s++) {
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? LONG_MIN : LONG_MAX;
        errno = ERANGE;
    } else if (!any) {
        noconv:
        errno = EINVAL;
    } else if (neg)
        acc = -acc;
    if (endptr != NULL)
        *endptr = (char *)(any ? s - 1 : nptr);
    return (long)(acc);
}

/* The errno are not set. (libc implementation) */
long long int strtoll(const char *__restrict nptr, char **__restrict endptr, int base) {

    const char *s;
    unsigned long long acc;
    char c;
    unsigned long long cutoff;
    int neg, any, cutlim;

    /*
     * See strtoq for comments as to the logic used.
     */
    s = nptr;
    do {
        c = *s++;
    } while (isspace((unsigned char)c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else {
        neg = 0;
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X') &&
        ((s[1] >= '0' && s[1] <= '9') ||
         (s[1] >= 'A' && s[1] <= 'F') ||
         (s[1] >= 'a' && s[1] <= 'f'))) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    acc = any = 0;
    if (base < 2 || base > 36)
        goto noconv;

    cutoff = ULLONG_MAX / base;
    cutlim = (int) ULLONG_MAX % base;
    for ( ; ; c = *s++) {
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = ULLONG_MAX;
        errno = ERANGE;
    } else if (!any) {
        noconv:
        errno = EINVAL;
    } else if (neg)
        acc = -acc;
    if (endptr != NULL)
        *endptr = (char *)(any ? s - 1 : nptr);
    return (long long)(acc);
}

/* The errno are not set. (libc implementation) */
unsigned long int strtoul(const char *__restrict nptr, char **__restrict endptr, int base) {
    const char *s;
    unsigned long acc;
    char c;
    unsigned long cutoff;
    int neg, any, cutlim;

    /*
     * See strtol for comments as to the logic used.
     */
    s = nptr;
    do {
        c = *s++;
    } while (isspace((unsigned char)c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else {
        neg = 0;
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X') &&
        ((s[1] >= '0' && s[1] <= '9') ||
         (s[1] >= 'A' && s[1] <= 'F') ||
         (s[1] >= 'a' && s[1] <= 'f'))) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    acc = any = 0;
    if (base < 2 || base > 36)
        goto noconv;

    cutoff = ULONG_MAX / base;
    cutlim = (int) ULONG_MAX % base;
    for ( ; ; c = *s++) {
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = ULONG_MAX;
        errno = ERANGE;
    } else if (!any) {
        noconv:
        errno = EINVAL;
    } else if (neg)
        acc = -acc;
    if (endptr != NULL)
        *endptr = (char *)(any ? s - 1 : nptr);
    return (acc);
}

/* The errno are not set. (libc implementation)*/
unsigned long long int strtoull(const char *__restrict nptr, char **__restrict endptr, int base) {

    const char *s;
    unsigned long long acc;
    char c;
    unsigned long long cutoff;
    int neg, any, cutlim;

    /*
     * Skip white space and pick up leading +/- sign if any.
     * If base is 0, allow 0x for hex and 0 for octal, else
     * assume decimal; if base is already 16, allow 0x.
     */
    s = nptr;
    do {
        c = *s++;
    } while (isspace((unsigned char)c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else {
        neg = 0;
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X') &&
        ((s[1] >= '0' && s[1] <= '9') ||
         (s[1] >= 'A' && s[1] <= 'F') ||
         (s[1] >= 'a' && s[1] <= 'f'))) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    acc = any = 0;
    if (base < 2 || base > 36)
        goto noconv;

    /*
     * Compute the cutoff value between legal numbers and illegal
     * numbers.  That is the largest legal value, divided by the
     * base.  An input number that is greater than this value, if
     * followed by a legal input character, is too big.  One that
     * is equal to this value may be valid or not; the limit
     * between valid and invalid numbers is then based on the last
     * digit.  For instance, if the range for quads is
     * [-9223372036854775808..9223372036854775807] and the input base
     * is 10, cutoff will be set to 922337203685477580 and cutlim to
     * either 7 (neg==0) or 8 (neg==1), meaning that if we have
     * accumulated a value > 922337203685477580, or equal but the
     * next digit is > 7 (or 8), the number is too big, and we will
     * return a range error.
     *
     * Set 'any' if any `digits' consumed; make it negative to indicate
     * overflow.
     */
    cutoff = neg ? (unsigned long long)-(LLONG_MIN + LLONG_MAX) + LLONG_MAX
                 : LLONG_MAX;
    cutlim = (int)cutoff % base;
    cutoff /= base;
    for ( ; ; c = *s++) {
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? LLONG_MIN : LLONG_MAX;
        errno = ERANGE;
    } else if (!any) {
        noconv:
        errno = EINVAL;
    } else if (neg)
        acc = -acc;
    if (endptr != NULL)
        *endptr = (char *)(any ? s - 1 : nptr);
    return (acc);
}

long int random(void) {
    return 0;
}

/* Example implementation from POSIX.1-2001. */
int rand(void) {
    next = next * 1103515245 + 12345;
    return (int) ((next / 65536) % RAND_MAD);
}
