
CC = wla-65816
CFLAGS = -o
LD = wlalink
LDFLAGS = -s

SFILES = main.s
IFILES =
OFILES = main.o

all: result.rom check

result.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

check: result.rom
	./check_symbols.sh result.sym

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym
