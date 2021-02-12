
CC = wla-8008
CFLAGS = -x -v -o
LD = wlalink
LDFLAGS = -v -d -s

SFILES = main.s
IFILES = 
OFILES = main.o library.lib

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

library.lib: library.s
	$(CC) -l library.lib library.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
