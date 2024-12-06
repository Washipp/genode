#ifndef GENODE_AFL_IOCTL_H
#define GENODE_AFL_IOCTL_H
#define TIOCGWINSZ	0x5413

/* Perform the I/O control operation specified by REQUEST on FD.
   One argument may follow; its presence and type depend on REQUEST.
   Return value depends on REQUEST.  Usually -1 indicates error.  */
extern int ioctl (int __fd, unsigned long int __request, ...);


struct winsize
{
    unsigned short int ws_row;
    unsigned short int ws_col;
    unsigned short int ws_xpixel;
    unsigned short int ws_ypixel;
};
#endif //GENODE_AFL_IOCTL_H
