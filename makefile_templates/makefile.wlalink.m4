include(host_defines)dnl
`CC' = CC
`CFLAGS' = CFLAGS
`LD' = LD
`LDFLAGS' = LDFLAGS
`RM' = RM

CFILES = main.c memory.c parse.c files.c check.c analyze.c write.c compute.c discard.c listfile.c
HFILES = main.h memory.h parse.h files.h check.h analyze.h write.h compute.h discard.h listfile.h defines.h
OFILES = main.`'OBJ_EXTENSION memory.`'OBJ_EXTENSION parse.`'OBJ_EXTENSION files.`'OBJ_EXTENSION check.`'OBJ_EXTENSION analyze.`'OBJ_EXTENSION write.`'OBJ_EXTENSION compute.`'OBJ_EXTENSION discard.`'OBJ_EXTENSION listfile.`'OBJ_EXTENSION


all: $(OFILES) makefile
	$(`LD') $(`LDFLAGS') $(OFILES) LDFLAGS_OUT wlalink.exe


main.o: main.c main.h defines.h
	$(`CC') $(`CFLAGS') main.c

memory.o: memory.c memory.h defines.h
	$(`CC') $(`CFLAGS') memory.c

parse.o: parse.c parse.h defines.h
	$(`CC') $(`CFLAGS') parse.c

files.o: files.c files.h defines.h
	$(`CC') $(`CFLAGS') files.c

check.o: check.c check.h defines.h
	$(`CC') $(`CFLAGS') check.c

analyze.o: analyze.c analyze.h defines.h
	$(`CC') $(`CFLAGS') analyze.c

write.o: write.c write.h defines.h
	$(`CC') $(`CFLAGS') write.c

compute.o: compute.c compute.h defines.h
	$(`CC') $(`CFLAGS') compute.c

discard.o: discard.c discard.h defines.h
	$(`CC') $(`CFLAGS') discard.c

listfile.o: listfile.c listfile.h defines.h
	$(`CC') $(`CFLAGS') listfile.c


$(OFILES): $(HFILES)


clean:
	$(`RM') -f $(OFILES) *~ wlalink.exe

install:
	make ; cp wlalink.exe /usr/local/bin
