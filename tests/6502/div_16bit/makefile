
CC = wla-6502
CFLAGS = -o
LD = wlalink
LDFLAGS = -v -s

SFILES = main.s
OFILES = main.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check:
	byte_tester -s main.s

$(OFILES): $(HFILES)

clean:
	rm -f $(OFILES) core *~ linked.rom *.sym

