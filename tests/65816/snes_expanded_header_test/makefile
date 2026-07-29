CC = $(WLAVALGRIND) wla-65816
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS =

SFILES = main.s
OFILES = main.o

all: result.rom check

result.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: result.rom
	byte_tester -s main.s
	! $(CC) $(CFLAGS) missing_snesheader.o missing_snesheader.s

clean:
	rm -f $(OFILES) missing_snesheader.o core *~ result.rom