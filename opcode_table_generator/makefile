CC=gcc
LD=gcc
CFLAGS = -c -O3 -ansi -pedantic-errors -Wall -Wextra  -DMSDOS -DWIN32 -D$(WLA_TARGET) -DQUIET=$(WLA_QUIET)
LDFLAGS = -s 
RM = rm -f 
RMFLAGS = 
CFILES = main.c
OFILES = main.o



all: $(OFILES) makefile
	$(LD) $(LDFLAGS) $(OFILES) -o gen.exe 

main.o: main.c makefile
	$(CC) $(CFLAGS) main.c


$(OFILES): $(CFILES)


clean:
	$(RM) $(RMFLAGS) $(OFILES) core *~ gen
