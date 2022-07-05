NAME = Pseudo

# Compilers: gcc, g++, clang, clang++
CC = g++
DBG = gdb

CFLAGS = -Wall
INCLUDE = include
SOURCE = src
BIN = bin

# Example: -lncurses -lyaml ...
LIBRARIES = 

ifeq ($(OS), Windows_NT)
	DEST = C:\\Programs\\$(NAME)
	RM = del
	RMDIR = rmdir
	MV = move
	EXECUTABLE = $(NAME).exe
	CP = copy
else
	DEST = /usr/local/bin
	RM = rm
	MV = mv
	EXECUTABLE = $(NAME)
	CP = cp
endif

default: build

build:
ifeq ($(wildcard $(BIN)/.*),)
	@mkdir $(BIN)
endif
	@$(CC) $(CFLAGS) -I $(INCLUDE) -c src/*.c
	@$(CC) *.o $(LIBRARIES) -o $(EXECUTABLE)
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
endif
	@$(CC) $(CFLAGS) -g -I $(INCLUDE) -c src/*.c
	@$(CC) *.o $(LIBRARIES) -o $(EXECUTABLE)
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
ifndef CH
	@read -p "Command line? (y/n) " choice; make install CH=$$choice --no-print-directory
else

ifeq ($(CH), $(filter $(CH), y Y 1))
ifneq ($(shell id -u), 0)
	@echo [Makefile] Root mode required!
else
	@$(CP) ./$(EXECUTABLE) $(DEST)
	@echo [Makefile] Installed
endif
else
	@echo [Makefile] Not installed
endif
endif

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