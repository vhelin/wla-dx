
CC = $(WLAVALGRIND) wla-8008
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -d -s

SFILES = main.s
IFILES = 
OFILES = main.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: linked.rom
	byte_tester -s main.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
