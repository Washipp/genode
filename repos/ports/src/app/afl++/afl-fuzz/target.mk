TARGET = afl-fuzz
LIBS += base
SRC_CC += main.cc

# AFL++_DIR = contrib/afl++-<hash>/
AFL++_DIR := $(call select_from_ports,afl++)/src/app/afl++

# $(PRG_DIR) = repos/ports/src/app/afl++/afl-fuzz
INC_DIR += $(PRG_DIR)
INC_DIR += $(PRG_DIR)/../libc/include
INC_DIR += $(AFL++_DIR)/include
INC_DIR += $(AFL++_DIR)/instrumentation

# ---------------
# Rules to create `afl-fuzz`
# ---------------
CC_C_OPT += -Wno-pointer-sign

# This variable can improve the speed of the fuzzer depending on the fuzzer. (for example add AVX2 support)
# We leave it empty for now.
SPECIAL_PERFORMANCE =

# These files are already in the inc-dir
# COMM_HDR = include/alloc_inl.h include/config.h include/debug.h include/types.h

AFL_FUZZ_FILES = $(notdir $(wildcard $(AFL++_DIR)/src/afl-fuzz*.c))

#src/afl-performance.o : $(COMM_HDR) src/afl-performance.c include/hash.h
#	$(CC) $(CFLAGS) $(CFLAGS_OPT) $(SPECIAL_PERFORMANCE) -Iinclude -c src/afl-performance.c -o src/afl-performance.o
# We set this flag for the xxh implementation.
#CC_OPT += -DXXH_NO_STDLIB
SRC_C += $(notdir $(AFL++_DIR)/src/afl-performance.c)

#src/afl-common.o : $(COMM_HDR) src/afl-common.c include/common.h
#	$(CC) $(CFLAGS) $(CFLAGS_FLTO) $(SPECIAL_PERFORMANCE) -c src/afl-common.c -o src/afl-common.o
SRC_C += $(notdir $(AFL++_DIR)/src/afl-common.c)

#src/afl-forkserver.o : $(COMM_HDR) src/afl-forkserver.c include/forkserver.h
#	$(CC) $(CFLAGS) $(CFLAGS_FLTO) $(SPECIAL_PERFORMANCE) -c src/afl-forkserver.c -o src/afl-forkserver.o
SRC_C += $(notdir $(AFL++_DIR)/src/afl-forkserver.c)

#src/afl-sharedmem.o : $(COMM_HDR) src/afl-sharedmem.c include/sharedmem.h
#	$(CC) $(CFLAGS) $(CFLAGS_FLTO) $(SPECIAL_PERFORMANCE) -c src/afl-sharedmem.c -o src/afl-sharedmem.o
# We probably need to rewrite this entire file.
SRC_C += $(notdir $(AFL++_DIR)/src/afl-sharedmem.c)


#afl-fuzz: $(COMM_HDR) include/afl-fuzz.h $(AFL_FUZZ_FILES) src/afl-common.o src/afl-sharedmem.o src/afl-forkserver.o src/afl-performance.o
#	$(CC) $(CFLAGS) $(COMPILE_STATIC) $(CFLAGS_FLTO) $(SPECIAL_PERFORMANCE) $(AFL_FUZZ_FILES) src/afl-common.o src/afl-sharedmem.o src/afl-forkserver.o src/afl-performance.o -o $@ $(PYFLAGS) $(LDFLAGS) -lm
CC_OPT += -lm

# Unneeded fuzz files. They contain extra functionality that we do not care about.
FILTER = $(AFL++_DIR)/src/afl-fuzz-statsd.c $(AFL++_DIR)/src/afl-fuzz-python.c $(AFL++_DIR)/src/afl-fuzz-statsd.c
#SRC_C += $(filter-out $(FILTER),$(AFL_FUZZ_FILES))
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz.c)
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-bitmap.c) # worked, probably not necessary
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-cmplog.c) # compiled
#SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-extras.c) # did not compile, but is optional, if no dictionaries are used
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-init.c) # compiled, necessary
#SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-mutators.c) # did not compile, but is only used for custom mutators
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-one.c) # compiled
#SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-python.c) # does not matter, we dont care about python implementation
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-queue.c) # compiled
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-redqueen.c) # compiled
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-run.c) # compiled
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-skipdet.c) # compiled
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-state.c) # necessary
SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-stats.c) # necessary, skipped for now, (see RUSAGE)
#SRC_C += $(notdir $(AFL++_DIR)/src/afl-fuzz-statsd.c) # not needed

vpath %.c $(AFL++_DIR)/src