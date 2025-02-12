AFL++_DIR := $(call select_from_ports,afl++)/src/app/afl++
CC_OPT += -fplugin=$(AFL++_DIR)/afl-gcc-pass.so