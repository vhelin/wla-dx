
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -i -s

SFILES = main.s more.s
IFILES = 
OFILES = main.o more.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

more.o: more.s
	$(CC) $(CFLAGS) more.o more.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ *.lst linked.rom linked.sym
