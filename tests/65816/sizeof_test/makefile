
CC = $(WLAVALGRIND) wla-65816
CFLAGS = -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s ram.s
IFILES = 
OFILES = main.o ram.o

all: linked.gb check

linked.gb: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.gb

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

ram.o: ram.s
	$(CC) $(CFLAGS) ram.o ram.s

check: linked.gb
	byte_tester -s main.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.gb linked.sym
