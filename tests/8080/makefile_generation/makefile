
CC = $(WLAVALGRIND) wla-8080
CFLAGS = -M

SFILES = main.s extras.s
OFILES = main.o extras.o

all: $(OFILES) makefile

main.o: main.s
	$(CC) $(CFLAGS) -o main.o main.s

extras.o: extras.s
	$(CC) $(CFLAGS) extras.s


clean:
	rm -f core *~ linked.rom linked.sym *.o
