#ifndef AFL_STDDEF_H
#define AFL_STDDEF_H

typedef long int __off_t;    /* Type of file sizes and offsets.  */
typedef unsigned long u_long;

#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)

#if !(defined (__GNUG__) && defined (size_t))
typedef __SIZE_TYPE__ size_t;
#endif /* !(defined (__GNUG__) && defined (size_t)) */


#ifndef __ssize_t_defined
typedef long int ssize_t;
# define __ssize_t_defined
#endif

#endif //AFL_STDDEF_H
