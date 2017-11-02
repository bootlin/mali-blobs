include ../Makefile.setup
include ../config.mk

ifeq ($(MALI_EGL_TYPE),x11)
all:
clean:
install:
	$(MAKE) -C x11 $(MAKECMDGOALS)
else
all:
clean:
install:
	$(MAKE) -C fbdev $(MAKECMDGOALS)
endif