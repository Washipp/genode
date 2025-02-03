#include "shm_session.h"

/* AFL-libc includes */
#include "sys/shm.h"

/* Genode includes */
#include <base/log.h>
#include <base/heap.h>
#include <base/component.h>
#include <base/attached_ram_dataspace.h>
#include <util/dictionary.h>


#define NOT_IMPLEMENTED Genode::log(__func__, " not implemented")

int shmget (int key, size_t size, int shmflg) {
    (void)key;
    (void)size;
    (void)shmflg;
    NOT_IMPLEMENTED;
    return -1;
}

int shmctl (int shmid, int cmd, struct shmid_ds *buf) {
    (void) shmid;
    (void)cmd;
    (void)buf;
    NOT_IMPLEMENTED;
    return -1;
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
    return -1;
}