INC_DIR += $(REP_DIR)/include/shm
INC_DIR += $(REP_DIR)/include/afl_libc
INC_DIR += $(REP_DIR)/include/compiler_rt

AFL++_DIR := $(call select_from_ports,afl++)/src/app/afl++
CC_OPT += -fplugin=$(AFL++_DIR)/afl-gcc-pass.so
