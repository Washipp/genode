#ifndef GENODE_AFL_STDIO_H
#define GENODE_AFL_STDIO_H

#include "sys/afl-null.h"
#include "afl-stddef.h"

#ifndef __FILE_defined
#define __FILE_defined 1
struct _IO_FILE {};
typedef struct _IO_FILE FILE;
#endif

#define SEEK_SET    0    /* Seek from beginning of file.  */
#define SEEK_END    2    /* Seek from end of file.  */

/* Standard streams.  */
extern FILE *stdin;        /* Standard input stream.  */
extern FILE *stdout;       /* Standard output stream.  */
extern FILE *stderr;       /* Standard error output stream.  */

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new stream that refers to an existing system file descriptor.  */
FILE *fdopen(int fd, const char *modes);

/* Flush STREAM, or all streams if STREAM is NULL.*/
int fflush(FILE *stream);

/* Get a newline-terminated string of finite length from STREAM. */
char *fgets(char *__restrict s, int n, FILE *__restrict stream);

int printf(const char *__restrict format, ...);

/* Write formatted output to STREAM.*/
int fprintf(FILE *__restrict stream, const char *__restrict format, ...);

int snprintf(char *__restrict s, size_t maxlen, const char *__restrict format, ...);

int sprintf(char *__restrict s, const char *__restrict format, ...);

/* Read formatted input from S.  */
int sscanf(const char *__restrict s, const char *__restrict format, ...);

/* Read formatted input from STREAM.*/
int fscanf(FILE *__restrict stream, const char *__restrict format, ...);

/* Rename file OLD to NEW.  */
int rename(const char *_old, const char *_new);

/* Seek to a certain position on STREAM */
int fseek(FILE *stream, long int off, int whence);

/* Close STREAM. */
int fclose(FILE *stream);

/* Open a file and create a new stream for it. */
FILE *fopen(const char *__restrict filename, const char *__restrict modes);

/* Print a message describing the meaning of the value of errno. */
void perror(const char *s);

/* Write chunks of generic data to STREAM. */
size_t fwrite(const void *__restrict ptr, size_t size, size_t n, FILE *__restrict s);

#ifdef __cplusplus
}
#endif

#endif //GENODE_AFL_STDIO_H
