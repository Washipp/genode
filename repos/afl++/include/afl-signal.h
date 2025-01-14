#ifndef GENODE_AFL_SIGNAL_H
#define GENODE_AFL_SIGNAL_H

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


#if !defined(__sigset_t)
#define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))
typedef struct
{
    unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t;
#endif
typedef void (*__sighandler_t) (int);
/* Structure describing the action to be taken when a signal arrives.  */
struct sigaction
{
    /* Signal handler.  */
    __sighandler_t sa_handler;

    /* Additional set of signals to be blocked.  */
    __sigset_t sa_mask;

    /* Special flags.  */
    int sa_flags;

    /* Restore handler.  */
    void (*sa_restorer) (void);

# define sa_sigaction	__sigaction_handler.sa_sigaction
};


typedef __sigset_t sigset_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Clear all signals from SET.  */
int sigemptyset(sigset_t *set);

int sigaction(int sig, const struct sigaction *__restrict act, struct sigaction *__restrict oact);

/* Set the handler for the signal SIG to HANDLER, returning the old
   handler, or SIG_ERR on error. By default, `signal` has the BSD semantic.  */
__sighandler_t signal(int sig, __sighandler_t handler);

/* Send signal SIG to process number PID.  If PID is zero,
   send SIG to all processes in the current process's process group.
   If PID is < -1, send SIG to all processes in process group - PID.  */
int kill(int pid, int sig);

/* Send SIG to all processes in process group PGRP.
   If PGRP is zero, send SIG to all processes in
   the current process's process group.  */
int killpg(int pgrp, int sig);

/* Raise signal SIG, i.e., send SIG to yourself.  */
int raise(int sig);

#ifdef __cplusplus
}
#endif

#endif //GENODE_AFL_SIGNAL_H
