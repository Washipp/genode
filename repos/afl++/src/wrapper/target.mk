TARGET = wrapper
SRC_CC += wrapper.cc shm_client.cc
LIBS += base libc

INC_DIR += $(REP_DIR)/include/shm
INC_DIR += $(REP_DIR)/include/afl_libc

AFL++_DIR := $(call select_from_ports,afl++)/src/app/afl++
INC_DIR += $(AFL++_DIR)/include
INC_DIR += $(AFL++_DIR)/instrumentation

SRC_O += /home/g-user/code/genode/build/x86_64/test/sut/main

vpath shm_client.cc $(REP_DIR)/src/shm
#vpath main.cc $(REP_DIR)/src/test/sut