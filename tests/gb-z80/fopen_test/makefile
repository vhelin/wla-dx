
CC = $(WLAVALGRIND) wla-gb
CFLAGS = -x -I data1 -I data2 -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s
IFILES = includes/defines.i
OFILES = main.o

all: result.gb check

result.gb: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.gb

main.o: main.s includes/defines.i
	$(CC) $(CFLAGS) main.o main.s

check: result.gb
	byte_tester -s main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.gb result.sym

