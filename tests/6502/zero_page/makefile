
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s
IFILES = defines.i
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s defines.i
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom *.sym
