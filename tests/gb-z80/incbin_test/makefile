
CC = $(WLAVALGRIND) wla-gb
CFLAGS = -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -d -v -s

SFILES = main.s next.s
IFILES = 
OFILES = main.o next.o

all: linked.gb check

linked.gb: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.gb

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

next.o: next.s
	$(CC) $(CFLAGS) next.o next.s

check: linked.gb
	byte_tester -s main.s
	byte_tester -s next.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.gb linked.sym
