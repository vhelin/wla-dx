
CC = $(WLAVALGRIND) wla-gb
CFLAGS = -i -x -o
LD = $(WLAVALRGIND) wlalink
LDFLAGS = -i -v -c -s

SFILES = bank0.s bank1.s
IFILES = 
OFILES = bank0.o bank1.o

all: linked.gb check

linked.gb: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.gb

bank0.o: bank0.s
	$(CC) $(CFLAGS) bank0.o bank0.s

bank1.o: bank1.s
	$(CC) $(CFLAGS) bank1.o bank1.s

check: linked.gb


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.gb linked.sym bank0.lst bank1.lst
