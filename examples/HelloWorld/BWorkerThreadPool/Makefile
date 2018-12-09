# tool marcros
CC := g++
CCFLAG := --std=c++11 -Wl,--no-as-needed `pkg-config --cflags protobuf`
DBGFLAG := -g
CCOBJFLAG := $(CCFLAG) -c

# path marcros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug
INC_PATH :=

# librarys
LIBS := -lbthreadpack `pkg-config --libs protobuf`

# protobuf file marcros
PROTOC = /usr/local/bin/protoc
PROTO_NAME := task_data.proto
SRC_PROTO := task_data.pb.cc
OBJ_PROTO := task_data.pb.o

# compile marcros
TARGET_NAME := HelloWorld
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)
MAIN_SRC := src/main.cpp

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*))) $(SRC_PATH)/$(SRC_PROTO)
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := 	$(TARGET) \
		$(TARGET_DEBUG) \
		$(DISTCLEAN_LIST) \
		$(SRC_PATH)/$(SRC_PROTO)

# default rule
default: all

# non-phony targets
$(TARGET): $(OBJ_PATH)/$(OBJ_PROTO) $(OBJ)
	@if [ -d $(BIN_PATH) ]; then :; else mkdir $(BIN_PATH) && echo "Dir bin doesn't exist, create it."; fi
	$(CC) $(CCFLAG) -o $@ $? $(LIBS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	@if [ -d $(OBJ_PATH) ]; then :; else mkdir $(OBJ_PATH) && echo "Dir obj doesn't exist, create it."; fi
	$(CC) $(CCOBJFLAG) -o $@ $< $(LIBS)

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	@if [ -d $(DBG_PATH) ]; then :; else mkdir $(DBG_PATH) && echo "Dir debug doesn't exist, create it."; fi
	$(CC) $(CCOBJFLAG) $(DBGFLAG) -o $@ $< $(LIBS)

$(OBJ_PATH)/$(OBJ_PROTO): $(SRC_PATH)/$(SRC_PROTO)
	$(CC) $(CCOBJFLAG) -o $@ $< $(LIBS)

$(DBG_PATH)/$(OBJ_PROTO): $(SRC_PATH)/$(SRC_PROTO)
	$(CC) $(CCOBJFLAG) $(DBGFLAG) -o $@ $< $(LIBS)

$(TARGET_DEBUG): $(OBJ_DEBUG) $(DBG_PATH)/$(OBJ_PROTO)
	$(CC) $(CCFLAG) $(DBGFLAG) $? -o $@ $(LIBS)

$(SRC_PATH)/$(SRC_PROTO): $(SRC_PATH)/$(PROTO_NAME)
	$(PROTOC) --proto_path=$(SRC_PATH) --cpp_out=$(SRC_PATH) $(SRC_PATH)/$(PROTO_NAME)

# phony rules
.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)
