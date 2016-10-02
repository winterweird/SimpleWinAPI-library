ARCHIVER = ar
AFLAGS = crv
CC = g++
CCFLAGS = -I$(INCLUDE_PATH) -L$(LIB_PATH)
INCLUDE_PATH = ./include/
SRC_PATH = ./src/
BIN_PATH = ./bin/
LIB_PATH = ./lib/
OBJFILES = $(BIN_PATH)winapiutil.o $(BIN_PATH)Console.o $(BIN_PATH)Console_clear.o $(BIN_PATH)Console_helper.o $(BIN_PATH)CursorPosition.o
LIBOUT_BASE = simplewinapi
LIBOUT = $(LIB_PATH)lib$(LIBOUT_BASE).a
TESTFILE = test2.cpp
BIN_EXT = .exe

all: $(LIBOUT)

test: $(LIBOUT)
	$(CC) $(CCFLAGS) $(SRC_PATH)$(TESTFILE) -l$(LIBOUT_BASE) -o $(BIN_PATH)$(TESTFILE:.cpp=$(BIN_EXT))

$(BIN_PATH)%.o: $(SRC_PATH)%.cpp
	$(CC) -c $(CCFLAGS) $< -o $@

$(LIBOUT): $(OBJFILES)
	$(ARCHIVER) $(AFLAGS) $@ $(OBJFILES)

# dependencies
$(BIN_PATH)Console.o: $(BIN_PATH)Console_helper.o $(INCLUDE_PATH)Console.hpp $(BIN_PATH)CursorPosition.o $(BIN_PATH)Console_clear.o
$(BIN_PATH)Console_helper.o: $(INCLUDE_PATH)Console.hpp $(INCLUDE_PATH)WinAPIException.hpp
$(BIN_PATH)winapiutil.o: $(BIN_PATH)Console.o $(INCLUDE_PATH)winapiutil.hpp
$(BIN_PATH)CursorPosition.o: $(INCLUDE_PATH)CursorPosition.hpp
$(BIN_PATH)Console_clear.o: $(INCLUDE_PATH)Console.hpp $(INCLUDE_PATH)WinAPIException.hpp