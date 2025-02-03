LIBS += base libc

SRC_CC += main.cc shm_client.cc
INC_DIR += $(REP_DIR)/src/afl-fuzz/include

# Here we abuse the OpenBSD macro checks in AFL to use libc write() instead of syscall(SYS_write)
CC_OPT += -D__OpenBSD__

AFL++_DIR := $(call select_from_ports,afl++)/src/app/afl++
INC_DIR += $(AFL++_DIR)/include

SRC_C += $(notdir $(AFL++_DIR)/instrumentation/afl-compiler-rt.o.c)
CC_C_OPT += -O3 -Wall -Wno-pointer-sign -Wno-unused-function -Wno-unused-result -fPIC

# Removes "warning: constructor priorities from 0 to 100 are reserved for the implementation"
CC_C_OPT += -Wno-prio-ctor-dtor

vpath %.c $(AFL++_DIR)/instrumentation/
vpath %.cc $(REP_DIR)/src/afl-compiler-rt/