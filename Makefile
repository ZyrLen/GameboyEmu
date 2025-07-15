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

CFLAGS=-Iinclude/SDL2 -Wall -Wextra -pedantic -std=c99

main: src/main.c
	$(CC) $(CFLAGS) src/main.c -o main $(LDFLAGS)

test: test.c
	$(CC) $(CFLAGS) test.c -o test $(LDFLAGS)