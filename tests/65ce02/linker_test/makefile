
CC = $(WLAVALGRIND) wla-65ce02
CFLAGS = -x -v -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -d -s -b -t CBMPRG -a LoadAddress

SFILES = main.s
IFILES = 
OFILES = main.o

all: linked.prg check

linked.prg: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.prg

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: linked.prg
	byte_tester -s main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.prg linked.sym
