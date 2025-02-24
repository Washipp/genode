#ifndef FORKSERVER_WRAPPER_H
#define FORKSERVER_WRAPPER_H

#ifdef __cplusplus
extern "C" {

#endif

// Forward declare the C++ class as a C struct
typedef void* AflFuzzMain;

// Required definition to start afl-fuzz' main function.
int start_afl_fuzz(int argc, char **argv_orig, char **envp, void* afl_fuzz);

// Declare the wrapper function
int call_report_new_forkserver(void* afl_fuzz, int st_pipe_0, int ctl_pipe_1, int out_fd,
                               int coverage_map_shmid, int fuzzing_shmid);

#ifdef __cplusplus
}
#endif

#endif //FORKSERVER_WRAPPER_H
