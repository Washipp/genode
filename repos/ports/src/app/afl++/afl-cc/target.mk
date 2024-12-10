TARGET = afl-cc
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
# Rules to create `afl-cc` / `afl-g++-fast` / `afl-gcc-fast`
# ---------------

#instrumentation/afl-common.o: ./src/afl-common.c
#	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LDFLAGS)
#
#./afl-cc: src/afl-cc.c instrumentation/afl-common.o
#	$(CC) $(CFLAGS) $(CPPFLAGS) $< instrumentation/afl-common.o -o $@ $(LDFLAGS) -lm
#CFLAGS          ?= -O3 -funroll-loops -fPIC
## -D_FORTIFY_SOURCE=1
#CFLAGS_SAFE     := -Wall -g -Wno-cast-qual -Wno-variadic-macros -Wno-pointer-sign \
#                   -I ./include/ -I ./instrumentation/ \
#                   -DAFL_PATH=\"$(HELPER_PATH)\" -DBIN_PATH=\"$(BIN_PATH)\" \
#                   -DLLVM_BINDIR=\"$(LLVM_BINDIR)\" -DVERSION=\"$(VERSION)\" \
#                   -DLLVM_LIBDIR=\"$(LLVM_LIBDIR)\" -DLLVM_VERSION=\"$(LLVMVER)\" \
#                   -DAFL_CLANG_FLTO=\"$(AFL_CLANG_FLTO)\" -DAFL_REAL_LD=\"$(AFL_REAL_LD)\" \
#                   -DAFL_CLANG_LDPATH=\"$(AFL_CLANG_LDPATH)\" -DAFL_CLANG_FUSELD=\"$(AFL_CLANG_FUSELD)\" \
#                   -DCLANG_BIN=\"$(CLANG_BIN)\" -DCLANGPP_BIN=\"$(CLANGPP_BIN)\" -DUSE_BINDIR=$(USE_BINDIR) \
#                   -Wno-unused-function $(AFL_CLANG_DEBUG_PREFIX)

## AFL requires certain variables to be set.
#BIN_PATH 		= "BIN_PATH/dir/bin"
#AFL_CLANG_FLTO 	= "AFL_CLANG_FLTO/dir/bin"
#AFL_PATH 		= "AFL_PATH/dir/bin"
#USE_BINDIR 		= "USE_BINDIR/dir/bin"
#LLVM_BINDIR 	= "LLVM_BINDIR/dir/bin"
#CLANGPP_BIN 	= "CLANGPP_BIN/dir/bin"
#CLANG_BIN 		= "CLANG_BIN/dir/bin"
#AFL_REAL_LD 	= "AFL_REAL_LD/dir/bin"
#LLVM_LIBDIR 	= "LLVM_LIBDIR/dir/bin"
#
#CC_OPT += -O3 -funroll-loops -fPIC \
#          -Wall -g -Wno-cast-qual -Wno-variadic-macros -Wno-pointer-sign \
#          -DBIN_PATH=\"$(BIN_PATH)\" -DAFL_CLANG_FLTO=\"$(AFL_CLANG_FLTO)\" \
#          -DAFL_PATH=\"$(AFL_PATH)\" -DUSE_BINDIR=\"$(USE_BINDIR)\" \
#          -DLLVM_BINDIR=\"$(LLVM_BINDIR)\" -DCLANGPP_BIN=\"$(CLANGPP_BIN)\"\
#          -DCLANG_BIN=\"$(CLANG_BIN)\" -DAFL_REAL_LD=\"$(AFL_REAL_LD)\"
#
#SRC_C += $(AFL++_DIR)/src/afl-common.c $(AFL++_DIR)/src/afl-cc.c


#afl-common.o: ./src/afl-common.c
#	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LDFLAGS)
#
#./afl-compiler-rt.o: instrumentation/afl-compiler-rt.o.c
#	$(CC) $(CFLAGS_SAFE) $(CPPFLAGS) -O3 -Wno-unused-result -fPIC -c $< -o $@
#
#./afl-compiler-rt-32.o: instrumentation/afl-compiler-rt.o.c
#	@printf "[*] Building 32-bit variant of the runtime (-m32)... "
#	@$(CC) $(CFLAGS_SAFE) $(CPPFLAGS) -O3 -Wno-unused-result -m32 -fPIC -c $< -o $@ 2>/dev/null; if [ "$$?" = "0" ]; then echo "success!"; else echo "failed (that's fine)"; fi
#
#./afl-compiler-rt-64.o: instrumentation/afl-compiler-rt.o.c
#	@printf "[*] Building 64-bit variant of the runtime (-m64)... "
#	@$(CC) $(CFLAGS_SAFE) $(CPPFLAGS) -O3 -Wno-unused-result -m64 -fPIC -c $< -o $@ 2>/dev/null; if [ "$$?" = "0" ]; then echo "success!"; else echo "failed (that's fine)"; fi
#
#$(PASSES): instrumentation/afl-gcc-common.h
#
#./afl-gcc-pass.so: instrumentation/afl-gcc-pass.so.cc | test_deps
#	$(CXX) $(CXXEFLAGS) $(PLUGIN_FLAGS) -shared $< -o $@
#	ln -sf afl-cc afl-gcc-fast
#	ln -sf afl-cc afl-g++-fast
#	ln -sf afl-cc.8 afl-gcc-fast.8
#	ln -sf afl-cc.8 afl-g++-fast.8
#
#./afl-gcc-cmplog-pass.so: instrumentation/afl-gcc-cmplog-pass.so.cc | test_deps
#	$(CXX) $(CXXEFLAGS) $(PLUGIN_FLAGS) -shared $< -o $@
#
#./afl-gcc-cmptrs-pass.so: instrumentation/afl-gcc-cmptrs-pass.so.cc | test_deps
#	$(CXX) $(CXXEFLAGS) $(PLUGIN_FLAGS) -shared $< -o $@

#INC_DIR += $(AFL++_DIR)/instrumentation
