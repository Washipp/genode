TARGET = afl-fuzz
SRC_CC += main.cc
LIBS += base libc

AFL++_DIR := $(call select_from_ports,afl++)/src/app/afl++
INC_DIR += $(AFL++_DIR)/include

PREFIX      ?= /usr/local
BIN_PATH     = $(PREFIX)/bin
DOC_PATH     = $(PREFIX)/share/doc/afl

CC_C_OPT += -DBIN_PATH=\"$(BIN_PATH)\" -DDOC_PATH=\"$(DOC_PATH)\"

# The following libs have also been included. We leave them for now
#  -ldl -lrt -lm -lz -lm
CC_C_OPT += -Wno-format-truncation -g -Wno-pointer-sign -Wno-variadic-macros -Wall -Wextra -Wno-pointer-arith -fPIC

# This variable can improve the speed of the fuzzer depending on the fuzzer. (for example add AVX2 support)
# We leave it empty for now.
SPECIAL_PERFORMANCE = #-DHAVE_ZLIB

SRC_C += afl-performance.c afl-common.c afl-forkserver.c afl-sharedmem.c

# Unneeded fuzz files. They contain extra functionality that we do not care about.
NOT_REQUIRED += afl-fuzz-extras.c afl-fuzz-mutators.c afl-fuzz-python.c afl-fuzz-statsd.c

AFL_FUZZ_FILES = $(notdir $(wildcard $(AFL++_DIR)/src/afl-fuzz*.c))
SRC_C += $(AFL_FUZZ_FILES)

vpath %.c $(AFL++_DIR)/src


