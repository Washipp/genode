TARGET   = test-afl-compiler-rt
SRC_CC   += main.cc
LIBS     += base

CC_OPT += -fplugin=/home/g-user/code/AFLplusplus/afl-gcc-pass.so

