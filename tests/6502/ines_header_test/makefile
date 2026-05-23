CC = $(WLAVALGRIND) wla-6502
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s
OFILES = main.o

all: linked.nes check

linked.nes: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.nes

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: linked.nes
	byte_tester testsfile

clean:
	rm -f $(OFILES) core *~ linked.nes *.sym