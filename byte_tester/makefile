
CC=gcc
LD=gcc

CFLAGS= -c -ansi -O3 -pedantic -Wall
LDFLAGS = 

CFILES = main.c
OFILES = main.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) $(OFILES) -o byte_tester

main.o: main.c makefile
	$(CC) $(CFLAGS) main.c

$(OFILES): $(HFILES) $(CFILES)

clean:
	rm $(OFILES) byte_tester *~

install:
	make ; cp byte_tester ../binaries
