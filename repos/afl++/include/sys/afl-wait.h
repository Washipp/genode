#ifndef GENODE_AFL_WAIT_H
#define GENODE_AFL_WAIT_H

#include "sys/afl-types.h"


/* Waitflags */
#define	WNOHANG		1	/* Don't block waiting.  */
#define	WUNTRACED	2	/* Report status of stopped children.  */

#ifdef __cplusplus
extern "C" {
#endif

/* Wait for a child matching PID to die.
   If PID is greater than 0, match any process whose process ID is PID.
   If PID is (pid_t) -1, match any process.
   If PID is (pid_t) 0, match any process with the
   same process group as the current process.
   If PID is less than -1, match any process whose
   process group is the absolute value of PID.
   If the WNOHANG bit is set in OPTIONS, and that child
   is not already dead, return (pid_t) 0.  If successful,
   return PID and store the dead child's status in STAT_LOC.
   Return (pid_t) -1 for errors.  If the WUNTRACED bit is
   set in OPTIONS, return status for stopped children; otherwise don't.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
__pid_t waitpid(__pid_t pid, int *stat_loc, int options);

#ifdef __cplusplus
}
#endif

#endif //GENODE_AFL_WAIT_H
