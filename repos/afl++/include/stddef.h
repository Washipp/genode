#ifndef GENODE_STDDEF_H
#define GENODE_STDDEF_H

typedef long int __off_t;    /* Type of file sizes and offsets.  */
typedef unsigned long u_long;

#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)

#if !(defined (__GNUG__) && defined (size_t))
typedef __SIZE_TYPE__ size_t;
#ifdef __BEOS__
typedef long ssize_t;
#endif /* __BEOS__ */
#endif /* !(defined (__GNUG__) && defined (size_t)) */


#endif //GENODE_STDDEF_H
