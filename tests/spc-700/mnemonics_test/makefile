
CC = $(WLAVALGRIND) wla-spc700
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -i -v -S

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
	rm -f $(OFILES) core *~ *.lst result.rom result.sym
