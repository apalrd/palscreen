PROG = palscreen
SOURCES = $(PROG).c gfx.c touch.c libevdev/libevdev-names.c libevdev/libevdev-uinput.c libevdev/libevdev.c
CFLAGS = -W -Wall -std=c99 —Ilibevdev $(CFLAGS_EXTRA)

all: $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)