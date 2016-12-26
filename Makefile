
CC = g++
CFLAGS = -std=c++11
SRCS = connection.cxx work.cxx result.cxx const_iterator.cxx field.cxx
OBJS = $(SRCS:.cxx=.o)

all: test
	env LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./test

test: test.cxx liblitepqxx.so
	$(CC) $(CFLAGS) -o test test.cxx -L. -lCppUTest -llitepqxx -lsqlite3

liblitepqxx.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o liblitepqxx.so $(OBJS)

%.o: %.cxx
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f test liblitepqxx.so *.o

