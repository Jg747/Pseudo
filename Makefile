NAME = pseudo

# Compilers: gcc, g++, clang, clang++
CC = g++
DBG = gdb

CFLAGS = -Wall -std=c++20
INCLUDE = include
SOURCE = src
BIN = bin

# Example: -lncurses -lyaml ...
LIBRARIES = -lncurses

RM = rm
MV = mv
CP = cp

ifeq ($(OS), Windows_NT)
	DEST = C:\\Programs\\$(NAME)
	EXECUTABLE = $(NAME).exe
	MK = mingw32-make
	OTHER = cparse/shunting-yard.cpp cparse/packToken.cpp cparse/functions.cpp cparse/containers.cpp
else
	DEST = /usr/local/bin
	EXECUTABLE = $(NAME)
	MK = make
	OTHER = $(BIN)/builtin-features.o $(BIN)/core-shunting-yard.o
endif

default: build

build:
ifeq ($(wildcard $(BIN)/.*),)
	@mkdir $(BIN)
	@cd $(INCLUDE)/cparse && $(MK) --no-print-directory
	@$(MV) $(INCLUDE)/cparse/*.o $(BIN)
endif
	@$(CC) $(CFLAGS) -I $(INCLUDE) -c src/*.cpp
	@$(CC) *.o $(LIBRARIES) $(OTHER) -o $(EXECUTABLE)
	@$(MV) *.o $(BIN)
	@echo [Makefile] Done

run:
ifeq ($(OS), Windows_NT)
	$(EXECUTABLE)
else
	./$(EXECUTABLE)
endif

debug:
ifeq ($(wildcard $(BIN)/.*),)
	@mkdir $(BIN)
	@cd $(INCLUDE)/cparse && $(MK) --no-print-directory
	@$(MV) $(INCLUDE)/cparse/*.o $(BIN)
endif
	@$(CC) $(CFLAGS) -I $(INCLUDE) -c src/*.cpp
	@$(CC) *.o $(LIBRARIES) $(OTHER) -o $(EXECUTABLE)
	@$(MV) *.o $(BIN)

ifeq ($(OS), Windows_NT)
	@$(DBG) $(EXECUTABLE)
else
	@$(DBG) ./$(EXECUTABLE)
endif

setup:
ifeq ($(wildcard $(SOURCE)/.*),)
	@mkdir $(SOURCE)
endif
ifeq ($(wildcard $(INCLUDE)/.*),)
	@mkdir $(INCLUDE)
endif
	@echo [Makefile] Done

clean:
ifeq ($(wildcard $(BIN)/.*),)
	@$(RM) $(EXECUTABLE)

ifeq ($(OS), Windows_NT)
	@$(RM) $(BIN)/* /q
	@$(RMDIR) $(BIN) /q
else
	@$(RM) -r $(BIN)
endif

endif
	@echo [Makefile] Done

install:
ifeq ($(OS), Windows_NT)
ifndef cmd
	# Add to cmd
endif
	# rest of the installation
else
	@$(CP) ./$(EXECUTABLE) $(DEST)
endif
	@echo [Makefile] Installed

remove:
ifneq ($(wildcard $(DEST)/.*),)
ifeq ($(OS), Windows_NT)
	@$(RM) $(DEST) /q
else
ifneq ($(shell id -u), 0)
	@echo [Makefile] Root mode required!
else
	@$(RM) $(DEST)/$(EXECUTABLE)
	@echo [Makefile] Done
endif
endif
else
	@echo [Makefile] Program not installed
endif