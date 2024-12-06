#ifndef GENODE_AFL_STDDEF_H
#define GENODE_AFL_STDDEF_H

#define __OFF_T_TYPE		long int
typedef __OFF_T_TYPE __off_t;	/* Type of file sizes and offsets.  */

#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)

#if !(defined (__GNUG__) && defined (size_t))
typedef __SIZE_TYPE__ size_t;
#ifdef __BEOS__
typedef long ssize_t;
#endif /* __BEOS__ */
#endif /* !(defined (__GNUG__) && defined (size_t)) */


#endif //GENODE_AFL_STDDEF_H
