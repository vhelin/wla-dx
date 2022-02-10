
CC = $(WLAVALGRIND) wla-6800
CFLAGS = -i -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -S -i

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym *.lst
