
CC = g++
CFLAGS = -std=c++11
SRC = src
OBJ = obj

SRCFILES = $(wildcard $(SRC)/*.cxx)
OBJFILES = $(patsubst $(SRC)/%.cxx, $(OBJ)/%.o, $(SRCFILES))

all: test.exe
	env LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./test.exe

test.exe: test.cxx liblitepqxx.so
	$(CC) $(CFLAGS) -o test.exe test.cxx -L. -lCppUTest -llitepqxx -lsqlite3

liblitepqxx.so: $(OBJFILES)
	$(CC) $(CFLAGS) -shared -o liblitepqxx.so $(OBJFILES)

$(OBJ)/%.o: $(SRC)/%.cxx $(OBJ)
	$(CC) $(CFLAGS) -o $@ -c $< -I.

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -f test.exe liblitepqxx.so
	rm -rf $(OBJ)

