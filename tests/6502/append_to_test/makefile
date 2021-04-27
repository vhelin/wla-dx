
CC = wla-6502
CFLAGS = -o
LD = wlalink
LDFLAGS = -v -s

SFILES = main.s more.s last.s
IFILES = 
OFILES = main.o more.o last.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

more.o: more.s
	$(CC) $(CFLAGS) more.o more.s

last.o: last.s
	$(CC) $(CFLAGS) last.o last.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
