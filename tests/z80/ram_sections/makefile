
CC = $(WLAVALGRIND) wla-z80
CFLAGS = -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -d -S

SFILES = sms.s
IFILES = 
OFILES = sms.o library.lib

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

sms.o: sms.s
	$(CC) $(CFLAGS) sms.o sms.s

library.lib: library.s
	$(CC) -l library.lib library.s

check: linked.rom
	byte_tester -s sms.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
