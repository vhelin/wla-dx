
CC = $(WLAVALGRIND) wla-gb
CFLAGS = -v -x -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -d -v -i -s

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.gb

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.gb linked.sym main.lst
