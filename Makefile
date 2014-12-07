PROG = palscreen
SOURCES = $(PROG).c gfx.c glcdfont.c
CFLAGS = -W -Wall $(CFLAGS_EXTRA)

all: $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)