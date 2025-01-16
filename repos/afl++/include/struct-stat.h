#ifndef GENODE_STRUCT_STAT_H
#define GENODE_STRUCT_STAT_H
struct stat
{
#ifdef __USE_TIME64_REDIRECTS
# include <bits/struct_stat_time64_helper.h>
#else
    unsigned long int st_dev;		/* Device.  */
# ifndef __x86_64__
    unsigned short int __pad1;
# endif
# if defined __x86_64__ || !defined __USE_FILE_OFFSET64
    unsigned long int st_ino;		/* File serial number.	*/
# else
    __ino_t __st_ino;			/* 32bit file serial number.	*/
# endif
# ifndef __x86_64__
    __mode_t st_mode;			/* File mode.  */
    __nlink_t st_nlink;			/* Link count.  */
# else
    unsigned long int st_nlink;		/* Link count.  */
    unsigned int st_mode;		/* File mode.  */
# endif
    unsigned int st_uid;		/* User ID of the file's owner.	*/
    unsigned int st_gid;		/* Group ID of the file's group.*/
# ifdef __x86_64__
    int __pad0;
# endif
    unsigned long int st_rdev;		/* Device number, if device.  */
# ifndef __x86_64__
    unsigned short int __pad2;
# endif
# if defined __x86_64__ || !defined __USE_FILE_OFFSET64
    long int st_size;			/* Size of file, in bytes.  */
# else
    __off64_t st_size;			/* Size of file, in bytes.  */
# endif
    long int st_blksize;	/* Optimal block size for I/O.  */
# if defined __x86_64__  || !defined __USE_FILE_OFFSET64
    long int st_blocks;		/* Number 512-byte blocks allocated. */
# else
    __blkcnt64_t st_blocks;		/* Number 512-byte blocks allocated. */
# endif
# ifdef __USE_XOPEN2K8
    /* Nanosecond resolution timestamps are stored in a format
       equivalent to 'struct timespec'.  This is the type used
       whenever possible but the Unix namespace rules do not allow the
       identifier 'timespec' to appear in the <sys/stat.h> header.
       Therefore we have to handle the use of this header in strictly
       standard-compliant sources special.  */
    struct timespec st_atim;		/* Time of last access.  */
    struct timespec st_mtim;		/* Time of last modification.  */
    struct timespec st_ctim;		/* Time of last status change.  */
#  define st_atime st_atim.tv_sec	/* Backward compatibility.  */
#  define st_mtime st_mtim.tv_sec
#  define st_ctime st_ctim.tv_sec
# else
    long int st_atime;			/* Time of last access.  */
    unsigned long int st_atimensec;	/* Nscecs of last access.  */
    long int st_mtime;			/* Time of last modification.  */
    unsigned long int st_mtimensec;	/* Nsecs of last modification.  */
    long int st_ctime;			/* Time of last status change.  */
    unsigned long int st_ctimensec;	/* Nsecs of last status change.  */
# endif
# ifdef __x86_64__
    long int __glibc_reserved[3];
# else
    #  ifndef __USE_FILE_OFFSET64
    unsigned long int __glibc_reserved4;
    unsigned long int __glibc_reserved5;
#  else
    __ino64_t st_ino;			/* File serial number.	*/
#  endif
# endif
#endif /* __USE_TIME64_REDIRECTS  */
};

#endif //GENODE_STRUCT_STAT_H
