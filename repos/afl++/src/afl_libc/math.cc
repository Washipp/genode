/* Genode include */
#include <base/log.h>

/* libc include */
#include <math.h>

#define NOT_IMPLEMENTED Genode::log("'", __func__, "()' not implemented")

// TODO: figure out how to make use of math functions.

double log(double x)
{
    (void) x;
    NOT_IMPLEMENTED;
    return -1;
}

double log2(double x)
{
    (void) x;
    NOT_IMPLEMENTED;
    return -1;
}

double log10(double x)
{
    (void) x;
    NOT_IMPLEMENTED;
    return -1;
}