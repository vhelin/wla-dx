
CC = wla-6502
CFLAGS = -i -o
LD = wlalink
LDFLAGS = -v -i -s

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ *.lst linked.rom linked.sym
