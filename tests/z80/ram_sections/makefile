
CC = wla-z80
CFLAGS = -x -o
LD = wlalink
LDFLAGS = -v -d -s

SFILES = sms.s
IFILES = 
OFILES = sms.o library.lib

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

sms.o: sms.s
	$(CC) $(CFLAGS) sms.o sms.s

library.lib: library.s
	$(CC) -l library.lib library.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
