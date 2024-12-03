#include "afl-genode.h"

/* headers from host libc to use */

pid_t _wait4() {
  lx_emul_trace_and_stop(__func__);
}

char *getenv(const char *__name) {
  lx_emul_trace_and_stop(__func__);
}

void _exit(int) {
  lx_emul_trace_and_stop(__func__);
}
int close(int) {
  lx_emul_trace_and_stop(__func__);
}
FILE *fopen(const char *__restrict file, const char *__restrict mode) {
  lx_emul_trace_and_stop(__func__);
}
int fclose(FILE *) {
  lx_emul_trace_and_stop(__func__);
}
pid_t fork(void) {
  lx_emul_trace_and_stop(__func__);
}
int fprintf(FILE *__restrict, const char *__restrict, ...) {
  lx_emul_trace_and_stop(__func__);
}
size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict) {
  lx_emul_trace_and_stop(__func__);
}
ssize_t read(int, void *, size_t) {
  lx_emul_trace_and_stop(__func__);
}
ssize_t write(int, const void *, size_t) {
  lx_emul_trace_and_stop(__func__);
}
int pipe (int __pipedes[2]) {
  lx_emul_trace_and_stop(__func__);
}
long syscall(long number, ...) {
  lx_emul_trace_and_stop(__func__);
}
long sysconf(int name) {
  lx_emul_trace_and_stop(__func__);
}

int kill (__pid_t __pid, int __sig) {
  lx_emul_trace_and_stop(__func__);
}
__pid_t waitpid (__pid_t __pid, int *__stat_loc, int __options) {
  lx_emul_trace_and_stop(__func__);
}

int sigaction(int __sig, const struct sigaction *__restrict __act,
                     struct sigaction *__restrict __oact) {
  lx_emul_trace_and_stop(__func__);
}
int raise (int __sig) {
  lx_emul_trace_and_stop(__func__);
}