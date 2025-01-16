#ifndef GENODE_FILE_H
#define GENODE_FILE_H

#ifndef __FILE_defined
#define __FILE_defined 1

struct _IO_FILE;

/* The opaque type of streams.  This is the definition used elsewhere.  */
typedef struct _IO_FILE FILE;

#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Apply or remove an advisory lock, according to OPERATION, on the file FD refers to.  */
int flock(int __fd, int __operation);

#ifdef __cplusplus
}
#endif

#endif //GENODE_FILE_H
