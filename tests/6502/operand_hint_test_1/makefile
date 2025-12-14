
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -D abba = 123h -D man=0ah -D name1=George -D name2="John" -D name3 = Peter -D name4 = "Susan" -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -i -s

SFILES = main.s
IFILES = 
OFILES = main.o

all: result.rom check

result.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: result.rom
	byte_tester -s main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym
