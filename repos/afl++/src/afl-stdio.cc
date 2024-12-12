#include "afl-stdio.h"

/* Create a new stream that refers to an existing system file descriptor.  */
FILE *fdopen(int fd, const char *__modes);

/* Flush STREAM, or all streams if STREAM is NULL.*/
int fflush(FILE *__stream);

/* Get a newline-terminated string of finite length from STREAM. */
char *fgets(char *__restrict s, int n, FILE *__restrict stream);

int printf(const char *__restrict format, ...) {
    (void)format;
    return 0;
}

/* Write formatted output to STREAM.*/
int fprintf(FILE *__restrict stream, const char *__restrict format, ...) {
    (void)format;
    (void)stream;
    return 0;
}

int snprintf(char *__restrict s, size_t maxlen, const char *__restrict format, ...) {
    (void)s;
    (void)maxlen;
    (void)format;
    return 0;
}

int sprintf(char *__restrict s, const char *__restrict format, ...);

/* Read formatted input from S.  */
int sscanf(const char *__restrict s, const char *__restrict format, ...);

/* Read formatted input from STREAM.*/
int fscanf(FILE *__restrict stream, const char *__restrict format, ...);

/* Rename file OLD to NEW.  */
int rename(const char *__old, const char *__new);

/* Seek to a certain position on STREAM */
int fseek(FILE *stream, long int off, int whence);

/* Close STREAM. */
int fclose(FILE *stream) {
    (void)stream;
    return 0;
}

/* Open a file and create a new stream for it. */
FILE *fopen(const char *__restrict filename, const char *__restrict modes) {
    (void)filename;
    (void) modes;
    return 0;
}