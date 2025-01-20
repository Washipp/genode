#ifndef GENODE_STDALIGN_H
#define GENODE_STDALIGN_H

/**
 * Used by xxhash.h
 */

#if defined(__cplusplus) || (defined(__STDC_VERSION__) && __STDC_VERSION__ < 202311L)
#ifndef __cplusplus
#define alignas _Alignas
#define alignof _Alignof
#endif

#define __alignas_is_defined 1
#define __alignof_is_defined 1
#endif /* __STDC_VERSION__ */

#endif //GENODE_STDALIGN_H
