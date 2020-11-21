# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: binary

binary: binary.o pixel_operations.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} binary

# END