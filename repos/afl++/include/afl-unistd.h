#ifndef GENODE_AFL_UNISTD_H
#define GENODE_AFL_UNISTD_H

#include "afl-stddef.h"


/* Values for the second argument to access. These may be OR'd together.  */
#define    R_OK    4        /* Test for read permission.  */
#define    W_OK    2        /* Test for write permission.  */
#define    X_OK    1        /* Test for execute permission.  */
#define    F_OK    0        /* Test for existence.  */

typedef unsigned int __useconds_t; /* Count of microseconds.  */

#ifdef __cplusplus
extern "C" {
#endif

/* Execute PATH with arguments ARGV and environment from `environ'.  */
int execv(const char *path, char *const argv[]);

/* Get the pathname of the current working directory,
   and put it in SIZE bytes of BUF.  Returns NULL if the
   directory couldn't be determined or SIZE was too small.
   If successful, returns BUF.  In GNU, if BUF is NULL,
   an array is allocated with `malloc'; the array is SIZE
   bytes long, unless SIZE == 0, in which case it is as
   big as necessary.  */
char *getcwd(char *buf, size_t size);

/* Make the process sleep for SECONDS seconds, or until a signal arrives
   and is not ignored.  The function returns the number of seconds less
   than SECONDS which it actually slept (thus zero if it slept the full time).
   If a signal handler does a `longjmp' or modifies the handling of the
   SIGALRM signal while inside `sleep' call, the handling of the SIGALRM
   signal afterwards is undefined.  There is no return value to indicate
   error, but if `sleep' returns SECONDS, it probably didn't work.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
unsigned int sleep(unsigned int _seconds);


/* Sleep USECONDS microseconds, or until a signal arrives that is not blocked
   or ignored.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
int usleep(__useconds_t _useconds);

/* Move FD's file position to OFFSET bytes from the
   beginning of the file (if WHENCE is SEEK_SET),
   the current position (if WHENCE is SEEK_CUR),
   or the end of the file (if WHENCE is SEEK_END).
   Return the new file position.  */
__off_t lseek(int _fd, __off_t _offset, int _whence);

/* Remove the link NAME.  */
int unlink(const char *_name);

/* Truncate the file FD is open on to LENGTH bytes.  */
int ftruncate(int _fd, __off_t _length);

/* Create a new session with the calling process as its leader.
   The process group IDs of the session and the calling process
   are set to the process ID of the calling process, which is returned.  */
int setsid(void);

/* Duplicate FD to FD2, closing FD2 and making it open on the same file.  */
int dup2(int __fd, int __fd2);

/* Test for access to NAME using the real UID and real GID.  */
int access(const char *__name, int __type);

/* Make a link to FROM named TO.  */
int link(const char *__from, const char *__to);

/* Remove the directory PATH.  */
int rmdir(const char *__path);

/* Close the file descriptor FD. */
int close(int fd);

/* Read NBYTES into BUF from FD.  Return the number read, -1 for errors or 0 for EOF.*/
long int read(int fd, void *buf, size_t nbytes);

/* Write N bytes of BUF to FD.  Return the number written, or -1.*/
long int write(int fd, const void *buf, size_t n);

/* Clone the calling process, creating an exact copy.
   Return -1 for errors, 0 to the new process, and the process ID of the new process to the old process.  */
int fork(void);

/* Create a one-way communication channel (pipe). If successful, two file descriptors are stored in PIPEDES;
   bytes written on PIPEDES[1] can be read from PIPEDES[0]. Returns 0 if successful, -1 if not.  */
int pipe(int pipedes[2]);


/* Make a symbolic link to FROM named TO.  */
int symlink(const char *from, const char *to);

/* Get the process ID of the calling process.  */
int getpid(void);

/* Get the process group ID of process PID.  */
int getpgid(int pid);

// TODO: The setup could be done using XML or some other way.
/* These three declarations are used to read the values from the command line when initializing the fuzzer. */
int getopt(int argc, char *const *argv, const char *shortopts);

char *optarg;
int optind;

/* Invoke `system call' number SYSNO, passing it the remaining arguments.
   This is completely system-dependent, and not often useful.

   In Unix, `syscall' sets `errno' for all errors and most calls return -1
   for errors; in many systems you cannot pass arguments or get return
   values for all system calls (`pipe', `fork', and `getppid' typically
   among them).

   In Mach, all system calls take normal arguments and always return an
   error code (zero for success).  */
long int syscall(long int sysno, ...);

/* Get the value of the system variable NAME.  */
long int sysconf(int name);

/* Terminate program execution with the low-order 8 bits of STATUS.  */
void _exit(int status) __attribute__ ((__noreturn__));

#ifdef __cplusplus
}
#endif

#endif //GENODE_AFL_UNISTD_H
