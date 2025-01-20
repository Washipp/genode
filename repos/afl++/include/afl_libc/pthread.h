/**
 * This header file is only used to provide the necessary implementations to successfully link cxx.a
 * The reason is, that cxx depends on the used compiler (afl-gcc-fast).
 * It expects additional functions, which are defined here and a stub has been implemented.
 */
#ifndef AFL_PTHREAD_H
#define AFL_PTHREAD_H

struct pthread_mutex_t {};
struct dl_find_object {};

#ifdef __cplusplus
extern "C" {
#endif

int pthread_mutex_lock(pthread_mutex_t *mutex);

int pthread_mutex_unlock(pthread_mutex_t *mutex);

int _dl_find_object(void *address, struct dl_find_object *result);

#ifdef __cplusplus
}
#endif

#endif //AFL_PTHREAD_H
