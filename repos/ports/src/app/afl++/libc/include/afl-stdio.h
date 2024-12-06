#ifndef GENODE_AFL_STDIO_H
#define GENODE_AFL_STDIO_H

#include "sys/afl-null.h"
#include "afl-stddef.h"

#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_END	2	/* Seek from end of file.  */

/* Standard streams.  */
extern FILE *stdin;		/* Standard input stream.  */
extern FILE *stdout;		/* Standard output stream.  */
extern FILE *stderr;		/* Standard error output stream.  */
/* C89/C99 say they're macros.  Make them happy.  */
#define stdin stdin
#define stdout stdout
#define stderr stderr


/* Create a new stream that refers to an existing system file descriptor.  */
extern FILE *fdopen (int __fd, const char *__modes);

/* Flush STREAM, or all streams if STREAM is NULL.*/
extern int fflush (FILE *__stream);

/* Get a newline-terminated string of finite length from STREAM. */
extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream);

extern int printf (const char *__restrict __format, ...);
/* Write formatted output to STREAM.*/
extern int fprintf (FILE *__restrict __stream, const char *__restrict __format, ...);
extern int snprintf (char *__restrict __s, size_t __maxlen, const char *__restrict __format, ...);
extern int sprintf (char *__restrict __s, const char *__restrict __format, ...);
/* Read formatted input from S.  */
extern int sscanf (const char *__restrict __s, const char *__restrict __format, ...);
/* Read formatted input from STREAM.*/
extern int fscanf (FILE *__restrict __stream, const char *__restrict __format, ...);
/* Rename file OLD to NEW.  */
extern int rename (const char *__old, const char *__new);
/* Seek to a certain position on STREAM */
extern int fseek (FILE *__stream, long int __off, int __whence);

/* Close STREAM. */
extern int fclose (FILE *__stream);

/* Open a file and create a new stream for it. */
extern FILE *fopen (const char *__restrict __filename, const char *__restrict __modes);

#endif //GENODE_AFL_STDIO_H
