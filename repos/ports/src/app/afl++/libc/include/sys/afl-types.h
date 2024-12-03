#ifndef _MACHINE__TYPES_H_
#define	_MACHINE__TYPES_H_

//#ifndef _SYS_CDEFS_H_
//#error this file needs sys/cdefs.h as a prerequisite
//#endif

/*
 * Basic types upon which most other types are built.
 */
typedef	signed char		__int8_t;
typedef	unsigned char		__uint8_t;
typedef	short			__int16_t;
typedef	unsigned short		__uint16_t;
typedef	int			__int32_t;
typedef	unsigned int		__uint32_t;
#ifndef lint
__extension__
#endif
/* LONGLONG */
typedef	long long		__int64_t;
#ifndef lint
__extension__
#endif
/* LONGLONG */
typedef	unsigned long long	__uint64_t;

/*
 * Standard type definitions.
 */
typedef	__uint32_t	__clock_t;		/* clock()... */
typedef	__int32_t	__critical_t;
#ifndef _STANDALONE
typedef	double		__double_t;
typedef	float		__float_t;
#endif
typedef	__int32_t	__intfptr_t;
typedef	__int64_t	__intmax_t;
typedef	__int32_t	__intptr_t;
typedef	__int32_t	__int_fast8_t;
typedef	__int32_t	__int_fast16_t;
typedef	__int32_t	__int_fast32_t;
typedef	__int64_t	__int_fast64_t;
typedef	__int8_t	__int_least8_t;
typedef	__int16_t	__int_least16_t;
typedef	__int32_t	__int_least32_t;
typedef	__int64_t	__int_least64_t;
typedef	__PTRDIFF_TYPE__	__ptrdiff_t;		/* ptr1 - ptr2 */
typedef	__int32_t	__register_t;
typedef	__int32_t	__segsz_t;		/* segment size (in pages) */
typedef	__SIZE_TYPE__	__size_t;		/* sizeof() */
typedef	__PTRDIFF_TYPE__	__ssize_t;		/* byte count or error */
typedef	__int64_t	__time_t;		/* time()... */
typedef	__uint32_t	__uintfptr_t;
typedef	__uint64_t	__uintmax_t;
typedef	__uint32_t	__uintptr_t;
typedef	__uint32_t	__uint_fast8_t;
typedef	__uint32_t	__uint_fast16_t;
typedef	__uint32_t	__uint_fast32_t;
typedef	__uint64_t	__uint_fast64_t;
typedef	__uint8_t	__uint_least8_t;
typedef	__uint16_t	__uint_least16_t;
typedef	__uint32_t	__uint_least32_t;
typedef	__uint64_t	__uint_least64_t;
typedef	__uint32_t	__u_register_t;
typedef	__uint32_t	__vm_offset_t;
typedef	__uint32_t	__vm_paddr_t;
typedef	__uint32_t	__vm_size_t;

typedef	unsigned int	___wchar_t;
#define	__WCHAR_MIN	0		/* min value for a wchar_t */
#define	__WCHAR_MAX	__UINT_MAX	/* max value for a wchar_t */

#endif /* !_MACHINE__TYPES_H_ */



#ifndef _SYS__SIGSET_H_
#define	_SYS__SIGSET_H_
/*
 * sigset_t macros.
 */
#define	_SIG_WORDS	4
#define	_SIG_MAXSIG	128
#define	_SIG_IDX(sig)	((sig) - 1)
#define	_SIG_WORD(sig)	(_SIG_IDX(sig) >> 5)
#define	_SIG_BIT(sig)	(1 << (_SIG_IDX(sig) & 31))
#define	_SIG_VALID(sig)	((sig) <= _SIG_MAXSIG && (sig) > 0)

typedef struct __sigset {
    __uint32_t __bits[_SIG_WORDS];
} __sigset_t;

#endif /* !_SYS__SIGSET_H_ */



#ifndef __pid_t_defined
#define	__S32_TYPE		int
#define __PID_T_TYPE		__S32_TYPE
# define __STD_TYPE		typedef
__STD_TYPE __PID_T_TYPE __pid_t;	/* Type of process identifications.  */

typedef __pid_t pid_t;
# define __pid_t_defined
#endif


#ifndef __FILE_defined
#define __FILE_defined 1
struct _IO_FILE;
typedef struct _IO_FILE FILE;
#endif

typedef __SIZE_TYPE__ size_t;
# define __SWORD_TYPE		long int
#define __SSIZE_T_TYPE		__SWORD_TYPE
__STD_TYPE __SSIZE_T_TYPE __ssize_t; /* Type of a byte count, or error.  */
typedef __ssize_t ssize_t;



#ifndef _BITS_STDINT_UINTN_H
#define _BITS_STDINT_UINTN_H	1


typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;

#endif /* bits/stdint-uintn.h */

#ifndef _BITS_STDINT_INTN_H
#define _BITS_STDINT_INTN_H	1

typedef __int8_t int8_t;
typedef __int16_t int16_t;
typedef __int32_t int32_t;
typedef __int64_t int64_t;

#endif /* bits/stdint-intn.h */


#if defined __GNUC__ && defined __GNUC_MINOR__
# define __GNUC_PREREQ(maj, min) \
((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define __GNUC_PREREQ(maj, min) 0
#endif
/* Return nonzero value if X is not +-Inf or NaN.  */
# if (__GNUC_PREREQ (4,4) && !defined __SUPPORT_SNAN__)
#  define isfinite(x) __builtin_isfinite (x)
# else
#  define isfinite(x) __MATH_TG ((x), __finite, (x))
# endif
/* Return nonzero value if X is a NaN.  We could use `fpclassify' but
   we already have this functions `__isnan' and it is faster.  */
# if (__GNUC_PREREQ (4,4) && !defined __SUPPORT_SNAN__)
#  define isnan(x) __builtin_isnan (x)
# else
#  define isnan(x) __MATH_TG ((x), __isnan, (x))
# endif

__STD_TYPE long int __off_t;	/* Type of file sizes and offsets.  */
#define PROT_READ	0x1		/* Page can be read.  */
#define MAP_PRIVATE	0x02		/* Changes are private.  */
#define MAP_FAILED	((void *) -1)