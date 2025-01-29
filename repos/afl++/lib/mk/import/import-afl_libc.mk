# When "afl-libc" is included as a LIBS, this file is automatically imported.
# I probably want to add these values here:

AFL++_DIR := $(call select_from_ports,afl++)/src/app/afl++

#LIBS   += afl_libc libc

#CC_OPT += -fplugin=$(AFL++_DIR)/afl-gcc-pass.so

# Maybe instead of linking here, build it!
#SRC_O += /home/g-user/code/genode/build/x86_64/afl-compiler-rt/afl-compiler-rt.o.o