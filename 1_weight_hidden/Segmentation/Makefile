# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra  -std=c99 -g -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

all:  segmentation

segmentation: ../ImageProcessing/pixel_operations.c ../ImageProcessing/binary.c segmentation.c save_letter.c

clean:
	${RM} *.o
	${RM} *.d
	${RM} segmentation

# END
