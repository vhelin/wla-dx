
CC = wla-65816
CFLAGS = -v -D NAME="kullervo" -o
LD = wlalink
LDFLAGS = -v -S

SFILES = main.s main.o
IFILES = defines.i
OFILES = main.o other.o

all: $(OFILES) test.lib makefile
	$(LD) $(LDFLAGS) linkfile result.smc

main.o: main.s defines.i
	$(CC) $(CFLAGS) main.o main.s

other.o: other.s defines.i
	$(CC) $(CFLAGS) other.o other.s

test.lib: test.s
	$(CC) -l test.lib test.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.smc *.lib *.sym
