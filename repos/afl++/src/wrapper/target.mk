TARGET = wrapper
SRC_CC += wrapper.cc shm_client.cc
LIBS += base

INC_DIR += $(REP_DIR)/include/shm
INC_DIR += $(REP_DIR)/include/afl_libc
INC_DIR += $(REP_DIR)/include/compiler_rt

SRC_O += /home/g-user/code/genode/build/x86_64/test/sut/main

vpath shm_client.cc $(REP_DIR)/src/shm