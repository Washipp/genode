LIBS += base
SRC_CC += shm_client.cc

INC_DIR += $(REP_DIR)/include/shm
INC_DIR += $(REP_DIR)/include/afl_libc
INC_DIR += $(REP_DIR)/include/compiler_rt

vpath shm_client.cc $(REP_DIR)/src/shm/
