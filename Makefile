UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	SDL_LIB := lib/linux/Debian
	LDFLAGS=-L$(SDL_LIB) -lSDL2
endif

ifeq ($(UNAME),Darwin)
    FRAMEWORK_DIR := lib/mac
    LDFLAGS = -F$(FRAMEWORK_DIR) -framework SDL2
endif

ifeq ($(UNAME), Windows_NT)
	SDL_LIB := lib/windows
	LDFLAGS=-L$(SDL_LIB) -lSDL2
endif

CFLAGS=-Wall -Wextra -pedantic -std=c11 -Iinclude
OBJS=src/main.o src/gameboy.o src/opcodes.o

TARGET=main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: test.c
	$(CC) $(CFLAGS) test.c -o test $(LDFLAGS)