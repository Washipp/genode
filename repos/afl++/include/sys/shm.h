#ifndef GENODE_SHM_H
#define GENODE_SHM_H

#include "stddef.h"
#include "types.h"

typedef unsigned long int __syscall_ulong_t;

typedef __syscall_ulong_t shmatt_t;

struct ipc_perm
{
    int __key;				/* Key.  */
    unsigned int uid;					/* Owner's user ID.  */
    unsigned int gid;					/* Owner's group ID.  */
    unsigned int cuid;					/* Creator's user ID.  */
    unsigned int cgid;					/* Creator's group ID.  */
    unsigned int mode;				/* Read/write permission.  */
    unsigned short int __seq;			/* Sequence number.  */
    unsigned short int __pad2;
    __syscall_ulong_t __glibc_reserved1;
    __syscall_ulong_t __glibc_reserved2;
};

/* Data structure describing a shared memory segment.  */
struct shmid_ds
{
#ifdef __USE_TIME64_REDIRECTS
# include <bits/types/struct_shmid64_ds_helper.h>
#else
    struct ipc_perm shm_perm;		/* operation permission struct */
    size_t shm_segsz;			/* size of segment in bytes */
# if __TIMESIZE == 32
    __time_t shm_atime;			/* time of last shmat() */
    unsigned long int __shm_atime_high;
    __time_t shm_dtime;			/* time of last shmdt() */
    unsigned long int __shm_dtime_high;
    __time_t shm_ctime;			/* time of last change by shmctl() */
    unsigned long int __shm_ctime_high;
# else
    __time_t shm_atime;			/* time of last shmat() */
    __time_t shm_dtime;			/* time of last shmdt() */
    __time_t shm_ctime;			/* time of last change by shmctl() */
# endif
    __pid_t shm_cpid;			/* pid of creator */
    __pid_t shm_lpid;			/* pid of last shmop */
    shmatt_t shm_nattch;		/* number of current attaches */
    __syscall_ulong_t __glibc_reserved5;
    __syscall_ulong_t __glibc_reserved6;
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

/* Get shared memory segment.  */
int shmget(int key, size_t size, int shmflg);

/* Shared memory control operation.  */
int shmctl(int shmid, int cmd, struct shmid_ds *buf);

/* Attach shared memory segment.  */
void *shmat(int shmid, const void *shmaddr, int shmflg);

/* Detach shared memory segment.  */
int shmdt(const void *shmaddr);

#ifdef __cplusplus
}
#endif

#endif //GENODE_SHM_H
