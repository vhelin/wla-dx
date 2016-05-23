
CC = wla-spc700
CFLAGS = -o
LD = wlalink
LDFLAGS = -v -S

SFILES = setup.s main.s
IFILES = defines.i
OFILES = main.o setup.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s defines.i
	$(CC) $(CFLAGS) main.o main.s

setup.o: setup.s defines.i
	$(CC) $(CFLAGS) setup.o setup.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym
