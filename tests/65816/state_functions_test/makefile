CC = $(WLAVALGRIND) wla-65816
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -i -s

all: linked.rom check

linked.rom: main.o linkfile makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s alignment.s
	$(CC) $(CFLAGS) main.o main.s

check: linked.rom
	byte_tester -s main.s

clean:
	rm -f main.o linked.rom linked.sym main.lst core *~
