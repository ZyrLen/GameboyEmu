UNAME := $(shell uname)

LDFLAGS=`pkg-config --libs sdl3`
CFLAGS=-g -Wall -Wextra -pedantic -std=c11 -Iinclude
OBJS=src/main.o src/gameboy.o src/opcodes.o src/CBopcodes.o

TARGET=main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: test.c
	$(CC) $(CFLAGS) test.c -o test $(LDFLAGS)
