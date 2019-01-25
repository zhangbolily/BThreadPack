# path marcros
LIB_PATH := lib
SYS_LIB_PATH := /usr/local/lib
SYS_INC_PATH := /usr/local/include

# libraries
LIBS := -lrt

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
OBJS := obj/BAbstractTask.o obj/BAbstractThreadPool.o obj/BGeneralThreadPool.o obj/BGeneralTask.o obj/BTimer.o obj/BWorkerTask.o
ALL_OBJS_DIR := obj/

# intermedia compile marcros
# NOTE: ALL_OBJS are intentionally left blank, no need to fill it
ALL_OBJS :=
CLEAN_FILES := $(TARGET)
DIST_CLEAN_FILES :=

# recursive wildcard
rwildcard=$(foreach d,$(wildcard $(addsuffix *,$(1))),$(call rwildcard,$(d)/,$(2))$(filter $(subst *,%,$(2)),$(d)))

# default target
default: show-info all

# non-phony targets
$(TARGET): $(ALL_OBJS_DIR) $(OBJS)
	@echo -e "\t" CC $(CCFLAG) $(OBJS) $(LIBS) -shared -o $@
	@if [ -d $(LIB_PATH) ]; then :; else mkdir $(LIB_PATH) && echo "Folder lib doesn't exist, creating it."; fi
	@$(CC) $(CCFLAG) $(OBJS) $(LIBS) -shared -o $@

$(ALL_OBJS_DIR)%.o: $(DIRS)/%.c*
	@echo -e "\t" CC $(OBJCCFLAG) $< -o $@   
	@$(CC) $(OBJCCFLAG) $< -o $@

# phony targets
.PHONY: all
all: $(TARGET)
	@echo Target $(TARGET) build finished.

.PHONY: clean
clean: find-all-objs
	@echo CLEAN $(CLEAN_FILES) $(LIB_PATH) $(ALL_OBJS)
	@rm -rf $(CLEAN_FILES) $(LIB_PATH) $(ALL_OBJS)

.PHONY: distclean
distclean:
	@echo CLEAN $(DIST_CLEAN_FILES) $(LIB_PATH)
	@rm -rf $(DIST_CLEAN_FILES) $(LIB_PATH)
	
.PHONY: install
install: all
	@echo -e Install libbthreadpack.so and header files into /usr/local
	@rm -rf $(SYS_INC_PATH)/BThreadPack
	@mkdir -p $(SYS_INC_PATH)/BThreadPack
	@cp -rf $(TARGET) $(SYS_LIB_PATH)
	@cp -rf include/* $(SYS_INC_PATH)/BThreadPack
	
.PHONY: $(ALL_OBJS_DIR)
$(ALL_OBJS_DIR):
	@if [ -d $(ALL_OBJS_DIR) ]; then :; else mkdir $(ALL_OBJS_DIR) && echo "Folder obj doesn't exist, creating it."; fi

# phony funcs
.PHONY: find-all-objs
find-all-objs:
	$(eval ALL_OBJS += $(call rwildcard,$(ALL_OBJS_DIR),*.o))

.PHONY: show-info
show-info:
	@echo Building Project BThreadPack......

# need to be placed at the end of the file
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
export PROJECT_PATH := $(patsubst %/,%,$(dir $(mkfile_path)))
export MAKE_INCLUDE=$(PROJECT_PATH)/config/make.global
include $(MAKE_INCLUDE)
