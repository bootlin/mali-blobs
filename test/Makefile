include ../Makefile.setup
include ../config.mk

CFLAGS ?= -Wall

all: test

test: ../config.mk test.c
ifeq ($(MALI_EGL_TYPE),x11)
	$(CC) $(CFLAGS) -o $@ test.c -lEGL -lGLESv2 -lX11
else
	$(CC) $(CFLAGS) -o $@ test.c -lEGL -lGLESv2
endif

clean:
	rm -f test
