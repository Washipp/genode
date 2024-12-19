#include "sys/afl-shm.h"
#include <base/log.h>

#define NOT_IMPLEMENTED Genode::log(__func__, " not implemented")

int shmget (int key, size_t size, int shmflg) {
    (void)key;
    (void)size;
    (void)shmflg;
    NOT_IMPLEMENTED;
    return 0;
}

int shmctl (int shmid, int cmd, struct shmid_ds *buf) {
    (void) shmid;
    (void)cmd;
    (void)buf;
    NOT_IMPLEMENTED;
    return 0;
}

void *shmat (int shmid, const void *shmaddr, int shmflg) {
    (void)shmid;
    (void)shmaddr;
    (void)shmflg;
    NOT_IMPLEMENTED;
    return nullptr;
}

int shmdt (const void *shmaddr) {
    (void)shmaddr;
    NOT_IMPLEMENTED;
    return 0;
}