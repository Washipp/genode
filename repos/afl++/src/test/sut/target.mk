TARGET   = sut
LIBS     += base
SRC_CC   += main.cc

CC_OPT += -fplugin=/home/g-user/code/AFLplusplus/afl-gcc-pass.so
