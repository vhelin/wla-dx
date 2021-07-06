
CC = wla-z80
CFLAGS = -o
LD = wlalink
LDFLAGS = -v -d -s

SFILES = sms.s
IFILES = 
OFILES = sms.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

sms.o: sms.s
	$(CC) $(CFLAGS) sms.o sms.s

check:
	byte_tester -s sms.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
