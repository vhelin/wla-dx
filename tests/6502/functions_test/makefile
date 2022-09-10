
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s bonus.s
IFILES = 
OFILES = main.o bonus.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

bonus.o: bonus.s
	$(CC) $(CFLAGS) bonus.o bonus.s

check: linked.rom
	byte_tester -s main.s
	byte_tester -s bonus.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
