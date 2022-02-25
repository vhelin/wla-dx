
CC = $(WLAVALGRIND) wla-z80
CFLAGS = -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -d -S

SFILES = sms.s
IFILES = 
OFILES = sms.o

all: linked.rom

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

sms.o: sms.s
	$(CC) $(CFLAGS) sms.o sms.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
