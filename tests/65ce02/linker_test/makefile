
CC = wla-65ce02
CFLAGS = -x -v -o
LD = wlalink
LDFLAGS = -v -d -s -b -t CBMPRG -a LoadAddress

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.prg

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.prg linked.sym
