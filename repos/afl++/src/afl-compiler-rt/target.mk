TARGET = afl-compiler-rt
LIBS += base afl_libc

AFL++_DIR := $(call select_from_ports,afl++)/src/app/afl++

INC_DIR += $(AFL++_DIR)/include
INC_DIR += $(REP_DIR)/include/afl_libc

SRC_C += $(notdir $(AFL++_DIR)/instrumentation/afl-compiler-rt.o.c)
CC_C_OPT += -O3 -Wall -Wno-pointer-sign -Wno-unused-function -Wno-unused-result -fPIC

# Removes "warning: constructor priorities from 0 to 100 are reserved for the implementation"
CC_C_OPT += -Wno-prio-ctor-dtor

vpath %.c $(AFL++_DIR)/instrumentation/

