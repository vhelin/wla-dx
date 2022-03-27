
CC = $(WLAVALGRIND) wla-65816
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -i -v -s

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym main.lst
