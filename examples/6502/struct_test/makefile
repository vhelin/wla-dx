
CC = ../../../binaries/wla-6502.exe
CFLAGS = -o
LD = ../../../binaries/wlalink.exe
LDFLAGS = -vs

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.s main.o


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
