#ifndef FORKSERVER_WRAPPER_H
#define FORKSERVER_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

struct Exec_data {
    int version;
    int status;
    int exec_ms;
};

// Required definition to start afl-fuzz' main function.
int main(int argc, char **argv_orig, char **envp);

// Declare the wrapper function
int call_report_new_forkserver(int coverage_map_shmid, int fuzzing_shmid, struct Exec_data *exec_data);

#ifdef __cplusplus
}
#endif

#endif //FORKSERVER_WRAPPER_H
