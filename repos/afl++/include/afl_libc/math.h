#ifndef GENODE_MATH_H
#define GENODE_MATH_H


# define isinf(x) __builtin_isinf_sign (x)

#if defined __GNUC__ && defined __GNUC_MINOR__
# define __GNUC_PREREQ(maj, min) \
((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define __GNUC_PREREQ(maj, min) 0
#endif
/* Return nonzero value if X is not +-Inf or NaN.  */
# if (__GNUC_PREREQ (4, 4) && !defined __SUPPORT_SNAN__)
#  define isfinite(x) __builtin_isfinite (x)
# else
#  define isfinite(x) __MATH_TG ((x), __finite, (x))
# endif
/* Return nonzero value if X is a NaN.  We could use `fpclassify' but
   we already have this functions `__isnan' and it is faster.  */
# if (__GNUC_PREREQ (4, 4) && !defined __SUPPORT_SNAN__)
#  define isnan(x) __builtin_isnan (x)
# else
#  define isnan(x) __MATH_TG ((x), __isnan, (x))
# endif

typedef long int __off_t;    /* Type of file sizes and offsets.  */
#define PROT_READ      0x1   /* Page can be read.  */
#define MAP_PRIVATE    0x02  /* Changes are private.  */
#define MAP_FAILED     ((void *) -1)

#ifdef __cplusplus
extern "C" {
#endif

double log(double x);

double log2(double x);

double log10(double x);

#ifdef __cplusplus
}
#endif

#endif //GENODE_MATH_H
