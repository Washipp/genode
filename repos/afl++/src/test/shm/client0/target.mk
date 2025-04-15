TARGET   = test-afl-shm-client0
LIBS     += base
SRC_CC   += main.cc shm_client.cc

INC_DIR += $(REP_DIR)/include/afl_libc
INC_DIR += $(REP_DIR)/include/shm

vpath shm_client.cc $(REP_DIR)/src/shm