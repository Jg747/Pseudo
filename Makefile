TARGET = pseudo
ifeq ($(SYSTEM),Windows_NT)
	TARGET := $(TARGET).exe
endif

# release, debug
MODE ?= release

SRC_DIR = src
INC_DIR = include
LIB_DIR = libs
OBJ_DIR = bin

CXX = g++
CXXFLAGS = -std=c++20 -Wall -I$(INC_DIR)
LDFLAGS = -L$(LIB_DIR)
LDLIBS =

ifeq ($(MODE),debug)
CXXFLAGS += -g -O0 -DDEBUG
else ifeq ($(MODE),release)
CXXFLAGS += -O2 -DNDEBUG
else
$(error MODE should be 'debug' or 'release')
endif

rwildcard = $(foreach d,$(wildcard $(1)/*),$(call rwildcard,$d,$2)) \
            $(filter $(subst *,%,$2),$(wildcard $(1)/$2))

SRCS = $(call rwildcard,$(SRC_DIR),*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild