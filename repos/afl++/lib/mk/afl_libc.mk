# This file actually contains the build instructions
# It does not specify the variable TARGET.
LIBS += base syscall-linux

IGNORE_CC_FILES = afl-file-system.cc
ALL_SRC_FILES = $(notdir $(wildcard $(REP_DIR)/src/afl_libc/*.cc))

INC_DIR +=  $(REP_DIR)/include/afl_libc

SRC_CC += $(filter-out $(IGNORE_CC_FILES), $(ALL_SRC_FILES))

vpath %.cc $(REP_DIR)/src/afl_libc
