
CC = wla-huc6280
CFLAGS = -i -x -o
LD = wlalink
LDFLAGS = -v -S -i

SFILES = main.s
IFILES = defines.i
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s defines.i
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym *.lst
