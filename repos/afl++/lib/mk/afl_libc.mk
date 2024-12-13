# This file actually contains the build instructions
# It does not specify the variable TARGET.
LIBS += base

SRC_CC += $(notdir $(wildcard $(REP_DIR)/src/*.cc))

vpath %.cc $(REP_DIR)/src