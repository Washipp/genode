#ifndef GENODE_AFL_DIRENT_H
#define GENODE_AFL_DIRENT_H

struct dirent {
#ifndef __USE_FILE_OFFSET64
    unsigned long int d_ino;
    long int d_off;
#else
    unsigned long int  d_ino;
    long int d_off;
#endif
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];        /* We must not include limits.h! */
};

typedef struct __dirstream DIR;

/* Read a directory entry from DIRP.  Return a pointer to a `struct
   dirent' describing the entry, or NULL for EOF or error.  The
   storage returned may be overwritten by a later readdir call on the
   same DIR stream.

   If the Large File Support API is selected we have to use the
   appropriate interface. */
extern struct dirent *readdir(DIR *__dirp);

/* Open a directory stream on NAME. Return a DIR stream on the directory, or NULL if it could not be opened. */
extern DIR *opendir(const char *__name);

/* Close the directory stream DIRP. Return 0 if successful, -1 if not. */
extern int closedir(DIR *__dirp);

/* Scan the directory DIR, calling SELECTOR on each directory entry.
   Entries for which SELECT returns nonzero are individually malloc'd,
   sorted using qsort with CMP, and collected in a malloc'd array in
   *NAMELIST.  Returns the number of entries selected, or -1 on error.*/
extern int
scandir(const char *__restrict __dir, struct dirent ***__restrict __namelist, int (*__selector)(const struct dirent *),
        int (*__cmp)(const struct dirent **, const struct dirent **));

/* Function to compare two `struct dirent's alphabetically.  */
extern int alphasort(const struct dirent **__e1, const struct dirent **__e2);

#endif //GENODE_AFL_DIRENT_H
