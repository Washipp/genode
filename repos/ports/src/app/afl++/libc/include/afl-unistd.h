#ifndef GENODE_AFL_UNISTD_H
#define GENODE_AFL_UNISTD_H

#include "afl-stddef.h"
#include "sys/afl-types.h"


/* Values for the second argument to access. These may be OR'd together.  */
#define	R_OK	4		/* Test for read permission.  */
#define	W_OK	2		/* Test for write permission.  */
#define	X_OK	1		/* Test for execute permission.  */
#define	F_OK	0		/* Test for existence.  */

/* Execute PATH with arguments ARGV and environment from `environ'.  */
extern int execv (const char *__path, char *const _argv[]);

/* Get the pathname of the current working directory,
   and put it in SIZE bytes of BUF.  Returns NULL if the
   directory couldn't be determined or SIZE was too small.
   If successful, returns BUF.  In GNU, if BUF is NULL,
   an array is allocated with `malloc'; the array is SIZE
   bytes long, unless SIZE == 0, in which case it is as
   big as necessary.  */
extern char *getcwd (char *_buf, size_t _size);

/* Make the process sleep for SECONDS seconds, or until a signal arrives
   and is not ignored.  The function returns the number of seconds less
   than SECONDS which it actually slept (thus zero if it slept the full time).
   If a signal handler does a `longjmp' or modifies the handling of the
   SIGALRM signal while inside `sleep' call, the handling of the SIGALRM
   signal afterwards is undefined.  There is no return value to indicate
   error, but if `sleep' returns SECONDS, it probably didn't work.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern unsigned int sleep (unsigned int _seconds);


typedef unsigned int __useconds_t; /* Count of microseconds.  */

/* Sleep USECONDS microseconds, or until a signal arrives that is not blocked
   or ignored.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int usleep (__useconds_t _useconds);

/* Move FD's file position to OFFSET bytes from the
   beginning of the file (if WHENCE is SEEK_SET),
   the current position (if WHENCE is SEEK_CUR),
   or the end of the file (if WHENCE is SEEK_END).
   Return the new file position.  */
extern __off_t lseek (int _fd, __off_t _offset, int _whence);

/* Remove the link NAME.  */
extern int unlink (const char *_name);

/* Truncate the file FD is open on to LENGTH bytes.  */
extern int ftruncate (int _fd, __off_t _length);

/* Create a new session with the calling process as its leader.
   The process group IDs of the session and the calling process
   are set to the process ID of the calling process, which is returned.  */
extern __pid_t setsid (void);

/* Duplicate FD to FD2, closing FD2 and making it open on the same file.  */
extern int dup2 (int __fd, int __fd2);

/* Test for access to NAME using the real UID and real GID.  */
extern int access (const char *__name, int __type);

/* Make a link to FROM named TO.  */
extern int link (const char *__from, const char *__to);

/* Remove the directory PATH.  */
extern int rmdir (const char *__path);

/* Close the file descriptor FD. */
extern int close (int __fd);

/* Read NBYTES into BUF from FD.  Return the number read, -1 for errors or 0 for EOF.*/
extern ssize_t read (int __fd, void *__buf, size_t __nbytes);

/* Write N bytes of BUF to FD.  Return the number written, or -1.*/
extern ssize_t write (int __fd, const void *__buf, size_t __n);

/* Clone the calling process, creating an exact copy.
   Return -1 for errors, 0 to the new process, and the process ID of the new process to the old process.  */
extern __pid_t fork (void);

/* Create a one-way communication channel (pipe). If successful, two file descriptors are stored in PIPEDES;
   bytes written on PIPEDES[1] can be read from PIPEDES[0]. Returns 0 if successful, -1 if not.  */
extern int pipe (int __pipedes[2]);

/* Make a symbolic link to FROM named TO.  */
extern int symlink (const char *__from, const char *__to);

/* Get the process ID of the calling process.  */
extern __pid_t getpid (void);

/* Get the process group ID of process PID.  */
extern __pid_t getpgid (__pid_t __pid);

// TODO: The setup could be done using XML or some other way.
/* These three declarations are used to read the values from the command line when initializing the fuzzer. */
extern int getopt (int ___argc, char *const *___argv, const char *__shortopts);
extern char *optarg;
extern int optind;
#endif //GENODE_AFL_UNISTD_H
