
CC = wla-gb
CFLAGS = -x -o
LD = wlalink
LDFLAGS = -v -s

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.gb
	diff linked.sym expected.sym
	md5sum -c linked.md5

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.gb linked.sym
