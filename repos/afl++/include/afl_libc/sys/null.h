#ifndef GENODE_NULL_H
#define GENODE_NULL_H

#ifndef NULL
#ifdef __GNUG__
#define NULL __null
#else   /* G++ */
#ifndef __cplusplus
#define NULL ((void *)0)
#else   /* C++ */
#define NULL 0
#endif  /* C++ */
#endif  /* G++ */
#endif  /* NULL */

#endif //GENODE_NULL_H
