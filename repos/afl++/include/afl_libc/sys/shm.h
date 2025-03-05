#ifndef AFL_SHM_H
#define AFL_SHM_H

#include "null.h"

/* Data structure describing a shared memory segment.  */
struct shmid_ds { };

#ifdef __cplusplus
extern "C" {
#endif

/* Get shared memory segment.  */
int shmget(int key, long unsigned int size, int shmflg);

/* Shared memory control operation.  */
int shmctl(int shmid, int cmd, struct shmid_ds *buf);

/* Attach shared memory segment.  */
void *shmat(int shmid, const void *shmaddr, int shmflg);

/* Detach shared memory segment.  */
int shmdt(const void *shmaddr);

#ifdef __cplusplus
}
#endif

#endif //AFL_SHM_H
