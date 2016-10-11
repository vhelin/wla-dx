
CC = wla-gb
CFLAGS = -x -o
LD = wlalink
LDFLAGS = -v -s

SFILES = main.s
IFILES = defines.i
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.gb

main.o: main.s defines.i
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.gb result.sym

