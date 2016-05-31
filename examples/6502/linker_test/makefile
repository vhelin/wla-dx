
CC = wla-6502
CFLAGS = -i -o
LD = wlalink
LDFLAGS = -v -i

SFILES = setup.s linker_test.s
IFILES = defines.i
OFILES = setup.o linker_test.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

linker_test.o: linker_test.s defines.i
	$(CC) $(CFLAGS) linker_test.o linker_test.s

setup.o: setup.s defines.i
	$(CC) $(CFLAGS) setup.o setup.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom *.lst
