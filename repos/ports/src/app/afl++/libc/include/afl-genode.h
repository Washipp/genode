#ifndef __GENODE_DEBUG__
#define __GENODE_DEBUG__

#include "sys/afl-types.h"

int _SC_PAGE_SIZE;// = 2048;

#ifdef __cplusplus
extern "C" {
#endif
  typedef void (*__sighandler_t)(int);
  struct sigaction
  {
    /* Signal handler.  */
    #if defined __USE_POSIX199309 || defined __USE_XOPEN_EXTENDED
    union
    {
      /* Used if SA_SIGINFO is not set.  */
      __sighandler_t sa_handler;
      /* Used if SA_SIGINFO is set.  */

    }
    __sigaction_handler;
    # define sa_handler	__sigaction_handler.sa_handler
    # define sa_sigaction	__sigaction_handler.sa_sigaction
    #else
    __sighandler_t sa_handler;
    #endif

    /* Additional set of signals to be blocked.  */
    __sigset_t sa_mask;

    /* Special flags.  */
    int sa_flags;

    /* Restore handler.  */
    void (*sa_restorer) (void);
  };

extern void *mmap (void *__addr, size_t __len, int __prot, int __flags, int __fd, __off_t __offset);
extern int munmap (void *__addr, size_t __len);

extern int sigaction(int __sig, const struct sigaction *__restrict __act,
                    struct sigaction *__restrict __oact);

__attribute__((noreturn)) void lx_emul_trace_and_stop(const char *func);
void lx_emul_trace(const char *s);
void lx_emul_backtrace();

pid_t        _wait4();
extern char *getenv(const char *__name);
void       _exit(int);
int        close(int);
FILE      *fopen(const char *__restrict file, const char *__restrict mode);
int        fclose(FILE *);
pid_t      fork(void);
int        fprintf(FILE *__restrict, const char *__restrict, ...);
size_t     fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);
ssize_t    read(int, void *, size_t);
ssize_t    write(int, const void *, size_t);
extern int pipe(int __pipedes[2]);
long       syscall(long number, ...);
long       sysconf(int name);

extern int *__errno_location(void);
#define errno (*__errno_location())

extern int            kill(__pid_t __pid, int __sig);
extern __pid_t        waitpid(__pid_t __pid, int *__stat_loc, int __options);
extern __sighandler_t signal(int __sig, __sighandler_t __handler);

extern int raise (int __sig);

/* Standard streams.  */
extern FILE *stdin;  /* Standard input stream.  */
extern FILE *stdout; /* Standard output stream.  */
extern FILE *stderr; /* Standard error output stream.  */
/* C89/C99 say they're macros.  Make them happy.  */
#define stdin stdin
#define stdout stdout
#define stderr stderr

#ifdef __cplusplus
}
#endif

/* Define necessary signals. */
#define SIG_DFL ((__sighandler_t)0) /* Default action.  */
#define	SIGKILL		9	/* Killed.  */
#define SIGCHLD 17                  /* Child terminated or stopped.  */
#define SIGTERM 15                  /* Termination request.  */
#define SIGCONT		18	/* Continue.  */
#define SIGSTOP		19	/* Stop, unblockable.  */
#define SIGTSTP		20	/* Keyboard stop.  */


#endif /* __GENODE_DEBUG__ */
