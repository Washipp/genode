#ifndef GENODE_AFL_STAT_H
#define GENODE_AFL_STAT_H

#include "afl-struct-stat.h"

/* Get file attributes for the file, device, pipe, or socket
   that file descriptor FD is open on and put them in BUF.  */
extern int fstat (int __fd, struct stat *__buf);

/* Get file attributes about FILE and put them in BUF. If FILE is a symbolic link, do not follow it.  */
extern int lstat (const char *__restrict __file, struct stat *__restrict __buf);

/* Get file attributes for FILE and put them in BUF.  */
extern int stat (const char *__restrict __file, struct stat *__restrict __buf);

/* Create a new directory named PATH, with permission bits MODE.  */
extern int mkdir (const char *__path, unsigned int __mode);

#define	__S_IFMT	0170000	/* These bits determine file type.  */
#define	__S_IFREG	0100000	/* Regular file.  */
#define	__S_IFDIR	0040000	/* Directory.  */
#define	__S_ISTYPE(mode, mask)	(((mode) & __S_IFMT) == (mask))
#define	S_ISDIR(mode)	 __S_ISTYPE((mode), __S_IFDIR)
#define	S_ISREG(mode)	 __S_ISTYPE((mode), __S_IFREG)
#endif //GENODE_AFL_STAT_H
