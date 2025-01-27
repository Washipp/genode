/* Afl-libc includes */
#include "stdio.h"
#include "sys/null.h"

/* Genode includes */
#include <base/log.h>
#include <base/sleep.h>

#define NOT_IMPLEMENTED Genode::log("'",__func__, "()' not implemented")

FILE *stdin = NULL;  /* Initialize standard input stream. */
FILE *stdout = NULL; /* Initialize standard output stream. */
FILE *stderr = NULL; /* Initialize standard error stream. */

FILE *fdopen(int fd, const char *modes)
{
    (void) fd;
    (void) modes;
    NOT_IMPLEMENTED;
    return stderr;
}

int fflush(FILE *stream)
{
    (void) stream;
    NOT_IMPLEMENTED;
    return 0;
}

char *fgets(char *__restrict s, int n, FILE *__restrict stream)
{
    (void) s;
    (void) n;
    (void) stream;
    NOT_IMPLEMENTED;
    return nullptr;
}

int printf(const char *__restrict format, ...)
{
    (void) format;
    return 0;
}

int fprintf(FILE *__restrict stream, const char *__restrict format, ...)
{
    (void) format;
    (void) stream;
    NOT_IMPLEMENTED;
    return 0;
}

int snprintf(char *__restrict s, size_t maxlen, const char *__restrict format, ...)
{
    (void) s;
    (void) maxlen;
    (void) format;
    NOT_IMPLEMENTED;
    return 0;
}

//int sprintf(char *__restrict s, const char *__restrict format, ...) {
//    (void) s;
//    (void)format;
//    NOT_IMPLEMENTED;
//    return 0;
//}

int sscanf(const char *__restrict s, const char *__restrict format, ...)
{
    (void) s;
    (void) format;
    NOT_IMPLEMENTED;
    return 0;
}

int fscanf(FILE *__restrict s, const char *__restrict format, ...)
{
    (void) s;
    (void) format;
    NOT_IMPLEMENTED;
    return 0;
}

int rename(const char *o, const char *n)
{
    (void) o;
    (void) n;
    NOT_IMPLEMENTED;
    return 0;
}

int fseek(FILE *stream, long int off, int whence)
{
    (void) stream;
    (void) off;
    (void) whence;
    NOT_IMPLEMENTED;
    return 0;
}


int fclose(FILE *stream)
{
    (void) stream;
    return 0;
}

FILE *fopen(const char *__restrict filename, const char *__restrict modes)
{
    (void) filename;
    (void) modes;
    return 0;
}


void perror(const char *s)
{
    (void) s;
    NOT_IMPLEMENTED;
    Genode::sleep_forever();
}

/* Write chunks of generic data to STREAM. */
size_t fwrite(const void *__restrict ptr, size_t size, size_t n, FILE *__restrict s)
{
    (void) ptr;
    (void) size;
    (void) n;
    (void) s;
    NOT_IMPLEMENTED;
    return 0;
}