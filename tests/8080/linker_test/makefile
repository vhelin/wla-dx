
CC = $(WLAVALGRIND) wla-8080
CFLAGS = -x -v -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -d -s

SFILES = main.s
IFILES = 
OFILES = main.o library.lib extras.lib

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

library.lib: library.s
	$(CC) -l library.lib library.s

extras.lib: extras.s
	$(CC) -l extras.lib extras.s

check: linked.rom
	byte_tester -s main.s
	byte_tester -s namespace.s
	byte_tester -s library.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
