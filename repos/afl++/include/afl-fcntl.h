#ifndef GENODE_AFL_FCNTL_H
#define GENODE_AFL_FCNTL_H

#define O_RDONLY	     00
#define O_WRONLY	     01
#define O_RDWR		     02
#define O_EXCL		     0200
#define O_CREAT	         0100
#define O_TRUNC	         01000

#define LOCK_EX	2	/* Exclusive lock.  */
#define LOCK_NB	4	/* Or'd with one of the above to prevent */

#define F_GETFD		1	/* Get file descriptor flags.  */

// TODO: Design a system, that read/writes. Probably something from vfs.h
/* Open FILE and return a new file descriptor for it, or -1 on error.
   OFLAG determines the type of access used.  If O_CREAT or O_TMPFILE is set
   in OFLAG, the third argument is taken as a `mode_t', the mode of the
   created file. */
extern "C" int open (const char *file, int oflag, ...);

/* Do the file control operation described by CMD on FD. The remaining arguments are interpreted depending on CMD.*/
extern "C" int fcntl (int fd, int cmd, ...);

#endif //GENODE_AFL_FCNTL_H
