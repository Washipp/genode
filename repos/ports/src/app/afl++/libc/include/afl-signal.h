#ifndef GENODE_AFL_SIGNAL_H
#define GENODE_AFL_SIGNAL_H

#include "sys/afl-types.h"

/* ISO C99 signals.  */
#define	SIGINT		2	/* Interactive attention signal.  */
#define	SIGILL		4	/* Illegal instruction.  */
#define	SIGABRT		6	/* Abnormal termination.  */
#define	SIGFPE		8	/* Erroneous arithmetic operation.  */
#define	SIGSEGV		11	/* Invalid access to storage.  */
#define	SIGTERM		15	/* Termination request.  */

/* Historical signals specified by POSIX. */
#define	SIGHUP		1	/* Hangup.  */
#define	SIGQUIT		3	/* Quit.  */
#define	SIGTRAP		5	/* Trace/breakpoint trap.  */
#define	SIGKILL		9	/* Killed.  */
#define	SIGPIPE		13	/* Broken pipe.  */
#define	SIGALRM		14	/* Alarm clock.  */

#define SIGUSR1		10	/* User-defined signal 1.  */
#define SIGWINCH	28	/* Window size change (4.3 BSD, Sun).  */
#define	SIG_IGN	 ((__sighandler_t)  1)	/* Ignore signal.  */

#define SIG_DFL ((__sighandler_t)0) /* Default action.  */
#define	SIGKILL		9	/* Killed.  */
#define SIGCHLD 17                  /* Child terminated or stopped.  */
#define SIGTERM 15                  /* Termination request.  */
#define SIGCONT		18	/* Continue.  */
#define SIGSTOP		19	/* Stop, unblockable.  */
#define SIGTSTP		20	/* Keyboard stop.  */

#define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))
typedef struct
{
    unsigned long int __val[_SIGSET_NWORDS];
} _sigset_t;
typedef void (*__sighandler_t) (int);
/* Structure describing the action to be taken when a signal arrives.  */
struct sigaction
{
    /* Signal handler.  */
    __sighandler_t sa_handler;

    /* Additional set of signals to be blocked.  */
    _sigset_t sa_mask;

    /* Special flags.  */
    int sa_flags;

    /* Restore handler.  */
    void (*sa_restorer) (void);

# define sa_sigaction	__sigaction_handler.sa_sigaction
};


typedef _sigset_t sigset_t;
/* Clear all signals from SET.  */
extern int sigemptyset (sigset_t *__set);

extern int sigaction(int __sig, const struct sigaction *__restrict __act,
                    struct sigaction *__restrict __oact);

/* Set the handler for the signal SIG to HANDLER, returning the old
   handler, or SIG_ERR on error. By default `signal' has the BSD semantic.  */
extern __sighandler_t signal (int __sig, __sighandler_t __handler);

/* Send signal SIG to process number PID.  If PID is zero,
   send SIG to all processes in the current process's process group.
   If PID is < -1, send SIG to all processes in process group - PID.  */
extern int kill (__pid_t __pid, int __sig);

/* Send SIG to all processes in process group PGRP.
   If PGRP is zero, send SIG to all processes in
   the current process's process group.  */
extern int killpg (__pid_t __pgrp, int __sig);

#endif //GENODE_AFL_SIGNAL_H
