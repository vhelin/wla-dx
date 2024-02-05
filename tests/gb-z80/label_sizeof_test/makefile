
CC = $(WLAVALGRIND) wla-gb
CFLAGS = -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -d -v -S

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.gb

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.gb linked.sym
