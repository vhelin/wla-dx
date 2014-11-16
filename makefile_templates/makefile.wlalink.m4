include(host_defines)dnl
`CC' = CC
`CFLAGS' = CFLAGS CFLAGS_HOST
`LD' = LD
`LDFLAGS' = LDFLAGS
`RM' = RM
`RMFLAGS' = RMFLAGS

CFILES = main.c memory.c parse.c files.c check.c analyze.c write.c compute.c discard.c listfile.c
HFILES = main.h memory.h parse.h files.h check.h analyze.h write.h compute.h discard.h listfile.h defines.h
OFILES = main`'OBJ_EXTENSION memory`'OBJ_EXTENSION parse`'OBJ_EXTENSION files`'OBJ_EXTENSION check`'OBJ_EXTENSION analyze`'OBJ_EXTENSION write`'OBJ_EXTENSION compute`'OBJ_EXTENSION discard`'OBJ_EXTENSION listfile`'OBJ_EXTENSION


all: $(OFILES) makefile
	$(`LD') $(`LDFLAGS') $(OFILES) LDFLAGS_OUT(wlalink`'EXE_EXTENSION) LIBM_FLAG


main`'OBJ_EXTENSION: main.c main.h defines.h
	$(`CC') $(`CFLAGS') main.c

memory`'OBJ_EXTENSION: memory.c memory.h defines.h
	$(`CC') $(`CFLAGS') memory.c

parse`'OBJ_EXTENSION: parse.c parse.h defines.h
	$(`CC') $(`CFLAGS') parse.c

files`'OBJ_EXTENSION: files.c files.h defines.h
	$(`CC') $(`CFLAGS') files.c

check`'OBJ_EXTENSION: check.c check.h defines.h
	$(`CC') $(`CFLAGS') check.c

analyze`'OBJ_EXTENSION: analyze.c analyze.h defines.h
	$(`CC') $(`CFLAGS') analyze.c

write`'OBJ_EXTENSION: write.c write.h defines.h
	$(`CC') $(`CFLAGS') write.c

compute`'OBJ_EXTENSION: compute.c compute.h defines.h
	$(`CC') $(`CFLAGS') compute.c

discard`'OBJ_EXTENSION: discard.c discard.h defines.h
	$(`CC') $(`CFLAGS') discard.c

listfile`'OBJ_EXTENSION: listfile.c listfile.h defines.h
	$(`CC') $(`CFLAGS') listfile.c


$(OFILES): $(HFILES)


clean:
	$(`RM') $(`RMFLAGS') $(OFILES) *~ wlalink`'EXE_EXTENSION

install:
	MAKE()
	INSTALL(wlalink`'EXE_EXTENSION, DEFAULT_DEST)
