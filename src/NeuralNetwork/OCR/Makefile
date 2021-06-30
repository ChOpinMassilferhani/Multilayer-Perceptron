# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra  -std=c99 -g -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

all:  OCR

OCR: OCR.c ../Network/network.c ../Tools/matrice.c ../Network/save.c ../Tools/image_matrice.c ../../ImageProcessing/binary.c ../../ImageProcessing/pixel_operations.c

clean:
	${RM} *.o
	${RM} *.d
	${RM} XOR
	${RM} OCR

# END
