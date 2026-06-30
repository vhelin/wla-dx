CC = $(WLAVALGRIND) wla-65816
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -g

OFILES = main.o

all: linked.rom

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s 3bytes.i
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)

clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym main.lst
