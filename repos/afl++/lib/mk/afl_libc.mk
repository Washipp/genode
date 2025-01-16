# This file actually contains the build instructions
# It does not specify the variable TARGET.
LIBS += base syscall-linux

IGNORE_FILES = afl-file-system.cc
ALL_SRC_FILES = $(notdir $(wildcard $(REP_DIR)/src/afl_libc/*.cc))

INC_DIR +=  $(REP_DIR)/include

SRC_CC += $(filter-out $(IGNORE_FILES), $(ALL_SRC_FILES))

vpath %.cc $(REP_DIR)/src/afl_libc
