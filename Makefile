PROG = palscreen
SOURCES = $(PROG).c gfx.c touch.c libevdev/libevdev-names.c libevdev/libevdev-uintput.c libevdev/libevdev.c
CFLAGS = -W -Wall -std=c99 —I=libevdev $(CFLAGS_EXTRA)

all: $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)