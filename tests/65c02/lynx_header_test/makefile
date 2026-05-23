CC = $(WLAVALGRIND) wla-65c02
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s
OFILES = main.o

all: linked.lnx check

linked.lnx: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.lnx

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: linked.lnx
	byte_tester testsfile

clean:
	rm -f $(OFILES) core *~ linked.lnx *.sym