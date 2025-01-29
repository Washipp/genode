/* Afl-libc includes */
#include "unistd.h"
#include "stddef.h"

/* Genode includes */
#include <base/log.h>
#include <base/sleep.h>

#define NOT_IMPLEMENTED Genode::log("'",__func__, "()' not implemented")


/* Values for the second argument to access. These may be OR'd together.  */
#define    R_OK    4        /* Test for read permission.  */
#define    W_OK    2        /* Test for write permission.  */
#define    X_OK    1        /* Test for execute permission.  */
#define    F_OK    0        /* Test for existence.  */

int execv(const char *path, char *const argv[])
{
    (void) path;
    (void) argv;
    NOT_IMPLEMENTED;
    return 0;
}

char *getcwd(char *buf, size_t size)
{
    (void) buf;
    (void) size;
    NOT_IMPLEMENTED;
    return nullptr;
}

unsigned int sleep(unsigned int seconds)
{
    (void) seconds;
    NOT_IMPLEMENTED;
    return 0;
}

int usleep(__useconds_t useconds)
{
    (void) useconds;
    NOT_IMPLEMENTED;
    return 0;
}

__off_t lseek(int fd, __off_t offset, int whence)
{
    (void) fd;
    (void) offset;
    (void) whence;
    NOT_IMPLEMENTED;
    return 0;
}

int unlink(const char *name)
{
    (void) name;
    NOT_IMPLEMENTED;
    return 0;
}

int ftruncate(int fd, __off_t length)
{
    (void) fd;
    (void) length;
    NOT_IMPLEMENTED;
    return 0;
}

int setsid(void)
{
    NOT_IMPLEMENTED;
    return 0;
}

int dup2(int fd, int fd2)
{
    (void) fd;
    (void) fd2;
    NOT_IMPLEMENTED;
    return 0;
}

int access(const char *name, int type)
{
    (void) name;
    (void) type;
    NOT_IMPLEMENTED;
    return 0;
}

int link(const char *from, const char *to)
{
    (void) from;
    (void) to;
    NOT_IMPLEMENTED;
    return 0;
}

int rmdir(const char *path)
{
    (void) path;
    NOT_IMPLEMENTED;
    return 0;
}

int close(int fd)
{
    (void) fd;
    NOT_IMPLEMENTED;
    return 0;
}

long int read(int fd, void *buf, size_t n)
{
    (void) buf;
    (void) n;
    Genode::log("read called: fd: ", fd);
    return -1;
}

long int write(int fd, const void *buf, size_t n)
{
    (void) buf;
    (void) n;
    Genode::log("write called: fd: ", fd);
    return -1;
}

int fork(void)
{
    NOT_IMPLEMENTED;
    return 0;
}

int pipe(int pipedes[2])
{
    (void) pipedes;
    NOT_IMPLEMENTED;
    return 0;
}

int symlink(const char *from, const char *to)
{
    (void) from;
    (void) to;
    NOT_IMPLEMENTED;
    return 0;
}

int getpid(void)
{
    NOT_IMPLEMENTED;
    return 0;
}

int getpgid(int pid)
{
    (void) pid;
    NOT_IMPLEMENTED;
    return 0;
}

// TODO: The setup could be done using XML or some other way.
int getopt(int argc, char *const *argv, const char *shortopts)
{
    (void) argc;
    (void) argv;
    (void) shortopts;
    NOT_IMPLEMENTED;
    return 0;
}

long int syscall(long int sysno, ...)
{
    Genode::log("syscall called. Sysno: ", sysno);
    return 0;
}

/* Get the value of the system variable NAME.  */
long int sysconf(int name)
{
    Genode::log("sysconf called. Name: ", name);
    return 0;
}

/* Terminate program execution with the low-order 8 bits of STATUS.  */
void _exit(int status)
{
    Genode::log("_exit called. Status: ", status);
    Genode::sleep_forever();

}
