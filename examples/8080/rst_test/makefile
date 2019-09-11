
CC = wla-8080
CFLAGS = -o
LD = wlalink
LDFLAGS = -v -d -s

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
