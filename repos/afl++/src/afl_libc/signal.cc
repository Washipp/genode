#include "signal.h"

#include <base/log.h>

int sigemptyset (sigset_t *set) {
    (void)set;
    Genode::log("sigemptyset called");
    return 0;
}

int sigaction(int sig, const struct sigaction *__restrict act, struct sigaction *__restrict oact) {
    (void)sig;
    (void)act;
    (void)oact;
    Genode::log("sigaction called");
    return 0;
}

__sighandler_t signal (int sig, __sighandler_t handler) {
    (void)sig;
    (void)handler;
    Genode::log("signal called");
    return handler;
}

int kill (int pid, int sig) {
    (void)pid;
    (void)sig;
    Genode::log("kill called. pid: ", pid, ", sig: ", sig);
    return 0;
}


int killpg (int pgrp, int sig) {
    (void)pgrp;
    (void)sig;
    Genode::log("kill called. pgrp: ", pgrp, ", sig: ", sig);
    return 0;
}

int raise (int sig) {
    (void)sig;
    Genode::log("raise() called. sig: ", sig);
    return 0;
}
