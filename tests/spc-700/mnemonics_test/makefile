
CC = wla-spc700
CFLAGS = -i -o
LD = wlalink
LDFLAGS = -i -v -S

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ *.lst result.rom result.sym
