OS ?= $(shell uname -s)

CFLAGS := -I../_build/headers -I../../basekit/_build/headers
LFLAGS := ../../basekit/_build/lib/libbasekit.a ../_build/lib/libjfile.a 

NEEDS_M := FreeBSD Linux NetBSD
ifneq (,$(findstring $(OS),$(NEEDS_M)))
LFLAGS +=-lm
endif

all:
	$(CC) $(CFLAGS) test.c -o test $(LFLAGS)

clean:
	-rm test

test:
	./test
