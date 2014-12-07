PROG = palscreen
SOURCES = $(PROG).c gfx.c
CFLAGS = -W -Wall -std=C99 $(CFLAGS_EXTRA)

all: $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)