TARGET = forkserver
SRC_CC += forkserver.cc shm_client.cc
LIBS += base libc

INC_DIR += $(REP_DIR)/include/afl_libc
INC_DIR += $(REP_DIR)/include/shm

AFL++_DIR := $(call select_from_ports,afl++)/src/app/afl++
INC_DIR += $(AFL++_DIR)/include

CC_C_OPT += -O3 -Wall -Wno-pointer-sign -Wno-unused-function -Wno-unused-result -fPIC

vpath shm_client.cc $(REP_DIR)/src/shm