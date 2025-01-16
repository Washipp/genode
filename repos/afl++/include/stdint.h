#ifndef GENODE_STDINT_H
#define GENODE_STDINT_H

#include "sys/types.h"

#define UINT64_C(c)    c ## UL
#define UINT64_MAX     (UINT64_C(18446744073709551615))

typedef unsigned long int	uintptr_t;

typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;

#endif //GENODE_STDINT_H

