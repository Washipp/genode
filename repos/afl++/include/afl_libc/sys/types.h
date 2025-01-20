#ifndef AFL_TYPES_H
#define AFL_TYPES_H

#define __SLONGWORD_TYPE    long int
#define __S32_TYPE          int

#define __SYSCALL_SLONG_TYPE    __SLONGWORD_TYPE

#define __PID_T_TYPE        __S32_TYPE
#define __TIME_T_TYPE        __SYSCALL_SLONG_TYPE

/* Fixed-size types, underlying types depend on word size and compiler.  */
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
#if __WORDSIZE == 64
typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;
#else
__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;
#endif

typedef __PID_T_TYPE __pid_t;    /* Type of process identifications.  */
typedef __TIME_T_TYPE __time_t;    /* Seconds since the Epoch.  */

typedef __time_t time_t;

#endif //AFL_TYPES_H