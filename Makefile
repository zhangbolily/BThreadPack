# path marcros
LIB_PATH := lib
SYS_LIB_PATH := /usr/local/lib
SYS_INC_PATH := /usr/local/include

# target marcros
TARGET_NAME := libbthreadpack
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .dll,$(TARGET_NAME))
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET_NAME := $(addsuffix .so,$(TARGET_NAME))
	endif
endif
TARGET := $(LIB_PATH)/$(TARGET_NAME)
MAIN_SRC := # FILL: src file which contains `main()`

# compile marcros
DIRS := src
OBJS :=

# intermedia compile marcros
# NOTE: ALL_OBJS are intentionally left blank, no need to fill it
ALL_OBJS :=
CLEAN_FILES := $(TARGET) $(OBJS)
DIST_CLEAN_FILES := $(OBJS)

# recursive wildcard
rwildcard=$(foreach d,$(wildcard $(addsuffix *,$(1))),$(call rwildcard,$(d)/,$(2))$(filter $(subst *,%,$(2)),$(d)))

# default target
default: show-info all

# non-phony targets
$(TARGET): build-subdirs $(OBJS) find-all-objs
	@echo -e "\t" mkdir $(LIB_PATH)
	@echo -e "\t" CC $(CCFLAG) $(ALL_OBJS) -shared -o $@
	@rm -rf $(LIB_PATH)
	@mkdir $(LIB_PATH)
	@$(CC) $(CCFLAG) $(ALL_OBJS) -shared -o $@

# phony targets
.PHONY: all
all: $(TARGET)
	@echo Target $(TARGET) build finished.

.PHONY: clean
clean: clean-subdirs
	@echo CLEAN $(CLEAN_FILES) $(LIB_PATH)
	@rm -rf $(CLEAN_FILES) $(LIB_PATH)

.PHONY: distclean
distclean: clean-subdirs
	@echo CLEAN $(DIST_CLEAN_FILES) $(LIB_PATH)
	@rm -rf $(DIST_CLEAN_FILES) $(LIB_PATH)
	
.PHONY: install
install: all
	@echo -e Install libbthreadpack.so and header files into /usr/local
	@rm -rf $(SYS_INC_PATH)/BThreadPack
	@mkdir -p $(SYS_INC_PATH)/BThreadPack
	@cp -rf $(TARGET) $(SYS_LIB_PATH)
	@cp -rf include/* $(SYS_INC_PATH)/BThreadPack

# phony funcs
.PHONY: find-all-objs
find-all-objs: build-subdirs
	$(eval ALL_OBJS += $(call rwildcard,$(DIRS),*.o))

.PHONY: show-info
show-info:
	@echo Building Project

# need to be placed at the end of the file
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
export PROJECT_PATH := $(patsubst %/,%,$(dir $(mkfile_path)))
export MAKE_INCLUDE=$(PROJECT_PATH)/config/make.global
export SUB_MAKE_INCLUDE=$(PROJECT_PATH)/config/submake.global
include $(MAKE_INCLUDE)
