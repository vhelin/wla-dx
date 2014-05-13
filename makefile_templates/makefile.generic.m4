dnl User needs to define out_name
include(host_defines)dnl
`CC' = CC
`CFLAGS' = CFLAGS
`LD' = LD
`LDFLAGS' = LDFLAGS
`RM' = RM

CFILES = main.c
HFILES = main.h
OFILES = main`'OBJ_EXTENSION

all: $(OFILES) makefile
	$(`LD') $(`LDFLAGS') $(OFILES) LDFLAGS_OUT`'out_name`'EXE_EXTENSION

main`'OBJ_EXTENSION: main.c main.h makefile
	$(`CC') $(`CFLAGS') main.c


$(OFILES): $(HFILES)


clean:
	$(`RM') -f $(OFILES) *~ wlad.exe

install:
	make ; cp wlad.exe /usr/local/bin
