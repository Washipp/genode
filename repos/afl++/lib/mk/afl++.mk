LIBS += base
SRC_CC += shm_client.cc

SHARED_LIB = yes

INC_DIR += $(REP_DIR)/include/shm
INC_DIR += $(REP_DIR)/include/afl_libc

vpath shm_client.cc $(REP_DIR)/src/shm/
