
CC = wla-z80
CFLAGS = -i -o
LD = wlalink
LDFLAGS = -v -s -i

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.prg

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.prg linked.sym main.lst
