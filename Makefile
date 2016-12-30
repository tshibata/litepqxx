
CC = g++
CFLAGS = -std=c++11
SRCS = connection.cxx work.cxx prepare.cxx result.cxx const_iterator.cxx field.cxx binarystring.cxx
OBJS = $(SRCS:.cxx=.o)

all: test.exe
	env LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./test.exe

test.exe: test.cxx liblitepqxx.so
	$(CC) $(CFLAGS) -o test.exe test.cxx -L. -lCppUTest -llitepqxx -lsqlite3

liblitepqxx.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o liblitepqxx.so $(OBJS)

%.o: %.cxx
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f test.exe liblitepqxx.so *.o

