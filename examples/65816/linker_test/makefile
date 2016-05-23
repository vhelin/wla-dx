
CC = wla-65816
CFLAGS = -v -D NAME = kullervo -o
LD = wlalink
LDFLAGS = -vS

SFILES = main.s syntax.s
IFILES = defines.i
OFILES = main.o syntax.o

all: $(OFILES) test.lib makefile
	$(LD) $(LDFLAGS) linkfile result.smc

main.o: main.s defines.i
	$(CC) $(CFLAGS) main.o main.s

syntax.o: syntax.s defines.i
	$(CC) $(CFLAGS) syntax.o syntax.s

test.lib: test.s
	$(CC) -l test.lib test.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.smc *.lib *.sym
