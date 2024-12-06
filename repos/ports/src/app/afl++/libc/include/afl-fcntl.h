#ifndef GENODE_AFL_FCNTL_H
#define GENODE_AFL_FCNTL_H

#define O_RDONLY	     00
#define O_WRONLY	     01
#define O_RDWR		     02
#define O_EXCL		     0200
#define O_CREAT	         0100
#define O_TRUNC	         01000

# define LOCK_EX	2	/* Exclusive lock.  */
# define LOCK_NB	4	/* Or'd with one of the above to prevent */


/* Open FILE and return a new file descriptor for it, or -1 on error.
   OFLAG determines the type of access used.  If O_CREAT or O_TMPFILE is set
   in OFLAG, the third argument is taken as a `mode_t', the mode of the
   created file. */
extern int open (const char *__file, int __oflag, ...);

#endif //GENODE_AFL_FCNTL_H
