#ifndef _MACHINE__TYPES_H_
#define	_MACHINE__TYPES_H_

/*
 * Basic types upon which most other types are built.
 * For now removed due to conflicting types.
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef long long __int64_t;
typedef unsigned long long __uint64_t;
*/

/* Types to compile afl-cc
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#ifdef WORD_SIZE_64
typedef unsigned __int128 uint128_t;
typedef uint128_t         u128;
#endif


typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
*/


/*
 * Standard type definitions.
 */
typedef int __critical_t;
#ifndef _STANDALONE
typedef double __double_t;
typedef float __float_t;
#endif
typedef __PTRDIFF_TYPE__ __ptrdiff_t;        /* ptr1 - ptr2 */
typedef int __register_t;
typedef int __segsz_t;        /* segment size (in pages) */
typedef __SIZE_TYPE__ __size_t;        /* sizeof() */
typedef __PTRDIFF_TYPE__ __ssize_t;        /* byte count or error */
#if defined(__time_t)
typedef long long __time_t;        /* time()... */
#endif

typedef unsigned int ___wchar_t;
#ifndef __WCHAR_MIN
#define    __WCHAR_MIN    0        /* min value for a wchar_t */
#endif
#ifndef __WCHAR_MAX
#define    __WCHAR_MAX    __UINT_MAX    /* max value for a wchar_t */
#endif

#endif /* !_MACHINE__TYPES_H_ */


#ifndef _SYS__SIGSET_H_
#define    _SYS__SIGSET_H_
/*
 * sigset_t macros.
 */
#define    _SIG_WORDS    4
#define    _SIG_MAXSIG    128
#define    _SIG_IDX(sig)    ((sig) - 1)
#define    _SIG_WORD(sig)    (_SIG_IDX(sig) >> 5)
#define    _SIG_BIT(sig)    (1 << (_SIG_IDX(sig) & 31))
#define    _SIG_VALID(sig)    ((sig) <= _SIG_MAXSIG && (sig) > 0)

#if !defined(__sigset_t)
typedef struct __sigset {
    unsigned int __bits[_SIG_WORDS];
} __sigset_t;
#endif

#endif /* !_SYS__SIGSET_H_ */


#ifndef __pid_t_defined
#define    __S32_TYPE        int
#define __PID_T_TYPE        __S32_TYPE
# define __STD_TYPE        typedef
__STD_TYPE __PID_T_TYPE __pid_t;    /* Type of process identifications.  */

typedef __pid_t pid_t;
# define __pid_t_defined
#endif


#ifndef __FILE_defined
#define __FILE_defined 1
struct _IO_FILE {};
typedef struct _IO_FILE FILE;
#endif

# define __SWORD_TYPE        long int
#define __SSIZE_T_TYPE        __SWORD_TYPE
__STD_TYPE __SSIZE_T_TYPE __ssize_t; /* Type of a byte count, or error.  */
typedef __ssize_t ssize_t;


#ifndef _BITS_STDINT_UINTN_H
#define _BITS_STDINT_UINTN_H    1


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#if !defined(uint64_t)
typedef unsigned long long uint64_t;
#endif

#endif /* bits/stdint-uintn.h */

#ifndef _BITS_STDINT_INTN_H
#define _BITS_STDINT_INTN_H    1

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;



#endif /* bits/stdint-intn.h */
