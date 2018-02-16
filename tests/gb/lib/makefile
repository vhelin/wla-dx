
CC = wla-gb
CFLAGS = -l

LFILES = speed.lib memcpy.lib spritedma.lib vbi.lib palette.lib screen.lib


all: $(LFILES) makefile


speed.lib: src/speed.s
	$(CC) $(CFLAGS) speed.lib src/speed.s

memcpy.lib: src/memcpy.s
	$(CC) $(CFLAGS) memcpy.lib src/memcpy.s

vbi.lib: src/vbi.s
	$(CC) $(CFLAGS) vbi.lib src/vbi.s

spritedma.lib: src/spritedma.s
	$(CC) $(CFLAGS) spritedma.lib src/spritedma.s

palette.lib: src/palette.s
	$(CC) $(CFLAGS) palette.lib src/palette.s

screen.lib: src/screen.s
	$(CC) $(CFLAGS) screen.lib src/screen.s


clean:
	rm -f $(LFILES) core src/*~
