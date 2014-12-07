PROG = palscreen
SOURCES = $(PROG).c gfx.c touch.c
CFLAGS = -W -Wall -std=c99 —I=libevdev $(CFLAGS_EXTRA)

all: $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)