
CC = $(WLAVALGRIND) wla-spc700
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -S

SFILES = setup.s main.s
IFILES = defines.i
OFILES = main.o setup.o

all: result.rom check

result.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s defines.i
	$(CC) $(CFLAGS) main.o main.s

setup.o: setup.s defines.i
	$(CC) $(CFLAGS) setup.o setup.s

check: result.rom
	byte_tester -s main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym
