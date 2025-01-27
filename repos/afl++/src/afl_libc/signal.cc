/* Afl-libc includes */
#include "signal.h"
#include "errno.h"

/* Genode includes */
#include <base/log.h>

#define NOT_IMPLEMENTED Genode::log("'",__func__, "()' not implemented")

int sigemptyset(sigset_t *set)
{
    (void) set;
    NOT_IMPLEMENTED;
    return 0;
}

int sigaction(int sig, const struct sigaction *__restrict act, struct sigaction *__restrict oact)
{
    (void) sig;
    (void) act;
    (void) oact;
    NOT_IMPLEMENTED;
    Genode::log("Sig: '", sig, "'");
    return 0;
}

__sighandler_t signal(int sig, __sighandler_t handler)
{
    (void) sig;
    (void) handler;
    NOT_IMPLEMENTED;
    Genode::log("Sig: '", sig, "'");
    return handler;
}

int kill(int pid, int sig)
{
    (void) pid;
    (void) sig;
    NOT_IMPLEMENTED;
    return 0;
}


int killpg(int pgrp, int sig)
{
    (void) pgrp;
    (void) sig;
    NOT_IMPLEMENTED;
    return 0;
}

int raise(int sig)
{
    (void) sig;
    NOT_IMPLEMENTED;
    return 0;
}
