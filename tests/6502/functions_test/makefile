
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -i -v -s

SFILES = main.s bonus.s simple.s copy.s
IFILES = 
OFILES = main.o bonus.o simple.o copy.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

bonus.o: bonus.s
	$(CC) $(CFLAGS) bonus.o bonus.s

simple.o: simple.s
	$(CC) $(CFLAGS) simple.o simple.s

copy.o: copy.s
	$(CC) $(CFLAGS) copy.o copy.s

check: linked.rom
	byte_tester -s main.s
	byte_tester -s bonus.s
	byte_tester -s simple.s
	byte_tester -s copy.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym *.lst
