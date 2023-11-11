
CC = $(WLAVALGRIND) wla-huc6280
CFLAGS = -D V1=ffeeddcch -D V2=%11111111000000001111111100000000 -D V3=0x01020304 -D V4=0b10101010101010101010101010101010 -i -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -S -i

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
