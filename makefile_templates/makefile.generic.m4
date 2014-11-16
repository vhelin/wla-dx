dnl User needs to define out_name
include(host_defines)dnl
`CC' = CC
`CFLAGS' = CFLAGS
`LD' = LD
`LDFLAGS' = LDFLAGS
`RM' = RM
`RMFLAGS' = RMFLAGS

CFILES = main.c
HFILES = main.h
OFILES = main`'OBJ_EXTENSION

all: $(OFILES) makefile
	$(`LD') $(`LDFLAGS') $(OFILES) LDFLAGS_OUT(out_name`'EXE_EXTENSION) LIBM_FLAG

main`'OBJ_EXTENSION: main.c main.h makefile
	$(`CC') $(`CFLAGS') main.c


$(OFILES): $(HFILES)


clean:
	$(`RM') $(`RMFLAGS') $(OFILES) *~ out_name`'EXE_EXTENSION

install:
	MAKE()
	INSTALL(out_name`'EXE_EXTENSION, DEFAULT_DEST)
