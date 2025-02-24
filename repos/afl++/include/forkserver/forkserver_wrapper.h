#ifndef FORKSERVER_WRAPPER_H
#define FORKSERVER_WRAPPER_H

#ifdef __cplusplus
extern "C" {

#endif

// Required definition to start afl-fuzz' main function.
int main(int argc, char **argv_orig, char **envp);

// Declare the wrapper function
int call_report_new_forkserver(int st_pipe_0, int ctl_pipe_1, int out_fd,
                               int coverage_map_shmid, int fuzzing_shmid);

#ifdef __cplusplus
}
#endif

#endif //FORKSERVER_WRAPPER_H
