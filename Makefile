PROG = palscreen
SOURCES = $(PROG).c gfx.c backlight.c touch.c touchstk.c
KAL = kalibrate
KALSRC = $(KAL).c gfx.c backlight.c touch.c
CFLAGS = -W -Wall -std=c99 $(CFLAGS_EXTRA)

all: $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)
	$(CC) -o $(KAL) $(KALSRC) $(CFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG) $(KAL)