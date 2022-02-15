
CC = $(WLAVALGRIND) wla-65816
CFLAGS = -h -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s
OFILES = main.o

all: result.rom check

result.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: result.rom
	byte_tester -s main.s

$(OFILES): $(HFILES)

clean:
	rm -f $(OFILES) core *~ linked.rom *.sym

