TARGET   = print_component
LIBS     += base libc
SRC_CC   += main.cc shm_client.cc

INC_DIR += $(REP_DIR)/include/afl_libc
INC_DIR += $(REP_DIR)/include/shm

vpath %.cc $(REP_DIR)/src/shm