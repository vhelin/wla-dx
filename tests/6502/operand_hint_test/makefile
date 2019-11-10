
CC = wla-6502
CFLAGS = -D abba = 123h -D man=0ah -D name1=George -D name2="John" -D name3 = Peter -D name4 = "Susan" -o
LD = wlalink
LDFLAGS = -v -i

SFILES = main.s
IFILES = 
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile result.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom
