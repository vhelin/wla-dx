# Auto-generated test: compile all instructions for wla-gb and validate output with byte_tester

CC = $(WLAVALGRIND) wla-gb
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -i -S

SFILES = main.s
OFILES = main.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: linked.rom
	byte_tester -s main.s

clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym main.lst
