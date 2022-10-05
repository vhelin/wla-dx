
CC = $(WLAVALGRIND) wla-z80
CFLAGS = -v -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -i -s

SFILES = main.s more.s
IFILES = 
OFILES = main.o more.o

all: result.rom check

result.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

check: result.rom
	byte_tester -s main.s

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

more.o: more.s
	$(CC) $(CFLAGS) more.o more.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym *.lst

