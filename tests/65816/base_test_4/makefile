
CC = $(WLAVALGRIND) wla-65816
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s next.s
IFILES = 
OFILES = main.o next.o

all: result.rom check

result.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

next.o: next.s
	$(CC) $(CFLAGS) next.o next.s

check: result.rom
	byte_tester -s main.s ; byte_tester -s next.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym
