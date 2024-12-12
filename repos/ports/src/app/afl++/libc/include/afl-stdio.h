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
extern "C" FILE *stdin;        /* Standard input stream.  */
extern "C" FILE *stdout;        /* Standard output stream.  */
extern "C" FILE *stderr;        /* Standard error output stream.  */
/* C89/C99 say they're macros.  Make them happy.  */
#define stdin stdin
#define stdout stdout
#define stderr stderr


/* Create a new stream that refers to an existing system file descriptor.  */
extern "C" FILE *fdopen(int fd, const char *modes);

/* Flush STREAM, or all streams if STREAM is NULL.*/
extern "C" int fflush(FILE *stream);

/* Get a newline-terminated string of finite length from STREAM. */
extern "C" char *fgets(char *__restrict s, int n, FILE *__restrict stream);

extern "C" int printf(const char *__restrict format, ...);

/* Write formatted output to STREAM.*/
extern "C" int fprintf(FILE *__restrict stream, const char *__restrict format, ...);

extern "C" int snprintf(char *__restrict s, size_t maxlen, const char *__restrict format, ...);

extern "C" int sprintf(char *__restrict s, const char *__restrict format, ...);

/* Read formatted input from S.  */
extern "C" int sscanf(const char *__restrict s, const char *__restrict format, ...);

/* Read formatted input from STREAM.*/
extern "C" int fscanf(FILE *__restrict stream, const char *__restrict format, ...);

/* Rename file OLD to NEW.  */
extern "C" int rename(const char *_old, const char *_new);

/* Seek to a certain position on STREAM */
extern "C" int fseek(FILE *stream, long int off, int whence);

/* Close STREAM. */
extern "C" int fclose(FILE *stream);

/* Open a file and create a new stream for it. */
extern "C" FILE *fopen(const char *__restrict filename, const char *__restrict modes);

/* Print a message describing the meaning of the value of errno. */
extern "C" void perror (const char *s);

#endif //GENODE_AFL_STDIO_H
