OS ?= $(shell uname -s)

CFLAGS := -I../_build/headers -I../../basekit/_build/headers -I../../jfile/_build/headers -I../../udb/_build/headers
LFLAGS := -L../_build/lib -lskipdb -L../_build/lib -ludb -L../../jfile/_build/lib -ljfile -L../../basekit/_build/lib -lbasekit

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
