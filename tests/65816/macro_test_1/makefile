
CC = $(WLAVALGRIND) wla-65816
CFLAGS = -v -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -i -s

SFILES = main.s
IFILES = 
OFILES = main.o

all: result.rom check

result.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: result.rom
	byte_tester -s main.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym *.lst

